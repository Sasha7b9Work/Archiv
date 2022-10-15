#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Settings/Settings.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageAccum;

const PageBase *PageDisplay::PageAccumulation::pointer = &pageAccum;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_REG_9(cAccum_Num,                                                                               //--- ������� - ���������� - ���������� ---
    "����������", "Number"
    ,
    "����� ������������ ���������� ��������� �������� �� ������. ���� � ��������� \"�����\" ������� \"�������������\", ����� ��������� ������ "
    "�������� ������ \"��������\"."
    "\"�������������\" - ������ ��������� ������� �� ������� �� ��� ���, ���� �� ����� ������ ������ \"��������\"."
    ,
    "Sets the maximum quantity of the last signals on the screen. If in control \"Mode\" it is chosen \"Infinity\", the screen is cleared only "
    "by pressing of the button \"Clear\"."
    "\"Infinity\" - each measurement remains on the display until the button \"Clear\" is pressed.",
    DISABLE_RU, DISABLE_EN,
    "2",        "2",
    "4",        "4",
    "8",        "8",
    "16",       "16",
    "32",       "32",
    "64",       "64",
    "128",      "128",
    "�������������", "Infinity",
    ENUM_ACCUM, pageAccum, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cAccum_Mode,                                                                                    //--- ������� - ���������� - ����� ---
    "�����", "Mode"
    ,
    "1. \"����������\" - ����� ���������� ��������� ���������� ��������� ���������� ������� �������. ���� ����� ������, ����� ������ �� ������� "
    "��� ���������� ������� ���������� ���������.\n"
    "2. \"�� ����������\" - �� ������� ������ ��������� �������� ��� ������� (� ������ �������� ������) ���������� ���������. ����������� �������� "
    "������� �������������� � ������������� ���������� ��������� ���������� ��������� ��� ���������� ������."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the display. This mode is convenient when memory "
    "isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of shortage of memory). Shortcoming "
    "is smaller speed and impossibility of accumulation of the set number of measurements at a lack of memory."
    ,
    "�� ����������", "Not to dump",
    "����������", "Dump",
    MODE_ACCUM, pageAccum, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Accum_Clear()
{
    return ENUM_ACCUM != Display::ENumAccum::_1 && !MODE_ACCUM_NO_RESET;
}

void PageDisplay::PageAccumulation::OnPress_Accumulation_Clear()
{
    NEED_FINISH_DRAW = 1;
}

DEF_BUTTON( bAccum_Clear,                                                                                    //--- ������� - ���������� - �������� ---
    "��������", "Clear",
    "������� ����� �� ����������� ��������.",
    "Clears the screen of the saved-up signals.",
    pageAccum, IsActive_Accum_Clear, PageDisplay::PageAccumulation::OnPress_Accumulation_Clear, FuncDraw
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Accum()
{
    return SET_TBASE > TBase::_20ns;
}

DEF_PAGE_3(pageAccum,                                                                                                   //--- ������� - ���������� ---
    "������", "ACCUM",
    "��������� ������ ����������� ��������� �������� �� ������",
    "Mode setting signals to display the last screen",
    &cAccum_Num,                          ///< ������� - ���������� - ����������
    &cAccum_Mode,                         ///< ������� - ���������� - �����
    &bAccum_Clear,                        ///< ������� - ���������� - ��������
    Page::Name::Display_Accumulation, PageDisplay::pointer, IsActive_Accum, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)
