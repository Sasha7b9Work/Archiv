#include "defines.h"
#include "Definition.h"
#include "FlashDrive/FlashDrive.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Utils/GlobalFunctions.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Types.h"
#include "Hardware/FSMC.h"
#include "Hardware/Sound.h"
#include "Hardware/FLASH.h"
#include "Menu/MenuFunctions.h"
#include "Log.h"


/** @addtogroup Menu
 *  @{
 *  @defgroup PageDebug
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Choice     mcStats;                            ///< ������� - ����������
static const Page       mpConsole;                          ///< ������� - �������
static const Governor   mgConsole_NumStrings;               ///< ������� - ������� - ����� �����
static const Choice     mcConsole_SizeFont;                 ///< ������� - ������� - ������ ������
static const Page       mpConsole_Registers;                ///< ������� - ������� - ��������
static const Choice     mcConsole_Registers_ShowAll;        ///< ������� - ������� - �������� - ���������� ���
static const Choice     mcConsole_Registers_RD_FL;          ///< ������� - ������� - �������� - RD_FL
static bool      IsActive_Console_Registers_RD_FL(void);
static const Choice     mcConsole_Registers_RShiftA;        ///< ������� - ������� - �������� - U ��. 1�
static const Choice     mcConsole_Registers_RShiftB;        ///< ������� - ������� - �������� - U ��. 2�
static const Choice     mcConsole_Registers_TrigLev;        ///< ������� - ������� - �������� - U �����.
static const Choice     mcConsole_Registers_RangeA;         ///< ������� - ������� - �������� - �����/��� 1
static const Choice     mcConsole_Registers_RangeB;         ///< ������� - ������� - �������� - �����/��� 2
static const Choice     mcConsole_Registers_TrigParam;      ///< ������� - ������� - �������� - �����. �����.
static const Choice     mcConsole_Registers_ChanParamA;     ///< ������� - ������� - �������� - �����. ���. 1
static const Choice     mcConsole_Registers_ChanParamB;     ///< ������� - ������� - �������� - �����. ���. 2
static const Choice     mcConsole_Registers_TBase;          ///< ������� - ������� - �������� - �����/���
static const Choice     mcConsole_Registers_tShift;         ///< ������� - ������� - �������� - � ��.
static const Page       mpADC;                              ///< ������� - ���
static const Page       mpADC_Balance;                      ///< ������� - ��� - ������
static const Choice     mcADC_Balance_Mode;                 ///< ������� - ��� - ������ - �����
static void     OnChanged_ADC_Balance_Mode(bool active);
static void          Draw_ADC_Balance_Mode(int x, int y);
static const Governor   mgADC_Balance_ShiftA;               ///< ������� - ��� - ������ - �������� 1
static bool      IsActive_ADC_Balance_Shift(void);
static void     OnChanged_ADC_Balance_ShiftA(void);
static const Governor   mgADC_Balance_ShiftB;               ///< ������� - ��� - ������ - �������� 2
static void     OnChanged_ADC_Balance_ShiftB(void);
static const Page       mpADC_Stretch;                      ///< ������� - ��� - ��������
static const Choice     mcADC_Stretch_Mode;                 ///< ������� - ��� - �������� - �����
static void     OnChanged_ADC_Stretch_Mode(bool active);
void LoadStretchADC(Channel chan);
static const Governor   mgADC_Stretch_ADC_A;                ///< ������� - ��� - �������� - �����. 1�
static bool      IsActive_ADC_Stretch_ADC(void);
static void     OnChanged_ADC_Stretch_ADC_A(void);
static const Governor   mgADC_Stretch_ADC_B;                ///< ������� - ��� - �������� - �����. 2�
static void     OnChanged_ADC_Stretch_ADC_B(void);
static const Page       mpADC_AltRShift;                    ///< ������� - ��� - ��� ����
static const Button     mbADC_AltRShift_Reset;              ///< ������� - ��� - ��� ���� - �����
static void       OnPress_ADC_AltRShift_Reset(void);
static const Governor   mbADC_AltRShift_2mV_DC_A;           ///< ������� - ��� - ��� ���� - �� 1� 2�� ����
static void     OnChanged_ADC_AltRShift_A(void);
static const Governor   mbADC_AltRShift_2mV_DC_B;           ///< ������� - ��� - ��� ���� - �� 2� 2�� ����
static void     OnChanged_ADC_AltRShift_B(void);
static const Governor   mbADC_AltRShift_5mV_DC_A;           ///< ������� - ��� - ��� ���� - �� 1� 5�� ����
static const Governor   mbADC_AltRShift_5mV_DC_B;           ///< ������� - ��� - ��� ���� - �� 2� 5�� ����
static const Governor   mbADC_AltRShift_10mV_DC_A;          ///< ������� - ��� - ��� ���� - �� 1� 10�� ����
static const Governor   mbADC_AltRShift_10mV_DC_B;          ///< ������� - ��� - ��� ���� - �� 2� 10�� ����
static const Page       mpRandomizer;                       ///< ������� - ����-���
static const Governor   mgRandomizer_SamplesForGates;       ///< ������� - ����-��� - ���-�/������
static void     OnChanged_Randomizer_SamplesForGates(void);
static const Governor   mgRandomizer_AltTShift0;            ///< ������� - ����-��� - tShift ���.
static void     OnChanged_Randomizer_AltTShift0(void);
static const Governor   mgRandomizer_Average;               ///< ������� - ����-��� - ������.
static const Choice     mcSizeSettings;                     ///< ������� - ������ ��������
static void        OnDraw_SizeSettings(int x, int y);
static const Button     mbSaveFirmware;                     ///< ������� - ����. ��������
static bool      IsActive_SaveFirmware(void);
static void       OnPress_SaveFirmware(void);
static const Page       mpSerialNumber;                     ///< ������� - �/�
static void       OnPress_SerialNumber(void);
static void          Draw_EnterSerialNumber(void);
static void      OnRegSet_SerialNumber(int);
static const SmallButton sbSerialNumber_Exit;               ///< ������� - �/� - �����
static void        OnPress_SerialNumber_Exit(void);
static const SmallButton sbSerialNumber_Delete;             ///< ������� - �/� - �������
static void        OnPress_SerialNumber_Delete(void);
static void           Draw_SerialNumber_Delete(int, int);
static const SmallButton sbSerialNumber_Backspace;          ///< ������� - �/� - Backspace
static void        OnPress_SerialNumber_Backspace(void);
static void           Draw_SerialNumber_Backspace(int, int);
static const SmallButton sbSerialNumber_Insert;             ///< ������� - �/� - ��������
static void        OnPress_SerialNumber_Insert(void);
static void           Draw_SerialNumber_Insert(int, int);
static const SmallButton sbSerialNumber_Save;               ///< ������� - �/� - ���������
static void        OnPress_SerialNumber_Save(void);
static void           Draw_SerialNumber_Save(int, int);

#define LENGTH_SN 15
static char stringSN[LENGTH_SN] = "";   ///< ����� �������� �������� ����� ��� ������ � OTP.

extern Page mainPage;


// ������� ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Page mpDebug =
{
    Item_Page, &mainPage, 0,
    {
        "�������", "DEBUG",
        "",
        ""
    },
    Page_Debug,
    {
        (void*)&mcStats,            // ������� - ����������
        (void*)&mpConsole,          // ������� - �������
        (void*)&mpADC,              // ������� - ���
        (void*)&mpRandomizer,       // ������� - ����-���
        (void*)&mcSizeSettings,     // ������� - ������ ��������
        (void*)&mbSaveFirmware,     // ������� - ����. ��������
        (void*)&mpSerialNumber      // ������� - �/�
        //(void*)&mspDebugInformation
    }
};

// ������� - ���������� ------------------------------------------------------------------------------------------------------------------------------
static const Choice mcStats =
{
    Item_Choice, &mpDebug, 0,
    {
        "����������", "Statistics",
        "����������/�� ���������� �����/����, ������ � �������, ���������� �������� � ���������� ����������� � ������/���������� ����������� � ������ ��������",
        "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory"
    },
    {
        {"�� ����������",   "Hide"},
        {"����������",      "Show"}
    },
    (int8*)&SHOW_STATS
};


// ������� - ������� /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page mpConsole =
{
    Item_Page, &mpDebug, 0,
    {
        "�������", "CONSOLE",
        "",
        ""
    },
    Page_DebugConsole,
    {
        (void*)&mgConsole_NumStrings,   // ������� - ������� - ����� �����
        (void*)&mcConsole_SizeFont,     // ������� - ������� - ������ ������
        (void*)&mpConsole_Registers     // ������� - ������� - ��������
    }
};

// ������� - ������� - ����� ����� -------------------------------------------------------------------------------------------------------------------
static const Governor mgConsole_NumStrings =
{
    Item_Governor, &mpConsole, 0,
    {
        "����� �����", "Number strings"
        "",
        ""
    },
    &NUM_STRINGS,  0, 33
};

// ������� - ������� - ������ ������ -----------------------------------------------------------------------------------------------------------------
static const Choice mcConsole_SizeFont =
{
    Item_Choice, &mpConsole, 0,
    {
        "������ ������", "Size font",
        "",
        ""
    },
    {
        {"5",   "5"},
        {"8",   "8"}
    },
    (int8*)&SIZE_FONT_CONSOLE
};


// ������� - ������� - �������� //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page mpConsole_Registers =
{
    Item_Page, &mpConsole, 0,
    {
        "��������", "REGISTERS",
        "",
        ""
    },
    Page_DebugShowRegisters,
    {
        (void*)&mcConsole_Registers_ShowAll,    // ������� - ������� - �������� - ���������� ���
        (void*)&mcConsole_Registers_RD_FL,      // ������� - ������� - �������� - RD_FL
        (void*)&mcConsole_Registers_RShiftA,    // ������� - ������� - �������� - U ��. 1�
        (void*)&mcConsole_Registers_RShiftB,    // ������� - ������� - �������� - U ��. 2�
        (void*)&mcConsole_Registers_TrigLev,    // ������� - ������� - �������� - U �����.
        (void*)&mcConsole_Registers_RangeA,     // ������� - ������� - �������� - �����/��� 1
        (void*)&mcConsole_Registers_RangeB,     // ������� - ������� - �������� - �����/��� 2
        (void*)&mcConsole_Registers_TrigParam,  // ������� - ������� - �������� - �����. �����.
        (void*)&mcConsole_Registers_ChanParamA, // ������� - ������� - �������� - �����. ���. 1
        (void*)&mcConsole_Registers_ChanParamB, // ������� - ������� - �������� - �����. ���. 2
        (void*)&mcConsole_Registers_TBase,      // ������� - ������� - �������� - �����/���
        (void*)&mcConsole_Registers_tShift      // ������� - ������� - �������� - � ��.
    }
};

// ������� - ������� - �������� - ���������� ��� -----------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_ShowAll =
{
    Item_Choice, &mpConsole_Registers, 0,
    {
        "���������� ���", "Show all",
        "���������� ��� ��������, ���������� � ��������",
        "To show all values transferred in registers"
    },
    {
        {"���", "No"},
        {"��",  "Yes"}
    },
    (int8*)&set.debug.showRegisters.all
};

// ������� - ������� - �������� - RD_FL --------------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_RD_FL =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "RD_FL", "RD_FL",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.flag
};

static bool IsActive_Console_Registers_RD_FL(void)
{
    return !set.debug.showRegisters.all;
}

// ������� - ������� - �������� - U ��. 1� -----------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_RShiftA =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "U ��. 1�", "U shift 1ch",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.rShiftA
};

// ������� - ������� - �������� - U ��. 2� -----------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_RShiftB =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "U ��. 2�", "U shift 2ch",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.rShiftB
};

// ������� - ������� - �������� - U �����. -----------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_TrigLev =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "U �����.", "U trig.",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.trigLev
};

// ������� - ������� - �������� - �����/��� 1 --------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_RangeA =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "�����/��� 1", "Range 1",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.range[0]
};

// ������� - ������� - �������� - �����/��� 2 --------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_RangeB =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "�����/��� 2", "Range 2",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.range[1]
};

// ������� - ������� - �������� - �����. �����. ------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_TrigParam =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "�����. �����.", "Trig param",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.trigParam
};

// ������� - ������� - �������� - �����. ���. 1 ------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_ChanParamA =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "�����. ���. 1",  "Chan 1 param",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.chanParam[0]
};

// ������� - ������� - �������� - �����. ���. 2 ------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_ChanParamB =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "�����. ���. 2", "Chan 2 param",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.chanParam[1]
};

// ������� - ������� - �������� - �����/��� ----------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_TBase =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "�����/���", "TBase",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.tBase
};

// ������� - ������� - �������� - � ��. --------------------------------------------------------------------------------------------------------------
static const Choice mcConsole_Registers_tShift =
{
    Item_Choice, &mpConsole_Registers, IsActive_Console_Registers_RD_FL,
    {
        "� ��.", "tShift",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&set.debug.showRegisters.tShift
};


// ������� - ��� /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page mpADC =
{
    Item_Page, &mpDebug, 0,
    {
        "���", "ADC",
        "",
        ""
    },
    Page_DebugADC,
    {
        (void*)&mpADC_Balance,      // ������� - ��� - ������
        (void*)&mpADC_Stretch,      // ������� - ��� - ��������
        (void*)&mpADC_AltRShift     // ������� - ��� - ��� ����
        //, (void*)&mspDebugADCaltShift
    }
};

// ������� - ��� - ������ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page mpADC_Balance =
{
    Item_Page, &mpADC, 0,
    {
        "������", "BALANCE",
        "",
        ""
    },
    Page_DebugADCbalance,
    {
        (void*)&mcADC_Balance_Mode,     // ������� - ��� - ������ - �����
        (void*)&mgADC_Balance_ShiftA,   // ������� - ��� - ������ - �������� 1
        (void*)&mgADC_Balance_ShiftB    // ������� - ��� - ������ - �������� 2
    }
};

// ������� - ��� - ������ - ����� --------------------------------------------------------------------------------------------------------------------
static const Choice mcADC_Balance_Mode =
{
    Item_Choice, &mpADC_Balance, 0,
    {
        "�����", "Mode",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {"��������",    "Real"},
        {"������",      "Manual"}
    },
    (int8*)&BALANCE_ADC_TYPE, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
};

static int16 shiftADCA;
static int16 shiftADCB;

static void OnChanged_ADC_Balance_Mode(bool active)
{
    Draw_ADC_Balance_Mode(0, 0);

    FPGA_WriteToHardware(WR_ADD_RSHIFT_DAC1, shiftADCA, false);
    FPGA_WriteToHardware(WR_ADD_RSHIFT_DAC2, shiftADCB, false);
}

static void Draw_ADC_Balance_Mode(int x, int y)
{
    int8 shift[2][3] =
    {
        {0, BALANCE_SHIFT_ADC_A, BALANCE_ADC_A},
        {0, BALANCE_SHIFT_ADC_B, BALANCE_ADC_B}
    };

    shiftADCA = shift[0][BALANCE_ADC_TYPE];
    shiftADCB = shift[1][BALANCE_ADC_TYPE];
}

// ������� - ��� - ������ - �������� 1 ---------------------------------------------------------------------------------------------------------------
static const Governor mgADC_Balance_ShiftA =
{
    Item_Governor, &mpADC_Balance, IsActive_ADC_Balance_Shift,
    {
        "�������� 1", "Offset 1",
        "",
        ""
    },
    &shiftADCA, -125, 125, OnChanged_ADC_Balance_ShiftA
};

static void OnChanged_ADC_Balance_ShiftA(void)
{
    BALANCE_ADC_A = shiftADCA;
    FPGA_WriteToHardware(WR_ADD_RSHIFT_DAC1, BALANCE_ADC_A, false);
}

static bool IsActive_ADC_Balance_Shift(void)
{
    return BALANCE_ADC_TYPE_IS_HAND;
}

// ������� - ��� - ������ - �������� 2----------------------------------------------------------------------------------------------------------------
static const Governor mgADC_Balance_ShiftB =
{
    Item_Governor, &mpADC_Balance, IsActive_ADC_Balance_Shift,
    {
        "�������� 2", "Offset 2",
        "",
        ""
    },
    &shiftADCB, -125, 125, OnChanged_ADC_Balance_ShiftB
};

static void OnChanged_ADC_Balance_ShiftB(void)
{
    BALANCE_ADC_B = shiftADCB;
    FPGA_WriteToHardware(WR_ADD_RSHIFT_DAC2, BALANCE_ADC_B, false);
}


// ������� - ��� - �������� //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page mpADC_Stretch =
{
    Item_Page, &mpADC, 0,
    {
        "��������", "STRETCH",
        "",
        ""
    },
    Page_DebugADCstretch,
    {
        (void*)&mcADC_Stretch_Mode,     // ������� - ��� - �������� - �����
        (void*)&mgADC_Stretch_ADC_A,    // ������� - ��� - �������� - �����. 1�
        (void*)&mgADC_Stretch_ADC_B     // ������� - ��� - �������� - �����. 2�
    }
};

// ������� - ��� - �������� - ����� ------------------------------------------------------------------------------------------------------------------
static const Choice mcADC_Stretch_Mode =
{
    Item_Choice, &mpADC_Stretch, 0,
    {
        "�����", "Mode",
        "",
        ""
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {"��������",    "Real"},
        {"������",      "Manual"}
    },
    (int8*)&DEBUG_STRETCH_ADC_TYPE, OnChanged_ADC_Stretch_Mode
};

static void OnChanged_ADC_Stretch_Mode(bool active)
{
    if(active)
    {
        LoadStretchADC(A);
        LoadStretchADC(B);
    }
}

void LoadStretchADC(Channel chan)
{
    if(DEBUG_STRETCH_ADC_TYPE_IS_DISABLED)
    {
        FPGA_WriteToHardware(chan == A ? WR_CAL_A : WR_CAL_B, 0x80, true);
    }
    else if(DEBUG_STRETCH_ADC_TYPE_IS_HAND)
    {
        FPGA_WriteToHardware(chan == A ? WR_CAL_A : WR_CAL_B, (uint8)DEBUG_STRETCH_ADC(chan), true);
    }
    else if(DEBUG_STRETCH_ADC_TYPE_IS_SETTINGS)
    {
        FPGA_LoadKoeffCalibration(chan);
    }
}

// ������� - ��� - �������� - �����. 1� --------------------------------------------------------------------------------------------------------------
static const Governor mgADC_Stretch_ADC_A =
{
    Item_Governor, &mpADC_Stretch, IsActive_ADC_Stretch_ADC,
    {
        "�����. 1�", "Koeff. 1ch",
        "",
        ""
    },
    &DEBUG_STRETCH_ADC_A, 0, 255, OnChanged_ADC_Stretch_ADC_A
};

static bool IsActive_ADC_Stretch_ADC(void)
{
    return DEBUG_STRETCH_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Stretch_ADC_A(void)
{
    FPGA_WriteToHardware(WR_CAL_A, (uint8)DEBUG_STRETCH_ADC_A, true);
}


// ������� - ��� - �������� - �����. 2� --------------------------------------------------------------------------------------------------------------
static const Governor mgADC_Stretch_ADC_B =
{
    Item_Governor, &mpADC_Stretch, IsActive_ADC_Stretch_ADC,
    {
        "�����. 2�", "Koeff. 2ch",
        "",
        ""
    },
    &DEBUG_STRETCH_ADC_B, 0, 255, OnChanged_ADC_Stretch_ADC_B
};

static void OnChanged_ADC_Stretch_ADC_B(void)
{
    FPGA_WriteToHardware(WR_CAL_B, (uint8)DEBUG_STRETCH_ADC_B, true);
}

// ������� - ��� - ��� ���� //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page mpADC_AltRShift =
{
    Item_Page, &mpADC, 0,
    {
        "��� ����", "ADD RSHFIT",
        "",
        ""
    },
    Page_DebugADCrShift,
    {
        (void*)&mbADC_AltRShift_Reset,          // ������� - ��� - ��� ���� - �����
        (void*)&mbADC_AltRShift_2mV_DC_A,       // ������� - ��� - ��� ���� - �� 1� 2�� ����
        (void*)&mbADC_AltRShift_2mV_DC_B,       // ������� - ��� - ��� ���� - �� 2� 2�� ����
        (void*)&mbADC_AltRShift_5mV_DC_A,       // ������� - ��� - ��� ���� - �� 1� 5�� ����
        (void*)&mbADC_AltRShift_5mV_DC_B,       // ������� - ��� - ��� ���� - �� 2� 5�� ����
        (void*)&mbADC_AltRShift_10mV_DC_A,      // ������� - ��� - ��� ���� - �� 1� 10�� ����
        (void*)&mbADC_AltRShift_10mV_DC_B       // ������� - ��� - ��� ���� - �� 2� 10�� ����
    }
};

// ������� - ��� - ��� ���� - ����� ------------------------------------------------------------------------------------------------------------------
static const Button mbADC_AltRShift_Reset =
{
    Item_Button, &mpADC_AltRShift, 0,
    {
        "�����", "Reset",
        "",
        ""
    },
    OnPress_ADC_AltRShift_Reset
};

static void OnPress_ADC_AltRShift_Reset(void)
{
    for(int chan = 0; chan < 2; chan++)
    {
        for(int mode = 0; mode < 2; mode++)
        {
            for(int range = 0; range < RangeSize; range++)
            {
                set.chan[chan].rShiftAdd[range][mode] = 0;
            }
        }
    }
    FPGA_SetRShift(A, RSHIFT_A);
    FPGA_SetRShift(B, RSHIFT_B);
}

// ������� - ��� - ��� ���� - �� 1� 2�� ���� ---------------------------------------------------------------------------------------------------------
static const Governor mbADC_AltRShift_2mV_DC_A =
{
    Item_Governor, &mpADC_AltRShift, 0,
    {
        "�� 1� 2�� ����", "Shift 1ch 2mV DC",
        "",
        ""
    },
    &set.chan[A].rShiftAdd[Range_2mV][ModeCouple_DC], -100, 100, OnChanged_ADC_AltRShift_A
};

static void OnChanged_ADC_AltRShift_A(void)
{
    FPGA_SetRShift(A, RSHIFT_A);
}

// ������� - ��� - ��� ���� - �� 2� 2�� ���� ---------------------------------------------------------------------------------------------------------
static const Governor mbADC_AltRShift_2mV_DC_B =
{
    Item_Governor, &mpADC_AltRShift, 0,
    {
        "�� 2� 2�� ����", "Shift 2ch 2mV DC",
        "",
        ""
    },
    &set.chan[B].rShiftAdd[Range_2mV][ModeCouple_DC], -100, 100, OnChanged_ADC_AltRShift_B
};

static void OnChanged_ADC_AltRShift_B(void)
{
    FPGA_SetRShift(B, RSHIFT_B);
}

// ������� - ��� - ��� ���� - �� 1� 5�� ���� ---------------------------------------------------------------------------------------------------------
static const Governor mbADC_AltRShift_5mV_DC_A =
{
    Item_Governor, &mpADC_AltRShift, 0,
    {
        "�� 1� 5�� ����", "Shift 1ch 5mV DC",
        "",
        ""
    },
    &set.chan[A].rShiftAdd[Range_5mV][ModeCouple_DC], -100, 100, OnChanged_ADC_AltRShift_A
};

// ������� - ��� - ��� ���� - �� 2� 5�� ���� ---------------------------------------------------------------------------------------------------------
static const Governor mbADC_AltRShift_5mV_DC_B =
{
    Item_Governor, &mpADC_AltRShift, 0,
    {
        "�� 2� 5�� ����", "Shift 2ch 5mV DC",
        "",
        ""
    },
    &set.chan[B].rShiftAdd[Range_5mV][ModeCouple_DC], -100, 100, OnChanged_ADC_AltRShift_B
};

// ������� - ��� - ��� ���� - �� 1� 10�� ���� --------------------------------------------------------------------------------------------------------
static const Governor mbADC_AltRShift_10mV_DC_A =
{
    Item_Governor, &mpADC_AltRShift, 0,
    {
        "�� 1� 10�� ����", "Shift 1ch 10mV DC",
        "",
        ""
    },
    &set.chan[A].rShiftAdd[Range_10mV][ModeCouple_DC], -100, 100, OnChanged_ADC_AltRShift_A
};

// ������� - ��� - ��� ���� - �� 2� 10�� ���� --------------------------------------------------------------------------------------------------------
static const Governor mbADC_AltRShift_10mV_DC_B =
{
    Item_Governor, &mpADC_AltRShift, 0,
    {
        "�� 2� 10�� ����", "Shift 2ch 10mV DC",
        "",
        ""
    },
    &set.chan[B].rShiftAdd[Range_10mV][ModeCouple_DC], -100, 100, OnChanged_ADC_AltRShift_B
};


// ������� - ����-��� ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page mpRandomizer =
{
    Item_Page, &mpDebug, 0,
    {
        "����-���", "RANDOMIZER",
        "",
        ""
    },
    Page_DebugRandomizer,
    {
        (void*)&mgRandomizer_SamplesForGates,   // ������� - ����-��� - ���-�/������
        (void*)&mgRandomizer_AltTShift0,        // ������� - ����-��� - tShift ���.
        (void*)&mgRandomizer_Average            // ������� - ����-��� - ������.
    }
};

// ������� - ����-��� - ���-�/������ -----------------------------------------------------------------------------------------------------------------
static const Governor mgRandomizer_SamplesForGates =
{
    Item_Governor, &mpRandomizer, 0,
    {
        "���-�/������", "Samples/gates",
        "",
        ""
    },
    &NUM_MEAS_FOR_GATES, 1, 2500, OnChanged_Randomizer_SamplesForGates
};

static void OnChanged_Randomizer_SamplesForGates(void)
{
    FPGA_SetNumberMeasuresForGates(NUM_MEAS_FOR_GATES);
}

// ������� - ����-��� - tShift ���. ------------------------------------------------------------------------------------------------------------------
static const Governor mgRandomizer_AltTShift0 =
{
    Item_Governor, &mpRandomizer, 0,
    {
        "tShift ���.", "tShift alt.",
        "",
        ""
    },
    &ADD_SHIFT_T0, 0, 510, OnChanged_Randomizer_AltTShift0
};

static void OnChanged_Randomizer_AltTShift0(void)
{
    FPGA_SetDeltaTShift(ADD_SHIFT_T0);
}

// ������� - ����-��� - ������. ----------------------------------------------------------------------------------------------------------------------
static const Governor mgRandomizer_Average =
{
    Item_Governor, &mpRandomizer, 0,
    {
        "������.", "Average",
        "",
        ""
    },
    &NUM_AVE_FOR_RAND, 1, 32
};


// ������� - ������ ��������  ------------------------------------------------------------------------------------------------------------------------
static const Choice mcSizeSettings =
{
    Item_Choice, &mpDebug, 0,
    {
        "������ ��������", "Size settings",
        "����� ������� ��������� Settings",
        "Show size of struct Settings"
    },
    {
        {"������", "Size"},
        {"������", "Size"}
    },
    0, 0, OnDraw_SizeSettings
};

static void OnDraw_SizeSettings(int x, int y)
{
    Painter_DrawFormText(x + 5, y + 21, COLOR_BLACK, "������ %d", sizeof(Settings));
}


// ������� - ����. �������� --------------------------------------------------------------------------------------------------------------------------
static const Button mbSaveFirmware =
{
    Item_Button, &mpDebug, IsActive_SaveFirmware,
    {
        "����. ��������", "Save firmware",
        "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
        "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored"
    },
    OnPress_SaveFirmware
};

static bool IsActive_SaveFirmware(void)
{
    return gBF.flashDriveIsConnected;
}

static void OnPress_SaveFirmware(void)
{
    StructForWrite structForWrite;

    FDrive_OpenNewFileForWrite("S8-53.bin", &structForWrite);

    uint8 *address = (uint8*)0x08020000;
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive_WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive_CloseFile(&structForWrite);

    Display_ShowWarningGood(FirmwareSaved);
}


// ������� - �/� /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page mpSerialNumber =
{
    Item_Page, &mpDebug, 0,
    {
        "�/�", "S/N",
        "������ ��������� ������ � ���������� ��������������� ������ (OTP memory)",
        "Serial number recording in one-time programmable memory (OTP)"
    },
    Page_SB_SerialNumber,
    {
        (void*)&sbSerialNumber_Exit,         // ������� - �/� - �����
        (void*)&sbSerialNumber_Delete,       // ������� - �/� - �������
        (void*)&sbSerialNumber_Backspace,    // ������� - �/� - Backspace
        (void*)&sbSerialNumber_Insert,       // ������� - �/� - ��������
        (void*)0,
        (void*)&sbSerialNumber_Save          // ������� - �/� - ���������
    },
    EmptyFuncVV, OnPress_SerialNumber, OnRegSet_SerialNumber
};

static void OnPress_SerialNumber(void)
{
    OpenPageAndSetItCurrent(Page_SB_SerialNumber);
    Display_SetAddDrawFunction(Draw_EnterSerialNumber);
}

static void Draw_EnterSerialNumber(void)
{
    int x0 = GridLeft() + 40;
    int y0 = GRID_TOP + 20;
    int width = GridWidth() - 80;
    int height = 160;

    Painter_DrawRectangleC(x0, y0, width, height, COLOR_FILL);
    Painter_FillRegionC(x0 + 1, y0 + 1, width - 2, height - 2, COLOR_BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while (symbolsAlphaBet[index][0] != ' ')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // ������ ������ ����� � ������
    position = 0;
    while (symbolsAlphaBet[index][0] != 'a')
    {
        DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    int x = Painter_DrawTextC(x0 + deltaX, y0 + 65, stringSN, COLOR_FILL);
    Painter_FillRegionC(x, y0 + 65, 5, 8, COLOR_FLASH_10);

    // ������ ������� ���������� �� ���������� ����� � OTP-������ ��� ������
    
    char buffer[20];
    
    int allShots = OTP_GetSerialNumber(buffer);
    
    Painter_DrawFormText(x0 + deltaX, y0 + 130, COLOR_FILL, "������� ���������� ����� %s", buffer);

    Painter_DrawFormText(x0 + deltaX, y0 + 100, COLOR_FILL, "�������� ����� ��� %d �������", allShots);
}

static void OnRegSet_SerialNumber(int angle)
{
extern void OnMemExtSetMaskNameRegSet(int angle, int maxIndex);

    OnMemExtSetMaskNameRegSet(angle, 0x27);
}


// ������� - �/� - ����� -----------------------------------------------------------------------------------------------------------------------------
static const SmallButton sbSerialNumber_Exit =
{
    Item_SmallButton, &mpSerialNumber,
    COMMON_BEGIN_SB_EXIT,
    OnPress_SerialNumber_Exit,
    DrawSB_Exit
};

static void OnPress_SerialNumber_Exit(void)
{
    Display_RemoveAddDrawFunction();
}

// ������� - �/� - ������� ---------------------------------------------------------------------------------------------------------------------------
static const SmallButton sbSerialNumber_Delete =
{
    Item_SmallButton, &mpSerialNumber, 0,
    {
        "�������", "Delete",
        "������� ��� �������� �������",
        "Deletes all entered symbols"
    },
    OnPress_SerialNumber_Delete,
    Draw_SerialNumber_Delete
};

static void OnPress_SerialNumber_Delete(void)
{
    stringSN[0] = 0;
}

static void Draw_SerialNumber_Delete(int x, int y)
{
    Painter_SetFont(TypeFont_UGO2);
    Painter_Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    Painter_SetFont(TypeFont_8);
}

// ������� - �/� - Backspace -------------------------------------------------------------------------------------------------------------------------
static const SmallButton sbSerialNumber_Backspace =
{
    Item_SmallButton, &mpSerialNumber, 0,
    {
        "Backspace", "Backspace",
        "������� ��������� �������� ������",
        "Delete the last entered symbol"
    },
    OnPress_SerialNumber_Backspace,
    Draw_SerialNumber_Backspace
};

static void OnPress_SerialNumber_Backspace(void)
{
    int index = strlen(stringSN);
    if (index)
    {
        stringSN[index - 1] = 0;
    }
}

static void Draw_SerialNumber_Backspace(int x, int y)
{
    Painter_SetFont(TypeFont_UGO2);
    Painter_Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_BACKSPACE);
    Painter_SetFont(TypeFont_8);
}

// ������� - �/� - �������� --------------------------------------------------------------------------------------------------------------------------
static const SmallButton sbSerialNumber_Insert =
{
    Item_SmallButton, &mpSerialNumber, 0,
    {
        "��������", "Insert",
        "��������� �������� ������",
        "Inserts the chosen symbol"
    },
    OnPress_SerialNumber_Insert,
    Draw_SerialNumber_Insert
};

static void OnPress_SerialNumber_Insert(void)
{
    int size = strlen(stringSN);
    if (size < LENGTH_SN - 1)
    {
        stringSN[size] = symbolsAlphaBet[INDEX_SYMBOL][0];
        stringSN[size + 1] = '\0';
    }
}

static void Draw_SerialNumber_Insert(int x, int y)
{
    Painter_SetFont(TypeFont_UGO2);
    Painter_Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_INSERT);
    Painter_SetFont(TypeFont_8);
}

// ������� - �/� - ��������� -------------------------------------------------------------------------------------------------------------------------
static const SmallButton sbSerialNumber_Save =
{
    Item_SmallButton, &mpSerialNumber, 0,
    {
        "���������", "Save",
        "���������� �������� ����� � OTP",
        "Records the serial number in OTP"
    },
    OnPress_SerialNumber_Save,
    Draw_SerialNumber_Save
};

static void OnPress_SerialNumber_Save(void)
{
    if (!OTP_SaveSerialNumber(stringSN))
    {
        Display_ShowWarningBad(FullyCompletedOTP);
    }
}

static void Draw_SerialNumber_Save(int x, int y)
{
    Painter_SetFont(TypeFont_UGO2);
    Painter_Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    Painter_SetFont(TypeFont_8);
}


/** @}  @}
 */

        /*
        static void OnChangeRShiftADC(bool active)
        {
            FPGA_SetRShift(A, RSHIFT_A);
            FPGA_SetRShift(B, RSHIFT_B);
        }

        const Choice mcDebugADCrShiftMode =
        {
            Item_Choice, &mpADC_AltRShift, {"�����",      "Mode"},
            {
                "",
                ""
            },
            EmptyFuncBV,
            {                                    {DISABLE_RU,   DISABLE_EN},
                                                 {"��������",   "Real"},
                                                 {"������",     "Hand"}
            },
            (int8*)&set.debug.rShiftADCtype, OnChangeRShiftADC
        };

        // ������� - ��� - ���. �������� - �������� 1� 2��
        static TimeStructGovernor tsRShiftADC;
        static bool IsActiveRShiftADC(void)
        {
            return set.debug.rShiftADCtype == RShiftADC_Hand;
        }

        static void OnChangeRShiftADC0(void)
        {
            FPGA_SetRShift(A, RSHIFT_A);
        }

        const Governor mgDebugRShiftADC00 =
        {
            Item_Governor, &mpADC_AltRShift,
            {
                "���� 1� 2��", "Shift 1ch"
            },
            {
                "",
                ""
            },
            IsActiveRShiftADC,
            &set.debug.rShiftADC[0][0], -100, 100, &tsRShiftADC, OnChangeRShiftADC0
        };

        static TimeStructGovernor tsRShiftADC01;
        const Governor mgDebugRShiftADC01 =
        {
            Item_Governor, &mpADC_AltRShift,
            {
                "���� 1� 500��", "Shift 1ch 500mV"
            },
            {
                "",
                ""
            },
            IsActiveRShiftADC,
            &set.debug.rShiftADC[1][0], -100, 100, &tsRShiftADC01, OnChangeRShiftADC0
        };

        static void OnChangeRShiftADC1(void)
        {
            FPGA_SetRShift(B, RSHIFT_B);
        }

        // ������� - ��� - ���. ����. ���.
const Page mspDebugADCaltShift;

/// ������� - ��� - ���. ����. ���. - ��������
const Governor mgDebugADCaltShift =
{
    Item_Governor, &mspDebugADCaltShift, 0,
    {
        "��������", "Value",
        "",
        ""
    },
    &set.debug.altShift, -2, 2
};

// ������� - ��� - ��� ���� ��� /////////////////////////////////////////////////////////////////////////////////////////////
const Page mspDebugADCaltShift =
{
    Item_Page, &mpADC, 0,
    {
        "��� ���� ���", "ALT SHIFT MEM",
        "",
        ""
    },
    Page_DebugADCaltShift,
    {
        (void*)&mgDebugADCaltShift
    }
};

const Governor mgDebugInfoNumber =
{
    Item_Governor, &mspDebugInformation,
    {
        "�����", "Number"
    },
    {
        "��������� ������ ��������� ������",
        "Installation numbers of the serial number"
    },
    EmptyFuncBV,
    &set.debug.infoNumber, 0, 100
};

const Governor mgDebugInfoYear =
{
    Item_Governor, &mspDebugInformation,
    {
        "���", "Year"
    },
    {
        "��������� ���� ��������� ������",
        "Installation year serial number"
    },
    EmptyFuncBV,
    &set.debug.infoYear, 2015, 2050
};

const Governor mgDebugInfoVersion =
{
    Item_Governor, &mspDebugInformation,
    {
        "������", "Version"
    },
    {
        "��������� ������ ������",
        "Setting the version number"
    },
    EmptyFuncBV,
    &set.debug.infoVer, 10, 100
};

// ������� - ���������� ////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Page mspDebugInformation =
{
    Item_Page, &mpDebug,
    {
        "����������", "INFORMATION",
    },
    {
        "��������� ������ ���������� ��� ���� ������-����������",
        "It allows you to enter information for SERVICE INFORMATION menu"
    },
    EmptyFuncBV, Page_DebugInformation,
    {
        (void*)&mgDebugInfoNumber,
        (void*)&mgDebugInfoYear,
        (void*)&mgDebugInfoVersion
    }
};
*/
