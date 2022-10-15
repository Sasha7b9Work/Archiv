#include "defines.h"
#include "Log.h"
#include "PageChannels.h"
#include "Settings/SettingsTypes.h"
#include "Menu/MenuItems.h"
#include "Hardware/Panel.h"
#include "FPGA/FPGA.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "Utils/Dictionary.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageChannels
 *  @{
 */

extern const PageBase mainPage;
extern const PageBase pChanA;
extern const PageBase pChanB;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char chanInputRu[] =   "1. \"���\" - �������� ������ �� �����.\n"
                                    "2. \"����\" - �� �������� ������ �� �����.";
static const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

static const char chanCoupleRu[] =  "����� ��� ����� � ���������� �������.\n"
                                    "1. \"����\" - �������� ����.\n"
                                    "2. \"�����\" - �������� ����.\n"
                                    "3. \"�����\" - ���� ������� � �����.";
static const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

static const char chanFiltrRu[] = "��������/��������� ������ ��� ����������� ������ �����������.";
static const char chanFiltrEn[] = "Includes/switches-off the filter for restriction of a pass-band.";

static const char chanInverseRu[] = "��� \"���\" ������ �� ������ ����� ����������� ������ ������������ U = 0�.";
static const char chanInverseEn[] = "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

static const char chanMultiplierRu[] = "���������� �������:\n\"x1\" - ������ �� �����������.\n\"x10\" - ������ ����������� � 10 ���";
static const char chanMultiplierEn[] = "Attenuation: \n\"x1\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";


//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannels::OnChanged_InputA(bool)
{
    Panel::EnableLEDChannelA(sChannel_Enabled(A));
}

DEF_CHOICE_2(       mcInputA,                                                                                                 //--- ����� 1 - ���� ---
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_ENABLED_A, pChanA, FuncActive, PageChannels::OnChanged_InputA, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannels::OnChanged_CoupleA(bool)
{
    FPGA::SetModeCouple(A, SET_COUPLE_A);
}

DEF_CHOICE_3(       mcCoupleA,                                                                                               //--- ����� 1 - ����� ---
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����",  "AC",
    "�����", "DC",
    "�����", "Ground",
    SET_COUPLE_A, pChanA, FuncActive, PageChannels::OnChanged_CoupleA, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannels::OnChanged_FiltrA(bool)
{
    FPGA::EnableChannelFiltr(A, FILTR_A);
}

DEF_CHOICE_2(       mcFiltrA,                                                                                               //--- ����� 1 - ������ ---
    "������", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    FILTR_A, pChanA, FuncActive, PageChannels::OnChanged_FiltrA, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_InverseA(bool)
{
    FPGA::SetRShift(A, SET_RSHIFT_A);
}

DEF_CHOICE_2(       mcInverseA,                                                                                           //--- ����� 1 - �������� ---
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_INVERSE_A, pChanA, FuncActive, OnChanged_InverseA, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcMultiplierA,                                                                                       //--- ����� 1 - ��������� ---
    "���������", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "�1",  "x1",
    "x10", "x10",
    SET_DIVIDER(A), pChanA, FuncActive, FuncChangedChoice, FuncDraw
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pChanA,                                                                                                             // ����� 1 ///
    "����� 1", "CHANNEL 1",
    "�������� ��������� ������ 1.",
    "Contains settings of the channel 1.",
    mcInputA,       // ����� 1 - ����
    mcCoupleA,      // ����� 1 - �����
    mcFiltrA,       // ����� 1 - ������
    mcInverseA,     // ����� 1 - ��������
    mcMultiplierA,  // ����� 1 - ���������
    Page_ChannelA, &mainPage, FuncActive, EmptyPressPage
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannels::OnChanged_InputB(bool)
{
    Panel::EnableLEDChannelB(sChannel_Enabled(B));
}

DEF_CHOICE_2(       mcInputB,                                                                                                 //--- ����� 2 - ���� ---
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_ENABLED_B, pChanB, FuncActive, PageChannels::OnChanged_InputB, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannels::OnChanged_CoupleB(bool)
{
    FPGA::SetModeCouple(B, SET_COUPLE_B);
}

DEF_CHOICE_3(       mcCoupleB,                                                                                               //--- ����� 2 - ����� ---
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����",  "AC",
    "�����", "DC",
    "�����", "Ground",
    SET_COUPLE_B, pChanB, FuncActive, PageChannels::OnChanged_CoupleB, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannels::OnChanged_FiltrB(bool)
{
    FPGA::EnableChannelFiltr(B, FILTR_B);
}

DEF_CHOICE_2(       mcFiltrB,                                                                                               //--- ����� 2 - ������ ---
    "������", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    FILTR_B, pChanB, FuncActive, PageChannels::OnChanged_FiltrB, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_InverseB(bool)
{
    FPGA::SetRShift(B, SET_RSHIFT_B);
}

DEF_CHOICE_2(       mcInverseB,                                                                                           //--- ����� 2 - �������� ---
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_INVERSE_B, pChanB, FuncActive, OnChanged_InverseB, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcMultiplierB,                                                                                       //--- ����� 2 - ��������� ---
    "���������", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "�1",  "x1",
    "x10", "x10",
    SET_DIVIDER(B), pChanB, FuncActive, FuncChangedChoice, FuncDraw
)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pChanB,                                                                                                             // ����� 2 ///
    "����� 2", "CHANNEL 2",
    "�������� ��������� ������ 2.",
    "Contains settings of the channel 2.",
    mcInputB,       // ����� 2 - ����
    mcCoupleB,      // ����� 2 - �����
    mcFiltrB,       // ����� 2 - ������
    mcInverseB,     // ����� 2 - ��������
    mcMultiplierB,  // ����� 2 - ���������
    Page_ChannelB, &mainPage, FuncActive, EmptyPressPage
)


/** @}  @}
 */
