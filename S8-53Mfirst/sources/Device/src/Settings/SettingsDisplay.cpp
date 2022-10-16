// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA.h"


int ENumSignalsInSec::NumSignalsInS(E value)
{
    static const int fps[Count] =
    {
        25, 10, 5, 2, 1
    };

    return fps[value];
}


uint ENumSignalsInSec::DeltaTime(E value)
{
    return 1000U / NumSignalsInS(value);
}


uint Smoothing::NumPoints(E value)
{
    if (SMOOTHING_IS_DISABLED)
    {
        return 0;
    }
    return (uint)value + 1;          // WARN Здесь очень коряво реализовано
}


BitSet64 SettingsDisplay::PointsOnDisplay()
{
    BitSet64 result;

    int num_points_on_screen = 281;

    result.first = SHIFT_IN_MEMORY_IN_POINTS;
    result.second = SHIFT_IN_MEMORY_IN_POINTS + num_points_on_screen;

    return result;
}


BitSet64 SettingsDisplay::BytesOnDisplay()
{
    BitSet64 result = PointsOnDisplay();

    if (PEAK_DET_IS_ENABLED)
    {
        result.first *= 2;
        result.second *= 2;
    }

    return result;
}


int MenuAutoHide::Time(E value)
{
    static const int times[] =
    {
        0, 5, 10, 15, 30, 60
    };
    return times[value] * 1000;
}


ModeAveraging::E ModeAveraging::Current()
{
    if (TBASE_IS_RAND)
    {
        return ModeAveraging::Around;
    }

    return MODE_AVERAGE;
}


int ENumAveraging::NumAverages(E value)
{
    if (TBASE_IS_RAND && (setNRST.rand.num_ave >= ENumAveraging::ToAbs(value)))
    {
        return setNRST.rand.num_ave;
    }

    return ENumAveraging::ToAbs(value);
}


int ENumAveraging::ToAbs(E value)
{
    return (1 << value);
}


int ENumAveraging::ToDisplayValue(E value)
{
    return (value == _1) ? 0 : ToAbs(value);
}


int ENumAccumulation::ToDisplayValue(E value)
{
    if (value == _1)            { return 0;  }
    else if (value == Infinity) { return -1; }

    return (1 << value);
}


int ENumMinMax::ToDisplayValue(E value)
{
    return (value == _1) ? 0 : ToAbs(value);
}
