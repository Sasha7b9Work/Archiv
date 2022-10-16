// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "common/Hardware/Sound_.h"
#include "Hardware/VCP/VCP_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Display/Display.h"
#include "Hardware/FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/LAN/LAN.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include "SCPI/SCPI.h"


namespace Device
{
    static void WriteFPS();

    bool needResetSettings = false;
}


void Device::Init()
{
    HAL::Init();
    setNRST.Load();
    set.Load();
    Panel::Init();
    Display::Init();
    Sound::Init();
    VCP::Init();
    LAN::Init();
    FDrive::Init();
    FPGA::OnPressStartStop();   // При включении прибор находится в состоянии "СТОП", поэтому нажатие кнопки "ПУСК"
                                // переведёт прибор в состояние сбора информации
    FPGA::Init();
    Menu::Init();
}


void Device::Update()
{
    HAL_TIM2::StartMultiMeasurement();
    FPGA::Update();
    SCPI::Update();
    Panel::Update();
    Display::Update();
    Menu::Update();
    FDrive::Update();
}


void Device::ResetAllSettings()
{
    Sector::Get(Sector::_12_NRST_1).Erase();
    Sector::Get(Sector::_13_NRST_2).Erase();

    Sector::Get(Sector::_10_SETTINGS_1).Erase();
    Sector::Get(Sector::_11_SETTINGS_2).Erase();
}


void Device::WriteFPS()
{
    static uint timePrev = 0;
    static int counter = 0;

    counter++;

    if (TIMER_MS - timePrev >= 1000)
    {
        LOG_WRITE("fps = %d", counter);

        counter = 0;
        timePrev = TIMER_MS;
    }
}


//static void ProcessingSignal()
//{
//    uint8 **data0 = &Storage::dataA;
//    uint8 **data1 = &Storage::dataB;
//    DataSettings **ds = &Storage::set;
//
//    int first = 0;
//    int last = 0;
//    SettingsDisplay::PointsOnDisplay(&first, &last);
//
//    if (MODE_WORK_IS_DIRECT)
//    {
//        Storage::GetDataFromEnd(0, &Storage::set, &Storage::dataA, &Storage::dataB);
//        if (ENumAveraging::NumAverages() != 1 || sTime_RandomizeModeEnabled())
//        {
//            Storage::dataA = Storage::GetAverageData(ChA);
//            Storage::dataB = Storage::GetAverageData(ChB);
//        }
//    }
//    else if (MODE_WORK_IS_LATEST)
//    {
//        data0 = &Storage::dataLastA;
//        data1 = &Storage::dataLastB;
//        ds = &Storage::dsLast;
//        Storage::GetDataFromEnd(PageMemory::PageLatest::currentSignal, &Storage::dsLast, &Storage::dataLastA, &Storage::dataLastB);
//    }
//    else if (MODE_WORK_IS_MEMINT)
//    {
//        data0 = &Storage::dataIntA;
//        data1 = &Storage::dataIntB;
//        ds = &Storage::dsInt;
//        EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);
//    }
//
//    if (MODE_WORK_IS_MEMINT)
//    {
//        if (!MODE_SHOW_MEMINT_IS_SAVED)
//        {
//            Processing::SetSignal(Storage::dataA, Storage::dataB, Storage::set, first, last);
//        }
//    }
//    else
//    {
//        Processing::SetSignal(*data0, *data1, *ds, first, last);
//    }
//
//    Cursors_Update();    // В случае, если находимся в режиме курсорных измерений, обновляем их положение, если нужно.
//}
