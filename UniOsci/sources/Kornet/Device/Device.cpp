#include "Device.h"
#include "Log.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Hardware/Timer.h"
#include "Hardware/Decoder.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <stdlib.h>
#include <math.h>
#include <stm32f4xx_hal.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Device device;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Device::Init()
{
    uint64 temp = (uint64)rand() % 4;
    temp++;
    
    hardware.Init();

    tester.Init();

    display.Init();

    fpga.Init();

    set.Load();

    fpga.LoadSettings();

    menu.Init();

    fpga.OnPressStart();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Device::Update()
{
    START_MULTI_MEASUREMENT();
    menu.Update();
    display.Update();
    decoder.Update();
    fpga.Update();
    menu.SaveSettings();
    tester.Update();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DeviceMode Device::CurrentMode()
{
    return currentMode;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Device::ChangeMode()
{
    CircleIncrease<uint8>((uint8 *)&currentMode, 0, NumDeviceModes - 1);

    if (currentMode == Mode_Tester)
    {
        tester.Enable();
    }
    else
    {
        tester.Disable();
    }
}
