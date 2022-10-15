#pragma once
#include "defines.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Tester
{
public:
    void Init();

    void Enable();

    void Disable();

    void Update();

    void ProcessStep();
    /// ��������� ���������� �� Settings
    void LoadPolarity();
    /// ������������� ��� ��������� ���������� � ����������� � ����������� Settings
    void LoadStep();

private:
    /// ��������� FPGA � ������������ � �������������� �����������
    void LoadFPGA();
    /// ������� ������ ��������� ���������
    void ReadData();
    /// ��������� ������� ������ ��������� ���������
    void StartFPGA();
    /// ������� ���
    int step;
    /// ��� ��������� ����������
    float stepU;
};


extern Tester tester;

#define NUM_STEPS       5

extern uint8 dataTester[NumChannels][NUM_STEPS][TESTER_NUM_POINTS];
