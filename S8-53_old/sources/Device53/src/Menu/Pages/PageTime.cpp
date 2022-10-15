#include "defines.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"
#include "PageMemory.h"
#include "PageTime.h"
#include "FPGA/FPGA.h"
#include "Log.h"
#include "Display/Display.h"
#include "Utils/Math.h"
#include "Hardware/CPU.h"
#include "Display/Grid.h"
#include "Utils/Dictionary.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageTime
 *  @{
 */


extern const PageBase pTime;
extern const PageBase mainPage;

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Sample()
{
    return sTime_RandomizeModeEnabled();
}

DEF_CHOICE_2(       mcSample,                                                                                            //--- ��������� - ������� ---
    "�������", "Sampling",
    "\"��������\" - \n"
    "\"�������������\" -",
    "\"Real\" - \n"
    "\"Equals\" - ",
    "�������� �����", "Real",
    "�������������",  "Equals",
   SAMPLE_TYPE, pTime, IsActive_Sample, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_PeakDet()
{
    return (SET_TBASE >= MIN_TBASE_PEC_DEAT);
}

void PageTime::OnChanged_PeakDet(bool active)
{
    if (active)
    {
        FPGA::SetPeackDetMode(SET_PEAKDET);
        FPGA::SetTBase(SET_TBASE);
        if (SET_PEAKDET_IS_DISABLED)
        {
            int8 shift[2][3] =
            {
                {0, (int8)SET_BALANCE_ADC_A, (int8)BALANCE_ADC_A},
                {0, (int8)SET_BALANCE_ADC_B, (int8)BALANCE_ADC_B}
            };

            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shift[0][BALANCE_ADC_TYPE], false);
            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shift[1][BALANCE_ADC_TYPE], false);
        }
        else
        {
            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // ������-�� ��� ������� ��������� �������� ����������. ��� ��� � ������������.
            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, 3, false);
        }
        if (SET_PEAKDET_IS_DISABLED)
        {
            int centerX = SHIFT_IN_MEMORY + Grid::Width() / 2;
            SHIFT_IN_MEMORY = (int16)(centerX * 2 - (Grid::Width() / 2));
            ENUM_POINTS = set.time.oldNumPoints;
            ChangeC_Memory_NumPoints(true);
        }
        else if (SET_PEAKDET_IS_ENABLED)
        {
            int centerX = SHIFT_IN_MEMORY + Grid::Width() / 2;
            SHIFT_IN_MEMORY = (int16)(centerX / 2 - Grid::Width() / 2);
            Limitation<int16>(&SHIFT_IN_MEMORY, 0, (int16)(sMemory_GetNumPoints(false) - Grid::Width()));
            ChangeC_Memory_NumPoints(true);
        }
    }
    else
    {
        Display::ShowWarningBad(TooSmallSweepForPeakDet);
    }
}

DEF_CHOICE_2(       mcPeakDet,                                                                                           //--- ��������� - ��� ��� ---
    "��� ���", "Pic deat",
    "��������/��������� ������� ��������.",
    "Turns on/off peak detector.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_PEAKDET, pTime, IsActive_PeakDet, PageTime::OnChanged_PeakDet, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageTime::OnChanged_TPos(bool active)
{
    ChangeC_Memory_NumPoints(active);
    FPGA::SetTShift(SET_TSHIFT);
}

DEF_CHOICE_3(       mcTPos,                                                                                                   //--- ��������� - To ---
    "\x7b", "\x7b",
    "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����.",
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "����",  "Left",
    "�����", "Center",
    "�����", "Right",
    TPOS, pTime, FuncActive, PageTime::OnChanged_TPos, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SelfRecorder()
{
    return SET_TBASE >= MIN_TBASE_P2P;
}

DEF_CHOICE_2(       mcSelfRecorder,                                                                                    //--- ��������� - ��������� ---
    "���������", "Self-Recorder",
    "��������/��������� ����� ���������. ���� ����� �������� �� ��������� 20��/��� � ����� ���������.",
    "Turn on/off the recorder. This mode is available for scanning 20ms/div and slower.",
    ENABLE_RU,  ENABLE_EN,
    DISABLE_RU, DISABLE_EN,
    SELFRECORDER, pTime, IsActive_SelfRecorder, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcDivRole,                                                                                      //--- ��������� - �-��� ��/��� ---
    "�-��� ��/���", "Func Time/DIV"
    ,
    "����� ������� ��� ����� �����/���: � ������ ����� ���������� (����/���� � ��������� ����):\n"
    "1. \"�����\" - ��������� �������� �� �������.\n"
    "2. \"������\" - ����������� �� ������."
    ,
    "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
    "1. \"Time\" - change the time shift.\n"
    "2. \"Memory\" - moving from memory.",
    "�����",  "Time",
    "������", "Memory",
    TIME_DIV_XPOS, pTime, FuncActive, FuncChangedChoice, FuncDraw
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pTime,                                                                                                            // ��������� ///
    "���������", "SCAN",
    "�������� ��������� ��������.",
    "Contains scan settings.",
    mcSample,       // ��������� - �������
    mcPeakDet,      // ��������� - ��� ���
    mcTPos,         // ��������� - To
    mcSelfRecorder, // ��������� - ���������
    mcDivRole,      // ��������� - �-��� ��/���
    Page_Time, &mainPage, FuncActive, EmptyPressPage
)


/** @}  @}
 */
