// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Timer_.h"
#include "common/Utils/Math_.h"
#include "Data/InputStorage/InputStorage.h"
#include "FPGA/AutoFinderFPGA.h"
#include "FPGA/FPGA.h"
#include "FPGA/ReaderFPGA.h"
#include "Panel/Panel.h"
#include "Settings/SettingsNRST.h"
#include <cstring>


namespace FPGA
{
    // Дать запуск, если в этом есть необходимость (например, включён автоматический режим запуска)
    static void StartIfNeed();

    // Функция вызывается, когда можно считывать очередной сигнал.
    static void OnTimerCanReadData();

    volatile static bool temporary_pause = false;
    volatile static bool can_read_data = true;

    FlagFPGA flag;
}


void FPGA::Init() 
{
    FPGA::LoadSettings();
    FPGA::SetNumSignalsInSec(ENumSignalsInSec::NumSignalsInS(ENUM_FPS));
    Randomizer::SetNumberMeasuresForGates(setNRST.rand.num_measures_for_gates);
    Randomizer::SetNumberMeasuresForGates(setNRST.rand.num_measures_for_gates);
    FreqMeter::Init();
} 


void FPGA::SetNumSignalsInSec(int num_sig_in_sec) 
{
    Timer::Enable(TypeTimer::NumSignalsInSec, (int)(1000.F / num_sig_in_sec), FPGA::OnTimerCanReadData);
}


void FPGA::OnTimerCanReadData()
{
    FPGA::can_read_data = 1;
}


// Производит действия, необходимые для поточечного режима
static void ProcessModeP2P()
{
    Timer::Disable(TypeTimer::P2P);

    if (TBASE_IS_P2P)
    {
        Timer::Enable(TypeTimer::P2P, 1, ReaderFPGA::ReadPointsP2P);
    }
}


void FPGA::Start()
{
    InputStorage::PrepareForNewCycle();

    ProcessModeP2P();

    BUS_FPGA::Write(WR_PRED, LaunchFPGA::PredForWrite(), false);

    BUS_FPGA::Write(WR_START, 1, false);

    TrigPolarity::timeSwitch = 0;
}


void FPGA::Stop()
{
    Timer::Disable(TypeTimer::P2P);
    BUS_FPGA::Write(WR_STOP, 1, false);
}


void FPGA::OnPressStartStop()
{
    if (StateWorkFPGA::IsStop())
    {
        StateWorkFPGA::SetWait();

        Start();
    }
    else
    {
        StateWorkFPGA::SetStop();

        Stop();
    }
}


static int CalculateNumberCycles()
{
    int result = 1;

    if (TBASE_IS_RAND)
    {
        static const int num[] = { 25, 25, 20, 10, 5, 2 };

        result = num[TBASE];
    }

    return result;
}


void FPGA::Update()
{
    if (StateWorkFPGA::IsStop())
    {
        return;
    }

    flag.Read();

    for (int i = 0; i < CalculateNumberCycles(); i++)
    {
        if (flag.IsPredLaunchReady())
        {
            if (flag.IsTrigReady())
            {
                StateWorkFPGA::SetWork();

                if (flag.IsDataReady())
                {
                    Stop();
                    ReaderFPGA::ReadData();
                    StartIfNeed();
                }
            }
        }
    }
}


void FPGA::StartIfNeed()
{
    if (!START_MODE_IS_SINGLE)
    {
        Start();
    }
}


bool FPGA::IsRunning()
{
    return !StateWorkFPGA::IsStop();
}


void FPGA::StartAutoFind()
{
    AutoFinderFPGA::auto_find_in_progress = true;
}


void StopTemporaryPause()
{
    FPGA::temporary_pause = false;
    Timer::Disable(TypeTimer::TemporaryPauseFPGA);
}


void FPGA::SetTemporaryPause()
{
    temporary_pause = true;
    Timer::Enable(TypeTimer::TemporaryPauseFPGA, 500, StopTemporaryPause);
}
