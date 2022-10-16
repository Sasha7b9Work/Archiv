// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"
#include "common/Utils/Containers/Queue_.h"


namespace Panel
{
    void Init();

    void Update();

    // ��������� ��������� �����. � ���� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� ��������
    // ������ ������� ��������
    void DisableInput();

    // �������� ��������� �����
    void EnableInput();

    // ������� ������ ����������
    void DisablePower();

    // ������� ������� �������.
    Key::E WaitPressingKey();

    // ������ ������� � ������������� ����� ���������� ������������
    uint TimePassedAfterLastEvent();

    // ��� ������� ������ ���������� �� ������� ������ SPI5
    void CallbackOnReceiveSPI5(const uint8 *data, uint size);

    // ��� ������� ���������� �� ���������� ������� SPI5
    uint8 NextData();

    // �������� ������� � �� ������ ����������.
    void TransmitData(uint8 data);
};


struct LED
{
    LED(TypeLED::E led);
    void Disable() { SwitchToState(false); }
    void Enable()  { SwitchToState(true);  }
    void SwitchToState(bool en)
    {
        if (enabled == en)
        {
            return;
        }

        enabled = en;

        uint8 value = (uint8)led;

        if (enabled)
        {
            value |= 0x80;
        }

        Panel::TransmitData(value);
    }
private:
    TypeLED::E led;
    bool enabled;
};


struct RegSetLED : public LED
{
    RegSetLED(TypeLED::E led) : LED(led) {}
    void Switch();
private:
    // ���������� true, ���� �������� ��������� ������ ������
    bool NeedForFire();
};


extern LED       led_Trig;
extern RegSetLED led_RegSet;
extern LED       led_ChA;
extern LED       led_ChB;
extern LED       led_Power;
