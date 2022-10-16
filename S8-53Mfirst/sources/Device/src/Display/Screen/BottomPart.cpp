// 2021/03/24 15:17:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Hardware/VCP/VCP_.h"
#include "Display/Display.h"
#include "Display/Screen/BottomPart.h"
#include "Display/Screen/Grid.h"
#include "Hardware/FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/FreqMeter.h"
#include "Hardware/LAN/LAN.h"
#include <cstdio>
#include <cstring>


namespace BottomPart
{
    static void WriteTextVoltage(const Chan &ch, int x, int y);

    // Если number == 0 - выводится прочерк, если number == -1 - выводится знак бесконечности
    static void WriteStringAndNumber(pchar text, int x, int y, int number);

    // Вывести текущее время.
    static void DrawTime(int x, int y);
}


void BottomPart::Draw()
{
    int y0 = Display::HEIGHT - 19;
    int y1 = Display::HEIGHT - 10;
    int x = -1;

    VLine vline(19);

    const int y = Grid::Bottom() + 1;

    vline.Draw(95, y, Color::FILL);
    vline.Draw(176, y);
    vline.Draw(221, y);
    vline.Draw(276, y);

    WriteTextVoltage(ChA, x + 1, y0);

    WriteTextVoltage(ChB, x + 1, y1);

    x += 98;

    if (!set.memory.mode_work.IsDirect())
    {
        //        DataSettings *ds = MODE_WORK_IS_LATEST ? Storage::dsLast : Storage::dsInt;
        //
        //        if (ds != 0)
        //        {
        //            tBase = ds->tBase;
        //            tShift = ds->tShift;
        //        }
    }

    Text("р\xa5%s", TBase::ToString(TBASE)).Draw(x, y0);

    Text("\xa5%s", TShift::ToString(TSHIFT).c_str()).Draw(x + 36, y0);

    if (set.memory.mode_work.IsDirect())
    {
        pchar source[3] = { "1", "2", "\x82" };

        Text("с\xa5\x10%s", source[TRIG_SOURCE]).Draw(x, y1, Color::Trig());

        static pchar couple[] =
        {
            "\x92",
            "\x91",
            "\x92",
            "\x92"
        };

        static pchar polar[] =
        {
            "\xa7",
            "\xa6"
        };

        static pchar filtr[] =
        {
            "\xb5\xb6",
            "\xb5\xb6",
            "\xb3\xb4",
            "\xb1\xb2"
        };

        Text("\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[TRIG_INPUT],
            polar[TRIG_POLARITY]).Draw(x + 18, y1);

        Char(filtr[TRIG_INPUT][0]).Draw(x + 45, y1);
        Char(filtr[TRIG_INPUT][1]).Draw(x + 53, y1);
    }

    if (set.memory.mode_work.IsDirect())
    {
        const char mode[] =
        {
            '\xb7',
            '\xa0',
            '\xb0'
        };

        Text("\xa5\x10%c", mode[START_MODE]).Draw(x + 63, y1);
    }

    x += 82;
    y0 = y0 - 3;
    y1 = y1 - 6;

    if (set.memory.mode_work.IsDirect())
    {
        int y2 = y1 + 6;
        WriteStringAndNumber(LANG_RU ? "накопл"    : "accum", x, y0,     ENumAccumulation::ToDisplayValue(ENUM_ACCUM));
        WriteStringAndNumber(LANG_RU ? "усредн"    : "ave", x, y1,       ENumAveraging::ToDisplayValue(ENUM_AVERAGE));
        WriteStringAndNumber(LANG_RU ? "мн\x93мкс" : "mn\x93max", x, y2, ENumMinMax::ToDisplayValue(ENUM_MIN_MAX));
    }

    x += 42;

    if (set.memory.mode_work.IsDirect())
    {
        Text mes_freq("\x7c=");

        float freq = FreqMeter::GetFrequency();

        mes_freq.Append(freq == 1.0f ? "******" : Frequency(freq).ToText().c_str()); //-V550

        mes_freq.Draw(x + 3, Grid::Bottom() + 2);
    }

    DrawTime(x + 3, Grid::Bottom() + 11);

    // Флешка
    if (FDrive::IsConnected())
    {
        Char(Symbol::UGO2::FLASH_DRIVE).Draw4SymbolsInRect(x + 57, Grid::Bottom() + 2, TypeFont::UGO2);
    }

    // Ethernet
    if ((LAN::clientIsConnected || LAN::cableIsConnected) && TIMER_MS > 2000)
    {
        Char(Symbol::UGO2::ETHERNET).Draw4SymbolsInRect(x + 87, Grid::Bottom() + 2, TypeFont::UGO2,
            LAN::clientIsConnected ? Color::FILL : Color::BLACK_WHITE_1);
    }

    if (VCP::connectToHost || VCP::cableIsConnected)
    {
        Char(Symbol::UGO2::USB).Draw4SymbolsInRect(x + 72, Grid::Bottom() + 2, TypeFont::UGO2, VCP::connectToHost ?
            Color::FILL : Color::BLACK_WHITE_1);
    }

    Color::FILL.SetAsCurrent();

    if (PEAK_DET_IS_ENABLED)
    {
        Char(Symbol::UGO2::PEAK_DET_1).Draw(x + 38, Grid::Bottom() + 11, TypeFont::UGO2);
        Char(Symbol::UGO2::PEAK_DET_2).Draw(x + 46, Grid::Bottom() + 11, TypeFont::UGO2);
    }

    if (set.memory.mode_work.IsDirect())
    {
        WriteStringAndNumber(LANG_RU ? "СГЛАЖ.:" : "SMOOTH", x + 57, Grid::Bottom() + 10, (int)Smoothing::NumPoints(SMOOTHING));
    }
}


void BottomPart::WriteTextVoltage(const Chan &ch, int x, int y)
{
    bool inverse = ch.IsInversed();
    ModeCouple::E mode_couple = MODE_COUPLE(ch);
    Divider::E multiplier = DIVIDER(ch);
    Range::E range = RANGE(ch);
    uint rShift = (uint)RSHIFT(ch);
    bool enable = ch.IsEnabled();

    if (!set.memory.mode_work.IsDirect())
    {
        //        DataSettings *ds = MODE_WORK_IS_DIRECT ? Storage::set : Storage::dsInt;
        //
        //        if (ds != 0)
        //        {
        //            inverse = ch.IsA() ? ds->inverseCh0 : ds->inverseCh1;
        //            modeCouple = ch.IsA() ? ds->modeCouple0 : ds->modeCouple1;
        //            multiplier = ch.IsA() ? ds->multiplier0 : ds->multiplier1;
        //            range = ds->range[ch];
        //            rShift = ch.IsA() ? ds->rShiftCh0 : ds->rShiftCh1;
        //            enable = ch.IsA() ? ds->enableCh0 : ds->enableCh1;
        //        }
    }

    if (enable)
    {
        Color color = ch.GetColor();
        Color colorDraw = inverse ? Color::WHITE : color;

        if (inverse)
        {
            const int widthField = 91;
            const int heightField = 8;
            Region(widthField, heightField).Fill(x, y, color);
        }

        static const char couple[] =
        {
            Symbol::S8::COUPLE_DC,
            Symbol::S8::COUPLE_AC,
            Symbol::S8::COUPLE_GND
        };

        Text("%s\xa5%c\xa5%s", ch.IsA() ? (LANG_RU ? "1к" : "1c") : (LANG_RU ? "2к" : "2c"), couple[mode_couple],
            Range::ToString(range, multiplier)).Draw(x + 1, y, colorDraw);

        Voltage shift = RShift::ToAbs((int16)rShift, range, multiplier);

        Text("\xa5%s", shift.ToText().c_str()).Draw(x + 46, y);
    }
}


void BottomPart::WriteStringAndNumber(pchar text, int x, int y, int number)
{
    Font::Set(TypeFont::S5);

    Text(text).Draw(x, y, Color::FILL);

    Text buffer;

    if (number == -1)
    {
        int dx = 29;
        Text(Symbol::S5::INVINITY_1).Draw(x + dx, y);
        Text(Symbol::S5::INVINITY_2).Draw(x + dx + 8, y);
    }
    else
    {
        if (number == 0)
        {
            buffer.Append("-");
        }
        else
        {
            buffer.Append(Int(number).ToText(false));
        }

        buffer.DrawRelativelyRight(x + 41, y);
    }

    Font::Set(TypeFont::S8);
}


void BottomPart::DrawTime(int x, int y)
{
//    const int dField = 13;
//    int dSeparator = -1;

    PackedTime time = HAL_RTC::GetPackedTime();

    Color::FILL.SetAsCurrent();

    //    if (MODE_WORK_IS_MEMINT || MODE_WORK_IS_LATEST)
    //    {
    //        DataSettings *ds = MODE_WORK_IS_MEMINT ? Storage::dsInt : Storage::dsLast;
    //
    //        if (ds != 0)
    //        {
    //            y -= 9;
    //            time.day = ds->time.day;
    //            time.hours = ds->time.hours;
    //            time.minutes = ds->time.minutes;
    //            time.seconds = ds->time.seconds;
    //            time.month = ds->time.month;
    //            time.year = ds->time.year;
    //            GF::Int2String((int)time.day, false, 2).Draw(x, y);
    //            Text(":").Draw(x + dField, y);
    //            GF::Int2String((int)time.month, false, 2).Draw(x + dField + dSeparator, y);
    //            Text(":").Draw(x + 2 * dField + dSeparator, y);
    //            GF::Int2String((int)time.year + 2000, false, 4).Draw(x + 2 * dField + 2 * dSeparator, y);
    //            y += 9;
    //        }
    //        else
    //        {
    //            return;
    //        }
    //    }


    x -= 1;
    Int((int)time.hours).ToText(false, 2).Draw(x, y);
    Text(":").Draw(x + 10, y);
    Int((int)time.minutes).ToText(false, 2).Draw(x + 12, y);
    Text(":").Draw(x + 22, y);
    Int((int)time.seconds).ToText(false, 2).Draw(x + 24, y);
}
