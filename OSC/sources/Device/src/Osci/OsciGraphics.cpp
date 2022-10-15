#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Osci.h"
#include "Display/Painter.h"
#include "Grid.h"
#include "BottomPart.h"
#include "HiPart.h"
#include "PainterData.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGATypes.h"
#include "FPGA/RShift.h"
#include "FrequencyCounter.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DELTA 5


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Graphics::Update()
{
    Painter::BeginScene(Color::BACK);

    Grid::Draw();
   
    PainterData::DrawData();

    Painter::DrawRectangle(Grid::Left(), Grid::Top(), Grid::Width(), Grid::Height(), Color::FILL);

    BottomPart::Draw();

    RShift::Draw();

    DrawCursorTrigLevel();

    HiPart::Draw();

    FrequencyCounter::Draw();

    Menu::Graphics::Draw();

    Measure::Graphics::Draw();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Graphics::DrawCursorTrigLevel()
{
    Trig::Source ch = TRIG_SOURCE;
    int trigLev = SET_TRIGLEV_SOURCE - (TRIG_SOURCE_IS_EXT ? 0 : -(SET_RSHIFT(ch) - RShift::ZERO));
    float scale = 1.0f / ((Trig::MAX - Trig::MIN) / 2.4f / Grid::Height());
    int y0 = (Grid::Top() + Grid::Bottom()) / 2 + (int)(scale * (Trig::ZERO - Trig::MIN));
    int y = y0 - (int)(scale * (trigLev - Trig::MIN));

    if (!TRIG_SOURCE_IS_EXT)
    {
        y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();
    }

    int x = Grid::Right();
    Painter::SetColor(Color::Trig());

    if (y > Grid::Bottom())
    {
        Painter::DrawChar(x + 3, Grid::Bottom() - 11, SYMBOL_TRIG_LEV_LOWER);
        Painter::SetPoint(x + 5, Grid::Bottom() - 2);
        y = Grid::Bottom() - 7;
        x--;
    }
    else if (y < Grid::Top())
    {
        Painter::DrawChar(x + 3, Grid::Top() + 2, SYMBOL_TRIG_LEV_ABOVE);
        Painter::SetPoint(x + 5, Grid::Top() + 2);
    }
    else
    {
        Painter::DrawChar(x + 1, y - 4, SYMBOL_TRIG_LEV_NORMAL);
    }

    Painter::SetFont(Font::Type::_5);

    const char symbols[3] = {'1', '2', '�'};
    int dY = 0;

    Painter::DrawChar(x + 5, y - 9 + dY, symbols[(uint8)TRIG_SOURCE], Color::BACK);
    Painter::SetFont(Font::Type::_8);

    DrawScaleLine(Display::WIDTH - 11, true);
    int left = Grid::Right() + 9;
    int height = Grid::Height() - 2 * DELTA;
    int shiftFullMin = RShift::MIN + Trig::MIN;
    int shiftFullMax = RShift::MAX + Trig::MAX;
    scale = (float)height / (shiftFullMax - shiftFullMin);
    int shiftFull = SET_TRIGLEV_SOURCE + (TRIG_SOURCE_IS_EXT ? 0 : SET_RSHIFT(ch));
    int yFull = Grid::Top() + DELTA + height - (int)(scale * (shiftFull - RShift::MIN - Trig::MIN) + 4);
    Painter::FillRegion(left + 2, yFull + 1, 4, 6, Color::Trig());
    Painter::SetFont(Font::Type::_5);
    Painter::DrawChar(left + 3, yFull - 5 + dY, symbols[(uint8)TRIG_SOURCE], Color::BACK);
    Painter::SetFont(Font::Type::_8);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Graphics::DrawScaleLine(int x, bool forTrigLev)
{
    int width = 6;
    int topY = Grid::Top() + DELTA;
    int x2 = width + x + 2;
    int bottomY = Grid::Bottom() - DELTA;
    int centerY = (Grid::Bottom() + Grid::Top()) / 2;
    int levels[] =
    {
        topY,
        bottomY,
        centerY,
        centerY - (bottomY - topY) / (forTrigLev ? 8 : 4),
        centerY + (bottomY - topY) / (forTrigLev ? 8 : 4)
    };

    for (int i = 0; i < 5; i++)
    {
        Painter::DrawLine(x + 1, levels[i], x2 - 1, levels[i], Color::FILL);
    }
}
