#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageSettings;

const PageBase *PageDisplay::PageSettings::pointer = &pageSettings;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_Settings_Brightness()
{
    Painter::SetBrightnessDisplay(BRIGHTNESS_DISPLAY);
}

DEF_GOVERNOR( gBrightness,                                                                                     //--- ������� - ��������� - ������� ---
    "�������", "Brightness",
    "��������� ������� �������� �������",
    "Setting the brightness of the display",
    BRIGHTNESS_DISPLAY, 0, 100, pageSettings, FuncActive, OnChanged_Settings_Brightness, FuncBeforeDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gLevels,                                                                                          //--- ������� - ��������� - ������ ---
    "������", "Levels",
    "����� �����, � ������� �������� ����� �������� ����� ������� ���������� �� ������ ������� ��������������� ����� ������ ��������",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    TIME_SHOW_LEVELS, 0, 125, pageSettings, FuncActive, FuncChanged, FuncBeforeDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gTime,                                                                                             //--- ������� - ��������� - ����� ---
    "�����", "Time",
    "��������� �������, � ������� �������� ��������� ����� ���������� �� ������",
    "Set the time during which the message will be on the screen",
    TIME_MESSAGES, 1, 99, pageSettings, FuncActive, FuncChanged, FuncBeforeDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3( cStringNavigation,                                                                           //--- ������� - ��������� - ������ ���� ---
    "������ ����", "Path menu",
    "��� ������ \n����������\n ����� ������ ������ ��������� ������ ���� �� ������� �������� ����", /// \todo ��������� �������
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "��������", "Temporary",    /// \todo ��������� �������
    "������", "All",
    "�������", "None",
    SHOW_STRING_NAVI, pageSettings, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_AltMarkers(bool active)
{
    Display::ChangedRShiftMarkers(active);
}

DEF_CHOICE_3( cAltMarkers,                                                                                //--- ������� - ��������� - ���. ������� ---
    "���. �������", "Alt. markers",
    "������������� ����� ����������� �������������� �������� ������� �������� � �������������:\n"
    "\"��������\" - �������������� ������� �� ������������,\n"
    "\"����������\" - �������������� ������� ������������ ������,\n"
    "\"����\" - �������������� ������� ������������ � ������� 5 ��� ����� �������� ����� �������� ������ �� ���������� ��� ������ �������������",
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level"
    ,
    "��������", "Hide",
    "����������", "Show",
    "����", "Auto",
    ALT_MARKERS, pageSettings, FuncActive, OnChanged_Settings_AltMarkers, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}

DEF_CHOICE_6( cAutoHide,                                                                                      //--- ������� - ��������� - �������� ---
    "��������", "Hide",
    "��������� ����� ���������� ������� ������ ��� �������� �����, �� ��������� �������� ���� ������������� ��������� � ������",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "�������", "Never",
    "����� 5 ���", "Through 5 s",
    "����� 10 ���", "Through 10 s",
    "����� 15 ���", "Through 15 s",
    "����� 30 ���", "Through 30 s",
    "����� 60 ���", "Through 60 s",
    MENU_AUTO_HIDE, pageSettings, FuncActive, OnChanged_Settings_AutoHide, FuncDraw
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7( pageSettings,                                                                                                //--- ������� - ��������� ---
    "���������", "SETTINGS",
    "�������������� ��������� �������",
    "Additional display settings",
    PageDisplay::PageSettings::PageColors::pointer,     ///< ������� - ��������� - �����
    &gBrightness,                                       ///< ������� - ��������� - �������
    &gLevels,                                           ///< ������� - ��������� - ������
    &gTime,                                             ///< ������� - ��������� - �����
    &cStringNavigation,                                 ///< ������� - ��������� - ������ ����
    &cAltMarkers,                                       ///< ������� - ��������� - ���. �������
    &cAutoHide,                                         ///< ������� - ��������� - ��������
    Page::Name::Display_Settings, PageDisplay::pointer, FuncActive, EmptyPressPage
)
