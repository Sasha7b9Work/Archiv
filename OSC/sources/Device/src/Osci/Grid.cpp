#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Grid.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Grid::Left()
{
    return LEFT;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Top()
{
    return 19;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Width()
{
    return 280;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Height()
{
    return 200;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Bottom()
{
    return Top() + Height();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::FullBottom()
{
    return Bottom();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Right()
{
    return Left() + Width();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::NumRows()
{
    return 10;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::MathTop()
{
    return Top();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::NumCols()
{
    return 14;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Delta()
{
    return 20;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::MathBottom()
{
    return Bottom();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::MathHeight()
{
    return Height();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::ChannelBottom()
{
    return (Display::IsSeparate()) ? (Top() + Height() / 2) : FullBottom();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::ChannelCenterHeight()
{
    return (Top() + Bottom()) / 2;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::Draw()
{
    if (Display::IsSeparate())
    {
        DrawGridSignal(Left(), Top(), Width(), Height() / 2);
        if (FFT_ENABLED)
        {
            DrawGridSpectrum();
        }
        if (FUNC_MODE_DRAW_IS_ENABLED)
        {
            DrawGridSignal(Left(), Top() + Height() / 2, Width(), Height() / 2);
        }
        Painter::DrawHLine(Top() + Height() / 2, Left(), Left() + Width(), Color::FILL);
    }
    else
    {
        DrawGridSignal(Left(), Top(), Width(), Height());
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::DrawGridSignal(int left, int top, int width, int height)
{
    int right = left + width;
    int bottom = top + height;

    Painter::SetColor(Color::FILL);

    if (top == Top())
    {
        Painter::DrawHLine(top, 1, left - 2);
        Painter::DrawHLine(top, right + 2, Display::WIDTH - 2);

        if (!Menu::IsMinimize() || !Menu::IsShown())
        {
            Painter::DrawVLine(1, top + 2, bottom - 2);
            Painter::DrawVLine(318, top + 2, bottom - 2);
        }
    }

    float deltaX = DeltaX() * (float)width / width;
    float deltaY = DeltaY() * (float)height / height;
    float stepX = deltaX / 5;
    float stepY = deltaY / 5;

    float centerX = (float)(left + width / 2);
    float centerY = (float)(top + height / 2);

    Painter::SetColor(Color::GRID);
    if (TYPE_GRID_1)
    {
        DrawGridType1(left, top, right, bottom, centerX, centerY, deltaX, deltaY, stepX, stepY);
    }
    else if (TYPE_GRID_2)
    {
        DrawGridType2(left, top, right, bottom, (int)deltaX, (int)deltaY, (int)stepX, (int)stepY);
    }
    else if (TYPE_GRID_3)
    {
        DrawGridType3(left, top, right, bottom, (int)centerX, (int)centerY, (int)deltaX, (int)deltaY, (int)stepX);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::DrawGridSpectrum()
{
    if (SCALE_FFT_IS_LOG)
    {
        static const int nums[] = {4, 6, 8};
        static pString strs[] = {"0", "-10", "-20", "-30", "-40", "-50", "-60", "-70"};
        int numParts = nums[MAX_DB_FFT];
        float scale = (float)MathHeight() / numParts;
        for (int i = 1; i < numParts; i++)
        {
            int y = MathTop() + (int)(i * scale);
            Painter::DrawHLine(y, Left(), Left() + 256, Color::GRID);
            if (!Menu::IsMinimize())
            {
                Painter::SetColor(Color::FILL);
                Painter::DrawText(3, y - 4, strs[i]);
            }
        }
        if (!Menu::IsMinimize())
        {
            Painter::SetColor(Color::FILL);
            Painter::DrawText(5, MathTop() + 1, "��");
        }
    }
    else if (SCALE_FFT_IS_LINEAR)
    {
        static pString strs[] = {"1.0", "0.8", "0.6", "0.4", "0.2"};
        float scale = (float)MathHeight() / 5;
        for (int i = 1; i < 5; i++)
        {
            int y = MathTop() + (int)(i * scale);
            Painter::DrawHLine(y, Left(), Left() + 256, Color::GRID);
            if (!Menu::IsMinimize())
            {
                Painter::DrawText(5, y - 4, strs[i], Color::FILL);
            }
        }
    }
    Painter::DrawVLine(Left() + 256, MathTop(), MathBottom(), Color::FILL);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Grid::DeltaY()
{
    float delta = (FullBottom() - Top()) / 10.0f;
    return Display::IsSeparate() ? (delta / 2.0f) : delta;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Grid::DeltaX()
{
    float delta = (Right() - Left()) / 14.0f;
    return delta;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY)
{
    uint16 masX[17];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 7; i++)
    {
        masX[i] = (uint16)(left + (int)(deltaX * i));
    }
    for (int i = 7; i < 10; i++)
    {
        masX[i] = (uint16)((int)centerX - 8 + i);
    }
    for (int i = 10; i < 16; i++)
    {
        masX[i] = (uint16)(centerX + deltaX * (i - 9));
    }
    masX[16] = (uint16)(right - 1);

    Painter::DrawMultiVPointLine(17, top + (int)stepY, masX, (int)stepY, DeltaVforLineGrid(), Color::GRID);

    uint8 mas[13];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 5; i++)
    {
        mas[i] = (uint8)(top + (int)(deltaY * i));
    }
    for (int i = 5; i < 8; i++)
    {
        mas[i] = (uint8)((int)(centerY)-6 + i);
    }
    for (int i = 8; i < 12; i++)
    {
        mas[i] = (uint8)((int)centerY + (int)(deltaY * (i - 7)));
    }
    mas[12] = (uint8)(bottom - 1);

    Painter::DrawMultiHPointLine(13, left + (int)stepX, mas, (int)stepX, DeltaHforLineGrid(), Color::GRID);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY)
{
    uint16 masX[15];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 14; i++)
    {
        masX[i] = (uint16)(left + (int)(deltaX * i));
    }
    masX[14] = (uint16)(right - 1);
    Painter::DrawMultiVPointLine(15, top + stepY, masX, stepY, DeltaVforLineGrid(), Color::GRID);

    uint8 mas[11];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 10; i++)
    {
        mas[i] = (uint8)(top + (int)(deltaY * i));
    }
    mas[10] = (uint8)(bottom - 1);
    Painter::DrawMultiHPointLine(11, left + stepX, mas, stepX, DeltaHforLineGrid(), Color::GRID);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX)
{
    Painter::DrawHPointLine(centerY, left + stepX, right, (float)stepX);
    uint8 masY[6] = {(uint8)(top + 1), (uint8)(top + 2), (uint8)(centerY - 1), (uint8)(centerY + 1), (uint8)(bottom - 2), (uint8)(bottom - 1)};
    Painter::DrawMultiHPointLine(6, left + deltaX, masY, deltaX, (right - top) / deltaX, Color::GRID);
    Painter::SetColor(Color::GRID);
    Painter::DrawVPointLine(centerX, top + stepX, bottom - stepX, (float)stepX);
    uint16 masX[6] = {(uint16)(left + 1), (uint16)(left + 2), (uint16)(centerX - 1), (uint16)(centerX + 1), (uint16)(right - 2), (uint16)(right - 1)};
    Painter::DrawMultiVPointLine(6, top + deltaY, masX, deltaY, (bottom - top) / deltaY, Color::GRID);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::DeltaVforLineGrid()
{
    if (SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (NUM_MEASURES_IS_1_5)
        {
            return VIEW_MEASURES_BOTH ? 55 : 59;
        }
        if (NUM_MEASURES_IS_2_5)
        {
            return VIEW_MEASURES_BOTH ? 69 : 51;
        }
        if (NUM_MEASURES_IS_3_5)
        {
            return VIEW_MEASURES_BOTH ? 54 : 68;
        }
    }

    return 49;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::DeltaHforLineGrid()
{
    if (MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (NUM_MEASURES_IS_6_1)
        {
            return 73;
        }
        if (NUM_MEASURES_IS_6_2)
        {
            return 83;
        }
    }
    return 69;
}
