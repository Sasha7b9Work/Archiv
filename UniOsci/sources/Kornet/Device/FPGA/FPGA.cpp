#include "FPGA.h"
#include "AD9286.h"
#include "FPGATypes.h"
#include "Log.h"
#include "FPGA/FPGA.h"
#include "Utils/Math.h"
#include "Display/Display.h"
#include "Hardware/FSMC.h"
#include "Hardware/GPIO.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stm32f4xx_hal.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NULL_TSHIFT 1000000


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FPGA fpga;
ADC_HandleTypeDef hADC;

static uint16 adcValueFPGA = 0;

struct PinStruct
{
    GPIO_TypeDef   *gpioTD;
    uint            pin;
};

static PinStruct pins[Num_Pins] =
{
    {GPIOC, GPIO_PIN_10},   // SPI3_SCK
    {GPIOC, GPIO_PIN_12},   // SPI3_DAT
    {GPIOD, GPIO_PIN_3},    // SPI3_CS1
    {GPIOG, GPIO_PIN_13},   // SPI3_CS2
    {GPIOD, GPIO_PIN_10},   // A1
    {GPIOD, GPIO_PIN_11},   // A2
    {GPIOD, GPIO_PIN_12},   // A3
    {GPIOD, GPIO_PIN_13},   // A4
    {GPIOG, GPIO_PIN_2},    // LF1
    {GPIOG, GPIO_PIN_3},    // LF2
    {GPIOG, GPIO_PIN_4},    // A1S
    {GPIOG, GPIO_PIN_5},    // A0S
    {GPIOG, GPIO_PIN_6}     // LFS
};


bool isRunning = false;

volatile static int numberMeasuresForGates = 1000;

static uint8 dataRand[NumChannels][FPGA_MAX_NUM_POINTS];    ///< Здесь будут данные рандомизатора
              //  2нс 5нс 10нс 20нс 50нс
const int Kr[] = {50, 20, 10,  5,   2};
/// Здесь хранится адрес, начиная с которого будем читать данные по каналам. Если addrRead == 0xffff, то адрес вначале нужно считать
static uint16 addrRead = 0xffff;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::Init()
{
    givingStart = false;

    GPIO_Init();
    
    SetPin(SPI3_CS1);
    SetPin(SPI3_CS2);

    ResetPin(SPI3_SCK);
    ResetPin(SPI3_DAT);

    ad9286.Init();

    ad9286.WriteByte(0x09, 0x09);
    /// \todo Это должно быть в функции записи
    ad9286.WriteByte(0xff, 0x01);   // Пишем бит подтверждения.

    // Теперь настроим вход АЦП от рандомизатора
    // PF10 - ADC3 IN8
    // PB11 - EXTI11
    // Режим работы:
    // - измерение по 1 регулярному каналу
    // - одиночное измерение по фронту внешнего запуска (прерывание от PB11)

    __ADC3_CLK_ENABLE();

    static GPIO_InitTypeDef isGPIOadc =
    {
        GPIO_PIN_10,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOF, &isGPIOadc);

    static GPIO_InitTypeDef isGPIOexti =
    {
        GPIO_PIN_11,
        GPIO_MODE_IT_RISING,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOB, &isGPIOexti);
    
    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    hADC.Instance = ADC3;
    hADC.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    hADC.Init.Resolution = ADC_RESOLUTION12b;
    hADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hADC.Init.ScanConvMode = DISABLE;
    hADC.Init.EOCSelection = ENABLE;
    hADC.Init.ContinuousConvMode = DISABLE;
    hADC.Init.DMAContinuousRequests = DISABLE;
    hADC.Init.NbrOfConversion = 1;
    hADC.Init.DiscontinuousConvMode = DISABLE;
    hADC.Init.NbrOfDiscConversion = 0;
    hADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hADC.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Ext_IT11;

    if (HAL_ADC_Init(&hADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&hADC, &sConfig) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_ADC_Start_IT(&hADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::GiveStart()
{
    uint8 value = (uint8)TRIG_POLARITY;
    fsmc.WriteToFPGA8(WR_TRIG, value++);
    fsmc.WriteToFPGA8(WR_TRIG, (uint8)(value % 2));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Stop()
{
    isRunning = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Update()
{
    if (!isRunning)
    {
        return;
    };

    uint8 flag = ReadFlag();

    if (_GET_BIT(flag, BIT_FLAG_PRED) == 1 && !givingStart)
    {
//        GiveStart();
        givingStart = true;
    }

    if (_GET_BIT(flag, BIT_FLAG_DATA_READY) == 1)
    {
        ReadData();
        Start();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 FPGA::ReadFlag()
{
    return fsmc.ReadFromFPGA(RD_FLAG_LO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint16 FPGA::ReadLastRecord()
{
    return (uint16)(fsmc.ReadFromFPGA(RD_LAST_RECORD_LO) + ((uint16)(fsmc.ReadFromFPGA(RD_LAST_RECORD_HI)) << 8));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::OnPressStart()
{
    isRunning = !isRunning;
    if (isRunning)
    {
        Start();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Start()
{
    givingStart = false;
    addrRead = 0xffff;

    uint16 gPost = (uint16)(-NUM_POINTS);
    uint16 gPred = (uint16)(-3);

    fsmc.WriteToFPGA16(WR_PRED_LO, gPred);
    fsmc.WriteToFPGA16(WR_POST_LO, gPost);
    fsmc.WriteToFPGA8(WR_START, 0xff);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::StartForTester(int)
{
    // У нас двенадцать делений. На двенадцать делений должно приходиться не менее 2.5 мс
    // 2.5мс / 12дел = 0.2 мс/дел = 10мкс/тчк

    SET_TBASE = TBase_500us;

    LoadTBase();

    uint16 gPost = (uint16)(-TESTER_NUM_POINTS);
    uint16 gPred = (uint16)~3;

    fsmc.WriteToFPGA16(WR_PRED_LO, gPred);
    fsmc.WriteToFPGA16(WR_POST_LO, gPost);
    fsmc.WriteToFPGA8(WR_START, 0xff);

    uint8 flag = 0;
    uint timeStart = gTimeUS;
    while (_GET_BIT(flag, BIT_FLAG_PRED) == 0)
    {
        flag = ReadFlag();
        if(gTimeUS - timeStart > 1000) /// \todo Временная затычка. Надо сделать так, чтобы такие ситуации были исключены. Сбои происходят, во время
        {                               /// нажатия кнопок
            return;
        }
    }

    GiveStart();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadForTester(uint8 *dataA, uint8 *dataB)
{
    uint8 flag = 0;

    uint timeStart = gTimeUS;
    while (_GET_BIT(flag, BIT_FLAG_DATA_READY) == 0)    // Ждём флага готовности данных
    {
        flag = ReadFlag();
        if(gTimeUS - timeStart > 10000) /// \todo Временная затычка. Надо сделать так, чтобы такие ситуации были исключены. Сбои происходят, во время
        {                               /// нажатия кнопок
            return;
        }
    }

    uint16 addr = (uint16)(ReadLastRecord() - TESTER_NUM_POINTS);

    fsmc.WriteToFPGA16(WR_PRED_LO, addr);           // Указываем адрес, с которого будем читать данные
    fsmc.WriteToFPGA8(WR_START_ADDR, 0xff);             // И даём команду ПЛИС, чтобы чтение начиналось с него

    uint8 *addrA = RD_DATA_A;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // Читаем данные первого канала
    {
        *dataA++ = *addrA;
    }

    fsmc.WriteToFPGA16(WR_PRED_LO, addr);           // Указываем адрес, с котонрого будем читать данные
    fsmc.WriteToFPGA8(WR_START_ADDR, 0xff);             // И даём команду ПЛИС, чтобы чтение начиналось с него

    uint8 *addrB = RD_DATA_B;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // Читаем данные второго канала
    {
        *dataB++ = *addrB;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadDataChanenl(Channel ch, uint8 data[FPGA_MAX_NUM_POINTS])
{
    if (addrRead == 0xffff)
    {
        addrRead = (uint16)(ReadLastRecord() - NUM_POINTS);
    }
    
    fsmc.WriteToFPGA16(WR_PRED_LO, addrRead);
    fsmc.WriteToFPGA8(WR_START_ADDR, 0xff);
    
    uint8 *address = (ch == A) ? RD_DATA_A : RD_DATA_B;

    if (IN_RANDOMIZE_MODE)
    {
        ReadDataChanenlRand(ch, address, data);
    }
    else
    {
        uint8 *p = data;

        for (int i = 0; i < NUM_POINTS / 4; ++i)
        {
            *p++ = *address;
            *p++ = *address;
            *p++ = *address;
            *p++ = *address;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadDataChanenlRand(Channel ch, uint8 *address, uint8 *data)
{
    int Tsm = CalculateShift();

    if (Tsm == NULL_TSHIFT)
    {
        return;
    }

    int step = Kr[SET_TBASE];

    /*
    if (Tsm < 2)
    {
        Tsm += step;
    }
    */

    int index = Tsm - step;

    while (index < 0)
    {
        index += step;
    }

    uint8 *dataRead = &dataRand[ch][0] + index;

    uint8 *last = &dataRand[ch][0] + NUM_POINTS;

    while (dataRead < last)
    {
        *dataRead = *address;
        dataRead += step;
    }

    memcpy(data, &dataRand[ch][0], NUM_POINTS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int FPGA::CalculateShift()
{
    uint16 min = 0;
    uint16 max = 0;

    if (!CalculateGate(adcValueFPGA, &min, &max))
    {
        return NULL_TSHIFT;
    }

    if (IN_RANDOMIZE_MODE)
    {
        float tin = (float)(adcValueFPGA - min) / (max - min);
        int retValue = (int)(tin * Kr[SET_TBASE] + 0.5f);

//        LOG_MESSAGE("%d - %d", adcValueFPGA, retValue);

        return retValue;
    }

    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{
    if (rand < 500 || rand > 4000)
    {
        LOG_MESSAGE("ОШИБКА!!! считано %d", rand);
        return false;
    }

    static float minGate = 0.0f;
    static float maxGate = 0.0f;
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

    if (minGate == 0.0f)
    {
        *eMin = min;
        *eMax = max;
        if (numElements < numberMeasuresForGates)
        {
            return true;
        }
        minGate = min;
        maxGate = max;
        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    if (numElements >= numberMeasuresForGates)
    {
        //minGate = (9.0f * minGate + min) * 0.1f;
        //maxGate = (9.0f * maxGate + max) * 0.1f;

        minGate = 0.8f * minGate + min * 0.2f;
        maxGate = 0.8f * maxGate + max * 0.2f;

        LOG_MESSAGE("%.1f %.1f", minGate, maxGate);

        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    *eMin = (uint16)(minGate);
    *eMax = (uint16)(maxGate);

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadData()
{
    uint8 *dataA = (uint8 *)malloc(NUM_POINTS);
    uint8 *dataB = (uint8 *)malloc(NUM_POINTS);

    ReadDataChanenl(A, dataA);
    ReadDataChanenl(B, dataB);

    storage.AddData(dataA, dataB);
    
    free(dataA);
    free(dataB);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::GPIO_Init()
{
    GPIO_InitTypeDef isGPIO =
    {
        0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };

    for (int i = 0; i < Num_Pins; i++)
    {
        isGPIO.Pin = GetPin((Pin)i);
        HAL_GPIO_Init(GetPort((Pin)i), &isGPIO);
    }

    for (int i = 0; i < Num_Pins; i++)
    {
        gpio.SetOutputPP_PullDown(GetPort((Pin)i), (uint)LowSignedBit(GetPin((Pin)i)));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadSettings()
{
    LoadRanges();
    LoadRShift(A);
    LoadRShift(B);
    LoadTrigSourceInput();
    LoadTBase();
    LoadTShift();

    isRunning = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::IncreaseRange(Channel ch)
{
    LimitationIncrease<uint8>((uint8 *)(&SET_RANGE(ch)), (uint8)(RangeSize - 1));
    LoadRanges();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::DecreaseRange(Channel ch)
{
    LimitationDecrease<uint8>((uint8 *)(&SET_RANGE(ch)), 0);
    LoadRanges();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::IncreaseTBase()
{
    LimitationIncrease<uint8>((uint8 *)(&SET_TBASE), (uint8)(TBaseSize - 1));
    LoadTBase();
    Start();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::DecreaseTBase()
{
    LimitationDecrease<uint8>((uint8 *)(&SET_TBASE), 0);
    LoadTBase();
    Start();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::RShiftChange(Channel ch, int delta)
{
    AddtionThisLimitation<uint16>(&SET_RSHIFT(ch), STEP_RSHIFT * delta, RShiftMin, RShiftMax);

    LoadRShift(ch);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::TrigLevChange(int delta)
{
    AddtionThisLimitation<uint16>(&SET_TRIGLEV, STEP_TRIGLEV * delta, TrigLevMin, TrigLevMax);

    LoadTrigLev();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::TShiftChange(int delta)
{
    AddtionThisLimitation<uint16>(&SET_TSHIFT, delta, TShiftMin, TShiftMax);

    LoadTShift();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::GetPin(Pin pin)
{
    return pins[pin].pin;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
GPIO_TypeDef *FPGA::GetPort(Pin pin)
{
    return pins[pin].gpioTD;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 ValueForRange(Channel ch)
{
#ifdef WIN32
#pragma warning(push)
#pragma warning(disable : 4310)
#endif

    static const uint8 datas[CoupleSize] =
    {
        BIN_U8(01000001),    // AC
        BIN_U8(10000001),    // DC
        BIN_U8(00000010)     // GND
    };

    if (SET_COUPLE(ch) == Couple_GND)
    {
        return datas[Couple_GND];
    }

    static const uint16 values[RangeSize][NumChannels] =
    {   //             A                    B
        { BIN_U8(00100101), BIN_U8(00010101) },   // 2mV
        { BIN_U8(00100101), BIN_U8(00010101) },   // 5mV
        { BIN_U8(00100101), BIN_U8(00010101) },   // 10mV
        { BIN_U8(00100101), BIN_U8(00010101) },   // 20mV
        { BIN_U8(00010101), BIN_U8(00100101) },   // 50mV
        { BIN_U8(00010101), BIN_U8(00100101) },   // 100mV
        { BIN_U8(00010101), BIN_U8(00100101) },   // 200mV
        { BIN_U8(00101001), BIN_U8(00011001) },   // 500mV
        { BIN_U8(00101001), BIN_U8(00011001) },   // 1V
        { BIN_U8(00101001), BIN_U8(00011001) },   // 2V
        { BIN_U8(00011001), BIN_U8(00101001) },   // 5V
        { BIN_U8(00011001), BIN_U8(00101001) },   // 10V
        { BIN_U8(00011001), BIN_U8(00101001) }    // 20V
    };

    return (uint8)((values[SET_RANGE(ch)][ch] | datas[SET_COUPLE(ch)]));

#ifdef WIN32
#pragma warning(pop)
#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadRanges()
{
    uint16 value = (uint16)(ValueForRange(B) + (ValueForRange(A) << 8));

    WriteRegisters(SPI3_CS2, value);

    PAUSE_ON_MS(10);              // Задержка нужна, чтобы импульсные реле успели отработать

    WriteRegisters(SPI3_CS2, 0);    // Записываем ноль, чтобы реле не потребляли энергии

    static const uint8 vals[RangeSize][NumChannels] =
    {
        { BIN_U8(00000000),  BIN_U8(00000000) },  // 2mV
        { BIN_U8(00000001),  BIN_U8(00000010) },  // 5mV
        { BIN_U8(00000010),  BIN_U8(00000001) },  // 10mV
        { BIN_U8(00000011),  BIN_U8(00000011) },  // 20mV
        { BIN_U8(00000001),  BIN_U8(00000010) },  // 50mV
        { BIN_U8(00000010),  BIN_U8(00000001) },  // 100mV
        { BIN_U8(00000011),  BIN_U8(00000011) },  // 200mV
        { BIN_U8(00000001),  BIN_U8(00000010) },  // 500mV
        { BIN_U8(00000010),  BIN_U8(00000001) },  // 1V
        { BIN_U8(00000011),  BIN_U8(00000011) },  // 2V
        { BIN_U8(00000001),  BIN_U8(00000010) },  // 5V
        { BIN_U8(00000010),  BIN_U8(00000001) },  // 10V
        { BIN_U8(00000011),  BIN_U8(00000011) }   // 20V
    };

    uint8 valueA = vals[SET_RANGE_A][A];

    WritePin(A1, _GET_BIT(valueA, 1));
    WritePin(A2, _GET_BIT(valueA, 0));

    uint8 valueB = vals[SET_RANGE_B][B];

    WritePin(A3, _GET_BIT(valueB, 1));
    WritePin(A4, _GET_BIT(valueB, 0));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteRegisters(Pin cs, uint16 value)
{
    ResetPin(cs);

    if (cs == SPI3_CS1)
    {
        for (int i = 15; i >= 0; --i)
        {
            WritePin(SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            SetPin(SPI3_SCK);
            ResetPin(SPI3_SCK);
        }
    }
    else if (cs == SPI3_CS2)
    {
        for (int i = 0; i < 16; ++i)
        {
            WritePin(SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            SetPin(SPI3_SCK);
            ResetPin(SPI3_SCK);
        }
    }

    SetPin(cs);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadRShift(Channel ch)
{
    static const uint16 mask[2] = {0xa000, 0x6000};

    WriteRegisters(SPI3_CS1, (uint16)(mask[ch] | (SET_RSHIFT(ch) << 2)));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigLev()
{
    WriteRegisters(SPI3_CS1, (uint16)(0x2000 | (SET_TRIGLEV << 2)));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WritePin(Pin pin, int enable)
{
    if (enable)
    {
        GetPort(pin)->BSRR = GetPin(pin);
    }
    else
    {
        GetPort(pin)->BSRR = (uint)GetPin(pin) << 16;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetPin(Pin pin)
{
    GetPort(pin)->BSRR = GetPin(pin);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ResetPin(Pin pin)
{
    GetPort(pin)->BSRR = (uint)GetPin(pin) << 16;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTShift()
{
    int pred = ~(2);
    int post = ~((1 << 13) - 1);

    fsmc.WriteToFPGA16(WR_PRED_LO, (uint16)pred);
    fsmc.WriteToFPGA16(WR_POST_LO, (uint16)post);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetRShift(Channel ch, uint16 rShift)
{
    Limitation<uint16>(&rShift, RShiftMin, RShiftMax);
    SET_RSHIFT(ch) = rShift;
    LoadRShift(ch);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTBase()
{
#ifdef WIN32
#pragma warning(push)
#pragma warning(disable : 4310)
#endif

    static const uint8 values[TBaseSize] =
    {
        BIN_U8(00000000),    // 2ns     1       200MHz
        BIN_U8(00000000),    // 5ns     1       200MHz
        BIN_U8(00000000),    // 10ns    1       200MHz
        BIN_U8(00000000),    // 20ns    1       200MHz
        BIN_U8(00000000),    // 50ns    1       200MHz
        BIN_U8(00000000),    // 100ns   1       200MHz
        BIN_U8(00100000),    // 200ns   2       100MHz
        BIN_U8(00100001),    // 500ns   5       40MHz
        BIN_U8(00100010),    // 1us     10      20MHz
        BIN_U8(00100011),    // 2us     20      10MHz
        BIN_U8(01000101),    // 5us     50      4MHz
        BIN_U8(01000110),    // 10us    100     2MHz
        BIN_U8(01000111),    // 20us    200     1MHz
        BIN_U8(01001001),    // 50us    500     400kHz
        BIN_U8(01001010),    // 100us   1k      200kHz
        BIN_U8(01001011),    // 200us   2k      100kHz
        BIN_U8(01001101),    // 500us   5k      40kHz
        BIN_U8(01001110),    // 1ms     10k     20kHz
        BIN_U8(01001111),    // 2ms     20k     10kHz
        BIN_U8(01010001),    // 5ms     50k     4kHz
        BIN_U8(01010010),    // 10ms    100k    2kHz
        BIN_U8(01010011),    // 20ms    200k    1kHz
        BIN_U8(01010101),    // 50ms    500k    400Hz
        BIN_U8(01010110),    // 100ms   1M      200Hz
        BIN_U8(01010111),    // 200ms   2M      100Hz
        BIN_U8(01011001),    // 500ms   5M      40Hz
        BIN_U8(01011010),    // 1s      10M     20Hz
        BIN_U8(01011011),    // 2s      20M     10Hz
        BIN_U8(01011101),    // 5s      50M     4Hz
        BIN_U8(01011110)     // 10s     100M    2Hz
    };

    memset(&dataRand[0][0], 0, FPGA_MAX_NUM_POINTS * 2);

    fsmc.WriteToFPGA8(WR_TBASE, values[SET_TBASE]);

#ifdef WIN32
#pragma warning(pop)
#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigMode()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigInput()
{
    LoadTrigSourceInput();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigSource()
{
    LoadTrigSourceInput();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigPolarity()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigSourceInput()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // ПС
        {BIN_U8(00000000), BIN_U8(00000110)}, // ВЧ
        {BIN_U8(00000011), BIN_U8(00000101)}  // НЧ
    };
    
    WritePin(A1S, _GET_BIT(datas[TRIG_INPUT][TRIGSOURCE], 2));
    WritePin(A0S, _GET_BIT(datas[TRIG_INPUT][TRIGSOURCE], 1));
    WritePin(LFS, _GET_BIT(datas[TRIG_INPUT][TRIGSOURCE], 0));
}

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ADC_IRQHandler()
{
    HAL_ADC_IRQHandler(&hADC);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    adcValueFPGA = (uint16)HAL_ADC_GetValue(hadc);
//    LOG_MESSAGE("%d", adcValueFPGA);
}

#ifdef __cplusplus
}
#endif
