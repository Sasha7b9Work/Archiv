#include "defines.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"
#include "PageMemory.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/FPGA.h"
#include "Log.h"
#include "Display/Display.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Math.h"
#include "Hardware/FSMC.h"
#include "Display/Grid.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageTime
 *  @{
 */


static const Choice mcSample;           // ��������� - �������
static bool  IsActive_Sample(void);
static const Choice mcPeakDet;          // ��������� - ��� ���
static bool  IsActive_PeakDet(void);
void        OnChanged_PeakDet(bool active);
static const Choice mcTPos;             // ��������� - To
void        OnChanged_TPos(bool active);
static const Choice mcSelfRecorder;     // ��������� - ���������
static bool  IsActive_SelfRecorder(void);
static const Choice mcDivRole;          // ��������� - �-��� ��/���


extern Page mainPage;


// ��������� //////////////////////////////////////////////////////////////////////////////////////////////////////////
const Page mpTime =      ///< ���������
{
    Item_Page, &mainPage, 0,
    {
        "���������", "SCAN",
        "�������� ��������� ��������.",
        "Contains scan settings."
    },
    Page_Time,
    {
        (void*)&mcSample,       // ��������� - �������
        (void*)&mcPeakDet,      // ��������� - ��� ���
        (void*)&mcTPos,         // ��������� - To
        (void*)&mcSelfRecorder, // ��������� - ���������
        (void*)&mcDivRole       // ��������� - �-��� ��/���
    }
};

// ��������� - ������� -------------------------------------------------------------------------------------------------------------------------------
static const Choice mcSample =
{
    Item_Choice, &mpTime, IsActive_Sample,
    {
        "�������", "Sampling"
        ,
        "\"��������\" - \n"
        "\"�������������\" -"
        ,
        "\"Real\" - \n"
        "\"Equals\" - "
    },
    {
        {"�������� �����",  "Real"},
        {"�������������",   "Equals"}
    },
    (int8*)&SAMPLE_TYPE
};

static bool IsActive_Sample(void)
{
    return sTime_RandomizeModeEnabled();
}

// ��������� - ��� ��� -------------------------------------------------------------------------------------------------------------------------------
static const Choice mcPeakDet =
{
    Item_Choice, &mpTime, IsActive_PeakDet,
    {
        "��� ���", "Pic deat",
        "��������/��������� ������� ��������.",
        "Turns on/off peak detector."
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
        /* , {"�������",   "Average"} */
    },
    (int8*)&PEAKDET, OnChanged_PeakDet
};

static bool IsActive_PeakDet(void)
{
    return (TBASE >= MIN_TBASE_PEC_DEAT);
}

void OnChanged_PeakDet(bool active)
{
    if (active)
    {
        FPGA_SetPeackDetMode(PEAKDET);
        FPGA_SetTBase(TBASE);
        if (PEAKDET_IS_DISABLE)
        {
            int8 shift[2][3] =
            {
                {0, BALANCE_SHIFT_ADC_A, BALANCE_ADC_A},
                {0, BALANCE_SHIFT_ADC_B, BALANCE_ADC_B}
            };

            FPGA_WriteToHardware(WR_ADD_RSHIFT_DAC1, shift[0][BALANCE_ADC_TYPE], false);
            FPGA_WriteToHardware(WR_ADD_RSHIFT_DAC2, shift[1][BALANCE_ADC_TYPE], false);
        }
        else
        {
            FPGA_WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // ������-�� ��� ������� ��������� �������� ����������. ��� ��� � ������������.
            FPGA_WriteToHardware(WR_ADD_RSHIFT_DAC2, 3, false);
        }
        if (PEAKDET_IS_DISABLE)
        {
            int centerX = SHIFT_IN_MEMORY + GridWidth() / 2;
            SHIFT_IN_MEMORY = centerX * 2 - GridWidth() / 2;
            ENUM_POINTS = set.time.oldNumPoints;
            ChangeC_Memory_NumPoints(true);
        }
        else if (PEAKDET_IS_ENABLE)
        {
            int centerX = SHIFT_IN_MEMORY + GridWidth() / 2;
            LIMITATION(SHIFT_IN_MEMORY, centerX / 2 - GridWidth() / 2, 0, sMemory_GetNumPoints(false) - GridWidth());
            ChangeC_Memory_NumPoints(true);
        }
    }
    else
    {
        Display_ShowWarningBad(TooSmallSweepForPeakDet);
    }
}

// ��������� - To ------------------------------------------------------------------------------------------------------------------------------------
static const Choice mcTPos =
{
    Item_Choice, &mpTime, 0,
    {
        "\x7b", "\x7b",
        "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����.",
        "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge."
    },
    {
        {"����",    "Left"},
        {"�����",   "Center"},
        {"�����",   "Right"}
    },
    (int8*)&TPOS, OnChanged_TPos
};

void OnChanged_TPos(bool active)
{
    ChangeC_Memory_NumPoints(active);
    FPGA_SetTShift(TSHIFT);
}

// ��������� - ��������� -----------------------------------------------------------------------------------------------------------------------------
static const Choice mcSelfRecorder =
{
    Item_Choice, &mpTime, IsActive_SelfRecorder,
    {
        "���������", "Self-Recorder",
        "��������/��������� ����� ���������. ���� ����� �������� �� ��������� 20��/��� � ����� ���������.",
        "Turn on/off the recorder. This mode is available for scanning 20ms/div and slower."
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&SELFRECORDER
};

static bool IsActive_SelfRecorder(void)
{
    return TBASE >= MIN_TBASE_P2P;
}

// ��������� - �-��� ��/��� --------------------------------------------------------------------------------------------------------------------------
static const Choice mcDivRole =
{
    Item_Choice, &mpTime, 0,
    {
        "�-��� ��/���", "Func Time/DIV"
        ,
        "����� ������� ��� ����� �����/���: � ������ ����� ���������� (����/���� � ��������� ����):\n"
        "1. \"�����\" - ��������� �������� �� �������.\n"
        "2. \"������\" - ����������� �� ������."
        ,
        "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
        "1. \"Time\" - change the time shift.\n"
        "2. \"Memory\" - moving from memory."
    },
    {
        {"�����",   "Time"},
        {"������",  "Memory"}
    },
    (int8*)&TIME_DIV_XPOS
};


/** @}  @}
 */
