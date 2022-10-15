#include "defines.h"
#include "Settings.h"
#include "Hardware/FLASH.h"
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Panel/Panel.h"
#include "Utils/Math.h"
#include "Display/Display.h"


#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Settings defaultSettings =
{
    // Display
    {
        5,                          // timeShowLevels
        512 - 140,                  // shiftInMemory
        5,                          // timeMessages
        100,                        // brightness
        20,                         // brightnessGrid
        {
            MAKE_COLOR(0, 0, 0),
            MAKE_COLOR(31, 63, 31),
            MAKE_COLOR(15, 31, 15),
            MAKE_COLOR(1, 50, 31),
            MAKE_COLOR(0, 63, 0),
            MAKE_COLOR(26, 54, 26),
            MAKE_COLOR(31, 44, 0),
            MAKE_COLOR(15, 22, 0),
            MAKE_COLOR(31, 63, 0),
            MAKE_COLOR(26, 34, 0),
            MAKE_COLOR(13, 17, 0),
            MAKE_COLOR(31, 51, 0),
            MAKE_COLOR(0, 0, 0),
            MAKE_COLOR(0, 0, 0),
            MAKE_COLOR(1, 40, 25),
            MAKE_COLOR(0, 51, 0)
        },
        ModeDrawSignal_Lines,       // modeDrawSignal
        TypeGrid_1,                 // typeGrid
        ENumAccum_1,                // numAccumulation
        ENumAverage_1,               // numAveraging
        Averaging_Accurately,       // modeAveraging
        NumMinMax_1,                // numMinMax
        DisplaySmooth_Disable,      // smoothing
        NumSignalsInSec_25,         // num signals in one second
        ChanA,                      // lastAffectedChannel
        ModeAccumulation_NoReset,   // modeAccumulation
        AM_Show,                    // altMarkers
        MenuAutoHide_None,          // menuAutoHide
        true,                       // showFullMemoryWindow
        false                       // showStringNavigation
    },
    // time
    {
        TBase_200us,
        0,                          // unUsed
        0,                          // set.time.tShiftRel
        FunctionTime_Time,
        TPos_Center,
        SampleType_Equal,
        PeakDet_Disabled,
        false,
        FNP_1024
    },
    0,
    0,
    // cursors
    {
        { CursCntrl_Disable, CursCntrl_Disable },   // CursCntrl U
        { CursCntrl_Disable, CursCntrl_Disable },   // CursCntrl T
        ChanA,                                      // source
        0,                                          // notUsed0
        0,                                          // notUsed1
        0,                                          // notUsed2
        { {60.0f,  140.0f}, {60.0f, 140.0f} },      // posCur U
        { {80.0f,  200.0f}, {80.0f, 200.0f} },      // posCur T
        { 80.0f,  80.0f },                          // ���������� ����� ��������� ���������� ��� 100%
        { 120.0f, 120.0f },                         // ���������� ����� ��������� ������� ��� 100%
        CursMovement_Points,                        // CursMovement
        CursActive_None,                            // CursActive
        { CursLookMode_None, CursLookMode_None },   // ����� �������� ��������.
        false,                                      // showFreq
        false,                                      // showCursors
        0,                                          // notUsed3
        0                                           // notUsed4
    },
    // memory
    {
        FNP_1024,
        ModeWork_Dir,
        FileNamingMode_Mask,
        "Signal_\x07\x03",      // \x07 - ��������, ��� ����� ������ ���� ��������� �����, \x03 - ������� �� ��� ����������
        "Signal01",
        0,
        {
            false
        },
        ModeShowIntMem_Both,    // modeShowIntMem
        false,                  // flashAutoConnect
        ModeBtnMemory_Menu,
        ModeSaveSignal_BMP
    },
    0,
    // math
    {
        ScaleFFT_Log,
        SourceFFT_Channel0,
        WindowFFT_Rectangle,
        FFTmaxDB_60,
        Function_Sum,
        0,
        {100, 256 - 100},
        1,
        1,
        1,
        1,
        false,
        FuncModeDraw_Disable,       // modeDrawMath
        ModeRegSet_Range,           // modeRegSet
        Range_50mV,                 // range
        Multiplier_1,               // multiplier
        0,                          // unUsed
        0                           // rShift
    },
    // service
    {
        true,                       // screenWelcomeEnable
        true,                       // soundEnable
        Calibrator_Freq,            // calibratorEnable
        0,                          // IP-����� (��������)  WARN
        ColorScheme_WhiteLetters    // colorScheme
    },
    0,                              // notUsed3
    // ethernet
    {
        0x8b, 0x2e, 0xaf, 0x8f, 0x13, 0x00, // mac
        192, 168, 1, 200,                   // ip
        7,
        255, 255, 255, 0,                   // netmask
        192, 168, 1, 1,                     // gateway
        false,                              // enable
        0                                   // notUsed0
    },
    // common
    {
        0,                      // countEnables
        0,                      // countErasedFlashData
        0,                      // countErasedFlashSettings
        0,                      // workingTimeInSecs
        Russian,                // language
        0,                      // notUsed0
        0,                      // notUsed1
        0                       // notUsed2
    },
    // debug
    {
        0,                      // numStrings
        0,                      // ������ ������ - 5
        0,                      // consoleInPause
        BalanceADC_Settings,    // balanceADCtype
        0,                      // notUsed0
        {0, 5},                 // balanceADC[2]
        StretchADC_Settings,    // stretchADCtype
        0,                      // notUsed1
        {128, 128},             // stretchADC[2]
        1000,                   // numMeasuresForGates
        0,                      // shiftT0
        false,                  // showStats
        0,                      // notUsed2
        6,                      // numAveForRand
        false,                  // view altera writting data
        false,                  // view all altera writting data
        0,                      // alt shift
        {
            false,              // ���������� �� ���� ����������
            false,              // flag
            false,              // rShift0
            false,              // rShift1
            false,              // trigLev
            {false, false},     // range[2]
            {false, false},     // chanParam[2]
            false,              // trigParam
            false,              // tShift
            false               // tBase
        }
    },
    0
};

Settings set;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::Load()
{
    set = defaultSettings;
}
