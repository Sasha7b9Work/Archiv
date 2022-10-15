#include "Menu/MenuItems.h"
#include "defines.h"
#include "Menu/Menu.h"
#include "Tables.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "FPGA/DataStorage.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Utils/Dictionary.h"


/** @addtogroup Menu
 *  @{
 *  @defgroup PageDisplay
 *  @{
 */

extern const PageBase mainPage;
extern const PageBase pDisplay;
extern const PageBase mspAccumulation;
extern const PageBase mspAveraging;
extern const PageBase mspGrid;
extern const PageBase mspSettings_Colors;
extern const PageBase mspSettings;

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcMapping,                                                                                         //--- ������� - ����������� ---
    "�����������", "View",
    "����� ����� ����������� �������.",
    "Sets the display mode signal.",
    "������", "Vector",
    "�����",  "Points",
    MODE_DRAW_SIGNAL, pDisplay, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_9(   mcAccumulation_Number,                                                                    // ������� - ���������� - ���������� ---
    "����������", "Number"
    ,
    "����� ������������ ���������� ��������� �������� �� ������. ���� � ��������� \"�����\" ������� \"�������������\", ����� ��������� ������ "
    "�������� ������ \"��������\"."
    "\"�������������\" - ������ ��������� ������� �� ������� �� ��� ���, ���� �� ����� ������ ������ \"��������\"."
    ,
    "Sets the maximum quantity of the last signals on the screen. If in control \"Mode\" it is chosen \"Infinity\", the screen is cleared only "
    "by pressing of the button \"Clear\"."
    "\"Infinity\" - each measurement remains on the display until the button \"Clear\" is pressed.",
    DISABLE_RU,      DISABLE_EN,
    "2",             "2",
    "4",             "4",
    "8",             "8",
    "16",            "16",
    "32",            "32",
    "64",            "64",
    "128",           "128",
    "�������������", "Infinity",
    ENUM_ACCUM, mspAccumulation, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcAccumulation_Mode,                                                                           // ������� - ���������� - ����� ---
    "�����", "Mode"
    ,
    "1. \"����������\" - ����� ���������� ��������� ���������� ��������� ���������� ������� �������. ���� ����� ������, ����� ������ �� ������� "
    "��� ���������� ������� ���������� ���������.\n"
    "2. \"�� ����������\" - �� ������� ������ ��������� �������� ��� ������� (� ������ �������� ������) ���������� ���������. ����������� �������� "
    "������� �������������� � ������������� ���������� ��������� ���������� ��������� ��� ���������� ������."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the Display:: This mode is convenient when memory "
    "isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of shortage of memory). Shortcoming "
    "is smaller speed and impossibility of accumulation of the set number of measurements at a lack of memory.",
    "�� ����������", "Not to dump",
    "����������",    "Dump",
    MODE_ACCUM, mspAccumulation, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Accumulation_Clear()
{
    return ENUM_ACCUM_IS_INFINITY;
}

void OnPress_Accumulation_Clear()
{
    Display::Redraw();
}

DEF_BUTTON(mcAccumulation_Clear,                                                                       // ������� - ���������� - �������� ///
    "��������", "Clear",
    "������� ����� �� ����������� ��������.",
    "Clears the screen of the saved-up signals.",
    mspAccumulation, IsActive_Accumulation_Clear, OnPress_Accumulation_Clear, FuncDraw
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Accumulation()
{
    return SET_TBASE > TBase_50ns;
}

DEF_PAGE_3(         mspAccumulation,                                                                                       // ������� - ���������� ///
    "����������", "ACCUMULATION",
    "��������� ������ ����������� ��������� �������� �� ������.",
    "Mode setting signals to display the last screen.",
    mcAccumulation_Number, // ������� - ���������� - ����������
    mcAccumulation_Mode,   // ������� - ���������� - �����
    mcAccumulation_Clear,  // ������� - ���������� - ��������
    Page_Display_Accum, &pDisplay, IsActive_Accumulation, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(  mcAveraging_Number,                                                                       // ������� - ���������� - ���������� ---
    "����������", "Number",
    "����� ���������� ��������� ���������, �� ������� ������������ ����������.",
    "Sets number of the last measurements on which averaging is made.",
    DISABLE_RU, DISABLE_EN,
    "2",   "2",
    "4",   "4",
    "8",   "8",
    "16",  "16",
    "32",  "32",
    "64",  "64",
    "128", "128",
    "256", "256",
    "512", "512",
    ENUM_AVE, mspAveraging, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcAveraging_Mode,                                                                              // ������� - ���������� - ����� ---
    "�����", "Mode"
    ,
    "1. \"�����\" - ������ ����� ����������, ����� � ������� ��������� ������ ��������� �������.\n"
    "2. \"��������������\" - ��������������� ����� ����������. ����� ����� ������������, ����� ������ ���������� ��������� �������, ��� ����� "
    "����������� � ������."
    ,
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory.",
    "�����",          "Accurately",
    "��������������", "Around",
    MODE_AVE, mspAveraging, FuncActive, FuncChangedChoice, FuncDraw
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Averaging()
{
    return true;
}

DEF_PAGE_2(         mspAveraging,                                                                                          // ������� - ���������� ///
    "����������", "AVERAGE",
    "��������� ������ ���������� �� ��������� ����������.",
    "Settings of the mode of averaging on the last measurements.",
    mcAveraging_Number,     // ������� - ���������� - ����������
    mcAveraging_Mode,       // ������� - ���������� - �����
   Page_Display_Average, &pDisplay, IsActive_Averaging, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_MinMax()
{
    return SET_TBASE > TBase_50ns;
}

static void OnChanged_MinMax(bool active)
{
}

DEF_CHOICE_8(       mcMinMax,                                                                                                // ������� - ��� ���� ---
    "��� ����", "Min Max"
    ,
    "����� ���������� ��������� ���������, �� ������� �������� ��������������� �����, ��������� �������� � ��������� ���������."
    ,
    "Sets number of the last measurements on which the limiting lines which are bending around minima and maxima of measurements are under ",
    DISABLE_RU, DISABLE_EN,
    "2",        "2",
    "4",        "4",
    "8",        "8",
    "16",       "16",
    "32",       "32",
    "64",       "64",
    "128",      "128",
    ENUM_MIN_MAX, pDisplay, IsActive_MinMax, OnChanged_MinMax, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(  mcSmoothing,                                                                                          // ������� - ����������� ---
    "�����������", "Smoothing",
    "������������� ���������� ����� ��� ������� ����������� �� �������� ������ �������.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points.",
    DISABLE_RU, DISABLE_EN,
    "2 �����",  "2 points",
    "3 �����",  "3 points",
    "4 �����",  "4 points",
    "5 �����",  "5 points",
    "6 �����",  "6 points",
    "7 �����",  "7 points",
    "8 �����",  "8 points",
    "9 �����",  "9 points",
    "10 �����", "10 points",
    SMOOTHING, pDisplay, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChanged_RefreshFPS(bool active)
{
    FPGA::SetNumSignalsInSec(sDisplay_NumSignalsInS());
}

DEF_CHOICE_5(       mcRefreshFPS,                                                                                      // ������� - ������� ������ ---
    "������� ������", "Refresh rate",
    "����� ������������ ����� ��������� � ������� ������.",
    "Sets the maximum number of the shots removed in a second.",
    "25", "25",
    "10", "10",
    "5",  "5",
    "2",  "2",
    "1",  "1",
    ENUM_SIGNALS_IN_SEC, pDisplay, FuncActive, OnChanged_RefreshFPS, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       mcGrid_Type,                                                                                          // ������� - ����� - ��� ---
    "���", "Type",
    "����� ���� �����.",
    "Choice like Grid::",
    "��� 1", "Type 1",
    "��� 2", "Type 2",
    "��� 3", "Type 3",
    "��� 4", "Type 4",
    TYPE_GRID, mspGrid, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorType colorTypeGrid = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::GRID};

void OnChanged_Grid_Brightness()
{
    colorTypeGrid.SetBrightness(BRIGHTNESS_GRID / 1e2f);
}

static void BeforeDraw_Grid_Brightness()
{
    colorTypeGrid.Init(false);
    BRIGHTNESS_GRID = (int16)(colorTypeGrid.brightness * 100.0f);
}

DEF_GOVERNOR(       mgGrid_Brightness,                                                                                // ������� - ����� - ������� ---
    "�������", "Brightness",
    "������������� ������� �����.",
    "Adjust the brightness of the Grid::",
    BRIGHTNESS_GRID, 0, 100, mspGrid, FuncActive, OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         mspGrid,                                                                                                    // ������� - ����� ///
    "�����", "GRID",
    "�������� ��������� ����������� ������������ �����.",
    "Contains settings of display of a coordinate Grid::",
    mcGrid_Type,        // ������� - ����� - ���
    mgGrid_Brightness,  // ������� - ����� - �������
    Page_Display_Grid, &pDisplay, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcTypeShift,                                                                                             // ������� - �������� ---
    "��������", "Offset"
    ,
    "����� ����� ��������� �������� �� ���������\n1. \"����������\" - ����������� ���������� ��������.\n2. \"�������\" - ����������� ��������� "
    "�������� �� ������."
    ,
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of "
    "the offset on the screen.",
    "����������", "Voltage",
    "�������",    "Divisions",
    LINKING_RSHIFT, pDisplay, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcSettings_Colors_Scheme,                                                   //--- ������� - ��������� - ����� - �������� ����� ---
    "�������� �����", "Color scheme",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "����� 1", "Scheme 1",
    "����� 2", "Scheme 2",
    COLOR_SCHEME, mspSettings_Colors, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static ColorType colorT1 = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::CHAN[A]};
DEF_GOVERNOR_COLOR( mgcSettings_Colors_ChannelA,                                                       //--- ������� - ��������� - ����� - ����� 1 ---
    "����� 1", "Channel 1",
    "",  "",
    colorT1, mspSettings_Colors
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static ColorType colorT2 = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::CHAN[B]};
DEF_GOVERNOR_COLOR( mgcSettings_Colors_ChannelB,                                                       //--- ������� - ��������� - ����� - ����� 2 ---
    "����� 2", "Channel 2",
    "",
    "",
    colorT2, mspSettings_Colors
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR_COLOR( mgcSettings_Colors_Grid,                                                             //--- ������� - ��������� - ����� - ����� ---
    "�����", "Grid",
    "������������� ���� �����",
    "Sets the grid color",
    colorTypeGrid, mspSettings_Colors
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4(         mspSettings_Colors,                                                                             // ������� - ��������� - ����� ///
    "�����", "COLORS",
    "����� ������ �������",
    "The choice of colors display",
    mcSettings_Colors_Scheme,    // ������� - ��������� - ����� - �������� �����
    mgcSettings_Colors_ChannelA, // ������� - ��������� - ����� - ����� 1
    mgcSettings_Colors_ChannelB, // ������� - ��������� - ����� - ����� 2
    mgcSettings_Colors_Grid,     // ������� - ��������� - ����� - �����
    Page_Service_Display_Colors, &mspSettings, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_Brightness()
{
    Painter::SetBrightnessDisplay(BRIGHTNESS);
}

DEF_GOVERNOR(       mgSettings_Brightness,                                                                        // ������� - ��������� - ������� ---
    "�������", "Brightness",
    "��������� ������� �������� �������",
    "Setting the brightness of the display",
    BRIGHTNESS, 0, 100, mspSettings, FuncActive, OnChanged_Settings_Brightness, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       mgSettings_Levels,                                                                             // ������� - ��������� - ������ ---
    "������", "Levels",
    "����� �����, � ������� �������� ����� �������� ����� ������� ���������� �� ������ ������� ��������������� ����� ������ ��������",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    TIME_SHOW_LEVELS, 0, 125, mspSettings, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       mgSettings_TimeMessages,                                                                        // ������� - ��������� - ����� ---
    "�����", "Time",
    "��������� �������, � ������� �������� ��������� ����� ���������� �� ������",
    "Set the time during which the message will be on the screen",
    TIME_MESSAGES, 1, 99, mspSettings, FuncActive, FuncChanged, FuncBeforeDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcSettings_ShowStringNavigation,                                                          // ������� - ��������� - ������ ���� ---
    "������ ����", "Path menu",
    "��� ������ \n����������\n ����� ������ ������ ��������� ������ ���� �� ������� �������� ����",
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "��������",   "Hide",
    "����������", "Show",
    SHOW_STRING_NAVIGATION, mspSettings, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_ShowAltMarkers(bool)
{
    Display::ChangedRShiftMarkers();
}

DEF_CHOICE_3(       mcSettings_ShowAltMarkers,                                                            //--- ������� - ��������� - ���. ������� ---
    "���. �������", "Alt. markers"
    ,
    "������������� ����� ����������� �������������� �������� ������� �������� � �������������:\n"
    "\"��������\" - �������������� ������� �� ������������,\n"
    "\"����������\" - �������������� ������� ������������ ������,\n"
    "\"����\" - �������������� ������� ������������ � ������� 5 ��� ����� �������� ����� �������� ������ �� ���������� ��� ������ �������������"
    ,
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level",
    "��������",   "Hide",
    "����������", "Show",
    "����",       "Auto",
    ALT_MARKERS, mspSettings, FuncActive, OnChanged_Settings_ShowAltMarkers, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}

DEF_CHOICE_6(       mcSettings_AutoHide,                                                                      //--- ������� - ��������� - �������� ---
    "��������", "Hide",
    "��������� ����� ���������� ������� ������ ��� �������� �����, �� ��������� �������� ���� ������������� ��������� � ������",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "�������",      "Never",
    "����� 5 ���",  "Through 5 s",
    "����� 10 ���", "Through 10 s",
    "����� 15 ���", "Through 15 s",
    "����� 30 ���", "Through 30 s",
    "����� 60 ���", "Through 60 s",
    MENU_AUTO_HIDE, mspSettings, FuncActive, OnChanged_Settings_AutoHide, FuncDraw
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7(         mspSettings,                                                                                            // ������� - ��������� ///
    "���������", "SETTINGS",
    "�������������� ��������� �������",
    "Additional display settings",
    mspSettings_Colors,              // ������� - ��������� - �����
    mgSettings_Brightness,           // ������� - ��������� - �������
    mgSettings_Levels,               // ������� - ��������� - ������
    mgSettings_TimeMessages,         // ������� - ��������� - �����
    mcSettings_ShowStringNavigation, // ������� - ��������� - ������ ����
    mcSettings_ShowAltMarkers,       // ������� - ��������� - ���. �������
    mcSettings_AutoHide,             // ������� - ��������� - ��������
    Page_Service_Display, &pDisplay, FuncActive, EmptyPressPage
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_9(         pDisplay,                                                                                                           // ������� ///
    "�������", "DISPLAY",
    "�������� ��������� ����������� �������.",
    "Contains settings of display of the Display::",
    mcMapping,       // ������� - �����������
    mspAccumulation, // ������� - ����������
    mspAveraging,    // ������� - ����������
    mcMinMax,        // ������� - ��� ����
    mcSmoothing,     // ������� - �����������
    mcRefreshFPS,    // ������� - ������� ������
    mspGrid,         // ������� - �����
    mcTypeShift,     // ������� - ��������
    mspSettings,     // ������� - ���������
    Page_Display, &mainPage, FuncActive, EmptyPressPage
);


/** @}  @}
 */
