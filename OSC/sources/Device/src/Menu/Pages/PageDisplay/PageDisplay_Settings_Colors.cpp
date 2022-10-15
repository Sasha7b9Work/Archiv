#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageColors;

const PageBase *PageDisplay::PageSettings::PageColors::pointer = &pageColors;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_ResetColors()
{
    Settings::ResetColors();
}

DEF_BUTTON( bReset,                                                                                   //--- ������� - ��������� - ����� - �������� ---
    "��������", "Reset",
    "����� ���� ������ �� �������� �� ���������",
    "Reset all colors to default values",
    pageColors, EmptyFuncBV, OnPress_ResetColors, EmptyFuncVII
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cScheme,                                                                          //--- ������� - ��������� - ����� - �������� ����� ---
    "�������� �����", "Color scheme",
    "��������� �������� �����",
    "Changing the color scheme",
    "����� 1", "Scheme 1",
    "����� 2", "Scheme 2",
    set.serv_colorScheme, pageColors, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeA = COLOR_TYPE(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::Channel(Chan::A))

DEF_GOVERNOR_COLOR( gcChannelA,                                                                        //--- ������� - ��������� - ����� - ����� 1 ---
    "����� 1", "Chan 1",
    "����� ����� ������ 1",
    "Choice of channel 1 color",
    PageDisplay::PageSettings::PageColors::colorTypeA, pageColors
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeB = COLOR_TYPE(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::Channel(Chan::B))

DEF_GOVERNOR_COLOR( gcChannelB,                                                                        //--- ������� - ��������� - ����� - ����� 2 ---
    "����� 2", "Chan 2",
    "����� ����� ������ 1",
    "Choice of channel 2 color",
    PageDisplay::PageSettings::PageColors::colorTypeB, pageColors
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeGrid = COLOR_TYPE(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::GRID)
DEF_GOVERNOR_COLOR( gcGrid,                                                                              //--- ������� - ��������� - ����� - ����� ---
    "�����", "Grid",
    "������������� ���� �����",
    "Sets the grid color",
    PageDisplay::PageSettings::PageColors::colorTypeGrid, pageColors
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageDisplay::PageSettings::PageColors::OnChanged_Settings_Colors_Background(bool)
{
    Color::InitGlobalColors();

    gcChannelA.ct->color = Color::Channel(Chan::A);
    gcChannelB.ct->color = Color::Channel(Chan::B);
    gcGrid.ct->color = Color::GRID;

    gcChannelA.ct->Init(true);
    gcChannelB.ct->Init(true);
    gcGrid.ct->Init(true);
}

DEF_CHOICE_2( cBackground,                                                                                 //--- ������� - ��������� - ����� - ��� ---
    "���", "Background",
    "����� ����� ����",
    "Choice of color of a background",
    "׸����", "Black",
    "�����", "White",
    BACKGROUND, pageColors, FuncActive, PageDisplay::PageSettings::PageColors::OnChanged_Settings_Colors_Background, FuncDraw
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \todo �������� �������������� ����� 1-�� � 2-�� �������
DEF_PAGE_6( pageColors,                                                                                          //--- ������� - ��������� - ����� ---
    "�����", "COLORS",
    "����� ������ �������",
    "The choice of colors display",
    &bReset,              ///< ������� - ��������� - ����� - ��������
    &cScheme,             ///< ������� - ��������� - ����� - �������� �����
    &gcChannelA,          ///< ������� - ��������� - ����� - ����� 1
    &gcChannelB,          ///< ������� - ��������� - ����� - ����� 2
    &gcGrid,              ///< ������� - ��������� - ����� - �����
    &cBackground,         ///< ������� - ��������� - ����� - ���
    Page::Name::Display_Settings_Colors, PageDisplay::PageSettings::pointer, FuncActive, EmptyPressPage
)
