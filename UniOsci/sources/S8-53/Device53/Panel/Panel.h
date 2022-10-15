#pragma once 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Panel
{
public:

    static void Init();

    static void EnableLEDRegSet(bool enable);
    /// ��������/��������� ��������� ����� 1.
    static void EnableLEDChannelA(bool enable);
    /// ��������/��������� ��������� ����� 2.
    static void EnableLEDChannelB(bool enable);
    /// ��������/��������� ��������� �����.
    static void EnableLEDTrig(bool enable);
    /// �������� ������� � �� ������ ����������.
    static void TransmitData(uint16 data);
    /// � ���������� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� �������� ������ ������� ��������.
    static void Disable();

    static void Enable();
    /// ������� ������� �������.
    static PanelButton WaitPressingButton();

    static bool ProcessingCommandFromPIC(uint16 command);

    static uint16 NextData();

    static bool IsFunctionalButton(PanelButton button);
};
