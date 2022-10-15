#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Osci/FrequencyCounter.h"
#include "Settings/Settings.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase ppFreqMeter;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_FreqMeter_Enable(bool)
{
    FrequencyCounter::Init();
}

static void OnChanged_FreqMeter_Frequency(bool)
{
    FrequencyCounter::LoadFreqSettings();
}

static void OnChanged_FreqMeter_Period(bool)
{
    FrequencyCounter::LoadPeriodSettings();
}


DEF_CHOICE_2(cFreqMeter_Enable,                                                                            //--- ������� - ���������� - ���������� ---
    "����������", "Freq meter",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    (FREQ_METER_ENABLED.value), ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(cFreqMeter_TimeF,                                                                          //--- ������� - ���������� - ����� ����� F ---
    "����� ����� F", "Time calc F",
    "��������� ������� �������� ��������� ������� - ��� ������ �����, ��� ������ �������� � ������ ����� ���������",
    "Allows to choose the accuracy of measurement of frequency - the more time, the accuracy more time of measurement is more",
    "100��", "100ms",
    "1�", "1s",
    "10�", "10ms",
    FREQ_METER_TIMECOUNTING, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Frequency, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(cFreqMeter_FreqClc,                                                                        //--- ������� - ���������� - ����� ������� ---
    "����� �������", "Timestamps",
    "����� ������� ���������� ������� ���������",
    "Choice of frequency of following of calculating impulses",
    "100���", "10MHz",
    "1���", "200MHz",
    "10���", "10MHz",
    "100���", "100MHz",
    FREQ_METER_FREQ_CLC, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Period, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(cFreqMeter_NumPeriods,                                                                   //--- ������� - ���������� - ���-�� �������� ---
    "��� ��������", "Num periods",
    "��������� ������� �������� ��������� ������� - ��� ������ �����, ��� ������ �������� � ������ ����� ���������",
    "Allows to choose the accuracy of measurement of period - the more time, the accuracy more time of measurement is more",
    "1", "1",
    "10", "10",
    "100", "100",
    FREQ_METER_NUM_PERIODS, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Period, FuncDraw
)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageFunction::PageFrequencyCounter::pointer = &ppFreqMeter;

const Choice *PageFunction::PageFrequencyCounter::GetChoiceTimeF()
{
    return (const Choice *)&cFreqMeter_TimeF;
}

const Choice *PageFunction::PageFrequencyCounter::GetChoiceFreqClc()
{
    return (const Choice *)&cFreqMeter_FreqClc;
}

const Choice *PageFunction::PageFrequencyCounter::GetChoiceNumPeriods()
{
    return (const Choice *)&cFreqMeter_NumPeriods;
}

DEF_PAGE_4(ppFreqMeter,                                                                                                 //--- ������� - ���������� ---
    "����������", "FREQ METER",
    "",
    "",
    &cFreqMeter_Enable,      // ��������� - ���������� - ����������
    &cFreqMeter_TimeF,       // ��������� - ���������� - ����� ����� F
    &cFreqMeter_FreqClc,     // ��������� - ���������� - ����� �������
    &cFreqMeter_NumPeriods,  // ��������� - ���������� - ���-�� ��������
    Page::Name::Function_FrequencyCounter, PageFunction::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)
