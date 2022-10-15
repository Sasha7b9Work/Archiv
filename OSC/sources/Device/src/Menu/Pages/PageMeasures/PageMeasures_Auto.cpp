#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Settings/SettingsTime.h"
#include "Utils/Measure.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageMeasures.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageAuto;

const PageBase *PageMeasures::PageAuto::pointer = &pageAuto;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(       cIsShow,                                                                                //--- ��������� - ������� - ���������� ---
    "����������", "Show",
    "�������� ��� �� �������� ��������� �� �����",
    "Output or output measurements on screen",
    "���", "No",
    "��",  "Yes",
    SHOW_MEASURES, pageAuto, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Number()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_7(       cNumber,                                                                                //--- ��������� - ������� - ���������� ---
    "����������", "Number"
    ,
    "������������� ������������ ���������� ��������� ���������:\n"
    "\"1\" - ���� ���������\n"
    "\"2\" - ��� ���������\n"
    "\"1�5\" - 1 ������ � ����� �����������\n"
    "\"2�5\" - 2 ������ � ����� ����������� � ������\n"
    "\"3�5\" - 3 ������ � ����� ����������� � ������\n"
    "\"6x1\" - 6 ����� �� ������ ��������� � ������\n"
    "\"6�2\" - 6 ����� �� ��� ��������� � ������"
    ,
    "Sets the maximum number of output measurements:\n"
    "\"1\" - one measurement\n"
    "\"2\" - two measurements\n"
    "\"1x5\" - 1 line with the five dimensions\n"
    "\"2x5\" - two rows with five measurements in each\n"
    "\"3x5\" - 3 lines with five measurements in each"
    "\"6x1\" - 6 lines, one in each dimension\n"
    "\"6x2\" - 6 lines of two dimensions in each\n",
    "1", "1",
    "2", "2",
    "1x5", "1x5",
    "2x5", "2x5",
    "3x5", "3x5",
    "6x1", "6x1",
    "6x2", "6x2",
    NUM_MEASURES, pageAuto, IsActive_Number, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Channels()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_3(       cChannels,                                                                                  //--- ��������� - ������� - ������ ---
    "������", "Channels",
    "�� ����� ������� �������� ���������",
    "Which channels to output measurement",
    "1",     "1",
    "2",     "2",
    "1 � 2", "1 and 2",
    SOURCE_MEASURES, pageAuto, IsActive_Channels, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
static bool IsActive_Mode()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_2(       cMode,                                                                                         //--- ��������� - ������� - ��� ---
    "���", "View",
    "��������� ��� ��� ���� ������ ������� ��� ���������� ���������� ��� ���������� ���������",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "��� ����",  "As is",
    "���������", "Reduce",
    MODE_VIEW_SIGNALS, pageAuto, IsActive_Mode, FuncChangedChoice, FuncDraw
)
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4(         pageAuto,                                                                                               //--- ��������� - ���� ---
    "�������", "AUTO",
    "������ � ���������� �������������� ���������",
    "Access to automatic measurement settings",
    &cIsShow,                           ///< ��������� - ������� - ����������
    &cNumber,                           ///< ��������� - ������� - ����������
    &cChannels,                         ///< ��������� - ������� - ������
    PageMeasures::PageAuto::PageTune::pointer,    ///< ��������� - ������� - ���������
    //&cMode,                           ///< ��������� - ������� - ���
    Page::Name::Measures_Auto, PageMeasures::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)
