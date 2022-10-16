// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Sound
{
    void Init();

    void ButtonPress();

    // ������� ������� ���� ����������� ������ ������ ���� ����� ���� ������������ ���� ������� ������.
    void ButtonRelease();

    void GovernorChangedValue();

    void RegulatorShiftRotate();

    void RegulatorSwitchRotate();

    void WarnBeepBad();

    void WarnBeepGood();

    // ��� ������� ���� �������� ����� �������/��������� ����. ���� ����������� � ����.
    void WaitForCompletion();

    // ���� ���������
    void DeviceEnabled();
};
