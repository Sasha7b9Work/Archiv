// 2021/05/12 16:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


ColorType PageDisplay::PageGrid::colorType = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID.index };


DEF_CHOICE_4(mcType, PageDisplay::PageGrid::self,
    "���", "Type",
    "����� ���� �����.",
    "Choice like Grid.",
    "��� 1", "Type 1",
    "��� 2", "Type 2",
    "��� 3", "Type 3",
    "��� 4", "Type 4",
    TYPE_GRID, nullptr, nullptr, nullptr
)


void PageDisplay::PageGrid::OnChangedBrightness()
{
    PageDisplay::PageGrid::colorType.SetBrightness(BRIGHTBESS_GRID / 1e2F);
}


DEF_GOVERNOR(mgBrightness, PageDisplay::PageGrid::self,
    "�������", "Brightness",
    "������������� ������� �����.",
    "Adjust the brightness of the Grid.",
    BRIGHTBESS_GRID, 0, 100, nullptr, PageDisplay::PageGrid::OnChangedBrightness
)


DEF_PAGE_2(pageGrid, PageDisplay::self, NamePage::DisplayGrid,
    "�����", "GRID",
    "�������� ��������� ����������� ������������ �����.",
    "Contains settings of display of a coordinate Grid::",
    mcType,
    mgBrightness,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDisplay::PageGrid::self = &pageGrid;
