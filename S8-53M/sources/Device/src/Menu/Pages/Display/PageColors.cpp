// 2021/05/12 17:14:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(mcScheme, PageDisplay::PageSettings::PageColors::self,
    "�������� �����", "Color scheme",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "����� 1", "Scheme 1",
    "����� 2", "Scheme 2",
    COLOR_SCHEME, nullptr, nullptr, nullptr
)


static ColorType colorT1 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_A.index };

DEF_GOVERNOR_COLOR(mgcChannelA, PageDisplay::PageSettings::PageColors::self,
    "����� 1", "Channel 1",
    "",
    "",
    colorT1, nullptr
)


static ColorType colorT2 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_B.index };

DEF_GOVERNOR_COLOR(mgcChannelB, PageDisplay::PageSettings::PageColors::self,
    "����� 2", "Channel 2",
    "",
    "",
    colorT2, nullptr
)


DEF_GOVERNOR_COLOR(mgcGrid, PageDisplay::PageSettings::PageColors::self,
    "�����", "Grid",
    "������������� ���� �����",
    "Sets the grid color",
    PageDisplay::PageGrid::colorType, nullptr
)


DEF_PAGE_4(pageColors, PageDisplay::PageSettings::self, NamePage::ServiceDisplayColors,
    "�����", "COLORS",
    "����� ������ �������",
    "The choice of colors display",
    mcScheme,
    mgcChannelA,
    mgcChannelB,
    mgcGrid,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageDisplay::PageSettings::PageColors::self = &pageColors;
