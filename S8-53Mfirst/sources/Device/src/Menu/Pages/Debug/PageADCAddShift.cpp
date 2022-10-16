// 2021/05/12 11:54:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


//DEF_GOVERNOR(gFirstByte, PageDebug::PageADC::PageAltRShift::self,
//    "������ ����", "First byte",
//    "",
//    "",
//    setNRST.adc.first_byte, -1, 1, nullptr, nullptr, nullptr
//)




static void OnChanged_Mode(bool)
{
    RShift::Load(ChA);
    RShift::Load(ChB);

    SettingsNRST::ExecuteOnChanged();
}

DEF_CHOICE_2( cMode, PageDebug::PageADC::PageAltRShift::self,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "�������",  "��������",
    setNRST.adc.type_rshift, nullptr, OnChanged_Mode, nullptr
)




static void OnChanged_A()
{
    RShift::Load(ChA);

    SettingsNRST::ExecuteOnChanged();
}

static void OnChanged_B()
{
    RShift::Load(ChB);

    SettingsNRST::ExecuteOnChanged();
}



static void OnPress_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                NRST_RSHIFT(ch, range, mode) = 0;
            }
        }
    }

    RShift::Load(ChA);
    RShift::Load(ChB);

    SettingsNRST::ExecuteOnChanged();
}


DEF_BUTTON(bReset, PageDebug::PageADC::PageAltRShift::self,
    "�����", "Reset",
    "", "",
    nullptr, OnPress_Reset
)



DEF_GOVERNOR(b2mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "�� 1� 2�� ����", "Shift 1ch 2mV DC",
    "",
    "",
    NRST_RSHIFT_A(Range::_2mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_A
)



DEF_GOVERNOR(b2mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "�� 2� 2�� ����", "Shift 2ch 2mV DC",
    "",
    "",
    NRST_RSHIFT_B(Range::_2mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_B
)



DEF_GOVERNOR(b5mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "�� 1� 5�� ����", "Shift 1ch 5mV DC",
    "",
    "",
    NRST_RSHIFT_A(Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_A
)



DEF_GOVERNOR(b5mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "�� 2� 5�� ����", "Shift 2ch 5mV DC",
    "",
    "",
    NRST_RSHIFT_B(Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_B
)



DEF_GOVERNOR(b10mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "�� 1� 10�� ����", "Shift 1ch 10mV DC",
    "",
    "",
    NRST_RSHIFT_A(Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_A
)



DEF_GOVERNOR(b10mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "�� 2� 10�� ����", "Shift 2ch 10mV DC",
    "",
    "",
    NRST_RSHIFT_B(Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_B
)



DEF_PAGE_2(pageAltShift, PageDebug::PageADC::self, NamePage::DebugADCrShift,
    "��� ����", "ADD RSHFIT",
    "",
    "",
//    gFirstByte,
    cMode,
    bReset,
//    b2mV_DC_A,
//    b2mV_DC_B,
//    b5mV_DC_A,
//    b5mV_DC_B,
//    b10mV_DC_A,
//    b10mV_DC_B,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDebug::PageADC::PageAltRShift::self = &pageAltShift;
