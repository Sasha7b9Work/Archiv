// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Data/DataSettings.h"
#include "Display/Display.h"
#include "Display/Warnings.h"
#include "FPGA/FPGA.h"
#include "FPGA/ReaderFPGA.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include <cstring>


uint TrigPolarity::timeSwitch = 0;


// Добавочные смещения по времени для разверёток режима рандомизатора.
static int16 deltaTShift[TBase::Count] = {505, 489, 464, 412, 258};


void FPGA::LoadSettings()
{
    TBase::Load();
    LaunchFPGA::Load();
    Range::Load(ChA);
    RShift::Load(ChA);
    Range::Load(ChB);
    RShift::Load(ChB);
    TrigLev::Load();
    TrigPolarity::Load();
    LoadRegUPR();
    TrigLev::Load();
    PageTime::OnChanged_PeakDet(true);
    TrigSource::Set(TRIG_SOURCE);

    /*
    switch(BALANCE_ADC_TYPE) 
    {
        case BalanceADCtype::Settings:
            WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)SET_BALANCE_ADC_A, false);
            WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)SET_BALANCE_ADC_B, false);
            break;
        case BalanceADCtype::Hand:
            SetPeackDetMode(PEAKDET);
            SetTBase(SET_TBASE);
            if (PEAKDET)
            {
                WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // Почему-то при пиковом детекторе смещение появляется. Вот его и компенсируем.
                WriteToHardware(WR_ADD_RSHIFT_DAC2, 3, false);
            }
            else
            {
                WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)BALANCE_ADC_A, false);
                WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)BALANCE_ADC_B, false);
            }
            break;
        case BalanceADCtype::Disable:
            break;
    }
    */
}


void Range::Load(const Chan &ch)
{
    BUS_FPGA::SetAttribChannelsAndTrig(TypeWriteAnalog::RangeA);
    RShift::Load(ch);
    if (ch == (Chan::E)TRIG_SOURCE)
    {
        TrigLev::Load();
    }
}


void TBase::Load()
{
    struct TBaseMaskStruct
    {
        uint8 maskNorm;      // Маска. Требуется для записи в аппаратную часть при выключенном режиме пикового детектора
        uint8 maskPeackDet;  // Маска. Требуется для записи в аппаратную часть при включенном режиме пикового детектора
    };

    static const TBaseMaskStruct masksTBase[TBase::Count] =
    {
        {BIN_U8(00000000), BIN_U8(00000000)}, // 1ns
        {BIN_U8(00000000), BIN_U8(00000000)}, // 2ns
        {BIN_U8(00000000), BIN_U8(00000000)}, // 5ns
        {BIN_U8(00000000), BIN_U8(00000000)}, // 10ns
        {BIN_U8(00000000), BIN_U8(00000000)}, // 20ns
        {BIN_U8(00000000), BIN_U8(00000000)}, // 50ns
        {BIN_U8(00100001), BIN_U8(00100001)}, // 100ns
        {BIN_U8(00100010), BIN_U8(00100010)}, // 200ns
        {BIN_U8(00100011), BIN_U8(00100010)}, // 500ns
        {BIN_U8(01000101), BIN_U8(00100011)}, // 1us
        {BIN_U8(01000110), BIN_U8(01000101)}, // 2us
        {BIN_U8(01000111), BIN_U8(01000110)}, // 5us
        {BIN_U8(01001001), BIN_U8(01000111)}, // 10us
        {BIN_U8(01001010), BIN_U8(01001001)}, // 20us
        {BIN_U8(01001011), BIN_U8(01001010)}, // 50us
        {BIN_U8(01001101), BIN_U8(01001011)}, // 100us
        {BIN_U8(01001110), BIN_U8(01001101)}, // 200us
        {BIN_U8(01001111), BIN_U8(01001110)}, // 500us
        {BIN_U8(01010001), BIN_U8(01001111)}, // 1ms
        {BIN_U8(01010010), BIN_U8(01010001)}, // 2ms
        {BIN_U8(01010011), BIN_U8(01010010)}, // 5ms
        {BIN_U8(01010101), BIN_U8(01010011)}, // 10ms
        {BIN_U8(01010110), BIN_U8(01010101)}, // 20ms
        {BIN_U8(01010111), BIN_U8(01010110)}, // 50ms
        {BIN_U8(01011001), BIN_U8(01010111)}, // 100ms
        {BIN_U8(01011010), BIN_U8(01011001)}, // 200ms
        {BIN_U8(01011011), BIN_U8(01011010)}, // 500ms
        {BIN_U8(01011101), BIN_U8(01011011)}, // 1s
        {BIN_U8(01011110), BIN_U8(01011101)}, // 2s
        {BIN_U8(01011111), BIN_U8(01011110)}, // 5s
        {BIN_U8(01111111), BIN_U8(01011111)}  // 10s
    };

    TBase::E base = TBASE;
    uint8 mask = !PEAK_DET_IS_ENABLED ? masksTBase[base].maskNorm : masksTBase[base].maskPeackDet;

    BUS_FPGA::Write(WR_RAZV, mask);
    setNRST.rand.shift_T0 = deltaTShift[base];
}


void RShift::Load(const Chan &ch)
{
    static const uint16 mask[2] = {0x2000, 0x6000};

    Range::E range = RANGE(ch);
    ModeCouple::E mode = MODE_COUPLE(ch);
    static const int index[3] = {0, 1, 1};
    int16 rShiftAdd = setNRST.chan[ch].AddRShift(range, index[mode]);

    uint16 rShift = (uint16)(RSHIFT(ch) + (ch.IsInversed() ? -1 : 1) * rShiftAdd);

    int16 delta = -(rShift - RShift::ZERO);
    if (ch.IsInversed())
    {
        delta = -delta;
    }
    rShift = (uint16)(delta + RShift::ZERO);

    rShift = (uint16)(RShift::MAX + RShift::MIN - rShift);

    BUS_FPGA::WriteToDAC
                        (ch.IsA() ? TypeWriteDAC::RShiftA : TypeWriteDAC::RShiftB, (uint16)(mask[ch] | (rShift << 2)));
}


void TrigLev::Set(TrigSource::E ch, int16 trigLev)
{
    Display::ChangedRShiftMarkers();
    if (trigLev < TrigLev::MIN || trigLev > TrigLev::MAX)
    {
        Warnings::ShowWarningBad("ПРЕДЕЛ РАЗВЕРТКА - УРОВЕНЬ", "LIMIT SWEEP - LEVEL");
    }

    Math::Limitation(&trigLev, TrigLev::MIN, TrigLev::MAX);

    if (trigLev > TrigLev::ZERO)
    {
        trigLev &= 0xfffe;
    }
    else if (trigLev < TrigLev::ZERO)
    {
        trigLev = (trigLev + 1) & 0xfffe;
    }

    if (TRIG_LEV(ch) != trigLev)
    {
        TRIG_LEV(ch) = trigLev;
        TrigLev::Load();
        Display::RotateTrigLev();
    }
};


void TrigLev::Set(const Chan &ch, int16 trigLev)
{
    Set(ch.IsA() ? TrigSource::A : TrigSource::B, trigLev);
}


void TrigLev::Load()
{
    uint16 data = 0xa000;
    uint16 trigLev = (uint16)TRIG_LEV_CURRENT;
    trigLev = (uint16)(TrigLev::MAX + TrigLev::MIN - trigLev);
    data |= trigLev << 2;
    BUS_FPGA::WriteToDAC(TypeWriteDAC::TrigLev, data);
}


void TShift::SetDelta(int16 shift)
{
    deltaTShift[TBASE] = shift;

    LaunchFPGA::Load();
}


String TShift::ToString(int shift)
{
    float abs = TShift::ToAbs(shift, TBASE);

    return Time(abs).ToText(true);
}


bool Range::Increase(const Chan &ch)
{
    bool result = false;

    if (RANGE(ch) < Range::Count - 1)
    {
        Range::Set(ch, (Range::E)(RANGE(ch) + 1));
        result = true;
    }
    else
    {
        Warnings::ShowWarningBadLimitRange(ch);
    }

    Display::Redraw();
 
    return result;
};


bool Range::Decrease(const Chan &ch)
{
    bool result = false;

    if (RANGE(ch) > 0)
    {
        Range::Set(ch, (Range::E)(RANGE(ch) - 1));
        result = true;
    }
    else
    {
        Warnings::ShowWarningBadLimitRange(ch);
    }

    Display::Redraw();

    return result;
};


void TrigPolarity::Set(E polarity)
{
    TRIG_POLARITY = polarity;
    TrigPolarity::Load();
}


void TrigPolarity::Switch()
{
    static const uint16 value[2][2] =
    {
        {1, 0},
        {0, 1}
    };

    HAL_FMC::Write(WR_TRIG, value[TRIG_POLARITY][0]);
    HAL_FMC::Write(WR_TRIG, value[TRIG_POLARITY][1]);

    timeSwitch = TIMER_MS;
}


void TrigPolarity::Load()
{
    BUS_FPGA::Write(WR_TRIG, TRIG_POLARITY_IS_FRONT ? 0U : 1U);
}


void FPGA::LoadRegUPR()
{
    uint16 data = 0;

    if (TBASE_IS_RAND)
    {
        _SET_BIT(data, 0);
    }

    const uint16 mask[3] =
    {
        (1 << UPR_BIT_CALIBRATOR_AC_DC),
        (1 << UPR_BIT_CALIBRATOR_VOLTAGE),
        (0)
    };

    data |= mask[CALIBRATOR_MODE];

    if (PEAK_DET_IS_ENABLED)
    {
        data |= (1 << UPR_BIT_PEAKDET);
    }

    BUS_FPGA::Write(WR_UPR, data);
}



void TrigLev::FindAndSet()
{
//    if (/*Storage::AllDatas() == 0 ||*/ TrigSource::IsExt())
//    {
//        return;
//    }
//
//    TrigSource::E trigSource = TrigSource::Get();
//
//    Chan::E chanTrig = (Chan::E)(trigSource);
//    uint8 *data0 = 0;
//    uint8 *data1 = 0;
//    DataSettings *ds_ = 0;
//
////    Storage::GetDataFromEnd(0, &ds_, &data0, &data1);
//
//    puchar data = (chanTrig == ChA) ? data0 : data1;
//
//    uint lastPoint = ds_->BytesInChannel() - 1;
//
//    uint8 min = Math::GetMinFromArray(data, 0, lastPoint);
//    uint8 max = Math::GetMaxFromArray(data, 0, lastPoint);
//
//    uint8 aveValue = (uint8)(((int)(min) + (int)(max)) / 2);
//
//    static const float scale = (float)(TrigLev::MAX - TrigLev::ZERO) / (float)(Value::MAX - Value::AVE) / 2.4F;
//
//    int16 trigLev = (int16)(TrigLev::ZERO + scale * ((int)(aveValue) - Value::AVE) -
//        (RSHIFT(chanTrig) - RShift::ZERO));
//
//    TrigLev::Set(trigSource, trigLev);
//
//    need_auto_find = false;
}
