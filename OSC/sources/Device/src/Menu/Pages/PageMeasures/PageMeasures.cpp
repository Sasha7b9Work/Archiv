#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Menu.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageMeasures;

const PageBase *PageMeasures::pointer = &pageMeasures;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageMeasures,                                                                                                          //--- ��������� ---
    "���������", "MEASURES",
    "������ � ���������� ��������� - ��������� � ��������������",
    "Access to measurement settings - cursor and automatic",
    PageMeasures::PageCursors::pointer,     ///< ��������� - �������
    PageMeasures::PageAuto::pointer,        ///< ��������� - �������
    Page::Name::Measures, Menu::pageMain, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

