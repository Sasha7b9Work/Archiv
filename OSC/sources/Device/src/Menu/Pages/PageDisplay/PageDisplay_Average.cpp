#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageAverage;

const PageBase *PageDisplay::PageAverage::pointer = &pageAverage;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_REG_10( cAverage_Num,                                                                           //--- ������� - ���������� - ���������� ---
    "����������", "Number",
    "����� ���������� ��������� ���������, �� ������� ������������ ����������.",
    "Sets number of the last measurements on which averaging is made.",
    DISABLE_RU, DISABLE_EN,
    "2", "2",
    "4", "4",
    "8", "8",
    "16", "16",
    "32", "32",
    "64", "64",
    "128", "128",
    "256", "256",
    "512", "512",
    ENUM_AVE, pageAverage, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cAverage_Mode,                                                                                    //--- ������� - ���������� - ����� ---
    "�����", "Mode"
    ,
    "1. \"�����\" - ������ ����� ����������, ����� � ������� ��������� ������ ��������� �������.\n"
    "2. \"��������������\" - ��������������� ����� ����������. ����� ����� ������������, ����� ������ ���������� ��������� �������, ��� ����� "
    "����������� � ������."
    ,
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory."
    ,
    "�����", "Accurately",
    "��������������", "Around",
    MODE_AVE, pageAverage, FuncActive, FuncChangedChoice, FuncDraw
)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Average()
{
    return true;
}

DEF_PAGE_2( pageAverage,                                                                                                //--- ������� - ���������� ---
    "������.", "AVERAGE",
    "��������� ������ ���������� �� ��������� ����������.",
    "Settings of the mode of averaging on the last measurements.",
    &cAverage_Num,         ///< ������� - ���������� - ����������
    &cAverage_Mode,        ///< ������� - ���������� - �����
    Page::Name::Display_Average, PageDisplay::pointer, IsActive_Average, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)
