#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageTime.h"
#include "FPGA/FPGA.h"
#include "Menu/MenuItems.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Settings/Settings.h"
#endif


extern const PageBase pTime;


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Sample()
{
    return IN_RANDOM_MODE && !START_MODE_IS_SINGLE;
}

DEF_CHOICE_2(       cSample,                                                                                             //--- ��������� - ������� ---
    "�������", "Sampling",
    "\"��������\" - \n"
    "\"�������������\" -",
    "\"Real\" - \n"
    "\"Equals\" - ",
    "�������� �����", "Real",
    "�������������",  "Equals",
    SAMPLE_TYPE, pTime, IsActive_Sample, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_PeakDet()
{
    return (SET_TBASE >= TBase::MIN_PEAK_DET);
}

void PageTime::OnChanged_PeakDet(bool active)
{
    if (active)
    {
        PageMemory::OnChanged_Points(true);
    }
    else
    {
        Display::ShowWarning(Warning::TooSmallSweepForPeakDet);
    }
}

DEF_CHOICE_2(       cPeakDet,                                                                                            //--- ��������� - ��� ��� ---
    "��� ���", "Pic deat",
    "��������/��������� ������� ��������.",
    "Turns on/off peak detector.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_PEAKDET, pTime, IsActive_PeakDet, PageTime::OnChanged_PeakDet, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageTime::OnChanged_TPos(bool active)
{
    PageMemory::OnChanged_Points(active);
    FPGA::SetTShift(SET_TSHIFT);
}

DEF_CHOICE_3(cTPos,                                                                                                    //--- ��������� - �� ---
    "\x7b", "\x7b",
    "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����.",
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "����", "Left",
    "�����", "Center",
    "�����", "Right",
    TPOS, pTime, FuncActive, PageTime::OnChanged_TPos, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cDivRole,                                                                                       //--- ��������� - �-��� ��/��� ---
    "�-��� ��/���", "Funct Time/DIV"
    ,
    "����� ������� ��� ����� �����/���: � ������ ����� ���������� (����/���� � ��������� ����):\n"
    "1. \"�����\" - ��������� �������� �� �������.\n"
    "2. \"������\" - ����������� �� ������."
    ,
    "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
    "1. \"Time\" - change the time shift.\n"
    "2. \"Memory\" - moving from memory."
    ,
    "�����", "Time",
    "������", "Memory",
    TIME_DIV_XPOS, pTime, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cShiftXtype,                                                                                        //--- ��������� - �������� ---
    "��������", "Offset"
    ,
    "����� ����� ��������� �������� �� �����������\n1. \"�����\" - ����������� ���������� �������� � ��������.\n2. \"�������\" - ����������� "
    "��������� ������� �� ������."
    ,
    "Sets the mode of retaining the horizontal displacement\n1. \"Time\" - saved the asbolutic offset in seconds.\n2. \"Divisions\" - retained "
    "the position of the offset on the screen."
    ,
    "�����", "Time",
    "�������", "Divisions",
    LINKING_TSHIFT, pTime, FuncActive, FuncChangedChoice, FuncDraw
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTime::pointer = &pTime;


DEF_PAGE_5(         pTime,                                                                                                            // ��������� ///
    "����.", "SCAN",
    "�������� ��������� ��������.",
    "Contains scan settings.",
    &cSample,        // ��������� - �������
    &cPeakDet,       // ��������� - ��� ���
    &cTPos,          // ��������� - ��
    &cDivRole,       // ��������� - �-��� ��/���
    &cShiftXtype,    // ��������� - ��������
    Page::Name::Time, Menu::pageMain, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)
