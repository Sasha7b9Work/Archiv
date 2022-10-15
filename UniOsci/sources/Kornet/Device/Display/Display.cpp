#include "Device.h"
#include "Display.h"
#include "Grid.h"
#include "Painter.h"
#include "PainterData.h"
#include "Log.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAMath.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <stdlib.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Display display;

uint8 color = 0;

#define SIZE_CONSOLE    20
static CHAR_BUF2(bufferConsole, SIZE_CONSOLE, 100);
static int stringInConsole = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    painter.SetPalette(Color::BACK,        0x00000000);
    painter.SetPalette(Color::FILL,        0x00ffffff);
    painter.SetPalette(Color::CHAN_A,      0x000000ff);
    painter.SetPalette(Color::CHAN_A_HALF, 0x00000080);
    painter.SetPalette(Color::CHAN_B,      0x0000ff00);
    painter.SetPalette(Color::CHAN_B_HALF, 0x00008000);
    painter.SetPalette(Color::GRID,        0x00afafaf);
    painter.SetPalette(Color::BLUE,        0x000000ff);
    painter.SetPalette(Color::GREEN,       0x0000ff00);
    painter.SetPalette(Color::RED,         0x00ff0000);

    inProcessDrawConsole = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    typedef void (Display::*pFuncDisplayVV)();

    static const pFuncDisplayVV funcs[NumDeviceModes] =
    {
        &Display::UpdateOsci,
        &Display::UpdateTester,
        &Display::UpdateMultimeter
    };

    pFuncDisplayVV func = funcs[device.CurrentMode()];

    (this->*func)();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::UpdateOsci()
{
    painter.BeginScene(Color::BACK);

    DrawGrid();

    WriteLowPart();

    DrawRShift();

    painterData.DrawData();
   
    DrawConsole();

    menu.Draw();

    painter.EndScene();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::UpdateTester()
{
    painter.BeginScene(Color::BACK);

    int size = 239;

    painter.DrawRectangle(0, 0, size, size, Color::FILL);
    painter.DrawRectangle(0, 0, Display::WIDTH - 1, Display::HEIGHT - 1);

    for (int i = 0; i < NUM_STEPS; i++)
    {
        DrawDataTester(i, 0, 0);
    }

    menu.Draw();

    DrawConsole();

    painter.EndScene();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawDataTester(int numStep, int x0, int y0)
{
    static const uint8 colors[5] = {Color::FILL, Color::GRID, Color::RED, Color::GREEN, Color::BLUE};

    uint8 *dataX = &dataTester[A][numStep][0];
    uint8 *dataY = &dataTester[B][numStep][0];

    painter.SetColor(colors[numStep]);
    
    Math::Smoothing(dataX, TESTER_NUM_POINTS, TESTER_NUM_SMOOTH + 1);
    Math::Smoothing(dataY, TESTER_NUM_POINTS, TESTER_NUM_SMOOTH + 1);

    if (VIEW_MODE_IS_LINES)
    {
        int x1 = x0 + TESTER_NUM_POINTS - (dataX[1] - MIN_VALUE);
        int y1 = y0 + dataY[1] - MIN_VALUE;
        for (int i = 2; i < TESTER_NUM_POINTS; i++)
        {
            int x2 = x0 + TESTER_NUM_POINTS - (dataX[i] - MIN_VALUE);
            int y2 = y0 + dataY[i] - MIN_VALUE;
            painter.DrawLine(x1, y1, x2, y2);
            x1 = x2;
            y1 = y2;
        }
    }
    else
    {
        for (int i = 0; i < TESTER_NUM_POINTS; i++)
        {
            int x = x0 + TESTER_NUM_POINTS - (dataX[i] - MIN_VALUE);
            int y = y0 + dataY[i] - MIN_VALUE;

            if (x > x0 && x < x0 + TESTER_NUM_POINTS && y > y0 && y < y0 + TESTER_NUM_POINTS)
            {
                painter.SetPoint(x, y);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::UpdateMultimeter()
{
    painter.BeginScene(Color::BACK);

    painter.DrawText(10, 10, "Мультиметр", Color::RED);

    painter.EndScene();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawGrid()
{
    int x0 = Grid::Left();
    int y0 = Grid::Top();

    painter.DrawVLine(x0 + Grid::WIDTH / 2, y0, y0 + Grid::HEIGHT, Color::GRID);

    painter.DrawHLine(y0 + Grid::HEIGHT / 2, x0, x0 + Grid::WIDTH);

    for (int x = x0; x < x0 + Grid::Width(); x += Grid::SIZE_CELL)
    {
        painter.DrawVLine(x, y0, y0 + Grid::HEIGHT);
    }

    for (int y = y0; y < y0 + Grid::HEIGHT; y += Grid::SIZE_CELL)
    {
        painter.DrawHLine(y, x0, x0 + Grid::WIDTH);
    }

    painter.DrawRectangle(x0, y0, Grid::WIDTH, Grid::HEIGHT, Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::WriteLowPart()
{
    int x = WriteChannel(A, Grid::LEFT, Grid::Bottom() + 1);
    WriteChannel(B, Grid::LEFT, Grid::Bottom() + 9);
    WriteTBase(x, Grid::Bottom() + 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Display::WriteChannel(Channel ch, int x, int y)
{
    painter.DrawText(x, y, ch == A ? "1:" : "2:", Color::Chan(ch));
    
    x += 7;

    static const char symbols[3] =
    {
        SYMBOL_COUPLE_AC, 
        SYMBOL_COUPLE_DC, 
        SYMBOL_COUPLE_GND
    };

    char string[2] = {symbols[SET_COUPLE(ch)], 0};

    painter.DrawText(x, y, string);

    x += 8;

    painter.DrawText(x, y, NameRange(SET_RANGE(ch)));

    x += 22;

    CHAR_BUF(buffer, 20);

    Voltage2String(RSHIFT_2_ABS(SET_RSHIFT(ch), SET_RANGE(ch)), true, buffer);

    painter.DrawText(x, y, buffer);

    return x + 47;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::WriteTBase(int x, int y)
{
    painter.DrawText(x, y, NameTBase(SET_TBASE), Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawRShift()
{
    DrawRShift(A);
    DrawRShift(B);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawRShift(Channel ch)
{
    painter.SetColor(Color::Chan(ch));

    int delta = (SET_RSHIFT(ch) - RShiftZero) / STEP_RSHIFT;

    int y = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top() - delta;

    painter.DrawChar(Grid::Left() - 8, y - 4, SYMBOL_RSHIFT_MARKER);

    painter.SetFont(TypeFont_5);

    painter.DrawChar(Grid::Left() - 7, y - 6, ch == A ? '1' : '2', Color::BACK);

    painter.SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LogEntity::AddToConsole(char *string)
{
    /// \todo Мы пропускаем некоторые строки. Сделать отложенное добавление
    if (!display.inProcessDrawConsole)      // Страхуемся на предмет того, что сейчас не происходит вывод консоли в другом потоке
    {
        static int count = 0;
        if (stringInConsole == SIZE_CONSOLE)
        {
            for (int i = 1; i < SIZE_CONSOLE; i++)
            {
                strcpy(bufferConsole[i - 1], bufferConsole[i]);
            }
            stringInConsole--;
        }
        sprintf(bufferConsole[stringInConsole], "%d %s", count++, string);
        stringInConsole++;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawConsole()
{
    inProcessDrawConsole = true;

    painter.SetFont(TypeFont_5);

    int y = 0;

    for (int i = 0; i < stringInConsole; i++)
    {
        painter.DrawText(1, y, bufferConsole[i], Color::FILL);
        y += 6;
    }

    painter.SetFont(TypeFont_8);

    inProcessDrawConsole = false;
}
