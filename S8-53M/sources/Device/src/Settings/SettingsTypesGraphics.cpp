// 2021/03/18 9:50:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Math_.h"
#include "Display/Warnings.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


void RShift::Draw()
{
    if (!MODE_DRAW_MATH_IS_DISABLED)
    {
        Draw(Chan::Math);
    }

    Draw(Chan::FirstDrawing());
    Draw(Chan::SecondDrawing());
}


void RShift::Draw(const Chan &ch)
{
    float x = (float)(Grid::Right() - Grid::Width() - TableAutoMeasures::GetDeltaGridLeft());

    if (ch.IsMath())
    {
        int rShift = RSHIFT_MATH;
        float scale = (float)Grid::Math::Height() / 960;
        float y = (Grid::Math::Top() + Grid::Math::Bottom()) / 2.0F - scale * (rShift - RShift::ZERO);
        Char(Symbol::S8::RSHIFT_NORMAL).Draw((int)(x - 9), (int)(y - 4), Color::FILL);
        Char('m').Draw((int)(x - 8), (int)(y - 5), Color::BACK);
        return;
    }
    if (!ch.IsEnabled())
    {
        return;
    }

    int rShift = RSHIFT(ch);

    float scale = Grid::Channel::Height() / (RShift::STEP() * 200.0F);

    float y = Grid::Channel::CenterHeight() - scale * (rShift - RShift::ZERO);

    if (rShift > RShift::ZERO)      // \todo Сильный говнокод
    {
        y += 1;
    }

    if (y > Grid::Channel::Bottom())
    {
        Char(Symbol::S8::RSHIFT_LOWER).Draw((int)(x - 7), Grid::Channel::Bottom() - 11, ch.GetColor());
        Point().Draw((int)(x - 5), Grid::Channel::Bottom() - 2);
        y = (float)(Grid::Channel::Bottom() - 7);
        x++;
    }
    else if (y < Grid::Top())
    {
        Char(Symbol::S8::RSHIFT_ABOVE).Draw((int)(x - 7), Grid::Top() + 2, ch.GetColor());
        Point().Draw((int)(x - 5), Grid::Top() + 2);
        y = Grid::Top() + 7.0f;
        x++;
    }
    else
    {
        Char(Symbol::S8::RSHIFT_NORMAL).Draw((int)(x - 8), (int)(y - 4), ch.GetColor());
        if ((show_line[ch]) && set.memory.mode_work.IsDirect())
        {
            DashedHLine(7, 6).Draw((int)(y), Grid::Left(), Grid::Right(), 0);
        }
    }

    Char(ch.IsA() ? '1' : '2').Draw((int)(x - 7), (int)(y - 9), TypeFont::S5, Color::BACK);
}


void TrigLev::Draw()
{
    if (TRIG_SOURCE_IS_EXT)
    {
        return;
    }

    int max = TrigLev::MAX;
    int min = TrigLev::MIN;
    int zero = TrigLev::ZERO;

    int level = TRIG_LEV(TRIG_SOURCE) + (RSHIFT(TRIG_SOURCE) - zero);
    float scale = 1.0F / ((max - min) / 2.4F / Grid::Channel::Height());

    int y = (int)(Grid::Channel::CenterHeight() - scale * (level - TrigLev::ZERO));

    int x = Grid::Right();
    Color::Trig().SetAsCurrent();

    if (y > Grid::Channel::Bottom())
    {
        Char(Symbol::S8::TRIG_LEV_LOWER).Draw(x + 2, Grid::Channel::Bottom() - 11);;
        Point().Draw(x + 4, Grid::Channel::Bottom() - 2);
        y = Grid::Channel::Bottom() - 7;
        x--;
    }
    else if (y < Grid::Top())
    {
        Char(Symbol::S8::TRIG_LEV_ABOVE).Draw(x + 2, Grid::Top() + 2);
        Point().Draw(x + 4, Grid::Top() + 2);
        y = Grid::Top() + 7;
        x--;
    }
    else
    {
        Char(Symbol::S8::TRIG_LEV_NORMAL).Draw(x, y - 4);
    }

    const char simbols[3] = { '1', '2', 'В' };

    Char(simbols[TRIG_SOURCE]).Draw(x + 4, y - 9, TypeFont::S5, Color::BACK);
}


void TrigLev::WriteValue()
{
    if (TRIG_SOURCE_IS_EXT)
    {
        return;
    }

    if (show_level && set.memory.mode_work.IsDirect())
    {
        float trigLev = RShift::ToAbs(TRIG_LEV_CURRENT, RANGE(TRIG_SOURCE));     // WARN Здесь для внешней
                                                                    // синхронизации неправильно рассчитывается уровень.
        int trigSource = TRIG_SOURCE;

        if (TRIG_INPUT_IS_AC && trigSource <= TrigSource::B)
        {
            int16 rShift = RSHIFT(trigSource);
            float rShiftAbs = RShift::ToAbs(rShift, RANGE(trigSource));
            trigLev += rShiftAbs;
        }

        Text text(LANG_RU ? "Ур синхр = " : "Trig lvl = ");
        text.Append(Voltage(trigLev).ToText());

        int width = 99;
        int x = (Grid::Width() - width) / 2 + Grid::Left();
        int y = Grid::Channel::Bottom() - (Grid::IsSeparate() ? 20 : 55);

        Rectangle(width, 13).Draw(x, y, Color::GRID);
        Region(width - 2, 11).Fill(x + 1, y + 1, Color::BACK);
        text.Draw(x + 3, y + 2, Color::FILL);
    }
}


void TShift::Draw()
{
    BitSet64 bs = SettingsDisplay::PointsOnDisplay();

    // Рисуем tShift
    int tshift = TPos::InPoints(ENUM_POINTS_FPGA, TPOS) - TSHIFT;

    if (Math::InRange(tshift, bs.first, bs.second))
    {
        int x = Grid::Left() + tshift - bs.first - 3;
        Char(Symbol::S8::TSHIFT_NORM_1).Draw2SymbolsInPosition(x, Grid::Top() - 1, Symbol::S8::TSHIFT_NORM_2, Color::BACK,
            Color::FILL);
    }
    else if (tshift < bs.first)
    {
        Char(Symbol::S8::TSHIFT_LEFT_1).Draw2SymbolsInPosition(Grid::Left() + 1, Grid::Top(), Symbol::S8::TSHIFT_LEFT_2,
            Color::BACK, Color::FILL);
        Line().Draw(Grid::Left() + 9, Grid::Top() + 1, Grid::Left() + 9, Grid::Top() + 7, Color::BACK);
    }
    else if (tshift > bs.second)
    {
        Char(Symbol::S8::TSHIFT_RIGHT_1).Draw2SymbolsInPosition(Grid::Right() - 8, Grid::Top(),
            Symbol::S8::TSHIFT_RIGHT_2, Color::BACK, Color::FILL);
        Line().Draw(Grid::Right() - 9, Grid::Top() + 1, Grid::Right() - 9, Grid::Top() + 7, Color::BACK);
    }
}


void TPos::Draw()
{
    int x = TPos::InPoints(ENUM_POINTS_FPGA, TPOS) - SHIFT_IN_MEMORY_IN_POINTS + Grid::Left();

    if (x >= Grid::Left() && x <= Grid::Right())
    {
        Draw(x, Grid::Top() - 1);
    }
}


void TPos::Draw(int x, int y)
{
    Region(6, 6).Fill(x - 3, y, Color::BACK);

    Char(Symbol::S8::TPOS_1).Draw(x - 3, y, Color::FILL);
}


Chan &Chan::FirstDrawing()
{
    return LAST_AFFECTED_CHANNEL_IS_A ? ChB : ChA;
}


Chan &Chan::SecondDrawing()
{
    return FirstDrawing().IsA() ? ChB : ChA;
}
