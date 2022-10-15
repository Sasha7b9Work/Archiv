#pragma once
#include "Hardware/Hardware.h"
#include "Multimeter/Multimeter.h"
#include "Osci/Osci.h"
#include "Tester/Tester.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum DeviceMode

{
    Mode_Osci,
    Mode_Tester,
    Mode_Multimeter,
    NumDeviceModes
};

class Device
{
public:
    Device() : currentMode(Mode_Osci) { };

    void Init();

    void Update();

    DeviceMode CurrentMode();
    /// Функция вызывается при нажатии на кнопку "Функция". Переключает устройство в следующий режим
    void ChangeMode();
private:
    Multimeter multimeter;
    Osci osci;
    Hardware hardware;
    DeviceMode currentMode;
};

extern Device device;
