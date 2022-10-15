#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageGrid;

const PageBase *PageDisplay::PageGrid::pointer = &pageGrid;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_4( cType,                                                                                                   //--- ������� - ����� - ��� ---
    "���", "Type",
    "����� ���� �����.",
    "Choice like Grid::",
    "��� 1", "Type 1",
    "��� 2", "Type 2",
    "��� 3", "Type 3",
    "��� 4", "Type 4",
    TYPE_GRID, pageGrid, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Grid_Brightness()
{
    PageDisplay::PageSettings::PageColors::colorTypeGrid.SetBrightness(BRIGHTNESS_GRID / 100.0f);
}

static void BeforeDraw_Grid_Brightness()
{
    PageDisplay::PageSettings::PageColors::colorTypeGrid.Init(false);
    BRIGHTNESS_GRID = (int16)(PageDisplay::PageSettings::PageColors::colorTypeGrid.brightness * 100.0f);
}

DEF_GOVERNOR( gBrightness,                                                                                         //--- ������� - ����� - ������� ---
    "�������", "Brightness",
    "������������� ������� �����.",
    "Adjust the brightness of the Grid::",
    BRIGHTNESS_GRID, 0, 100, pageGrid, FuncActive, OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(pageGrid,                                                                                                         //--- ������� - ����� ---
    "�����", "GRID",
    "�������� ��������� ����������� ������������ �����.",
    "Contains settings of display of a coordinate Grid::",
    &cType,                    ///< ������� - ����� - ���
    &gBrightness,              ///< ������� - ����� - �������
    Page::Name::Display_Grid, PageDisplay::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)
