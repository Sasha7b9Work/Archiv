#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Tester/Tester.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pTesterU;
extern const PageBase pTesterI;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_Polarity(bool)
{
    Tester::LoadPolarity();
}


DEF_CHOICE_2(   cPolarity,                                                                                     //--- ������-��������� - ���������� ---
    "����������", "Polarity",
    "���������� �������������� �����������",
    "Polarity of the test exposure",
    "+", "+",
    "-", "-",
    TESTER_POLARITY, pTesterU, FuncActive, OnChanged_Polarity, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageFunction::PageTester::OnChanged_Control(bool)
{
    if(TESTER_CONTROL_IS_U)
    {
        pointer = &pTesterU;
    }
    else
    {
        pointer = &pTesterI;
    }

    Tester::LoadStep();
}


DEF_CHOICE_2(   cControl,                                                                                      //--- ������-��������� - ���������� ---
    "����������", "Control",
    "��� �������������� �����������",
    "Type of test exposure",
    "����������", "Voltage",
    "���", "Current",
    TESTER_CONTROL, pTesterU, FuncActive, PageFunction::PageTester::OnChanged_Control, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Step(bool)
{
    Tester::LoadStep();
}

DEF_CHOICE_2(   cStepU,                                                                                                 //--- ������-��������� - ��� ---
    "���", "Step",
    "��� �������������� ����������",
    "Test voltage step",
    "100 ��", "100 mV",
    "500 ��", "500 mV",
    TESTER_STEP_U, pTesterU, FuncActive, OnChanged_Step, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cStepI,
    "���", "Step",
    "��� ��������������� ����",
    "Step test current",
    "4 ��",  "4 mA",
    "20 ��", "20 mA",
    TESTER_STEP_I, pTesterI, FuncActive, OnChanged_Step, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cViewMode,
    "���������", "Paint",
    "", "",
    "�����", "Lines",
    "�����", "Points",
    TESTER_VIEW_MODE, pTesterU, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_5(   cSmoothing,
    "�����������", "Smoothing",
    "", "",
    "1", "1",
    "2", "2",
    "3", "3",
    "4", "4",
    "5", "5",
    TESTER_NUM_SMOOTH, pTesterU, FuncActive, FuncChangedChoice, FuncDraw
)


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const PageBase *PageFunction::PageTester::pointer = &pTesterU;

void PageFunction::PageTester::Init()
{
    OnChanged_Control(true);
}

static void OnEnterExit_Tester(bool enter)
{
    Device::SetMode(enter ? Device::Mode::Tester : Device::Mode::Osci);
}


DEF_PAGE_5( pTesterU,
    "������", "TESTER",
    "", "",
    &cControl,
    &cStepU,
    &cPolarity,
    &cViewMode,
    &cSmoothing,
    Page::Name::Function_Tester, PageFunction::pointer, FuncActive, OnEnterExit_Tester, FuncDrawPage, FuncRegSetPage
)

DEF_PAGE_5(pTesterI,
    "������", "TESTER",
    "", "",
    &cControl,
    &cStepI,
    &cPolarity,
    &cViewMode,
    &cSmoothing,
    Page::Name::Function_Tester, PageFunction::pointer, FuncActive, OnEnterExit_Tester, FuncDrawPage, FuncRegSetPage
)
