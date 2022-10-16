// 2021/05/12 12:01:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"



DEF_GOVERNOR( gGateBottom, PageDebug::PageRandomizer::self,
    "������ ���", "Gates bottom",
    "������������� ����������� ���������� �� �������� ����� ��� �������������",
    "Sets the considered distance from the minimum gate for the randomizer",
    setNRST.rand.gate_min, 0, 20, nullptr, nullptr
)




DEF_GOVERNOR(gGateTop, PageDebug::PageRandomizer::self,
    "������ ����", "Gates top",
    "������������� ����������� ���������� �� ��������� ����� ��� �������������",
    "Sets the considered distance from the maximum gate for the randomizer",
    setNRST.rand.gate_max, 0, 20, nullptr, nullptr
)




DEF_GOVERNOR(gNumAverages, PageDebug::PageRandomizer::self,
    "������.", "Average",
    "",
    "",
    setNRST.rand.num_ave, 1, 32, nullptr, SettingsNRST::ExecuteOnChanged
)



DEF_GOVERNOR(gNumSmooth, PageDebug::PageRandomizer::self,
    "�����������", "Smoothing",
    "",
    "",
    setNRST.rand.num_smooth, 1, 10, nullptr, SettingsNRST::ExecuteOnChanged
)



static void OnChanged_SamplesForGates()
{
    Randomizer::SetNumberMeasuresForGates(setNRST.rand.num_measures_for_gates);

    SettingsNRST::ExecuteOnChanged();
}


DEF_GOVERNOR(gSamplesForGates, PageDebug::PageRandomizer::self,
    "���-�/������", "Samples/gates",
    "",
    "",
    setNRST.rand.num_measures_for_gates, 1, 2500, nullptr, OnChanged_SamplesForGates
)



DEF_CHOICE_2(gShowInfo, PageDebug::PageRandomizer::self,
    "����������", "Information",
    "���������� ���������� � ������� �������������",
    "To show information on randomizer gate",
    "�� ����������", "Hide",
    "����������", "Show",
    setNRST.rand.show_info, nullptr, SettingsNRST::ExecuteOnChanged, nullptr
)



static void OnChanged_AltTShift0()
{
    TShift::SetDelta(setNRST.rand.shift_T0);

    SettingsNRST::ExecuteOnChanged();
}


DEF_GOVERNOR(gAltTShift0, PageDebug::PageRandomizer::self,
    "tShift ���.", "tShift alt.",
    "",
    "",
    setNRST.rand.shift_T0, 0, 510, nullptr, OnChanged_AltTShift0
)



DEF_PAGE_4(pageRandomizer, PageDebug::self, NamePage::DebugRandomizer,
    "����-���", "RANDOMIZER",
    "",
    "",
    gGateBottom,
    gGateTop,
    gNumAverages,
    gSamplesForGates,
//    gNumSmooth,
//    gShowInfo,
//    gAltTShift0,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageRandomizer::self = &pageRandomizer;
