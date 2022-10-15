#include "defines.h"
#include "main.h"
#include "Hardware/Timer.h"
#include "Log.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Ethernet/Ethernet.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/DataStorage.h"
#include "Hardware/Sound.h"
#include "Menu/Pages/PageCursors.h"
#include "Menu/Menu.h"
#include "Hardware/CPU.h"
#include "Hardware/Panel.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static void FuncConnect(int _id);
//static void FuncReciever(int _id, const char *_buffer, uint _length);
static void ProcessingSignal();

#define TICS ((gTimerTics - time) / 120.0f)

//----------------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
    CPU::Init();

    Timer::Init();

    Sound::Init();

    Panel::Init();

    FDrive::Init();
    
    VCP::Init();

    Settings::Load(false);
    FPGA::Init();    
    Timer::PauseOnTime(250);
    FPGA::OnPressStartStop();
    Ethernet::Init();
    Display::Init();

    while(1)
    {
        Timer::StartMultiMeasurement();      // ����� ������� ��� ������ ������������ ��������� ���������� � ������� ����� �������� �����.
        FDrive::Update();
        Ethernet::Update(0);
        FPGA::Update();                      // ��������� ���������� �����.
        ProcessingSignal();
        Display::Update();               // ������ �����.
        Menu::UpdateInput();                 // ��������� ��������� ����
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingSignal() 
{
    uint8 **data0 = &gData0;
    uint8 **data1 = &gData1;
    DataSettings **ds = &gDSet;

    int first = 0;
    int last = 0;
    sDisplay_PointsOnDisplay(&first, &last);

    if (MODE_WORK_IS_DIR)
    {
        Storage::GetDataFromEnd(0, &gDSet, &gData0, &gData1);
        if (sDisplay_NumAverage() != 1 || sTime_RandomizeModeEnabled())
        {
            gData0 = Storage::GetAverageData(A);
            gData1 = Storage::GetAverageData(B);
        }
    }
    else if (MODE_WORK_IS_RAM)
    {
        data0 = &gData0memLast;
        data1 = &gData1memLast;
        ds = &gDSmemLast;
        Storage::GetDataFromEnd(gMemory.currentNumLatestSignal, &gDSmemLast, &gData0memLast, &gData1memLast);
    }
    else if (MODE_WORK_IS_ROM)
    {
        data0 = &gData0memInt;
        data1 = &gData1memInt;
        ds = &gDSmemInt;
        CPU::FLASH_::GetData(gMemory.currentNumIntSignal, &gDSmemInt, &gData0memInt, &gData1memInt);
    }

    if (MODE_WORK_IS_ROM)
    { 
        if (!MODE_SHOW_MEMINT_IS_SAVED)
        {
            Processing::SetSignal(gData0, gData1, gDSet, first, last);
        }
    }
    else
    {
        Processing::SetSignal(*data0, *data1, *ds, first, last);
    }

    Cursors_Update();    // � ������, ���� ��������� � ������ ��������� ���������, ��������� �� ���������, ���� �����.
}
