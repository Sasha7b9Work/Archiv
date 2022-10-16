// 2021/05/12 11:20:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Settings/Settings.h"


static void OnChanged_A()
{  
    SettingsNRST::ExecuteOnChanged();
}


DEF_GOVERNOR(gShiftA, PageDebug::PageADC::PageBalance::self,
    "�������� 1", "Offset 1",
    "",
    "",
    NRST_BALANCE_ADC_A, -125, 125, nullptr, OnChanged_A
)



static void OnChanged_ShiftB()
{
    SettingsNRST::ExecuteOnChanged();
}


DEF_GOVERNOR(gShiftB, PageDebug::PageADC::PageBalance::self,
    "�������� 2", "Offset 2",
    "",
    "",
    NRST_BALANCE_ADC_B, -125, 125, nullptr, OnChanged_ShiftB
)



DEF_PAGE_2(pageBalanceADC, PageDebug::PageADC::self, NamePage::DebugADCbalance,
    "������", "BALANCE",
    "",
    "",
    gShiftA,
    gShiftB,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageADC::PageBalance::self = &pageBalanceADC;
