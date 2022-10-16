// 2021/08/10 09:52:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "FPGA/FreqMeter.h"
#include "FPGA/TypesFPGA.h"


namespace FreqMeter
{
    static volatile bool readPeriod = false;    // Установленный в true флаг означает, что частоту нужно считать по счётчику периода.

    static BitSet32 freqSet;
    static BitSet32 periodSet;

    static float frequency = 0.0f;             // Частота, намеренная альтерой
    static float prevFreq = 0.0f;

    namespace Frequency
    {
        static float FromFrequencyCounter(const BitSet32 *fr) { return fr->word * 10.0F; }

        static float FromPeriodCounter(const BitSet32 *period)
        {
            if (period->word == 0) { return 0.0F; }

            return 10e6F / period->word;
        }

        static float FromFrequencySet(const BitSet32 *fr) { return (fr->word * 10.0f); }

        static float FromPeriodSet(const BitSet32 *period)
        {
            if (period->word == 0) { return 0.0f; }

            return (10e5f / (float)period->word);
        }
    }

    static BitSet32 ReadRegPeriod()
    {
        BitSet32 period;
        period.half_word0 = (uint8)HAL_FMC::Read(RD_PERIOD_LOW);
        period.half_word1 = (uint8)HAL_FMC::Read(RD_PERIOD_HI);
        return period;
    }

    static BitSet32 ReadRegFrequency()
    {
        BitSet32 fr;

        fr.half_word[0] = (uint8)HAL_FMC::Read(RD_FREQ_LOW);
        fr.half_word[1] = (uint8)HAL_FMC::Read(RD_FREQ_HI);

        return fr;
    }

    static void ReadFrequency()
    {
        freqSet.half_word0 = *RD_FREQ_LOW;
        freqSet.half_word1 = *RD_FREQ_HI;

        if (freqSet.word < 1000)
        {
            readPeriod = true;
        }
        else
        {
            float fr = Frequency::FromFrequencySet(&freqSet);

            if (fr < prevFreq * 0.9f || fr > prevFreq * 1.1f)
            {
                frequency = ERROR_VALUE_FLOAT;
            }
            else
            {
                frequency = fr;
            }
            prevFreq = fr;
        }
    }

    static void ReadPeriod()
    {
        periodSet.half_word0 = *RD_PERIOD_LOW;
        periodSet.half_word1 = *RD_PERIOD_HI;

        float fr = Frequency::FromPeriodSet(&periodSet);

        if (fr < prevFreq * 0.9f || fr > prevFreq * 1.1f)
        {
            frequency = ERROR_VALUE_FLOAT;
        }
        else
        {
            frequency = fr;
        }

        prevFreq = fr;
        readPeriod = false;
    }
}


void FreqMeter::Init()
{
    uint16 data = 0;

    _SET_BIT(data, 2);

    *WR_FREQ_METER_PARAMS = data;
}


void FreqMeter::Update(uint16 flag)
{
    bool freqReady = _GET_BIT(flag, FL_FREQ_READY) == 1;
    bool periodReady = _GET_BIT(flag, FL_PERIOD_READY) == 1;

    if (freqReady)
    {
        if (!readPeriod)
        {
            ReadFrequency();
        }
    }

    if (periodReady)
    {
        if (readPeriod)
        {
            ReadPeriod();
        }
    }
}

float FreqMeter::GetFrequency()
{
    return frequency;
}


float FreqMeter::CalculateFrequencyFromCounterFrequency()
{
    frequency = 0.0f;

    while (_GET_BIT(HAL_FMC::Read(RD_FL), FL_FREQ_READY) == 0) {
    };

    ReadRegFrequency();

    while (_GET_BIT(HAL_FMC::Read(RD_FL), FL_FREQ_READY) == 0) {
    };

    BitSet32 fr = ReadRegFrequency();

    if (fr.word >= 5)
    {
        frequency = Frequency::FromFrequencyCounter(&fr);
    }

    return 0.0F;
}


float FreqMeter::CalculateFrequencyFromCounterPeriod()
{
    frequency = 0.0f;

    uint time = TIMER_MS;

    while (TIMER_MS - time < 1000 && _GET_BIT(HAL_FMC::Read(RD_FL), FL_PERIOD_READY) == 0) {
    };

    ReadRegPeriod();

    time = TIMER_MS;

    while (TIMER_MS - time < 1000 && _GET_BIT(HAL_FMC::Read(RD_FL), FL_PERIOD_READY) == 0) {
    };

    BitSet32 period = ReadRegPeriod();

    if (period.word > 0 && (TIMER_MS - time < 1000))
    {
        frequency = Frequency::FromPeriodCounter(&period);
    }

    return frequency;
}
