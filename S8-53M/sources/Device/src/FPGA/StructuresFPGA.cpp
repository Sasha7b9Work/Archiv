// 2022/01/31 12:04:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/StructuresFPGA.h"
#include "FPGA/ReaderFPGA.h"
#include "Settings/Settings.h"
#include "FPGA/FPGA.h"
#include "common/Utils/Math_.h"
#include <climits>


StateWorkFPGA::E StateWorkFPGA::value = StateWorkFPGA::Stop;


namespace LaunchFPGA
{
    static void Calculate();

    static void CalculateReal();

    static void CalculateRandomize();

    static int pred = 0;
    static int post = 0;

    static const int8 d_pred[TBase::Count] =   // Дополнительное смещение для предзапуска
    {//  1    2    5   10   20   50  100  200
        10,  10,  10,  10,  10,  10,   5,   3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    static const int8 d_post[TBase::Count] =   // Дополнительное смещение для послезапуска
    {//  1    2    5   10   20   50  100  200
        10,  10,  20,  10,  10,  10,   5,   3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    static const int8 d_read[TBase::Count] =   // Дополнительное смещение для чтения адреса
    {//  1    2    5   10   20   50  100  200
        0,   0,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
}


namespace SyncPulse
{
    bool is_fired = false;
    uint time_fire = 0;

    static void Enable()
    {
        is_fired = true;
        time_fire = TIMER_MS;
        led_Trig.Enable();
    }

    static void Disable()
    {
        is_fired = false;
        led_Trig.Disable();
    }
}


namespace BUS_FPGA
{
    static const uint8 masksRange[Range::Count] =
    {
        BIN_U8(0000000),
        BIN_U8(1000000),
        BIN_U8(1100000),
        BIN_U8(1010000),
        BIN_U8(1000100),
        BIN_U8(1100100),
        BIN_U8(1010100),
        BIN_U8(1000010),
        BIN_U8(1100010),
        BIN_U8(1010010),
        BIN_U8(1000110),
        BIN_U8(1100110),
        BIN_U8(1010110)
    };
}


namespace Randomizer
{
    volatile static int number_measures_for_gates = 1000;
}


void SyncPulse::Update(uint flag)
{
    if (TrigPolarity::timeSwitch != 0)      // произошёл принудительный запуск
    {
        Disable();
    }
    else if (_GET_BIT(flag, FL_TRIG_READY) == 1)
    {
        Enable();
    }
    else if (is_fired && (TIMER_MS - time_fire > 1000))
    {
        Disable();
    }
}


void LaunchFPGA::Load()
{
    if (!ReaderFPGA::mutex_read.IsLocked())
    {
        Calculate();

        if (setNRST.show_registers.launch)
        {
            LOG_WRITE("запуск %d:%d", pred, post);
        }

        BUS_FPGA::Write(WR_POST, PostForWrite());
        BUS_FPGA::Write(WR_PRED, PredForWrite());
    }
}


uint16 LaunchFPGA::Pred()
{
    return (uint16)pred;
}


void LaunchFPGA::Calculate()
{
    if (TBASE_IS_RAND)
    {
        CalculateRandomize();
    }
    else
    {
        CalculateReal();
    }
}


void LaunchFPGA::CalculateRandomize()
{
    int k = TBase::StepRand();
    int shift = TShift::ForLaunchFPGA();

    if (TBASE == TBase::_50ns)      shift += 2;
    else if (TBASE == TBase::_20ns) shift += 5;
    else if (TBASE == TBase::_10ns) shift += 20;
    else if (TBASE == TBase::_5ns)  shift -= 140;
    else if (TBASE == TBase::_2ns)  shift += 100;
    else if (TBASE == TBase::_1ns)  shift += 190;

    int num_points = (int)EnumPointsFPGA::PointsInChannel();
    int equal_points = num_points / k;      // Эквивалентное количество точек, которые нужно считать. Оно в коэффициент
                                            // растяжки меньше реального количесва точек, очевидно.
    post = shift / k;

    pred = equal_points - post;

    if (pred + post < equal_points)
    {
        pred = equal_points - post;
    }
}


void LaunchFPGA::CalculateReal()
{
    const int shift = TShift::ForLaunchFPGA();

    post = shift / 2;

    int pointsInChannelHalf = (int)EnumPointsFPGA::PointsInChannel() / 2;

    pred = Math::Limitation(pointsInChannelHalf - post, 0, INT_MAX);

    if (post + pred < pointsInChannelHalf)
    {
        pred = pointsInChannelHalf - post;
    }

    if (shift < 0)
    {
        post = 0;
    }
}


uint16 LaunchFPGA::PredForWrite()
{
    int result = pred + d_pred[TBASE];

    if (PEAK_DET_IS_ENABLED)
    {
        result *= 2;
    }

    return (uint16)(~result);
}


uint16 LaunchFPGA::PostForWrite()
{
    int result = post + d_post[TBASE];

    if (PEAK_DET_IS_ENABLED)
    {
        result *= 2;
    }

    return (uint16)(~result);
}


int LaunchFPGA::DeltaReadAddress()
{
    int result = 0;

    if (TShift::ForLaunchFPGA() < 0)
    {
        result += TShift::ForLaunchFPGA();
    }

    return result - d_read[TBASE];
}


int LaunchFPGA::AdditionalOffsetIndexFirst()
{
    int result = 0;

    int shift = TSHIFT;

    shift = (shift + 10000) % TBase::StepRand();

    //                            1ns 2ns 5ns 10ns 20ns
    static const int deltas[] = {97, 37, 7,  7,   2};

    int d = deltas[TBASE];

    if (shift <= d)
        result = d - shift;
    else
        result = TBase::StepRand() - (shift - d);

    return result;
}


void BUS_FPGA::Write(uint16 *const address, uint16 data, bool restart)
{
    bool is_running = FPGA::IsRunning();

    if (restart)
    {
        FPGA::Stop();
    }

    HAL_FMC::Write(address, data);

    if (is_running && restart)
    {
        FPGA::Start();
    }
}


void BUS_FPGA::WriteWithoutStart(uint16 *const address, uint16 data)
{
    HAL_FMC::Write(address, data);
}


#define CLC_HI      Pin::SPI4_CLK.Set();
#define CLC_LOW     Pin::SPI4_CLK.Reset();
#define DATA_SET(x) Pin::SPI4_DAT.Write(x);


void BUS_FPGA::WriteToAnalog(TypeWriteAnalog::E type, uint data)
{
#define STR_VALUE Uint16((uint16)(data)).ToStringBin().c_str()

    if (type == TypeWriteAnalog::RangeA && setNRST.show_registers.rangeA)
    {
        LOG_WRITE("range 0 = %s", STR_VALUE);
    }
    else if (type == TypeWriteAnalog::RangeB && setNRST.show_registers.rangeB)
    {
        LOG_WRITE("range 1 = %s", STR_VALUE);
    }
    else if (type == TypeWriteAnalog::TrigParam && setNRST.show_registers.trigParam)
    {
        LOG_WRITE("парам. синхр. = %s", STR_VALUE);
    }
    else if (type == TypeWriteAnalog::ChanParamA && setNRST.show_registers.chanParamA)
    {
        LOG_WRITE("парам. кан. 1 = %s", STR_VALUE);
    }
    else if (type == TypeWriteAnalog::ChanParamB && setNRST.show_registers.chanParamB)
    {
        LOG_WRITE("парам. кан. 2 = %s", STR_VALUE);
    }
    else if (type == TypeWriteAnalog::All && (
        setNRST.show_registers.trigParam ||
        setNRST.show_registers.rangeA ||
        setNRST.show_registers.rangeB ||
        setNRST.show_registers.chanParamA ||
        setNRST.show_registers.chanParamB))
    {
        LOG_WRITE("полная запись в аналоговую часть = %s", STR_VALUE);
    }

    Pin::SPI4_CS2.Reset();

    for (int i = 23; i >= 0; i--)
    {
        DATA_SET((data & (1 << i)) ? 1 : 0);
        CLC_HI
            CLC_LOW
    }

    Pin::SPI4_CS2.Set();
}


void BUS_FPGA::WriteToDAC(TypeWriteDAC::E type, uint16 data)
{
#define STR_VALUE Uint16((uint16)(data)).ToStringBin().c_str()

    if (type == TypeWriteDAC::RShiftA && setNRST.show_registers.rShiftA)
    {
        LOG_WRITE("rShift 0 = %s", STR_VALUE);
    }
    else if (type == TypeWriteDAC::RShiftB && setNRST.show_registers.rShiftB)
    {
        LOG_WRITE("rShfit 1 = %s", STR_VALUE);
    }
    else if (type == TypeWriteDAC::TrigLev && setNRST.show_registers.trigLev)
    {
        LOG_WRITE("trigLev = %s", STR_VALUE);
    }

    Pin::SPI4_CS1.Reset();

    for (int i = 15; i >= 0; i--)
    {
        DATA_SET((data & (1 << i)) ? 1 : 0);
        CLC_HI
            CLC_LOW
    }

    Pin::SPI4_CS1.Set();
}


void BUS_FPGA::SetAttribChannelsAndTrig(TypeWriteAnalog::E type)
{
    uint data = 0;

    /*
    b0...b7 - Channel1
    b8...b15 - Channel2
    b16...b23 - Trig
    Передаваться биты должны начиная с b0
    */

    // Range0, Range1
    data |= masksRange[RANGE_A];
    data |= (masksRange[RANGE_B] << 8);

    // Параметры каналов
    static const uint maskCouple[2][3] =
    {
        {0x0008, 0x0000, 0x0030},
        {0x0800, 0x0000, 0x3000}
    };

    data |= maskCouple[ChA][MODE_COUPLE_A];
    data |= maskCouple[ChB][MODE_COUPLE_B];

    static const uint maskFiltr[2][2] =
    {
        {0x0000, 0x0080},
        {0x0000, 0x0100}
    };

    data |= maskFiltr[ChA][FILTR_A];
    data |= maskFiltr[ChB][FILTR_B];


    // Параметры синхронизации
    static const uint maskSource[3] = {0x000000, 0x800000, 0x400000};
    data |= maskSource[TRIG_SOURCE];

    static const uint maskInput[4] = {0x000000, 0x030000, 0x020000, 0x010000};
    data |= maskInput[TRIG_INPUT];

    BUS_FPGA::WriteToAnalog(type, data);
}


void Randomizer::SetNumberMeasuresForGates(int number)
{
    number_measures_for_gates = number;
}


bool Randomizer::CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{
    if (rand < 500 || rand > 3500)
    {
        return false;
    }

    if (Panel::TimePassedAfterLastEvent() < 20)
    {
        return false;
    }

    static const int numValues = 1000;

    static float minGate = 0.0F;
    static float maxGate = 0.0F;
    static int numElements = 0;
    static uint16 min = 0xffff;
    static uint16 max = 0;

    numElements++;
    if (rand < min)
    {
        min = rand;
    }
    if (rand > max)
    {
        max = rand;
    }

    if (minGate == 0.0F) //-V550
    {
        *eMin = min;
        *eMax = max;
        if (numElements < numValues)
        {
            //          LOG_WRITE("min %u, max %u, rand %d", *eMin, *eMax, rand);
            return true;
        }
        minGate = min;
        maxGate = max;
        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    if (numElements == numValues)
    {
        minGate = 0.9F * minGate + 0.1F * min;
        maxGate = 0.9F * maxGate + 0.1F * max;
        //        LOG_WRITE("вор %.0F ... %.0F, min = %u, max = %u", minGate, maxGate, min, max);
        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    *eMin = (uint16)(minGate); //-V519
    *eMax = (uint16)(maxGate); //-V519

//  LOG_WRITE("min %u, max %u, rand %d", *eMin, *eMax, rand);
    return true;
}
