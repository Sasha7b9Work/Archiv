// 2022/01/31 12:01:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Panel/Panel.h"
#include "common/Utils/Containers/Buffer_.h"
#include "Settings/Settings.h"
#include "FPGA/FreqMeter.h"


struct StateWorkFPGA {
    enum E {
        Stop,    // ���� - �� ���������� ����������� ����������.
        Wait,    // ��� ����������� ��������������.
        Work     // ��� ������.
    };

    static E value;

    static bool IsStop() { return value == Stop; }
    static bool IsWait() { return value == Wait; }
    static bool IsWork() { return value == Work; }

    static void SetWait() { value = Wait; }
    static void SetWork() { value = Work; }
    static void SetStop() { value = Stop; }
};



namespace SyncPulse
{
    void Update(uint flag);

    extern bool is_fired;       // ���� true - ����� ����������

    extern uint time_fire;      // ����� ���������� "�������"
}


namespace Randomizer
{
    // ���������� ���������� ���������, �� ������� ����� �������������� ������ � ������ �������������.
    void SetNumberMeasuresForGates(int number);

    bool CalculateGate(uint16 rand, uint16 *min, uint16 *max);
};


namespace Calibrator
{
    // ������ ������� ������ ���������� - �������� � ��������
    void PerformCalibration();

    bool CalibrationChannel(const Chan &ch);

    namespace Balancer
    {
        // ������ ������������ �� ���� �����
        void PerformOnGround(const Chan &ch);

        // ������ ������������ ��� AC � DC (�� ��������� ����������)
        bool PerformNormal(const Chan &ch);
    };

    namespace Stretcher
    {
        bool Perform(const Chan &ch);
    };
};


// ��������� ��� ���������� ����/�����- ���������
namespace LaunchFPGA
{
    // ��������� � ���������� ����� ����- � �����- �������
    void Load();

    uint16 Pred();

    // ���������� ��������, ������� ��� ������ � ����
    uint16 PredForWrite();
    uint16 PostForWrite();

    // �� ������� ������ ����� ������� ������
    int DeltaReadAddress();

    // �������������� �������� ��� ������ ����������� ����� (����� ��� ����, ����� ������, ��� � ������ �������������
    // ��������� tShift �������� ����������� �� ������ �� �� 1 �������)
    int AdditionalOffsetIndexFirst();
};


namespace BUS_FPGA
{
    void Write(uint16 *const address, uint16 data, bool restart = true);

    // ��� ������� ������������ �����, ����� ������ � ������� �� ������� ����������� �������� ���������. ��������,
    // � �������, ����������� ������� �����������
    void WriteWithoutStart(uint16 *const address, uint16 data);

    void WriteToDAC(TypeWriteDAC::E type, uint16 data);

    void WriteToAnalog(TypeWriteAnalog::E type, uint data);

    // ��������� ��� ��������� ���������� ������� � ������������� � ���������� �����.
    void SetAttribChannelsAndTrig(TypeWriteAnalog::E type);
};


struct FlagFPGA
{
    FlagFPGA() : flag(0), time_pred_launch_ready(0) {}

    inline bool IsTrigReady()       const { return _GET_BIT(flag, FL_TRIG_READY) == 1; }
    inline bool IsDataReady()       const { return _GET_BIT(flag, FL_DATA_READY) == 1; };
    inline bool IsPointReady()      const { return _GET_BIT(flag, FL_POINT_READY) == 1; };
    inline bool IsPredLaunchReady() const { return _GET_BIT(flag, FL_PRED_READY) == 1; }
    inline bool IsFirstByte0()      const { return _GET_BIT(flag, FL_FIRST_BYTE) == 0; }

    void Read()
    {
        flag = HAL_FMC::Read(RD_FL);

        // ������������ �����, ����� ��� ���������� ���� �����������

        if (_GET_BIT(flag, FL_PRED_READY) == 1)
        {
            if (time_pred_launch_ready == 0)
            {
                time_pred_launch_ready = TIMER_MS;
            }
        }
        else
        {
            time_pred_launch_ready = 0;
        }


        // ��������� ������ ������������, ���� ����� (���� � ��� �������� �������������� �������������, ����� �������
        // ����������� ������ ����� �������, � ������������� ��� � �� ������, ����� ������ �������������� �������������
        if (START_MODE_IS_AUTO)
        {
            if ((_GET_BIT(flag, FL_PRED_READY) == 1) &&
                (_GET_BIT(flag, FL_TRIG_READY) == 0) &&
                (TIMER_MS - time_pred_launch_ready) > 1000)
            {
                static uint timePrev = 0;

                if (TIMER_MS - timePrev >= 100)
                {
                    timePrev = TIMER_MS;

                    TrigPolarity::Switch();
                }
            }
        }

        FreqMeter::Update(flag);

        SyncPulse::Update(flag);
    }

    uint16 flag;
    uint time_pred_launch_ready;     // �����, ����� ���� PredReady �����������. ���� == 0, �� ����������
};
