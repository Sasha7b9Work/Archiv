// 2021/03/15 13:29:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/DataSettings.h"
#include "FPGA/FPGA.h"


void DataSettings::Fill()
{
    enabled[Chan::A] = ChA.IsEnabled();
    enabled[Chan::B] = ChB.IsEnabled();
    r_shift_a = (uint)RSHIFT_A;
    r_shift_b = (uint)RSHIFT_B;
    trig_lev_a = (uint)TRIG_LEV_A;
    trig_lev_b = (uint)TRIG_LEV_B;
    couple_a = MODE_COUPLE_A;
    couple_b = MODE_COUPLE_B;
    tbase = TBASE;
    tshift = TSHIFT;
    range[0] = RANGE_A;
    range[1] = RANGE_B;
    enabled_peak_det = PEAK_DET_IS_ENABLED;
    multiplier_a = DIVIDER_A;
    multiplier_b = DIVIDER_B;
    enum_points = ENUM_POINTS_FPGA;
    time = HAL_RTC::GetPackedTime();
}


int DataSettings::BytesInData() const
{
    int result = 0;

    int bytes_in_channel = BytesInChannel();

    if (enabled[Chan::A])
    {
        result += bytes_in_channel;
    }

    if (enabled[Chan::B])
    {
        result += bytes_in_channel;
    }

    return result;
}
