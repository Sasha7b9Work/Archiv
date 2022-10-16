// 2021/05/12 17:08:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Menu/Menu.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


void PageDisplay::PageSettings::LoadBrightness()
{
    HAL_DAC1::SetBrightness(BRIGHTNESS_DISPLAY);
}


DEF_GOVERNOR(mgBrightnessDisplay, PageDisplay::PageSettings::self,
    "�������", "Brightness",
    "��������� ������� �������� �������",
    "Setting the brightness of the display",
    BRIGHTNESS_DISPLAY, 0, 100, nullptr, PageDisplay::PageSettings::LoadBrightness
)


DEF_GOVERNOR(mgLevels, PageDisplay::PageSettings::self,
    "������", "Levels"
    ,
    "����� �����, � ������� �������� ����� �������� ����� ������� ���������� �� ������ ������� ��������������� ����� "
    "������ ��������"
    ,
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary "
    "label offset level"
    ,
    TIME_SHOW_LEVELS, 0, 125, nullptr, nullptr
)


DEF_GOVERNOR(mgTimeMessages, PageDisplay::PageSettings::self,
    "�����", "Time",
    "��������� �������, � ������� �������� ��������� ����� ���������� �� ������",
    "Set the time during which the message will be on the screen",
    TIME_MESSAGES, 1, 99, nullptr, nullptr
)


DEF_CHOICE_2(mcShowStringNavigation, PageDisplay::PageSettings::self,
    "������ ����", "Path menu",
    "��� ������ \n����������\n ����� ������ ������ ��������� ������ ���� �� ������� �������� ����",
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "��������", "Hide",
    "����������", "Show",
    SHOW_STRING_NAVIGATION, nullptr, nullptr, nullptr
)


static void OnChanged_ShowAltMarkers(bool)
{
    Display::ChangedRShiftMarkers();
}


DEF_CHOICE_3(mcShowAltMarkers, PageDisplay::PageSettings::self,
    "���. �������", "Alt. markers"
    ,
    "������������� ����� ����������� �������������� �������� ������� �������� � �������������:\n"
    "\"��������\" - �������������� ������� �� ������������,\n"
    "\"����������\" - �������������� ������� ������������ ������,\n"
    "\"����\" - �������������� ������� ������������ � ������� 5 ��� ����� �������� ����� �������� ������ �� ���������� "
    "��� ������ �������������"
    ,
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or "
    "trigger level"
    ,
    "��������",   "Hide",
    "����������", "Show",
    "����",       "Auto",
    MODE_ALT_MARKERS, nullptr, OnChanged_ShowAltMarkers, nullptr
)


static void OnChanged_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}


DEF_CHOICE_6(mcAutoHide, PageDisplay::PageSettings::self,
    "��������", "Hide"
    ,
    "��������� ����� ���������� ������� ������ ��� �������� �����, �� ��������� �������� ���� ������������� ��������� "
    "� ������"
    ,
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear"
    ,
    "�������",      "Never",
    "����� 5 ���",  "Through 5 s",
    "����� 10 ���", "Through 10 s",
    "����� 15 ���", "Through 15 s",
    "����� 30 ���", "Through 30 s",
    "����� 60 ���", "Through 60 s",
    MENU_AUTO_HIDE, nullptr, OnChanged_AutoHide, nullptr
)


DEF_PAGE_7(pageSettings, PageDisplay::self, NamePage::DisplaySettings,
    "���������", "SETTINGS",
    "�������������� ��������� �������",
    "Additional display settings",
    *PageDisplay::PageSettings::PageColors::self,
    mgBrightnessDisplay,
    mgLevels,
    mgTimeMessages,
    mcShowStringNavigation,
    mcShowAltMarkers,
    mcAutoHide,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDisplay::PageSettings::self = &pageSettings;
