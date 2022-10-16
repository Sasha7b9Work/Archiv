// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Warnings.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"


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
static const char chanInverseEn[] =
                         "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

static const char chanMultiplierRu[] =
                          "���������� �������:\n\"x1\" - ������ �� �����������.\n\"x10\" - ������ ����������� � 10 ���";
static const char chanMultiplierEn[] =
                "Attenuation: \n\"x1\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";


void PageChannel::EnableFiltr(const Chan &ch, bool enable)
{
    FILTR(ch) = enable;

    BUS_FPGA::SetAttribChannelsAndTrig(ch.IsA() ? TypeWriteAnalog::ChanParamA : TypeWriteAnalog::ChanParamB);
}


void PageChannel::A::OnChanged_Input(bool)
{
    led_ChA.SwitchToState(ChA.IsEnabled());
}

DEF_CHOICE_2(mcInputA, PageChannel::A::self,
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    ENABLED_A, nullptr, PageChannel::A::OnChanged_Input, nullptr
)

void PageChannel::A::OnChanged_Couple(bool)
{
    ModeCouple::Set(ChA, MODE_COUPLE_A);
}

DEF_CHOICE_3(mcCoupleA, PageChannel::A::self,
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����",  "AC",
    "�����", "DC",
    "�����", "Ground",
    MODE_COUPLE_A, nullptr, PageChannel::A::OnChanged_Couple, nullptr
)

void PageChannel::A::OnChanged_Filtr(bool)
{
    EnableFiltr(ChA, FILTR_A);
}

DEF_CHOICE_2(mcFiltrA, PageChannel::A::self,
    "������", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    FILTR_A, nullptr, PageChannel::A::OnChanged_Filtr, nullptr
)

static void OnChanged_InverseA(bool)
{
    RShift::Set(ChA, RSHIFT_A);
}

DEF_CHOICE_2(mcInverseA, PageChannel::A::self,
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    INVERSED_A, nullptr, OnChanged_InverseA, nullptr
)

DEF_CHOICE_2(mcMultiplierA, PageChannel::A::self,
    "���������", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "�1", "x1",
    "x10", "x10",
    DIVIDER_A, nullptr, nullptr, nullptr
)

void PageChannel::A::OnPress_Balance()
{
    Calibrator::Balancer::PerformOnGround(ChA);
}

DEF_BUTTON(mbBalanceA, PageChannel::A::self,
    "�������������", "Balance",
    "������������� �����",
    "Balancing channel",
    nullptr, PageChannel::A::OnPress_Balance
)

DEF_PAGE_5(pageChanA, PageMain::self, NamePage::ChannelA,
    "����� 1", "CHANNEL 1",
    "�������� ��������� ������ 1.",
    "Contains settings of the channel 1.",
    mcInputA,
    mcCoupleA,
    mcFiltrA,
    mcInverseA,
    mcMultiplierA,
//    mbBalanceA,
    nullptr, nullptr, nullptr, nullptr
)


void PageChannel::B::OnChanged_Input(bool)
{
    led_ChB.SwitchToState(ChB.IsEnabled());
}

DEF_CHOICE_2(mcInputB, PageChannel::B::self,
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    ENABLED_B, nullptr, PageChannel::B::OnChanged_Input, nullptr
)


void PageChannel::B::OnChanged_Couple(bool)
{
    ModeCouple::Set(ChB, MODE_COUPLE_B);
}

DEF_CHOICE_3(mcCoupleB, PageChannel::B::self,
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����", "AC",
    "�����", "DC",
    "�����", "Ground",
    MODE_COUPLE_B, nullptr, PageChannel::B::OnChanged_Couple, nullptr
)


void PageChannel::B::OnChanged_Filtr(bool)
{
    EnableFiltr(ChB, FILTR_B);
}

DEF_CHOICE_2(mcFiltrB, PageChannel::B::self,
    "������", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    FILTR_B, nullptr, PageChannel::B::OnChanged_Filtr, nullptr
)

static void OnChanged_InverseB(bool)
{
    RShift::Set(ChB, RSHIFT_B);
}

DEF_CHOICE_2(mcInverseB, PageChannel::B::self,
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    INVERSED_B, nullptr, OnChanged_InverseB, nullptr
)

DEF_CHOICE_2(mcMultiplierB, PageChannel::B::self,
    "���������", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "�1", "x1",
    "x10", "x10",
    DIVIDER_B, nullptr, nullptr, nullptr
)

void PageChannel::B::OnPress_Balance()
{
    Calibrator::Balancer::PerformOnGround(ChB);
}

DEF_BUTTON(mbBalanceB, PageChannel::B::self,
    "�������������", "Balance",
    "������������� �����",
    "Balancing channel",
    nullptr, PageChannel::B::OnPress_Balance
)


DEF_PAGE_5(pageChanB, PageMain::self, NamePage::ChannelB,
    "����� 2", "CHANNEL 2",
    "�������� ��������� ������ 2.",
    "Contains settings of the channel 2.",
    mcInputB,
    mcCoupleB,
    mcFiltrB,
    mcInverseB,
    mcMultiplierB,
//    mbBalanceB,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageChannel::A::self = &pageChanA;
const Page *PageChannel::B::self = &pageChanB;
