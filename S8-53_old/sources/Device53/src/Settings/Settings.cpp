#include "Settings.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Panel.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGATypes.h"
#include "Utils/Math.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Log.h"
#include <string.h>


/** @addtogroup Settings
 *  @{
 */


static const Settings defaultSettings =
{
    0,                              // size
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
        ENumAverage_1,              // numAveraging
        Averaging_Accurately,       // modeAveraging
        ENumMinMax_1,               // enumMinMax
        Smoothing_Disable,          // smoothing
        NumSignalsInSec_25,         // num signals in one second
        A,                          // lastAffectedChannel
        ModeAccumulation_NoReset,   // modeAccumulation
        AM_Show,                    // altMarkers
        MenuAutoHide_None,          // menuAutoHide
        true,                       // showFullMemoryWindow
        false,                      // showStringNavigation
        LinkingRShift_Voltage,      // linkingRShift
        0                           // notUsed
    },
    0,  // notUsed2
    0,  // notUsed1
    // channels
    {
        {
            1.0f,                   // ����������� ����������
            RShiftZero,
            {{0}, {0}},             // rShiftAdd
            ModeCouple_AC,          // ModCouple
            Divider_1,              // Divider
            Range_500mV,            // range
            true,                   // enable
            false,                  // inverse
            false,                  // filtr
			0,                      // addShiftADC
            0,                      // notUsed0
            0,                      // notUsed1
            0                       // notUsed2
        },
        {
            1.0f,                   // ����������� ����������
            RShiftZero,
            {{0}, {0}},             // rShiftAdd
            ModeCouple_AC,          // ModCouple
            Divider_1,              // Divider
            Range_500mV,            // range
            true,                   // enable
            false,                  // inverse
            false,                  // filtr
			5,                      // addShiftADC
            0,                      // notUsed0;
            0,                      // notUsed1;
            0                       // notUsed2;
        }
        
    },
    // trig
    {
        StartMode_Auto,
        TrigSource_A,
        TrigPolarity_Front,
        TrigInput_Full,
        {TrigLevZero, TrigLevZero, TrigLevZero},
        ModeLongPressTrig_Auto,
        TrigModeFind_Hand
    },
    // time
    {
        TBase_200us,                // tBase
        0,                          // notUsed
        0,                          // set.time.tShiftRel
        FunctionTime_Time,          // timeDivXPos
        TPos_Center,                // tPos
        SampleType_Equal,           // sampleType
        PeakDet_Disabled,           // peakDet
        false,                      // selfRecorder
        FNP_1024                    // oldNumPoints
    },
    0,  // notUsed2
    0,  // notUsed3
    // cursors
    {
        { CursCntrl_Disable, CursCntrl_Disable },   // CursCntrl U
        { CursCntrl_Disable, CursCntrl_Disable },   // CursCntrl T
        A,                                          // source
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
    0,  // notUsed4
    // measures
    {
        MN_1,                       // measures number
        A_B,                        // source
        ModeViewSignals_AsIs,       // mode view signal
        {
            VoltageMax,     VoltageMin,  VoltagePic,      VoltageMaxSteady,  VoltageMinSteady,
            VoltageAverage, VoltageAmpl, VoltageRMS,      VoltageVybrosPlus, VoltageVybrosMinus,
            Period,         Freq,        TimeNarastaniya, TimeSpada,         DurationPlus
        },
        false,                      // show
        MeasuresField_Screen,       // ����� ��� ��������� ��������, ������� �� ������
        //{0, 200},                 // ��������� �������� �������� ���������� ��� ���� ���������
        //{372, 652},               // ��������� �������� �������� ������� ��� ���� ���������
        {50, 150},                  // ��������� �������� �������� ���������� ��� ���� ���������
        {422, 602},                 // ��������� �������� �������� ������� ��� ���� ���������
        CursCntrl_1,                // ��������� ������ �������� ����������
        CursCntrl_1,                // ��������� ������ �������� �������
        CursActive_T,               // ������� ������� �������.
        Meas_None                   // marked Measure
    },
    // math
    {
        ScaleFFT_Log,               // scaleFFT
        SourceFFT_A,                // sourceFFT
        WindowFFT_Rectangle,        // windowFFT
        FFTmaxDB_60,                // fftMaxDB
        Function_Sum,               // func
        0,
        {100, 256 - 100},
        1,
        1,
        1,
        1,
        false,
        FuncModeDraw_Disable,       // modeDrawMath
        ModeRegSet_Range,           // modeRegSet
        Range_50mV,
        Divider_1,                  // multiplier
        0,                          // notUsed0
        RShiftZero                  // rShift
    },
    // service
    {
        true,                       // screenWelcomeEnable
        true,                       // soundEnable
        Calibrator_Freq,            // calibratorEnable
        0,                          // IP-����� (��������)  WARN
        ColorScheme_WhiteLetters    // colorScheme
    },
    0,  // notUsed5
    // eth
    {
        0x8b, 0x2e, 0xaf, 0x8f, 0x13, 0x00, // mac
        192, 168, 1, 200,                   // ip
        7,
        255, 255, 255, 0,                   // netmask
        192, 168, 1, 1,                     // gateway
        false,                              // enable
        0                                   // notUsed
    },
    // common
    {
        0,          // countEnables
        0,          // countErasedFlashData
        0,          // countErasedFlashSettings
        0,          // workingTimeInSecs
        Russian,    // lang
        0,          // notUsed0
        0,          // notUsed1
        0           // notUsed2
    },
    // menu
    {
        {0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f},
        {   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0},
        true,       // notUsing
        0,          // isShown
        0           // notUsed
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
        false,                  // viewAlteraWrittingData
        false,                  // viewAllAlteraWrittingData
            
        0,                      // altShift
        {
            false,              // all
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
    0                           // crc32
};

Settings set;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::Load(bool _default)
{
    set = defaultSettings;
    
    CPU::FLASH_::LoadSettings();

    if (_default)                                                               // ���� ����� �������� ���������
    {
        int16 rShiftAdd[2][RangeSize][2];                                      // ������� ��������� �������������� ���������
        for (int chan = 0; chan < 2; chan++)
        {
            for (int i = 0; i < RangeSize; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    rShiftAdd[chan][i][j] = set.chan[chan].rShiftAdd[i][j];
                }
            }
        }

        int16  balanceADC0 = BALANCE_ADC_A;
        int16  balanceADC1 = BALANCE_ADC_B;
        int16  numAverageForRand = NUM_AVE_FOR_RAND;
        BalanceADCtype balanceType = BALANCE_ADC_TYPE;

        memcpy((void*)&set, (void*)(&defaultSettings), sizeof(set));            // ����� ��������� ���������� �� ���������

        for (int chan = 0; chan < 2; chan++)                                    // � ��������������� �������������� ���������
        {
            for (int i = 0; i < RangeSize; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    set.chan[chan].rShiftAdd[i][j] = rShiftAdd[chan][i][j];
                }
            }
        }
        BALANCE_ADC_A = balanceADC0;
        BALANCE_ADC_B = balanceADC1;
        NUM_AVE_FOR_RAND = numAverageForRand;
        BALANCE_ADC_TYPE = balanceType;
    }


    FPGA::LoadSettings();
    FPGA::SetNumSignalsInSec(sDisplay_NumSignalsInS());
    Panel::EnableLEDChannelA(sChannel_Enabled(A));
    Panel::EnableLEDChannelB(sChannel_Enabled(B));
    FPGA::SetNumberMeasuresForGates(NUM_MEAS_FOR_GATES);
    Menu::SetAutoHide(true);
    Display::ChangedRShiftMarkers();

    SETTINGS_IS_LOADED = 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save()
{
    CPU::FLASH_::SaveSettings();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Range& operator++(Range &range)
{
    range = (Range)((int)range + 1);
    return range;

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Range& operator--(Range &range)
{
    range = (Range)((int)range - 1);
    return range;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float MaxDBforFFT(FFTmaxDB maxDB)
{
    static const float arrayMAX_DB_FOR_FFT[] = {-40.0f, -60.0f, -80.0f};

    return arrayMAX_DB_FOR_FFT[maxDB];
}

/** @}
 */

