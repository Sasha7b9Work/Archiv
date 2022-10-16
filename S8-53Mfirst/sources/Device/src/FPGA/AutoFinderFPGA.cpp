// 2021/03/17 8:01:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Display/Display.h"
#include "Display/Warnings.h"
#include "FPGA/FPGA.h"
#include "FPGA/FreqMeter.h"
#include "FPGA/AutoFinderFPGA.h"
#include "Menu/Pages/Definition.h"


namespace AutoFinderFPGA
{
    static uint8 CalculateMinWithout0(const uint8 buffer[100]);

    static uint8 CalculateMaxWithout255(const uint8 buffer[100]);

    static TBase::E CalculateTBase(float freq_);

    bool auto_find_in_progress = false;
}


void AutoFinderFPGA::Find()
{
    if (!FindWave(Chan::A))
    {
        if (!FindWave(Chan::B))
        {
            Warnings::ShowWarningBad("������ �� ������", "SIGNAL IS NOT FOUND");
        }
    }

    FPGA::Init();
    FPGA::Start();

    auto_find_in_progress = true;
}


bool AutoFinderFPGA::FindWave(const Chan &ch)
{
    Settings settings = set;    // ��������� ���������� ���������

    FPGA::Stop();
    ch.Enable();
    TrigSource::Set((TrigSource::E)ch.value);
    TrigLev::Set((TrigSource::E)(ch.value), TrigLev::ZERO);
    RShift::Set(ch, RShift::ZERO);
    ModeCouple::Set(ch, ModeCouple::AC);
    Range::E range = AccurateFindRange(ch);

    if (range != Range::Count)
    {
        RANGE(ch) = range;
        TBase::E tBase = AccurateFindTBase(ch);

        if (tBase != TBase::Count)
        {
            TBase::Set(tBase);
            TRIG_SOURCE = (TrigSource::E)(ch.value);
            return true;
        }
    }

    set = settings;
    FPGA::LoadSettings();
    return false;
}


Range::E AutoFinderFPGA::AccurateFindRange(const Chan &ch)
{
    /*
    �������� ������.
    ������������� �������� 50 ��.
    ������������� �������� ����.
    �������� ������� ��������.
    1. ������������� �������� 20�/���.
    �� ��������� ��, �������� ��������� ����� � ���������� ������ � ��������� �� 50 ���� - 2 ������ � ���������. �� ���
        ����� �� ������ �������� ���� ����� ������ ������� ������� 10 ��.
    ��������� ��������� ���������� �� ����� �� ������� ������. ���� ����� - ���� ���������� �������� (��������, 2�
        ������ 1�) � ���������� � � �������� ����������.
    ���� �� ����� - ���� ��������� �������� � ��������� ���� � 1.
    */

    uint8 buffer[100];  // ���� ����� ��������� �����

    TBase::Set(TBase::_50ms);
    ModeCouple::Set(ch, ModeCouple::AC);
    PeakDetMode::E peakDetMode = MODE_PEAK_DET;
    PageTime::SetPeakDetMode(PeakDetMode::Enable);

    for (int range = Range::Count - 1; range >= 0; range--)
    {
        //Timer::LogPointMS("1");
        FPGA::Stop();
        Range::Set(ch, (Range::E)range);
        HAL_TIM2::DelayMS(10);
        FPGA::Start();

        for (int i = 0; i < 50; i++)
        {
            while (_GET_BIT(HAL_FMC::Read(RD_FL), FL_POINT_READY) == 0) {};
            HAL_FMC::Read(RD_ADC_B); //-V530
            HAL_FMC::Read(RD_ADC_B); //-V530
            HAL_FMC::Read(RD_ADC_A); //-V530
            HAL_FMC::Read(RD_ADC_A); //-V530
        }

        if (ch.IsA())
        {
            for (int i = 0; i < 100; i += 2)
            {
                while (_GET_BIT(HAL_FMC::Read(RD_FL), FL_POINT_READY) == 0) {};
                HAL_FMC::Read(RD_ADC_B); //-V530
                HAL_FMC::Read(RD_ADC_B); //-V530
                buffer[i] = (uint8)HAL_FMC::Read(RD_ADC_A);
                buffer[i + 1] = (uint8)HAL_FMC::Read(RD_ADC_A);
            }
        }
        else
        {
            for (int i = 0; i < 100; i += 2)
            {
                while (_GET_BIT(HAL_FMC::Read(RD_FL), FL_POINT_READY) == 0) {};
                buffer[i] = (uint8)HAL_FMC::Read(RD_ADC_B);
                buffer[i + 1] = (uint8)HAL_FMC::Read(RD_ADC_B);
                HAL_FMC::Read(RD_ADC_A); //-V530
                HAL_FMC::Read(RD_ADC_A); //-V530
            }
        }

        /*
        if(ch.IsA())
        {
            LOG_WRITE("min = %d, max = %d", CalculateMinWithout0(buffer), CalculateMaxWithout255(buffer));
        }
        */

        if (CalculateMinWithout0(buffer) < ValueFPGA::MIN || CalculateMaxWithout255(buffer) > ValueFPGA::MAX)
        {
            if (range < Range::_20V)
            {
                range++;
            }

            PageTime::SetPeakDetMode(peakDetMode);
            return (Range::E)(range);
        }

        uint8 min = ValueFPGA::AVE - 30;
        uint8 max = ValueFPGA::AVE + 30;

        if (range == Range::_2mV && CalculateMinWithout0(buffer) > min && CalculateMaxWithout255(buffer) < max)
        {
            PageTime::SetPeakDetMode(peakDetMode);
            return Range::Count;
        }
    }

    PageTime::SetPeakDetMode(peakDetMode);
    return Range::Count;
}


TBase::E AutoFinderFPGA::FindTBase(const Chan &)
{
    TrigInput::Set(TrigInput::Full);

    HAL_TIM2::DelayMS(10);
    FPGA::Stop();
    float fr = FreqMeter::CalculateFrequencyFromCounterFrequency();

    TrigInput::Set(fr < 1e6F ? TrigInput::LPF : TrigInput::Full);

    fr = FreqMeter::CalculateFrequencyFromCounterFrequency();

    TBase::E tBase = TBase::Count;

    if (fr >= 50.0F)
    {
        tBase = CalculateTBase(fr);
        TBase::Set(tBase);
        FPGA::Start();
        TrigInput::Set(fr < 500e3F ? TrigInput::LPF : TrigInput::HPF);
        return tBase;
    }
    else
    {
        TrigInput::Set(TrigInput::LPF);
        
        FreqMeter::CalculateFrequencyFromCounterPeriod();

        if (fr > 0.0F)
        {
            tBase = CalculateTBase(fr);
            TBase::Set(tBase);
            HAL_TIM2::DelayMS(10);
            FPGA::Start();
            return tBase;
        }
    }

    return TBase::Count;
}


TBase::E AutoFinderFPGA::AccurateFindTBase(const Chan &ch)
{
    for (int i = 0; i < 5; i++)
    {
        TBase::E tBase = FindTBase(ch);
        TBase::E secondTBase = FindTBase(ch); //-V656

        if (tBase == secondTBase && tBase != TBase::Count)
        {
            return tBase;
        }
    }
    return TBase::Count;
}


static TBase::E AutoFinderFPGA::CalculateTBase(float freq_)
{
    if (freq_ >= 100e6F) { return TBase::_2ns; }
    else if (freq_ >= 40e6F) { return TBase::_5ns; }
    else if (freq_ >= 20e6F) { return TBase::_10ns; }
    else if (freq_ >= 10e6F) { return TBase::_20ns; }
    else if (freq_ >= 3e6F) { return TBase::_50ns; }
    else if (freq_ >= 2e6F) { return TBase::_100ns; }
    else if (freq_ >= 900e3F) { return TBase::_200ns; }
    else if (freq_ >= 400e3F) { return TBase::_500ns; }
    else if (freq_ >= 200e3F) { return TBase::_1us; }
    else if (freq_ >= 90e3F) { return TBase::_2us; }
    else if (freq_ >= 30e3F) { return TBase::_5us; }
    else if (freq_ >= 20e3F) { return TBase::_10us; }
    else if (freq_ >= 10e3F) { return TBase::_20us; }
    else if (freq_ >= 4e3F) { return TBase::_50us; }
    else if (freq_ >= 2e3F) { return TBase::_100us; }
    else if (freq_ >= 1e3F) { return TBase::_200us; }
    else if (freq_ >= 350.0F) { return TBase::_500us; }
    else if (freq_ >= 200.0F) { return TBase::_1ms; }
    else if (freq_ >= 100.0F) { return TBase::_2ms; }
    else if (freq_ >= 40.0F) { return TBase::_5ms; }
    else if (freq_ >= 20.0F) { return TBase::_10ms; }
    else if (freq_ >= 10.0F) { return TBase::_20ms; }
    else if (freq_ >= 4.0F) { return TBase::_50ms; }
    else if (freq_ >= 2.0F) { return TBase::_100ms; }
    return TBase::_200ms;
}


uint8 AutoFinderFPGA::CalculateMinWithout0(const uint8 buffer[100])
{
    // \todo �� ����� ���������� ��� ����������� ����, ����� ��� ������� ���������� �������� 0 � 255 � ��������� ������
    // ��� ����� �������� �� ������ ����, ����� ���� ��������.
    uint8 min = 255;

    for (int i = 1; i < 100; i++)
    {
        if (buffer[i] > 0 && buffer[i] < min)
        {
            min = buffer[i];
        }
    }
    return min;
}


uint8 AutoFinderFPGA::CalculateMaxWithout255(const uint8 buffer[100])
{
    // \todo �� ����� ���������� ��� ����������� ����, ����� ��� ������� ���������� �������� 0 � 255 � ��������� ������
    // ��� ����� �������� �� ������ ����, ����� ���� ��������.
    uint8 max = 0;
    for (int i = 1; i < 100; i++)
    {
        if (buffer[i] < 255 && buffer[i] > max)
        {
            max = buffer[i];
        }
    }
    return max;
}
