#pragma once
#include "defines.h"
#include "FPGATypes.h"
#include "Settings/SettingsTypes.h"
#include <stm32f4xx.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RShiftMin       20
#define RShiftZero      500
#define RShiftMax       980
#define TrigLevZero     500
#define TrigLevMax      980
#define TrigLevMin      20
#define TShiftZero      500
#define TShiftMin       0
#define TShiftMax       60000

enum Pin
{
    SPI3_SCK,
    SPI3_DAT,
    SPI3_CS1,
    SPI3_CS2,
    A1,
    A2,
    A3,
    A4,
    LF1,
    LF2,
    A1S,
    A0S,
    LFS,
    Num_Pins
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HardwareFPGA
{
    friend class FPGA;
private:
};

class FPGA
{
public:

    void Init();

    void Update();

    void Start();

    void OnPressStart();

    void GiveStart();

    void LoadSettings();

    void IncreaseRange(Channel ch);

    void DecreaseRange(Channel ch);

    void IncreaseTBase();

    void DecreaseTBase();

    void RShiftChange(Channel ch, int delta);

    void TrigLevChange(int delta);

    void TShiftChange(int delta);

    void SetRShift(Channel ch, uint16 rShift);

    void LoadTrigPolarity();

    void LoadTrigMode();

    void LoadTrigSource();

    void LoadTrigInput();

    void Stop();

    void LoadRanges();
    /// Запустить цикл чтения для тестер-компонента. В течение time секунд должно быть считано numPoints точек
    void StartForTester(int numPoints);

    void ReadForTester(uint8 *dataA, uint8 *dataB);

private:

    void GPIO_Init();

    void LoadTBase();

    void LoadTrigLev();

    void LoadRShift(Channel ch);

    void LoadTShift();

    void WritePin(Pin pin, int enable);

    void SetPin(Pin pin);

    void ResetPin(Pin pin);

    uint GetPin(Pin pin);

    GPIO_TypeDef *GetPort(Pin pin);

    void WriteRegisters(Pin cs, uint16 value);

    void ReadData();

    uint8 ReadFlag();

    uint16 ReadLastRecord();

    void ReadDataChanenl(Channel ch, uint8 data[FPGA_MAX_NUM_POINTS]);
    /// Установить в соотвествующие положения выводы, отвечающие за источник и вход синхронизации
    void LoadTrigSourceInput();
    /// Читать канал в рандомизаторе с адреса address
    void ReadDataChanenlRand(Channel ch, uint8 *address, uint8 *data);

    bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax);

    int CalculateShift();

    HardwareFPGA hardware;

    bool isRunning;
    /// True, если дан запуск
    bool givingStart;
};


extern FPGA fpga;
