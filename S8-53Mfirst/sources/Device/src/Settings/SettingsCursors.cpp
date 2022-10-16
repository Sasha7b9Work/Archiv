// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Utils/Math_.h"
#include "common/Utils/Containers/Values_.h"
#include "FPGA/MathFPGA.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstring>


float Cursors::GetPosU(const Chan &ch, int num)
{
    return CURS_U_POS(ch, num) / (Grid::Channel::Bottom() == Grid::FullBottom() ? 1.0F : 2.0F);
}


bool Cursors::NecessaryDraw()
{
    return (
        !CURS_U_CONTROL_IS_DISABLED(CURS_SOURCE) || !CURS_T_CONTROL_IS_DISABLED(CURS_SOURCE)
        ) &&
        (CURS_SHOW || Menu::GetNameOpenedPage() == NamePage::CursorsSet_SB);
}


Voltage Cursors::GetVoltage(const Chan &source, int numCur)
{
    return GetVoltage(GetPosU(source, numCur), RANGE(source), RSHIFT(source));
}


Voltage Cursors::GetVoltage(float shift_cur, Range::E range, int16 rshift)
{
    return MathFPGA::MaxVoltageOnScreen(range) - shift_cur * MathFPGA::voltsInPixel[range] - RShift::ToAbs(rshift, range);
}


Time Cursors::GetTime(const Chan &source, int num_cur)
{
    return GetTime(CURS_T_POS(source, num_cur), TBASE);
}


Time Cursors::GetTime(float shift_cur, TBase::E tBase)
{
    return shift_cur * TShift::abs_step[tBase];
}


Text Cursors::GetPercentsU(const Chan &source)
{
    float dPerc = CURS_100_PERCENTS_DU(source); 

    float dValue = std::fabsf(GetPosU(source, 0) - GetPosU(source, 1));

    Text result = Float(dValue / dPerc * 100.0F).ToText(false, 5);

    result.Append("%");

    return result;
}


Text Cursors::GetPercentsT(const Chan &source)
{
    float dPerc = CURS_100_PERCENTS_DT(source);

    float dValue = std::fabsf(CURS_T_POS(source, 0) - CURS_T_POS(source, 1));

    Text result = Float(dValue / dPerc * 100.0F).ToText(false, 6);

    result.Append("%");

    return result;
}


void Cursors::Draw()
{
    Color::Cursors(CURS_SOURCE).SetAsCurrent();

    if (Cursors::NecessaryDraw())
    {
        // Признак того, что включены и вертикальные и горизонтальные курсоры - надо нарисовать квадраты в местах
        // пересечения
        bool bothCursors = !CURS_T_CONTROL_IS_DISABLED(CURS_SOURCE) && !CURS_U_CONTROL_IS_DISABLED(CURS_SOURCE);  

        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = (int)(Grid::Left() + CURS_T_POS(CURS_SOURCE, 0));
            x1 = (int)(Grid::Left() + CURS_T_POS(CURS_SOURCE, 1));
            y0 = (int)(Grid::Top() + Cursors::GetPosU(CURS_SOURCE, 0));
            y1 = (int)(Grid::Top() + Cursors::GetPosU(CURS_SOURCE, 1));

            Rectangle(5, 5).Draw(x0 - 2, y0 - 2);
            Rectangle(5, 5).Draw(x1 - 2, y1 - 2);
        }

        if (!CURS_T_CONTROL_IS_DISABLED(CURS_SOURCE))
        {
            DrawVertical((int)(CURS_T_POS(CURS_SOURCE, 0)), y0);
            DrawVertical((int)(CURS_T_POS(CURS_SOURCE, 1)), y1);
        }

        if (!CURS_U_CONTROL_IS_DISABLED(CURS_SOURCE))
        {
            DrawHorizontal((int)(Cursors::GetPosU(CURS_SOURCE, 0)), x0);
            DrawHorizontal((int)(Cursors::GetPosU(CURS_SOURCE, 1)), x1);
        }
    }
}


void Cursors::DrawVertical(int x, int yTearing)
{
    x += Grid::Left();
    int y = Grid::Channel::Bottom() - 2;

    if (yTearing == -1)
    {
        DashedVLine(1, 1).Draw(x, Grid::Top() + 2, y, 0);
    }
    else
    {
        DashedVLine(1, 1).Draw(x, Grid::Top() + 2, yTearing - 2, 0);
        DashedVLine(1, 1).Draw(x, yTearing + 2, y, 0);
    }

    Rectangle(3, 3).Draw(x - 1, Grid::Top() - 1);
    Rectangle(3, 3).Draw(x - 1, y);
}


void Cursors::DrawHorizontal(int y, int xTearing)
{
    y += Grid::Top();
    int x = Grid::Right() - 2;

    if (xTearing == -1)
    {
        DashedHLine(1, 1).Draw(y, Grid::Left() + 2, x, 0);
    }
    else
    {
        DashedHLine(1, 1).Draw(y, Grid::Left() + 2, xTearing - 2, 0);
        DashedHLine(1, 1).Draw(y, xTearing + 2, x, 0);
    }

    Rectangle(3, 3).Draw(Grid::Left() - 1, y - 1);
    Rectangle(3, 3).Draw(x, y - 1);
}
