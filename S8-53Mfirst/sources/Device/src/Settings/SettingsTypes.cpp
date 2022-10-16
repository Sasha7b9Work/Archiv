// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/Timer_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include "Display/Warnings.h"
#include "Display/Screen/Grid.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include <climits>


int TShift::add_rand = 0;

bool RShift::show_line[Chan::Count] = { false, false };

bool TrigLev::need_auto_find = false;
bool TrigLev::show_level = false;

TBase::E TBase::MIN_P2P = TBase::_20ms;
TBase::E TBase::MIN_PEAK_DET = TBase::_1us;
TBase::E TBase::MAX_RANDOMIZE = TBase::_50ns;

Chan ChA(Chan::A);
Chan ChB(Chan::B);


const float RShift::abs_step[Range::Count] =
{
    2e-3F   / 20 / RShift::STEP(),
    5e-3F   / 20 / RShift::STEP(),
    10e-3F  / 20 / RShift::STEP(),
    20e-3F  / 20 / RShift::STEP(),
    50e-3F  / 20 / RShift::STEP(),
    100e-3F / 20 / RShift::STEP(),
    200e-3F / 20 / RShift::STEP(),
    500e-3F / 20 / RShift::STEP(),
    1.0F    / 20 / RShift::STEP(),
    2.0F    / 20 / RShift::STEP(),
    5.0F    / 20 / RShift::STEP(),
    10.0F   / 20 / RShift::STEP(),
    20.0F   / 20 / RShift::STEP()
};


const float TShift::abs_step[TBase::Count] =
{
    1e-9F / 20, 2e-9F / 20, 5e-9F / 20, 10e-9F / 20, 20e-9F / 20, 50e-9F / 20, 100e-9F / 20, 200e-9F / 20, 500e-9F / 20,
    1e-6F / 20, 2e-6F / 20, 5e-6F / 20, 10e-6F / 20, 20e-6F / 20, 50e-6F / 20, 100e-6F / 20, 200e-6F / 20, 500e-6F / 20,
    1e-3F / 20, 2e-3F / 20, 5e-3F / 20, 10e-3F / 20, 20e-3F / 20, 50e-3F / 20, 100e-3F / 20, 200e-3F / 20, 500e-3F / 20,
    1.0F  / 20, 2.0F  / 20, 5.0F  / 20, 10.0F  / 20
};


int Divider::ToAbs(const Chan &ch)
{
    return ToAbs(DIVIDER(ch));
}


pchar Range::Name(Range::E range)
{
    static pchar names[Range::Count + 1] =
    {
        "2mV",
        "5mV",
        "10mV",
        "20mV",
        "50mV",
        "100mV",
        "200mV",
        "500mV",
        "1V",
        "2V",
        "5V",
        "10V",
        "20V",
        "Range::Count"
    };
    return names[range];
}


pchar Range::ToString(Range::E range, Divider::E multiplier)
{
    // Структура для описания диапазона масштаба по напряжению.
    struct RangeStruct
    {
        pchar name[2][2];     // Название диапазона в текстовом виде, пригодном для вывода на экран.
    };

    // Массив структур описаний масштабов по напряжению.
    static const RangeStruct ranges[Range::Count] =
    {
        {{{"2\x10мВ",     "20\x10мВ"}, {"2\x10mV",  "20\x10mV"}}},
        {{{"5\x10мВ",     "50\x10мВ"}, {"5\x10mV",  "50\x10mV"}}},
        {{{"10\x10мВ",    "0.1\x10В"}, {"10\x10mV", "0.1\x10V"}}},
        {{{"20\x10мВ",    "0.2\x10В"}, {"20\x10mV", "0.2\x10V"}}},
        {{{"50\x10мВ",    "0.5\x10В"}, {"50\x10mV", "0.5\x10V"}}},
        {{{"0.1\x10В",    "1\x10В"},   {"0.1\x10V", "1\x10V"}}},
        {{{"0.2\x10В",    "2\x10В"},   {"0.2\x10V", "2\x10V"}}},
        {{{"0.5\x10В",    "5\x10В"},   {"0.5\x10V", "5\x10V"}}},
        {{{"1\x10В",      "10\x10В"},  {"1\x10V",   "10\x10V"}}},
        {{{"2\x10В",      "20\x10В"},  {"2\x10V",   "20\x10V"}}},
        {{{"5\x10В",      "50\x10В"},  {"5\x10V",   "50\x10V"}}},
        {{{"10\x10В",     "100\x10В"}, {"10\x10V",  "100\x10V"}}},
        {{{"20\x10В",     "200\x10В"}, {"20\x10V",  "200\x10V"}}}
    };

    return ranges[range].name[LANG][multiplier];
}


pchar TBase::Name(TBase::E tBase)
{
    static pchar names[TBase::Count + 1] =
    {
        "1ns"
        "2ns",
        "5ns",
        "10ns",
        "20ns",
        "50ns",
        "100ns",
        "200ns",
        "500ns",
        "1us",
        "2us",
        "5us",
        "10us",
        "20us",
        "50us",
        "100us",
        "200us",
        "500us",
        "1ms",
        "2ms",
        "5ms",
        "10ms",
        "20ms",
        "50ms",
        "100ms",
        "200ms",
        "500ms",
        "1s",
        "2s",
        "5s",
        "10s",
        "TBase::Count"
    };

    return names[tBase];
}


pchar TBase::ToString(E tbase)
{
    struct TBaseStruct
    {
        pchar name[2];
    };

    static const TBaseStruct tbases[TBase::Count] =
    {
        {"1\x10нс",     "1\x10ns"},
        {"2\x10нс",     "2\x10ns"},
        {"5\x10нс",     "5\x10ns"},
        {"10\x10нс",    "10\x10ns"},
        {"20\x10нс",    "20\x10ns"},
        {"50\x10нс",    "50\x10ns"},
        {"0.1\x10мкс",  "0.1\x10us"},
        {"0.2\x10мкс",  "0.2\x10us"},
        {"0.5\x10мкс",  "0.5\x10us"},
        {"1\x10мкс",    "1\x10us"},
        {"2\x10мкс",    "2\x10us"},
        {"5\x10мкс",    "5\x10us"},
        {"10\x10мкс",   "10\x10us"},
        {"20\x10мкс",   "20\x10us"},
        {"50\x10мкс",   "50\x10us"},
        {"0.1\x10мс",   "0.1\x10ms"},
        {"0.2\x10мс",   "0.2\x10ms"},
        {"0.5\x10мс",   "0.5\x10ms"},
        {"1\x10мс",     "1\x10ms"},
        {"2\x10мс",     "2\x10ms"},
        {"5\x10мс",     "5\x10ms"},
        {"10\x10мс",    "10\x10ms"},
        {"20\x10мс",    "20\x10ms"},
        {"50\x10мс",    "50\x10ms"},
        {"0.1\x10с",    "0.1\x10s"},
        {"0.2\x10с",    "0.2\x10s"},
        {"0.5\x10с",    "0.5\x10s"},
        {"1\x10с",      "1\x10s"},
        {"2\x10с",      "2\x10s"},
        {"5\x10с",      "5\x10s"},
        {"10\x10с",     "10\x10s"}
    };

    return tbases[tbase].name[LANG];
}


pchar TBase::ToStringEN(TBase::E tbase)
{
    Language::E lang = LANG;

    LANG = Language::English;

    pchar result = ToString(tbase);

    LANG = lang;

    return result;
}


void RShift::Set(const Chan &ch, int16 rShift)
{
    if (!ch.IsEnabled())
    {
        return;
    }

    Display::ChangedRShiftMarkers();

    if (rShift > RShift::MAX || rShift < RShift::MIN)
    {
        Warnings::ShowWarningBadLimitRShift(ch);
    }

    Math::Limitation(&rShift, RShift::MIN, RShift::MAX);

    if (rShift > RShift::ZERO)
    {
        rShift &= 0xfffe;                                   // Делаем кратным двум, т.к. у нас 800 значений на 400 точек
    }
    else if (rShift < RShift::ZERO)
    {
        rShift = (rShift + 1) & 0xfffe;
    }

    RSHIFT(ch) = rShift;

    RShift::Load(ch);

    OnChanged(ch);
};


void TBase::Decrease()
{
    if (PEAK_DET_IS_ENABLED && (TBASE <= TBase::MIN_PEAK_DET))
    {
        Warnings::ShowWarningBad("ПРЕДЕЛ РАЗВЕРТКА - ВРЕМЯ/ДЕЛ", "LIMIT SWEEP - TIME/DIV");
        Warnings::ShowWarningBad("ВКЛЮЧЕН ПИКОВЫЙ ДЕТЕКТОР", "PEAK DETECTOR IS ENABLED");
        return;
    }

    if (TBASE > 0)
    {
        if (PageTime::InSelfRecoredMode() && (TBASE == TBase::MIN_P2P))
        {
            Warnings::ShowWarningBad("САМОПИСЕЦ НЕ МОЖЕТ РАБОТАТЬ НА БЫСТРЫХ РАЗВЁРТКАХ",
                                     "SELF-RECORDER DOES NOT WORK FAST SCANS");
        }
        else
        {
            E base = (E)(TBASE - 1);
            Set(base);
        }
    }
    else
    {
        Warnings::ShowWarningBad("ПРЕДЕЛ РАЗВЕРТКА - ВРЕМЯ/ДЕЛ", "LIMIT SWEEP - TIME/DIV");
    }
}


void TBase::Increase()
{
    if (TBASE < (TBase::Count - 1))
    {
        E base = (E)(TBASE + 1);
        Set(base);
    }
    else
    {
        Warnings::ShowWarningBad("ПРЕДЕЛ РАЗВЕРТКА - ВРЕМЯ/ДЕЛ", "LIMIT SWEEP - TIME/DIV");
    }
}


void TBase::Set(TBase::E tBase)
{
    if (!ChA.IsEnabled() && !ChB.IsEnabled())
    {
        return;
    }
    if (tBase < TBase::Count)
    {
        float tShiftAbsOld = TShift::ToAbs(TSHIFT, TBASE);
        TBASE = tBase;
        Load();
        TShift::Set((int)(TShift::ToRel(tShiftAbsOld, TBASE)));
        Display::Redraw();
    }
    else
    {
        Warnings::ShowWarningBad("ПРЕДЕЛ РАЗВЕРТКА - ВРЕМЯ/ДЕЛ", "LIMIT SWEEP - TIME/DIV");
    }
};


void TShift::Set(int tShift)
{
    if (!ChA.IsEnabled() && !ChB.IsEnabled())
    {
        return;
    }

    if (tShift < TShift::Min() || tShift > TShift::MAX)
    {
        Math::Limitation<int>(&tShift, TShift::Min(), TShift::MAX);

        Warnings::ShowWarningBad("ПРЕДЕЛ РАЗВЕРТКА - \x97", "LIMIT SWEEP - \x97");
    }

    TSHIFT = (int16)tShift;

    LaunchFPGA::Load();

    Display::Redraw();
};


bool Chan::IsEnabled() const
{
    if (value == Chan::Math)
    {
        return !MODE_DRAW_MATH_IS_DISABLED;
    }
    else if (value == Chan::A_B)
    {
        return false;
    }

    return ENABLED(value);
}


bool Chan::IsInversed() const
{
    return INVERSED(value);
}


int Chan::ToNumber() const
{
    return (value == A) ? 1 : 2;
}


int TPos::InPoints(EnumPointsFPGA::E enum_points, TPos::E t_pos)
{
    static const int div[TPos::Count] = { INT_MAX, 2, 1 };   // INT_MAX выбрано, потому что при делении на него
        // любого допустимого enum_points получается нольkei

    return (int)EnumPointsFPGA::ToPoints(enum_points) / div[t_pos];
}


int16 TShift::Min()
{
    int16 result = -(int16)EnumPointsFPGA::PointsInChannel();

    switch (TPOS)
    {
    case TPos::Left:                    break;
    case TPos::Center:  result /= 2;    break;
    case TPos::Right:   result = 0;     break;
    case TPos::Count:                   break;
    }

    return result;
}


int EnumPointsFPGA::PointsInChannel()
{
    return EnumPointsFPGA::ToPoints(ENUM_POINTS_FPGA);
}


int EnumPointsFPGA::BytesInChannel()
{
    int result = PointsInChannel();

    if (PEAK_DET_IS_ENABLED)
    {
        result *= 2;
    }

    return result;
}


EnumPointsFPGA::E EnumPointsFPGA::FromPoints(uint num_points)
{
    struct Struct
    {
        uint num_points;
        E enum_points;
    };

    static const Struct structs[Count] =
    {
        { 512,  _1k},
        { 281,  _512 },
        { 1,    _281}
    };

    for (int i = 0; i < Count; i++)
    {
        if (num_points > structs[i].num_points)
        {
            return structs[i].enum_points;
        }
    }

    return EnumPointsFPGA::_1k;
}


float MaxDbFFT::GetReal(E value)
{
    static const float db[] = { -40.0F, -60.0F, -80.0F };
    return db[value];
}


int16 RShift::STEP()
{
    return (int16)(((RShift::MAX - RShift::MIN) / 24) / Grid::Delta());
}


Time TShift::ToAbs(float shift, TBase::E base)
{
    return abs_step[base] * shift;
}


Time TShift::ToAbs(int shift, TBase::E base)
{
    return ToAbs((float)shift, base);
}


float TShift::ToRel(float shift, TBase::E base)
{
    return shift / abs_step[base];
}


int TBase::StepRand()
{
#define N_KR 100
    static const int steps[] = { N_KR / 1, N_KR / 2, N_KR / 5, N_KR / 10, N_KR / 20, N_KR / 50 };

    return TBASE_IS_RAND ? steps[TBASE] : 1;
}


void RShift::OnChanged(const Chan &ch)
{
    LAST_AFFECTED_CHANNEL = ch.value;

    if (TIME_SHOW_LEVELS)
    {
        show_line[ch] = true;

        Timer::Enable(ch.IsA() ? TypeTimer::ShowLevelRShiftA : TypeTimer::ShowLevelRShiftB,
            TIME_SHOW_LEVELS * 1000, ch.IsA() ? FuncOnTimerDisableShowA :
            FuncOnTimerDisableShowB);
    };

    Display::Redraw();
};


void RShift::FuncOnTimerDisableShowA()
{
    DisableShowLine(ChA);
}


void RShift::FuncOnTimerDisableShowB()
{
    DisableShowLine(ChB);
}


void RShift::DisableShowLine(const Chan &ch)
{
    static const TypeTimer::E types[Chan::Count] = { TypeTimer::ShowLevelRShiftA, TypeTimer::ShowLevelRShiftB };

    show_line[ch] = false;

    Timer::Disable(types[ch]);
}


int TShift::ForLaunchFPGA()
{
    return TSHIFT - TShift::Min();
}


void TrigSource::Set(E source)
{
    TRIG_SOURCE = source;

    BUS_FPGA::SetAttribChannelsAndTrig(TypeWriteAnalog::TrigParam);

    if (!TRIG_SOURCE_IS_EXT)
    {
        TrigLev::Set(TRIG_SOURCE, TRIG_LEV_CURRENT);
    }
}


void TrigSource::Set(const Chan &ch)
{
    Set(ch.IsA() ? TrigSource::A : TrigSource::B);
}


void ModeCouple::Set(const Chan &ch, ModeCouple::E mode)
{
    MODE_COUPLE(ch) = mode;

    BUS_FPGA::SetAttribChannelsAndTrig(ch.IsA() ? TypeWriteAnalog::ChanParamA : TypeWriteAnalog::ChanParamB);

    RShift::Set(ch, RSHIFT(ch));
}


void Range::Set(const Chan &ch, Range::E range)
{
    if (!ch.IsEnabled())
    {
        return;
    }
    if (range < Range::Count)
    {
        RANGE(ch) = range;

        if (LINKING_RSHIFT_IS_VOLTAGE)
        {
            float rShiftAbs = RShift::ToAbs(RSHIFT(ch), RANGE(ch));
            float trigLevAbs = RShift::ToAbs(TRIG_LEV(ch), RANGE(ch));
            RSHIFT(ch) = (int16)RShift::ToRel(rShiftAbs, range);
            TRIG_LEV((TrigSource::E)ch.value) = (int16)RShift::ToRel(trigLevAbs, range);
        }

        Range::Load(ch);
    }
    else
    {
        Warnings::ShowWarningBadLimitRange(ch);
    }
}


void TrigInput::Set(TrigInput::E input)
{
    TRIG_INPUT = input;

    BUS_FPGA::SetAttribChannelsAndTrig(TypeWriteAnalog::TrigParam);
}
