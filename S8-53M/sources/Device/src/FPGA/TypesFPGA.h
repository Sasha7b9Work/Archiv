// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/Values_.h"
#include "common/Settings/SettingsTypes_.h"


#define FPGA_MAX_POINTS_FOR_CALCULATE   900


#define WR_START                (HAL_FMC::ADDR_FPGA + 0x00)
#define WR_RAZV                 (HAL_FMC::ADDR_FPGA + 0x01)
#define WR_PRED                 (HAL_FMC::ADDR_FPGA + 0x02)
#define WR_POST                 (HAL_FMC::ADDR_FPGA + 0x03)
#define WR_TRIG                 (HAL_FMC::ADDR_FPGA + 0x04)
#define WR_UPR                  (HAL_FMC::ADDR_FPGA + 0x05)
#define UPR_BIT_PEAKDET                 1   // ������� �������� - 0/1 ����/���
#define UPR_BIT_CALIBRATOR_AC_DC        2   // ����������/����������
#define UPR_BIT_CALIBRATOR_VOLTAGE      3   // 0/4�
#define UPR_BIT_RECORDER                4   // 0 - ������� �����, 1 - �����������
#define WR_ADDR_READ            (HAL_FMC::ADDR_FPGA + 0x06) // ��� ������� ���������� ����� ������ � ������� �����������
#define WR_FREQ_METER_PARAMS    (HAL_FMC::ADDR_FPGA + 0x0a)
#define WR_STOP                 (HAL_FMC::ADDR_FPGA + 0x1f)


#define RD_ADC_A                (HAL_FMC::ADDR_FPGA + 0x00)
#define RD_ADC_B                (HAL_FMC::ADDR_FPGA + 0x02)
#define RD_ADDR_LAST_RECORD     (HAL_FMC::ADDR_FPGA + 0x08) // ������ ������ ��������� ������
#define RD_FREQ_LOW             (HAL_FMC::ADDR_FPGA + 0x10)
#define RD_FREQ_HI              (HAL_FMC::ADDR_FPGA + 0x18)
#define RD_PERIOD_LOW           (HAL_FMC::ADDR_FPGA + 0x20)
#define RD_PERIOD_HI            (HAL_FMC::ADDR_FPGA + 0x28)
#define RD_FL                   (HAL_FMC::ADDR_FPGA + 0x30)
#define FL_DATA_READY                   0   // 0 - ������ ������
#define FL_TRIG_READY                   1   // 1 - ������� ��������������
#define FL_PRED_READY                   2   // 2 - ��������� ����� �����������
#define FL_POINT_READY                  3   // 3 - ������� ����, ��� ����� ������ (� ���������� ������)
#define FL_FREQ_READY                   4   // 4 - ����� ��������� �������
#define FL_PERIOD_READY                 5   // 5 - ����� ��������� ������
#define FL_FIRST_BYTE                   6   // 6 - ������� ��������� ������ - ����������, ����� ���� ������� ������
#define FL_OVERFLOW_FREQ                8   // 8 - ������� ������������ �������� �������
#define FL_OVERFLOW_PERIOD              9   // 9 - ������� ������������ �������� �������


struct TypeWriteAnalog { enum E
{
    All,
    RangeA,
    RangeB,
    TrigParam,
    ChanParamA,
    ChanParamB
};};

struct TypeWriteDAC { enum E
{
    RShiftA,
    RShiftB,
    TrigLev
};};


struct ValueFPGA
{
    static const uint8 MIN = 2;
    static const uint8 AVE = 127;
    static const uint8 MAX = 252;
    static const uint8 NONE = 0;

    static void ToVoltage(puchar points, int numPoints, Range::E, int16 rShift, float *voltage);
    static Voltage ToVoltage(uint8 value, Range::E, int16 rshift);
    static void FromVoltage(const float *voltage, int numPoints, Range::E, int16 rShift, uint8 *points);
    static uint8 FromVoltage(const Voltage &, Range::E, int16 rshift);
    // ������������ � y-���������� �������
    static uint8 ConvertToDisplay(uint8);
};


extern const uint16 * const addresses_ADC[2];
#define ADDRESS_READ(ch) addresses_ADC[ch]
