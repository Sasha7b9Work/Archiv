// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"
#include "common/Utils/Containers/Queue_.h"


namespace Panel
{
    void Init();

    void Update();

    // Отключить обработку ввода. В этом режиме панель лишь обновляет состояние переменной pressedButton, не выполняя
    // больше никаких действий
    void DisableInput();

    // Включить обработку ввода
    void EnableInput();

    // Послать сигнал выключения
    void DisablePower();

    // Ожидать нажатие клавиши.
    Key::E WaitPressingKey();

    // Прошло времени в миллисекундах после последнего переключения
    uint TimePassedAfterLastEvent();

    // Эта функция должна вызываться из приёмной фунции SPI5
    void CallbackOnReceiveSPI5(const uint8 *data, uint size);

    // Эта функция вызывается из передающей функции SPI5
    uint8 NextData();

    // Передать даннные в мк панели управления.
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
    // Возвращает true, если лампочка УСТАНОВКА должна гореть
    bool NeedForFire();
};


extern LED       led_Trig;
extern RegSetLED led_RegSet;
extern LED       led_ChA;
extern LED       led_ChB;
extern LED       led_Power;
