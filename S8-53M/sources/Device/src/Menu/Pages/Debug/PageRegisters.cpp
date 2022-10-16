// 2021/05/12 11:06:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Settings/SettingsNRST.h"




DEF_CHOICE_2(cLaunchFPGA, PageDebug::PageConsole::PageRegisters::self,
    "������", "Launch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.launch, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cRD_FL, PageDebug::PageConsole::PageRegisters::self,
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.flag, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cRShiftA, PageDebug::PageConsole::PageRegisters::self,
    "U ��. 1�", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rShiftA, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cRShiftB, PageDebug::PageConsole::PageRegisters::self,
    "U ��. 2�", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rShiftB, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cTrigLev, PageDebug::PageConsole::PageRegisters::self,
    "U �����.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.trigLev, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cRangeA, PageDebug::PageConsole::PageRegisters::self,
    "�����/��� 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rangeA, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cRangeB, PageDebug::PageConsole::PageRegisters::self,
    "�����/��� 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.rangeB, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cTrigParam, PageDebug::PageConsole::PageRegisters::self,
    "�����. �����.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.trigParam, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cChanParamA, PageDebug::PageConsole::PageRegisters::self,
    "�����. ���. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.chanParamA, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cChanParamB, PageDebug::PageConsole::PageRegisters::self,
    "�����. ���. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.chanParamB, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cTBase, PageDebug::PageConsole::PageRegisters::self,
    "�����/���", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.tBase, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_CHOICE_2(cTShift, PageDebug::PageConsole::PageRegisters::self,
    "� ��.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    setNRST.show_registers.tShift, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



DEF_PAGE_12(pageRegisters, PageDebug::PageConsole::self, NamePage::DebugShowRegisters,
    "��������", "REGISTERS",
    "",
    "",
    cLaunchFPGA,
    cRD_FL,
    cRShiftA,
    cRShiftB,
    cTrigLev,
    cRangeA,
    cRangeB,
    cTrigParam,
    cChanParamA,
    cChanParamB,
    cTBase,
    cTShift,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageConsole::PageRegisters::self = &pageRegisters;
