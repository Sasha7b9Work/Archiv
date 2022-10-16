// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/Buffer_.h"
#include "FPGA/FreqMeter.h"
#include "FPGA/TypesFPGA.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include "FPGA/StructuresFPGA.h"


namespace FPGA
{
    extern FlagFPGA flag;

    void Init();

    // ���������� ���������� ����������� �������� � �������.
    void SetNumSignalsInSec(int num_sig_in_sec);

    void Update();

    // ������ �������� ����� ����������.
    void Start();

    // ��������� ������� ����� ����������.
    void Stop();

    // ������� �� ������ ����/����
    void OnPressStartStop();

    // ���������� true, ���� ������ ��������� �� � �������� ����� ����������.
    bool IsRunning();

    // ��������� ������� ������ �������.
    void StartAutoFind();

    // ���������� ��������� ����� ����� ��������� ����� - ����� ��������� ������ ������������� �� ��������� �����
    void SetTemporaryPause();

    // ��������� ��������� � ���������� ����� �� ���������� ��������� SSettings.
    void LoadSettings();

    void LoadRegUPR();
};
