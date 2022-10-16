// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Hardware/HAL/HAL_PINS_.h"
#include "common/Hardware/Memory/Sector_.h"
#include "common/Utils/Containers/Values_.h"
#include "common/Hardware/HAL/HAL_TIM2.h"


struct PackedTime
{
    uint hours : 5;
    uint minutes : 6;
    uint seconds : 6;
    uint year : 7;
    uint month : 4;
    uint day : 5;
};


#define ERROR_HANDLER() HAL::ErrorHandler(__FILE__, __LINE__)


namespace HAL
{
    void Init();
    void DeInit();
    void SystemReset();
    // ������� ������������ � ���������� ��� �������� �� �������� ���������
    void JumpToApplication();
    void ErrorHandler(pchar file, int line);
};


// ��� �������������
namespace HAL_ADC1
{
    void Init();
    Uint16 GetValue();

    // ���������� ����� ������� �������
    void ResetValue();

    bool CallbackOnIRQ();
};


namespace HAL_CRC
{
    void Init();

    uint Calculate(uint address = 0x08020000, uint numBytes = 128 * 1024 * 3 / 4);

    // ����������� ����������� ����� ������������������ ���� �� buffer. ���� size �� ������ ������, ��������� ����� ��
    // ��������� � ���������� ����������� �����
    uint Calculate8bit(uint8 *buffer, int size);
};


// ������������ ��� ���������� �������� �������
namespace HAL_DAC1
{
    void Init();

    // �������� value �� 0 �� 100
    void SetBrightness(int value);
};


// �������
namespace HAL_DAC2
{
    void Init();
    void StartDMA(uint8 *points, int numPoints);
    void StopDMA();
};


namespace HAL_ETH
{
    void Init();
};


// ������������ ��� ����� � FPGA � RAM
namespace HAL_FMC
{
    void Init();
    inline static uint16 Read(volatile const uint16 * const address) { return *address; };

    void Write(uint16 *const address, uint16 value);

    extern uint16 *const ADDR_FPGA;                 // ����� ������ � �������

    extern uint16 *const ADDR_RAM_DISPLAY_FRONT;    // ������� ����������� �������. ��������� � RAM � �� ����� ������-
                                                    // �������� ��� ������ �����
    extern uint8 *const ADDR_RAM_BEGIN;             // ������ ��������� ����� � RAM.
    extern uint8 *const ADDR_RAM_END;               // ��������� �� ������ ������������ ����� RAM. ������������ �����
                                                    // ������ [ADDR_RAM_BEGIN...(ADDR_RAM_END - 1)]
};


namespace HAL_HCD
{
    extern void *handle;    // HCD_HandleTypeDef
};


namespace HAL_LTDC
{
    void Init(uint8 *frontBuffer, uint8 *backBuffer);

    void LoadPalette();

    void ToggleBuffers();
};


namespace HAL_OTP
{
    bool SaveSerialNumber(char *serialNumber);

    // ������� ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������. 
    int GetSerialNumber(char buffer[17]);
};


// ������������� ��� VCP
namespace HAL_PCD
{
    extern void *handle;    // PCD_HandleTypeDef
};


namespace HAL_ROM
{
    extern const Sector sectors[Sector::Count];

    void Init();

    void WriteBytes(uint address, const uint8 *data, uint size);
    void WriteBufferBytes(uint address, const void *data, uint size);
    void Fill(uint address, uint8 value, int size);
};


namespace HAL_RTC
{
    void Init();

    bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);

    PackedTime GetPackedTime();
};


namespace HAL_SPI1
{
    void Init();
};


// ��� ����� � �������
namespace HAL_SPI5
{
    void Init();

    extern void *handle;       // SPI_HandleTypeDef
};


namespace HAL_TIM
{
    uint TimeMS();
};


namespace HAL_TIM6
{
    void Init();

    bool CallbackOnIRQ();
};


// ��������������� ������ ��� DAC2
namespace HAL_TIM7
{
    void Init();

    void Config(uint16 prescaler, uint16 period);
};
