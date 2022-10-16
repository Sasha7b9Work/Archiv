// 2021/05/05 15:03:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "HAL.h"
#include "LEDS.h"
#include "Keyboard.h"
#include "PowerSupply.h"


namespace PowerSupply
{
    static uint timeEnabled = (uint)-1;    // ����� ���������. ���� (-1), �� ���������

    static uint timeDisable = 0;    // ����� ���������� ����������
}


bool PowerSupply::IsEnabled()
{
    return timeEnabled != (uint)-1;
}


bool PowerSupply::AttemptToTurnOn(const KeyboardEvent &event)
{
    if (IsEnabled())
    {
        return false;
    }

    if (TIME_MS - timeDisable < 100)
    {
        return false;
    }

    if (event.key == Key::Power)
    {
        if (timeEnabled == (uint)-1)
        {
            pinPower.On();

            timeEnabled = TIME_MS;

            return true;
        }
    }

    return false;
}


void PowerSupply::TurnOff()
{
    pinPower.Off();

    timeEnabled = (uint)-1;

    LED::DisableAll();

    timeDisable = TIME_MS;
}
