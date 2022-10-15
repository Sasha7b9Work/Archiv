#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/FLASH.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Menu/MenuDrawing.h"
#include "Menu/MenuFunctions.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Panel/Panel.h"
#include "Display/Colors.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageService
 *  @{
 */


static const Button bResetSettings;            ///< ������ - ����� ��������
static const Button bAutoSearch;               ///< ������ - ����� �������
static const Page pCalibrator;                 ///< ������ - ����������
static const Page pMath;                       ///< ������ - ����������
static const Page pEthernet;                   ///< ������ - ETHERNET
static const Choice cSound;                    ///< ������ - ����
static const Choice cLang;                     ///< ������ - ����
static const Time tTime;                       ///< ������ - �����
static const Choice cModeLongPressButtonTrig; ///< ������ - ��� ���� �����
static const Page pInformation;                ///< ������ - ����������


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern Page mainPage;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnTimerDraw()
{
    Display_Update();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncDraw()
{
    Painter_BeginScene(COLOR_BACK);

    Painter_DrawTextInRectWithTransfersC(30, 110, 300, 200, "����������� ����� �������� �������� ������ ����/����.\n"
                                        "������� ����� ������ ������, ���� ����� �� �����.", COLOR_FILL);

    Painter_EndScene();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPressResetSettings()
{
    Panel_Disable();
    Display_SetDrawMode(DrawMode_Hand, FuncDraw);
    Timer_Enable(kTimerDrawHandFunction, 100, OnTimerDraw);

    if (Panel_WaitPressingButton() == B_Start)
    {
        Settings_Load(true);
    }

    Timer_Disable(kTimerDrawHandFunction);
    Display_SetDrawMode(DrawMode_Auto, 0);
    Panel_Enable();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChangedModeCalibrator(bool active)
{
    FPGA_SetCalibratorMode(CALIBRATOR);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressPrevSettings()
{

}


// ������ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Page mpService;

// ������ - ����� ������
static void StartAutoSearch(void)
{
    FPGA_StartAutoFind();
};

// ������ - ����� ��������
const Button mbServicePreviousSettings =
{
    Item_Button, &mpService, 0,
    {
        "����� ��������", "Return settings",
        "���������� ��������� ������������, ������� ���� � ������ ������� \"����� �������\"",
        "Returns the oscilloscope settings that were in when you press \"Searching for signal\""
    },
    OnPressPrevSettings
};

// ������ - ���������� ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page pCalibrator;

// ������ - ���������� - �����������

void OnPressCalibration()
{
    gStateFPGA.needCalibration = true;
}

const Button mbServiceCalibration =
{
    Item_Button, &pCalibrator, 0,
    {
        "�����������", "Calibrate",
        "������ ��������� ����������",
        "Running the calibration procedure"
    },
    OnPressCalibration
};

// ������ - ���������� - �����
const Choice mcServiceModeCalibrator =
{
    Item_Choice, &pCalibrator, 0,
    {
        "����������",  "Calibrator",
        "����� ������ �����������",
        "Mode of operation of the calibrator"
    },
    {
        {"�����",   "DC"},
        {"����",    "AC"},
        {"0�",      "OV"}
    },
    (int8*)&CALIBRATOR, OnChangedModeCalibrator
};

// ������ - ������� - ����� ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Page mspSettings_Colors;


// ������ - ������� - ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Page mspServDisplMess;
        
// ������ - ���� //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Page mspServiceSound;

#include "PageServiceMath.h"

void ReInitAllColorGovernor()
{
    for(int i = 0; i < MAX_NUM_ITEMS_IN_PAGE; i++)
    {
        void *item = Item(&mspSettings_Colors, i);
        if(item && (TypeMenuItem(item) == Item_GovernorColor))
        {
            GovernorColor *governor = (GovernorColor*)item;
            governor->colorType->alreadyUsed = false;
            Color_Init(governor->colorType);
        }
    }
}

const Page pEthernet;

static void FuncOfChangedEthernetSettings(bool active)
{
    Display_ShowWarningGood(NeedRebootDevice2);
    Display_ShowWarningGood(NeedRebootDevice1);
}

static const Choice mcServEthEnable =
{
    Item_Choice, &pEthernet, 0,
    {
        "Ethernet", "Ethernet"
        ,
        "����� ������������� ethernet, �������� \"��������\" � ��������� ������.\n"
        "����� ��������� ethernet, �������� \"���������\" � ��������� ������."
        ,
        "To involve ethernet, choose \"Included\" and switch off the device.\n"
        "To disconnect ethernet, choose \"Disconnected\" and switch off the device."
    },
    {
        {"��������",    "Included"},
        {"���������",   "Disconnected"}
    },
    (int8*)&ETH_ENABLE, FuncOfChangedEthernetSettings
};

static const IPaddress ipAddress =
{
    Item_IP, &pEthernet, 0,
    {
        "IP �����", "IP-address",
        "��������� IP ������",
        "Set of IP-address"
    },
    &IP_ADDR0, &IP_ADDR1, &IP_ADDR2, &IP_ADDR3,
    FuncOfChangedEthernetSettings,
    &PORT
};

static const IPaddress ipNetMask =
{
    Item_IP, &pEthernet, 0,
    {
        "����� �������", "Network mask",
        "��������� ����� �������",
        "Set of network mask"
    },
    &NETMASK_ADDR0, &NETMASK_ADDR1, &NETMASK_ADDR2, &NETMASK_ADDR3,
    FuncOfChangedEthernetSettings
};

static const IPaddress ipGateway =
{
    Item_IP, &pEthernet, 0,
    {
        "����", "Gateway",
        "��������� ������ ��������� �����",
        "Set of gateway address"
    },
    &GW_ADDR0, &GW_ADDR1, &GW_ADDR2, &GW_ADDR3,
    FuncOfChangedEthernetSettings
};

static const MACaddress macMAC =
{
    Item_MAC, &pEthernet, 0,
    {
        "��� �����", "MAC-address",
        "��������� ����������� ������",
        "Set of MAC-address"
    },
    &MAC_ADDR0, &MAC_ADDR1, &MAC_ADDR2, &MAC_ADDR3, &MAC_ADDR4, &MAC_ADDR5,
    FuncOfChangedEthernetSettings
};

const Page pInformation;

static void PressSB_Information_Exit()
{
    Display_SetDrawMode(DrawMode_Auto, 0);
    Display_RemoveAddDrawFunction();
}

const SmallButton sbExitInformation =
{
    Item_SmallButton, &pInformation,
    COMMON_BEGIN_SB_EXIT,
    PressSB_Information_Exit,
    DrawSB_Exit
};

static void Information_Draw()
{
    Painter_BeginScene(COLOR_BACK);
    int x = 70;
    int y = 40;
    int width = SCREEN_WIDTH - 2 * x;
    int height = 120;
    Painter_FillRegionC(x, y, width, height, COLOR_GRID);
    Painter_DrawRectangleC(x, y, width, height, COLOR_FILL);
    Painter_DrawStringInCenterRect(x, y, width, 30, LANG_RU ? "����������" : "INFORMATION");
    Painter_DrawText(x + 20, y + 30, LANG_RU ? "������ �8-53/1" : "Model S8-53/1");
    char buffer[100];
    OTP_GetSerialNumber(buffer);
    if (buffer[0])
    {
        Painter_DrawFormText(x + 20, y + 42, COLOR_FILL, (LANG_RU ? "�������� �����: %s" : "Serial number: %s"), buffer);
    }
    Painter_DrawStringInCenterRect(x, y + 50, width, 30, LANG_RU ? "����������� �����������:" : "Software:");
    sprintf(buffer, LANG_RU ? "������ %s" : "version %s", NUM_VER);
    Painter_DrawText(x + 20, y + 79, buffer);
    Painter_DrawText(x + 20, y + 95, "CRC32 A1C8760F");

    int dY = -10;
    Painter_DrawStringInCenterRect(0, 190 + dY, 320, 20, "��� ��������� ������ ������� � ����������� ������ ������");
    Painter_DrawStringInCenterRect(0, 205 + dY, 320, 20, "����� ����������: ���./����. 8-017-262-57-50");
    Painter_DrawStringInCenterRect(0, 220 + dY, 320, 20, "������������: e-mail: mnipi-24(@)tut.by, ���. 8-017-262-57-51");

    Menu_Draw();
    Painter_EndScene();
}

static void OnPressInformation()
{
    OpenPageAndSetItCurrent(Page_SB_Information);
    Display_SetDrawMode(DrawMode_Hand, Information_Draw);
}

// ������ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page mpService =                   ///< ������
{
    Item_Page, &mainPage, 0,
    {
        "������", "SERVICE",
        "�������������� ���������, ����������, ����� �������, �������������� �������",
        "Additional settings, calibration, signal search, mathematical functions"
    },
    Page_Service,
    {
        (void*)&bResetSettings,                // ������ - ����� ��������
        (void*)&bAutoSearch,                   // ������ - ����� �������
        (void*)&pCalibrator,                   // ������ - ����������
        (void*)&pMath,                         // ������ - ����������
        (void*)&pEthernet,                     // ������ - ETHERNET
        (void*)&cSound,                        // ������ - ����
        (void*)&cLang,                         // ������ - ����
        (void*)&tTime,                         // ������ - �����
        (void*)&cModeLongPressButtonTrig,     // ������ - ��� ���� �����
        (void*)&pInformation                   // ������ - ����������
    }
};

// ������ - ����� �������� ---------------------------------------------------------------------------------------------------------------------------
static const Button bResetSettings =
{
    Item_Button, &mpService, 0,
    {
        "����� ��������", "Reset settings",
        "����� �������� �� ��������� �� ���������",
        "Reset to default settings"
    },
    OnPressResetSettings
};


// ������ - ����� ������� ----------------------------------------------------------------------------------------------------------------------------
static const Button bAutoSearch =
{
    Item_Button, &mpService, 0,
    {
        "����� �������", "Find signal",
        "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
        "Sets optimal settings for the oscilloscope signal on channel 1"
    },
    StartAutoSearch
};

// ������ - ���������� ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page pCalibrator =
{
    Item_Page, &mpService, 0,
    {
        "����������", "CALIBRATOR",
        "����������� ������������ � ���������� ������������",
        "Control of the calibrator and calibration of an oscillograph"
    },
    Page_ServiceCalibrator,
    {
        (void*)&mcServiceModeCalibrator,
        (void*)&mbServiceCalibration
    }
};

// ������ - ���������� ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page pMath =
{
    Item_Page, &mpService, 0,
    {
        "����������", "MATH",
        "�������������� ������� � ���",
        "Mathematical functions and FFT"
    },
    Page_Math,
    {
        (void*)&mspServiceMathFunction,
        (void*)&mspFFT
    }
};

// ������ - ETHERNET /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page pEthernet =
{
    Item_Page, &mpService, 0,
    {
        "ETHERNET", "ETHERNET",
        "��������� ethernet",
        "Settings of ethernet"
    },
    Page_ServiceEthernet,
    {
        (void*)&mcServEthEnable,
        (void*)&ipAddress,
        (void*)&ipNetMask,
        (void*)&ipGateway,
        (void*)&macMAC
    }
};

// ������ - ���� -------------------------------------------------------------------------------------------------------------------------------------
static const Choice cSound =
{
    Item_Choice, &mpService, 0,
    {
        "����", "Sound",
        "���������/���������� �����",
        "Inclusion/switching off of a sound"
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&SOUND_ENABLED
};


// ������ - ���� -------------------------------------------------------------------------------------------------------------------------------------
static const Choice cLang =
{
    Item_Choice, &mpService, 0,
    {
        "����", "Language",
        "��������� ������� ���� ����",
        "Allows you to select the menu language"
    },
    {
        {"�������",     "Russian"},
        {"����������",  "English"}
    },
    (int8*)&LANG
};


// ������ - ����� ------------------------------------------------------------------------------------------------------------------------------------
static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
static const Time tTime =
{
    Item_Time, &mpService, 0,
    {
        "�����", "Time"
        ,
        "��������� �������� �������.\n������� ������:\n"
        "������ �� ������� ���� \"�����\". ��������� ���� ��������� �������� �������. ��������� ��������� ������ �� �������� ����������, �������������� "
        "�������� ���������� \"�����\", �������� ����, ������, �������, ���, �����, ��� �����. ���������� ������� ������������ �������� ��������. "
        "��������� ����� ��������� ���������� ����������� ��������. ����� �������� ����� \"���������\", ������ � ���������� ����� 0.5 ��� ������ �� ������ "
        "����������. ���� ��������� �������� ������� ��������� � ����������� ������ �������� �������. ������� ���������� ��������� ������ �� ����� ������ �������� "
        "������� � �������� ���� ��������� �������� ������� ��� ���������� ������ �������� �������"
        ,
        "Setting the current time. \nPoryadok work:\n"
        "Click on the menu item \"Time\".The menu set the current time.By briefly pressing the button on the numeric keypad of conformity "
        "Control \"Time\", highlight the hours, minutes, seconds, year, month, or a number.The selected item is indicated by a flashing area. "
        "Turn the setting knob to set the desired value. Then highlight \"Save\", press and udrezhivat more than 0.5 seconds, the button on the panel "
        "Control. Menu Setting the current time will be closed to the conservation of the new current time. Pressing a button on the prolonged retention of any other element "
        "will lead to the closure of the current time setting menu without saving the new current time"
    },
    &dServicetime, &hours, &minutes, &secondes, &month, &day, &year
};

// ������ - ��� ���� ����� ---------------------------------------------------------------------------------------------------------------------------
static const Choice cModeLongPressButtonTrig =
{
    Item_Choice, &mpService, 0,
    {
        "��� ���� �����", "Mode long �����"
        ,
        "������������� �������� ��� ����������� ������� ������ �����:\n\"����� ������\" - ��������� ������ ������������� � ����,\n\"�����������\" "
        "- �������������� ����������� � ��������� ������ �������������"
        ,
        "Sets the action for long press CLOCK:\n\"Reset trig lvl\" - to set the trigger level to zero, \n\"Auto level\""
        "- Automatically detect and install the trigger level"
    },
    {
        {"����� ������",    "Reset trig level"},
        {"�����������",     "Autolevel"}
    },
    (int8*)&MODE_LONG_PRESS_TRIG
};

// ������ - ���������� -------------------------------------------------------------------------------------------------------------------------------
static const Page pInformation =
{
    Item_Page, &mpService, 0,
    {
        "����������", "INFORMATION",
        "������� �� ����� ����������������� ������ ������������",
        "Displays identification data of the oscilloscope"
    },
    Page_SB_Information,
    {
        (void*)&sbExitInformation,
        (void*)0,
        (void*)0,
        (void*)0,
        (void*)0,
        (void*)0
    },
    OnPressInformation, EmptyFuncVV, EmptyFuncVI
};


/** @}  @}
 */
