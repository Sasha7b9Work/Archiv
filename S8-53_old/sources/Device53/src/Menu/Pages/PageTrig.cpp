#include "defines.h"
#include "PageTrig.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageTrig
 *  @{
 */

extern const PageBase pTrig;
extern const PageBase mainPage;
extern const PageBase mpAutoFind;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageTrig::OnChanged_TrigMode(bool)
{
    FPGA::Stop(false);
    if (!START_MODE_IS_SINGLE)
    {
        FPGA::Start();
    }
}

DEF_CHOICE_3(       mcMode,                                                                                                    //--- ����� - ����� ---
    "�����", "Mode"
    ,
    "����� ����� �������:\n"
    "1. \"����\" - ������ ���������� �������������.\n"
    "2. \"������\" - ������ ���������� �� ������ �������������.\n"
    "3. \"�����������\" - ������ ���������� �� ���������� ������ ������������� ���� ���. ��� ���������� ��������� ����� ������ ������ ����/����."
    ,
    "Sets the trigger mode:\n"
    "1. \"Auto\" - start automatically.\n"
    "2. \"Standby\" - the launch takes place at the level of synchronization.\n"
    "3. \"Single\" - the launch takes place on reaching the trigger levelonce. For the next measurement is necessary to press the START/STOP."
    ,
    "���� ",       "Auto",
    "������",      "Wait",
    "�����������", "Single",
    START_MODE, pTrig, FuncActive, PageTrig::OnChanged_TrigMode, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Source(bool)
{
    FPGA::SetTrigSource(TRIGSOURCE);
}

DEF_CHOICE_3(       mcSource,                                                                                               //--- ����� - �������� ---
    "��������", "Source",
    "����� ��������� ������� �������������.",
    "Synchronization signal source choice.",
    "����� 1", "Channel 1",
    "����� 2", "Channel 2",
    "�������", "External",
    TRIGSOURCE, pTrig, FuncActive, OnChanged_Source, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Polarity(bool)
{
    FPGA::SetTrigPolarity(TRIG_POLARITY);
}

DEF_CHOICE_2(       mcPolarity,                                                                                           //--- ����� - ���������� ---
    "����������", "Polarity"
    ,
    "1. \"�����\" - ������ ���������� �� ������ ��������������.\n"
    "2. \"����\" - ������ ���������� �� ����� ��������������."
    ,
    "1. \"Front\" - start happens on the front of clock pulse.\n"
    "2. \"Back\" - start happens on a clock pulse cut."
    ,
    "�����", "Front",
    "����",  "Back",
   TRIG_POLARITY, pTrig, FuncActive, OnChanged_Polarity, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Input(bool)
{
    FPGA::SetTrigInput(TRIG_INPUT);
}

DEF_CHOICE_4(       mcInput,                                                                                                    //--- ����� - ���� ---
    "����", "Input"
    ,
    "����� ����� � ���������� �������������:\n"
    "1. \"��\" - ������ ������.\n"
    "2. \"��\" - �������� ����.\n"
    "3. \"���\" - ������ ������ ������.\n"
    "4. \"���\" - ������ ������� ������."
    ,
    "The choice of communication with the source of synchronization:\n"
    "1. \"SS\" - a full signal.\n"
    "2. \"AS\" - a gated entrance.\n"
    "3. \"LPF\" - low-pass filter.\n"
    "4. \"HPF\" - high-pass filter frequency."
    ,
    "��",  "Full",
    "��",  "AC",
    "���", "LPF",
    "���", "HPF",
    TRIG_INPUT, pTrig, FuncActive, OnChanged_Input, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcAutoFind_Mode,                                                                                   //--- ����� - ����� - ����� ---
    "�����", "Mode"
    ,
    "����� ������ ��������������� ������ �������������:\n"
    "1. \"������\" - ����� ������������ �� ������� ������ \"�����\" ��� �� ��������� � ������� 0.5� ������ �����, ���� "
    "����������� \"������\x99��� ���� �����\x99�����������\".\n"
    "2. \"��������������\" - ����� ������������ �������������."
    ,
    "Selecting the automatic search of synchronization:\n"
    "1. \"Hand\" - search is run on pressing of the button \"Find\" or on deduction during 0.5s the ����� button if it is "
    "established \"SERVICE\x99Mode long �����\x99 Autolevel\".\n"
    "2. \"Auto\" - the search is automatically."
    ,
    "������",         "Hand",
    "��������������", "Auto",
    TRIG_MODE_FIND, mpAutoFind, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_AutoFind_Search()
{
    return TRIG_MODE_FIND_IS_HAND;
}

static void OnPress_AutoFind_Search()
{
    FPGA::FindAndSetTrigLevel();
}

DEF_BUTTON(         mbAutoFind_Search,                                                                                 //--- ����� - ����� - ����� ---
    "�����", "Search",
    "���������� ����� ������ �������������.",
    "Runs for search synchronization level.",
    mpAutoFind, IsActive_AutoFind_Search, OnPress_AutoFind_Search, FuncDraw
)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         mpAutoFind,                                                                                                   // ����� - ����� ///
    "�����", "SEARCH",
    "���������� �������������� ������� ������ �������������.",
    "Office of the automatic search the trigger level.",
    mcAutoFind_Mode,    // ����� - ����� - �����
    mbAutoFind_Search,  // ����� - ����� - �����
    Page_Trig_Search, &pTrig, FuncActive, EmptyPressPage
)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pTrig,                                                                                                                // ����� ///
    "�����", "TRIG",
    "�������� ��������� �������������.",
    "Contains synchronization settings.",
    mcMode,         // ����� - �����
    mcSource,       // ����� - ��������
    mcPolarity,     // ����� - ����������
    mcInput,        // ����� - ����
    mpAutoFind,     // ����� - �����
    Page_Trig, &mainPage, FuncActive, EmptyPressPage
)


/** @}  @}
 */
