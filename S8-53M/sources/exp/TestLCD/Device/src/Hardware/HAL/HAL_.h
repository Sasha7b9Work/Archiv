// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL_PINS_.h"


struct PackedTime
{
    uint hours : 5;
    uint minutes : 6;
    uint seconds : 6;
    uint year : 7;
    uint month : 4;
    uint day : 5;
};

/**
  * TIME_TICKS - ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement().
  * � ����� ������� 120.000.000 �����. ������������ ������� �������, ������� ����� ��������� � � ������� - 35 ���.
  * ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement(). �� ����� (1 << 32).
***/
#define TIME_TICKS HAL_TIM2::GetTicks()
#define TIME_MS    HAL_TIM::TimeMS()


#define ERROR_HANDLER() HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL
{
    static void Init();
    static void DeInit();
    static void SystemReset();
    // ������� ������������ � ���������� ��� �������� �� �������� ���������
    static void JumpToApplication();
    static void ErrorHandler(pchar file, int line);
private:
    static void EnablePeripherals();
    static void ConfigSystemClock();
};


// ������������ ��� ���������� �������� �������
struct HAL_DAC1
{
    static void Init();
    // �������� value �� 0 �� 100
    static void SetValue(int value);
};


struct HAL_ETH
{
    static void Init();
};


// ������������ ��� ����� � FPGA � RAM
struct HAL_FMC 
{
    static void Init();
    inline static uint16 Read(uint16 * const address)               { return *address; };

    static void Write(uint16 *const address, uint16 value);

private:
    
    static void InitRAM();
    static void InitFPGA();

public:

    static uint16 *const ADDR_FPGA;                 // ����� ������ � �������

    static uint16 *const ADDR_RAM_DISPLAY_FRONT;    // ������� ����������� �������. ��������� � RAM � �� ����� ������-
                                                    // �������� ��� ������ �����
    static uint8 *const ADDR_RAM_BEGIN;             // ������ ��������� ����� � RAM.
    static uint8 *const ADDR_RAM_END;               // ��������� �� ������ ������������ ����� RAM. ������������ �����
                                                    // ������ [ADDR_RAM_BEGIN...(ADDR_RAM_END - 1)]
};


struct HAL_HCD
{
    static void *handle;    // HCD_HandleTypeDef
};


struct HAL_LTDC
{
    static void Init(uint8 *frontBuffer, uint8 *backBuffer);

    static void LoadPalette();

    static void ToggleBuffers();

private:

    static void SetBuffers(uint8 *frontBuffer, uint8 *backBuffer);
};


// ������������� ��� VCP
struct HAL_PCD
{
    static void *handle;    // PCD_HandleTypeDef
};


struct HAL_TIM
{
    static uint TimeMS();
};

