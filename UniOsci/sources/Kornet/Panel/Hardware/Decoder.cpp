#include "defines.h"
#include "Decoder.h"
#include "FSMC.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Keyboard/Keyboard.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Decoder decoder;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Decoder::Decoder() : curFunc(0), step(0)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::AddData(uint8 data)
{
    static const pFuncBU8 command[NUM_COMMANDS] =
    {
        &Decoder::EmptyFunc,
        &Decoder::InButtonPress,
        &Decoder::BeginScene,
        &Decoder::EndScene,
        &Decoder::SetColor,
        &Decoder::FillRegion,
        &Decoder::DrawText,
        &Decoder::SetPalette,
        &Decoder::DrawRectangle,
        &Decoder::DrawVLine,
        &Decoder::DrawHLine,
        &Decoder::SetFont,
        &Decoder::SetPoint,
        &Decoder::DrawLine
    };

    if (step == 0)
    {
        if (data < NUM_COMMANDS)
        {
            curFunc = command[data];
        }
        else
        {
            FinishCommand();
            return;     // Простенькая защита от ошибок
        }
    }

    if ((this->*curFunc)(data))
    {
        FinishCommand();
    }
    else
    {
        step++;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::InButtonPress(uint8)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        return false;
    }
    if (step == 2)
    {
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::BeginScene(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        painter.BeginScene((Color)data);
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::EndScene(uint8)
{
    painter.EndScene();
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::SetColor(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        painter.SetColor((Color)data);
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::FillRegion(uint8 data)
{
    static int x;
    static int y;
    static int width;
    static int height;

    switch (step)
    {
        case 0:                                     break;
        case 1:     x = data;                       break;
        case 2:     x += (((int)data) << 8);        break;
        case 3:     y = data;                       break;
        case 4:     width = data;                   break;
        case 5:     width += (((int)data) << 8);    break;
        case 6:     height = (int)data;
            painter.FillRegion(x, y, width, height);
            return true;
        default:
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawRectangle(uint8 data)
{
    static int x;
    static int y;
    static int width;
    static int height;

    switch (step)
    {
        case 0:                                     break;
        case 1:     x = data;                       break;
        case 2:     x += (((int)data) << 8);        break;
        case 3:     y = data;                       break;
        case 4:     width = data;                   break;
        case 5:     width += (((int)data) << 8);    break;
        case 6:     height = (int)data;
            painter.DrawRectangle(x, y, width, height);
            return true;
        default:
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawVLine(uint8 data)
{
    static int x;
    static int y0;
    static int y1;

    switch (step)
    {
        case 0:                             break;
        case 1: x = data;                   break;
        case 2: x += (((int)data) << 8);    break;
        case 3: y0 = data;                  break;
        case 4: y1 = data;
            painter.DrawVLine(x, y0, y1);
            return true;
        default:
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawHLine(uint8 data)
{
    __IO static int y;  /// \todo эти штуки __IO вставлены потому, что без них c оптимизацией экран ничего не хочет выводить. Надо потом разобраться
    __IO static int x0;
    __IO static int x1;

    switch (step)
    {
        case 0:                             break;
        case 1:     y = data;               break;
        case 2:     x0 = data;              break;
        case 3:     x0 += (int)data << 8;   break;
        case 4:     x1 = data;              break;
        case 5:     x1 += (int)data << 8;
            painter.DrawHLine(y, x0, x1);
            return true;
        default:
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawLine(uint8 data)
{
    __IO static int x0;
    __IO static int y0;
    __IO static int x1;

    switch (step)
    {
        case 0:                         break;
        case 1: x0 = data;              break;
        case 2: x0 += ((int)data << 8); break;
        case 3: y0 = data;              break;
        case 4: x1 = data;              break;
        case 5: x1 += ((int)data << 8); break;
        case 6: painter.DrawLine(x0, y0, x1, data);
            return true;
        default:
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::SetPoint(uint8 data)
{
    static int x = 0;

    switch (step)
    {
        case 0:                         break;
        case 1: x = data;               break;
        case 2: x += ((int)data << 8);  break;
        case 3: painter.SetPoint(x, data);
            return true;
        default:
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawText(uint8 data)
{
    static int x;
    static int y;
    static int numSymbols;
    static int readingSymbols;
    static char *buffer;

    switch (step)
    {
        case 0:                             break;
        case 1:     x = data;               break;
        case 2:     x += (int)data << 8;    break;
        case 3:     y = data;               break;
        case 4:
            numSymbols = data;
            readingSymbols = 0;
            buffer = new char[(uint)(numSymbols + 1)];
            break;
        default:
            buffer[readingSymbols++] = (char)data;
            if (readingSymbols == numSymbols)
            {
                buffer[readingSymbols] = 0;
                painter.DrawText(x, y, buffer);
                delete [] buffer;
                return true;
            }
            break;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::SetPalette(uint8 data)
{
    static uint8 numColor;
    static uint valueColor;

    switch (step)
    {
        case 0:                                 break;
        case 1: numColor = data;                break;
        case 2: valueColor = data;              break;
        case 3: valueColor |= (uint)data << 8;  break;
        case 4: valueColor |= (uint)data << 16; break;
        case 5: valueColor |= (uint)data << 24;
            painter.SetPalette(numColor, valueColor);
            return true;
        default:
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::SetFont(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    painter.SetFont((TypeFont)data);

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::FinishCommand()
{
    step = 0;
    curFunc = 0;
}
