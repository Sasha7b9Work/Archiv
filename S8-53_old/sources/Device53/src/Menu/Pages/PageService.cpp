#include "Menu/Pages/Definition.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAMath.h"
#include "Menu/Menu.h"
#include "Hardware/CPU.h"
#include "Hardware/Panel.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"

/** @addtogroup Menu
 *  @{
 *  @addtogroup PageService
 *  @{
 */

extern const PageBase mainPage;
extern const PageBase pService;
extern const PageBase ppCalibrator;
extern const PageBase pppMath_Function;
extern const PageBase ppMath;
extern const PageBase pppMath_FFT;
extern const PageBase ppppMath_FFT_Cursors;
extern const PageBase ppEthernet;
extern const PageBase ppInformation;

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncDraw()
{
    Painter::BeginScene(Color::BACK);

    Painter::DrawTextInRectWithTransfersC(30, 110, 300, 200, "����������� ����� �������� �������� ������ ����/����.\n"
                                         "������� ����� ������ ������, ���� ����� �� �����.", Color::FILL);

    Painter::EndScene();
}

static void OnTimerDraw()
{
    Display::Update();
}

static void OnPress_ResetSettings()
{
    Panel::Disable();
    Display::SetDrawMode(DrawMode_Hand, FuncDraw);
    Timer::SetAndEnable(kTimerDisplay, OnTimerDraw, 100);

    if (Panel::WaitPressingButton() == B_Start)
    {
        Settings::Load(true);
    }

    Timer::Disable(kTimerDisplay);
    Display::SetDrawMode(DrawMode_Auto, 0);
    Panel::Enable();
}

DEF_BUTTON(         bResetSettings,                                                                                  //--- ������ - ����� �������� ---
    "����� ��������", "Reset settings",
    "����� �������� �� ��������� �� ���������",
    "Reset to default settings",
    pService, FuncActive, OnPress_ResetSettings, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_AutoSearch()
{
    FPGA::StartAutoFind();
}

DEF_BUTTON(         bAutoSearch,                                                                                      //--- ������ - ����� ������� ---
    "����� �������", "Find signal",
    "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
    "Sets optimal settings for the oscilloscope signal on channel 1",
    pService, FuncActive, OnPress_AutoSearch, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Calibrator_Mode(bool)
{
    FPGA::SetCalibratorMode(CALIBRATOR);
}

DEF_CHOICE_3(       cCalibrator_Mode,                                                                       //--- ������ - ���������� - ���������� ---
    "����������", "Calibrator",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "�����", "DC",
    "����",  "AC",
    "0�",    "OV",
    CALIBRATOR, ppCalibrator, FuncActive, OnChanged_Calibrator_Mode, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrator_Calibrate()
{
    gStateFPGA.needCalibration = true;
}

DEF_BUTTON(         cCalibrator_Calibrate,                                                                 //--- ������ - ���������� - ����������� ---
    "�����������", "Calibrate",
    "������ ��������� ����������",
    "Running the calibration procedure",
    ppCalibrator, FuncActive, OnPress_Calibrator_Calibrate, FuncDraw
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppCalibrator,                                                                                           // ������ - ���������� ///
    "����������", "CALIBRATOR",
    "����������� ������������ � ���������� ������������",
    "Control of the calibrator and calibration of an oscillograph",
    cCalibrator_Mode,       // ������ - ���������� - ����������
    cCalibrator_Calibrate,  // ������ - ���������� - �����������
    Page_Service_Calibrator, &pService, FuncActive, EmptyPressPage
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  sbMath_Function_Exit,                                                          //--- ������ - ���������� - ������� - ����� ---
    pppMath_Function, FuncActive, FuncPress, DrawSB_Exit
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Math_Function_ModeDraw_Disable(int x, int y)
{
    Painter::DrawText(x + 2 + (set.common.lang == English ? 2 : 0), y + 5, set.common.lang == Russian ? "���" : "Dis");
}

static void Draw_Math_Function_ModeDraw_Separate(int x, int y)
{
    Painter::DrawRectangle(x + 3, y + 5, 13, 9);
    Painter::DrawHLine(y + 9, x + 3, x + 16);
    Painter::DrawHLine(y + 10, x + 3, x + 16);
}

static void Draw_Math_Function_ModeDraw_Together(int x, int y)
{
    Painter::DrawRectangle(x + 3, y + 5, 13, 9);
}

static void OnPress_Math_Function_ModeDraw()
{
    if (FFT_ENABLED)
    {
        Display::ShowWarningBad(ImpossibleEnableMathFunction);
    }
    else
    {
        CircleIncrease<int8>((int8 *)&FUNC_MODE_DRAW, 0, 2);
    }
}

static void Draw_Math_Function_ModeDraw(int x, int y)
{
    static const pFuncVII funcs[3] =
    {
        Draw_Math_Function_ModeDraw_Disable,
        Draw_Math_Function_ModeDraw_Separate,
        Draw_Math_Function_ModeDraw_Together
    };
    funcs[FUNC_MODE_DRAW](x, y);
}

DEF_SMALL_BUTTON_HINTS_3(   sbMath_Function_ModeDraw,                                                  //--- ������ - ���������� - ������� - ����� ---
    "�����", "Display",
    "�������� ����� ����������� ��������������� �������",
    "Chooses the mode of display of a mathematical signal",
    pppMath_Function, FuncActive, OnPress_Math_Function_ModeDraw, Draw_Math_Function_ModeDraw,
    Draw_Math_Function_ModeDraw_Disable,   {"����� �������������� ������� ��������",
                                           "The conclusion of mathematical function is disconnected"},
    Draw_Math_Function_ModeDraw_Separate,  {"������� � �������������� ������� ��������� � ������ �����",
                                           "Signals and mathematical function are removed in different windows"},
    Draw_Math_Function_ModeDraw_Together,  {"������� � �������������� ������� ��������� � ����� ����",
                                           "Signals and mathematical function are removed in one window"}
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Math_Function_Type_Sum(int x, int y)
{
    Painter::DrawHLine(y + 9, x + 4, x + 14);
    Painter::DrawVLine(x + 9, y + 4, y + 14);
}

static void Draw_Math_Function_Type_Mul(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 4, y + 3, SYMBOL_MATH_FUNC_MUL);
    Painter::SetFont(TypeFont_8);
}

static void OnPress_Math_Function_Type()
{
    CircleIncrease<int8>((int8 *)&MATH_FUNC, 0, 1);
}

static void Draw_Math_Function_Type(int x, int y)
{
    const pFuncVII funcs[2] = {Draw_Math_Function_Type_Sum, Draw_Math_Function_Type_Mul};
    funcs[MATH_FUNC](x, y);
}

DEF_SMALL_BUTTON_HINTS_2(   sbMath_Function_Type,                                                        //--- ������ - ���������� - ������� - ��� ---
    "���", "Type",
    "����� �������������� �������",
    "Choice of mathematical function",
    pppMath_Function, FuncActive, OnPress_Math_Function_Type, Draw_Math_Function_Type,
    Draw_Math_Function_Type_Sum, {"��������",  "Addition"},
    Draw_Math_Function_Type_Mul, {"���������", "Multiplication"}
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Math_Function_ModeRegSet_Range(int x, int y)
{
    Painter::DrawChar(x + 7, y + 5, set.common.lang == Russian ? 'M' : 'S');
}

static void Draw_Math_Function_ModeRegSet_RShift(int x, int y)
{
    Painter::DrawText(x + 5 - (set.common.lang == English ? 3 : 0), y + 5, set.common.lang == Russian ? "��" : "Shif");
}

static void Draw_Math_Function_ModeRegSet(int x, int y)
{
    const pFuncVII funcs[2] = {Draw_Math_Function_ModeRegSet_Range, Draw_Math_Function_ModeRegSet_RShift};
    funcs[MATH_MODE_REG_SET](x, y);
}

static void OnPress_Math_Function_ModeRegSet()
{
    CircleIncrease<int8>((int8 *)&MATH_MODE_REG_SET, 0, 1);
}

DEF_SMALL_BUTTON_HINTS_2(   sbMath_Function_ModeRegSet,                                //--- ������ - ���������� - ������� - ����� ����� ��������� ---
    "����� ����� ���������", "Mode regulator SET",
    "����� ������ ����� ��������� - ���������� ��������� ��� ���������",
    "Choice mode regulcator ��������� - management of scale or shift",
    pppMath_Function, FuncActive, OnPress_Math_Function_ModeRegSet, Draw_Math_Function_ModeRegSet,
    Draw_Math_Function_ModeRegSet_Range,  {"���������� ���������", "Management of scale"},
    Draw_Math_Function_ModeRegSet_RShift, {"���������� ���������", "Management of shift"}
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Math_Function_RangeA()
{
    SET_RANGE_MATH = SET_RANGE_A;
    MATH_MULTIPLIER = SET_DIVIDER_A;
}

static void Draw_Math_Function_RangeA(int x, int y)
{
    Painter::DrawChar(x + 8, y + 5, '1');
}

DEF_SMALL_BUTTON(   sbMath_Function_RangeA,                                              //--- ������ - ���������� - ������� - ������� 1-�� ������ ---
    "������� 1-�� ������", "Scale of the 1st channel",
    "���� ������� ��� ��������������� ������� �� ������� ������",
    "Takes scale for a mathematical signal from the first channel",
    pppMath_Function, FuncActive, OnPress_Math_Function_RangeA, Draw_Math_Function_RangeA
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Math_Function_RangeB()
{
    SET_RANGE_MATH = SET_RANGE_B;
    MATH_MULTIPLIER = SET_DIVIDER_B;
}

static void Draw_Math_Function_RangeB(int x, int y)
{
    Painter::DrawChar(x + 8, y + 5, '2');
}

DEF_SMALL_BUTTON(   sbMath_Function_RangeB,                                              //--- ������ - ���������� - ������� - ������� 2-�� ������ ---
    "������� 2-�� ������", "Scale of the 2nd channel",
    "���� ������� ��� ��������������� ������� �� ������� ������",
    "Takes scale for a mathematical signal from the second channel",
    pppMath_Function, FuncActive, OnPress_Math_Function_RangeB, Draw_Math_Function_RangeB
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Math_Function()
{
    return !FFT_ENABLED;
}

static void OnPress_Math_Function()
{
    if (FFT_ENABLED)
    {
        Display::ShowWarningBad(ImpossibleEnableMathFunction);
    }
}

static void OnRegSet_Math_Function(int delta)
{
    if (FUNC_MODE_DRAW_IS_DISABLED)
    {
        return;
    }

    if (MATH_MODE_REG_SET_IS_RSHIFT)
    {
        int16 prevRShift = SET_RSHIFT_MATH;
        int16 rShift = prevRShift;
        if (delta > 0)
        {
            if (rShift < RShiftMax)
            {
                rShift += 4 * STEP_RSHIFT;
                LIMIT_ABOVE(rShift, RShiftMax);
                if (prevRShift < RShiftZero && rShift > RShiftZero)
                {
                    rShift = RShiftZero;
                }
                Sound::RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
        else if (delta < 0)
        {
            if (rShift > RShiftMin)
            {
                rShift -= 4 * STEP_RSHIFT;
                LIMIT_BELOW(rShift, RShiftMin);
                if (prevRShift > RShiftZero && rShift < RShiftZero)
                {
                    rShift = RShiftZero;
                }
                Sound::RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
    }
    if (MATH_MODE_REG_SET_IS_RANGE)
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = RSHIFT_2_ABS(SET_RSHIFT_MATH, SET_RANGE_MATH);

        if (sum > 2)
        {
            if (SET_RANGE_MATH < RangeSize - 1)
            {
                ++SET_RANGE_MATH;
                SET_RSHIFT_MATH = (int16)RSHIFT_2_REL(rShiftAbs, SET_RANGE_MATH);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (SET_RANGE_MATH > 0)
            {
                --SET_RANGE_MATH;
                SET_RSHIFT_MATH = (int16)RSHIFT_2_REL(rShiftAbs, SET_RANGE_MATH);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
    }
}

DEF_PAGE_SB(        pppMath_Function,                                                                             // ������ - ���������� - ������� ///
    "�������", "FUNCTION",
    "��������� � ����� �������������� ������� - �������� ��� ���������",
    "Installation and selection of mathematical functions - addition or multiplication",
    &sbMath_Function_Exit,       // ������ - ���������� - ������� - �����
    &sbMath_Function_ModeDraw,   // ������ - ���������� - ������� - �����
    &sbMath_Function_Type,       // ������ - ���������� - ������� - ���
    &sbMath_Function_ModeRegSet, // ������ - ���������� - ������� - ����� ����� ���������
    &sbMath_Function_RangeA,     // ������ - ���������� - ������� - ������� 1-�� ������
    &sbMath_Function_RangeB,     // ������ - ���������� - ������� - ������� 2-�� ������
    PageSB_Service_Function, &ppMath, IsActive_Math_Function, OnPress_Math_Function, EmptyFuncVV, OnRegSet_Math_Function
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cMath_FFT_Enable,                                                             //--- ������ - ���������� - ������ - ����������� ---
    "�����������", "Display",
    "�������� � ��������� ����������� �������",
    "Enables or disables the display of the spectrum",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    FFT_ENABLED, pppMath_FFT, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cMath_FFT_Scale,                                                                    //--- ������ - ���������� - ������ - ����� ---
    "�����", "Scale",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "��������", "Log",
    "��������", "Linear",
    SCALE_FFT, pppMath_FFT, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(       cMath_FFT_Source,                                                                //--- ������ - ���������� - ������ - �������� ---
    "��������", "Source",
    "����� ��������� ��� ������� �������",
    "Selecting the source for the calculation of the spectrum",
    "����� 1",     "Channel 1",
    "����� 2",     "Channel 2",
    "����� 1 + 2", "Channel 1 + 2",
    SOURCE_FFT, pppMath_FFT, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       cMath_FFT_Window,                                                                    //--- ������ - ���������� - ������ - ���� ---
    "����", "Window",
    "����� ���� ��� ������� �������",
    "Sets the window to calculate the spectrum",
    "�����������", "Rectangle",
    "��������",    "Hamming",
    "��������",    "Blackman",
    "�����",       "Hann",
    WINDOW_FFT, pppMath_FFT, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Math_FFT_Cursors_Exit()
{
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON_EXIT(  cMath_FFT_Cursors_Exit,                                               //--- ������ - ���������� - ������ - ������� - ����� ---
    ppppMath_FFT_Cursors, 
    FuncActive, OnPress_Math_FFT_Cursors_Exit, DrawSB_Exit
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Math_FFT_Cursors_Source()
{
    FFT_CUR_CURSOR = (FFT_CUR_CURSOR + 1) % 2;
}

static void Draw_Math_FFT_Cursors_Source(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, FFT_CUR_CURSOR_IS_0 ? "1" : "2");
}

DEF_SMALL_BUTTON(   cMath_FFT_Cursors_Source,                                              //--- ������ - ���������� - ������ - ������� - �������� ---
    "��������", "Source",
    "����� ��������� ��� ������� �������",
    "Source choice for calculation of a range",
    ppppMath_FFT_Cursors, FuncActive, OnPress_Math_FFT_Cursors_Source, Draw_Math_FFT_Cursors_Source
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Math_FFT_Cursors()
{
    return FFT_ENABLED;
}

static void OnRegSet_Math_FFT_Cursors(int angle)
{
    FFT_POS_CURSOR(FFT_CUR_CURSOR) += (uint8)angle;
    Sound::RegulatorShiftRotate();
}

DEF_PAGE_SB(        ppppMath_FFT_Cursors,                                                                // ������ - ���������� - ������ - ������� ///
    "�������", "CURSORS",
    "�������� ������� ��� ��������� ���������� �������",
    "Includes cursors to measure the parameters of the spectrum",
    &cMath_FFT_Cursors_Exit,   // ������ - ���������� - ������ - ������� - �����
    &cMath_FFT_Cursors_Source, // ������ - ���������� - ������ - ������� - ��������
    0,
    0,
    0,
    0,
    PageSB_Service_FFT_Cursors, &pppMath_FFT, IsActive_Math_FFT_Cursors, EmptyPressPage, FuncDrawPage, OnRegSet_Math_FFT_Cursors
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Math_FFT_Limit()
{
    return SCALE_FFT_IS_LOG;
}

DEF_CHOICE_3(       cMath_FFT_Limit,                                                                 //--- ������ - ���������� - ������ - �������� ---
    "��������", "Range",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "Here you can set the limit of monitoring the power spectrum",
    "-40��", "-40dB",
    "-60��", "-60dB",
    "-80��", "-80dB",
    MAX_DB_FFT, pppMath_FFT, IsActive_Math_FFT_Limit, FuncChangedChoice, FuncDraw
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Math_FFT()
{
    return FUNC_MODE_DRAW_IS_DISABLED;
}

static void OnPress_Math_FFT()
{
    if (!IsActive_Math_FFT())
    {
        Display::ShowWarningBad(ImpossibleEnableFFT);
    }
}

DEF_PAGE_6(         pppMath_FFT,                                                                                   // ������ - ���������� - ������ ///
    "������", "SPECTRUM",
    "����������� ������� �������� �������",
    "Mapping the input signal spectrum",
    cMath_FFT_Enable,       // ������ - ���������� - ������ - �����������
    cMath_FFT_Scale,        // ������ - ���������� - ������ - �����
    cMath_FFT_Source,       // ������ - ���������� - ������ - ��������
    cMath_FFT_Window,       // ������ - ���������� - ������ - ����
    ppppMath_FFT_Cursors,   // ������ - ���������� - ������ - �������
    cMath_FFT_Limit,        // ������ - ���������� - ������ - ��������
    Page_MathFFT, &ppMath, IsActive_Math_FFT, OnPress_Math_FFT
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppMath,                                                                                                 // ������ - ���������� ///
    "����������", "MATH",
    "�������������� ������� � ���",
    "Mathematical functions and FFT",
    pppMath_Function,   // ������ - ���������� - �������
    pppMath_FFT,        // ������ - ���������� - ������
    Page_Math, &pService, FuncActive, FuncPress
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Ethernet_Enable(bool)
{
    Display::ShowWarningGood(NeedRebootDevice2);
    Display::ShowWarningGood(NeedRebootDevice1);
}

DEF_CHOICE_2(       cEthernet_Enable,                                                                           //--- ������ - ETHERNET - Ethernet ---
    "Ethernet", "Ethernet"
    ,
    "����� ������������� ethernet, �������� \"��������\" � ��������� ������.\n"
    "����� ��������� ethernet, �������� \"���������\" � ��������� ������."
    ,
    "To involve ethernet, choose \"Included\" and switch off the device.\n"
    "To disconnect ethernet, choose \"Disconnected\" and switch off the device.",
    "��������",  "Included",
    "���������", "Disconnected",
    ETH_ENABLE, ppEthernet, FuncActive, OnChanged_Ethernet_Enable, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_IP_ADDRESS(     ipEthernet_IP,                                                                              //--- ������ - ETHERNET - IP ����� ---
    "IP �����", "IP-address",
    "��������� IP ������",
    "Set of IP-address",
    IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3, &PORT,
    ppEthernet, FuncActive, OnChanged_Ethernet_Enable
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_IP_ADDRESS(     ipEthernet_Mask,                                                                       //--- ������ - ETHERNET - ����� ������� ---
    "����� �������", "Network mask",
    "��������� ����� �������",
    "Set of network mask",
    NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3, 0,
    ppEthernet, FuncActive, OnChanged_Ethernet_Enable
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_IP_ADDRESS(     ipEthernet_Gateway,                                                                             //--- ������ - ETHERNET - ���� ---
    "����", "Gateway",
    "��������� ������ ��������� �����",
    "Set of gateway address",
    GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3, 0,
    ppEthernet, FuncActive, OnChanged_Ethernet_Enable
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_MAC_ADDRESS(    macEthernet_MAC,                                                                           //--- ������ - ETHERNET - ��� ����� ---
    "��� �����", "MAC-address",
    "��������� ����������� ������",
    "Set of MAC-address",
    MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5,
    ppEthernet, FuncActive, OnChanged_Ethernet_Enable
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         ppEthernet,                                                                                               // ������ - ETHERNET ///
    "ETHERNET", "ETHERNET",
    "��������� ethernet",
    "Settings of ethernet",
    cEthernet_Enable,       // ������ - ETHERNET - Ethernet
    ipEthernet_IP,          // ������ - ETHERNET - IP �����
    ipEthernet_Mask,        // ������ - ETHERNET - ����� �������
    ipEthernet_Gateway,     // ������ - ETHERNET - ����
    macEthernet_MAC,        // ������ - ETHERNET - ��� �����
    Page_Service_Ethernet, &pService, FuncActive, FuncPress
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cSound,                                                                                                    //--- ������ - ���� ---
    "����", "Sound",
    "���������/���������� �����",
    "Inclusion/switching off of a sound",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SOUND_ENABLED, pService, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cLang,                                                                                                     //--- ������ - ���� ---
    "����", "Language",
    "��������� ������� ���� ����",
    "Allows you to select the menu language",
    "�������",    "Russian",
    "����������", "English",
    LANG, pService, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
DEF_TIME(           tTime,                                                                                                    //--- ������ - ����� ---
    "�����", "Time"
    ,
    "��������� �������� �������.\n������� ������:\n"
    "������ �� ������� ���� \"�����\". ��������� ���� ��������� �������� �������. ��������� ��������� ������ �� �������� ����������, �������������� "
    "�������� ���������� \"�����\", �������� ����, ������, �������, ���, �����, ��� �����. ���������� ������� ������������ �������� ��������. "
    "��������� ����� ��������� ���������� ����������� ��������. ����� �������� ����� \"���������\", ������ � ���������� ����� 0.5 ��� ������ �� "
    "������ ����������. ���� ��������� �������� ������� ��������� � ����������� ������ �������� �������. ������� ���������� ��������� ������ �� "
    "����� ������ �������� ������� � �������� ���� ��������� �������� ������� ��� ���������� ������ �������� �������"
    ,
    "Setting the current time. \nPoryadok work:\n"
    "Click on the menu item \"Time\".The menu set the current time.By briefly pressing the button on the numeric keypad of conformity "
    "Control \"Time\", highlight the hours, minutes, seconds, year, month, or a number.The selected item is indicated by a flashing area. "
    "Turn the setting knob to set the desired value. Then highlight \"Save\", press and udrezhivat more than 0.5 seconds, the button on the panel "
    "Control. Menu Setting the current time will be closed to the conservation of the new current time. Pressing a button on the prolonged retention "
    "of any other element will lead to the closure of the current time setting menu without saving the new current time"
    ,
    pService, FuncActive, dServicetime, hours, minutes, secondes, month, day, year
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cModeLongPressButtonTrig,                                                                        //--- ������ - ��� ���� ����� ---
    "��� ���� �����", "Mode long �����"
    ,
    "������������� �������� ��� ����������� ������� ������ �����:\n\"����� ������\" - ��������� ������ ������������� � ����,\n\"�����������\" "
    "- �������������� ����������� � ��������� ������ �������������"
    ,
    "Sets the action for long press CLOCK:\n\"Reset trig lvl\" - to set the trigger level to zero, \n\"Auto level\""
    "- Automatically detect and install the trigger level",
    "����� ������", "Reset trig level",
    "�����������",  "Autolevel",
    MODE_LONG_PRESS_TRIG, pService, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Information_Exit()
{
    Display::SetDrawMode(DrawMode_Auto, 0);
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON_EXIT(  sbInformation_Exit,                                                                      //--- ������ - ���������� - ����� ---
    ppInformation, FuncActive, OnPress_Information_Exit, DrawSB_Exit
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Information_Draw()
{
    Language lang = LANG;

    Painter::BeginScene(Color::BLACK);
    int x = 100;
    int dY = 20;
    int y = 20;
    Painter::DrawRectangle(0, 0, 319, 239, Color::FILL);
    y += dY;
    Painter::DrawText(x, y, lang == Russian ? "����������" : "INFORMATION");
    y += dY;
    Painter::DrawText(x, y, lang == Russian ? "������ : �8-53/1" : "Model : S8-53/1");
    y += dY;

    char buffer[100];
    Painter::DrawText(x, y, lang == Russian ? "����������� �����������:" : "Software:");
    y += dY;
    sprintf(buffer, (const char*)((lang == Russian) ? "������ %s" : "version %s"), NUM_VER);
    Painter::DrawText(x, y, buffer);
    y += dY;

    Painter::SetColor(Color::FILL);
    Painter::DrawFormatText(x, y, "CRC32 : %X", CPU::CalculateCRC32());

    dY = -10;
    Painter::DrawStringInCenterRect(0, 190 + dY, 320, 20, "��� ��������� ������ ������� � ����������� ������ ������");
    Painter::DrawStringInCenterRect(0, 205 + dY, 320, 20, "����� ����������: ���./����. 8-017-262-57-50");
    Painter::DrawStringInCenterRect(0, 220 + dY, 320, 20, "������������: e-mail: mnipi-24(@)tut.by, ���. 8-017-262-57-51");

    Menu::Draw();
    Painter::EndScene();
}

static void OnPress_Information()
{
    Display::SetDrawMode(DrawMode_Hand, Information_Draw);
}

DEF_PAGE_SB(        ppInformation,                                                                                          // ������ - ���������� ///
    "����������", "INFORMATION",
    "������� �� ����� ����������������� ������ ������������",
    "Displays identification data of the oscilloscope",
    &sbInformation_Exit, // ������ - ���������� - �����
    0,
    0,
    0,
    0,
    0,
    PageSB_Service_Information, &pService, FuncActive, OnPress_Information, FuncDrawPage, FuncRegSetPage
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_10(        pService,                                                                                                            // ������ ///
    "������", "SERVICE",
    "�������������� ���������, ����������, ����� �������, �������������� �������",
    "Additional settings, calibration, signal search, mathematical functions",
    bResetSettings,           // ������ - ����� ��������
    bAutoSearch,              // ������ - ����� �������
    ppCalibrator,             // ������ - ����������
    ppMath,                   // ������ - ����������
    ppEthernet,               // ������ - ETHERNET
    cSound,                   // ������ - ����
    cLang,                    // ������ - ����
    tTime,                    // ������ - �����
    cModeLongPressButtonTrig, // ������ - ��� ���� �����
    ppInformation,            // ������ - ����������
    Page_Service, &mainPage, FuncActive, FuncPress
)


/** @}  @}
 */
