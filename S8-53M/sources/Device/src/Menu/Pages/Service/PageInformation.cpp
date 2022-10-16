// 2021/05/12 16:00:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Menu/Menu.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static void OnPress_Exit()
{
    Display::SetDrawMode(Display::DrawMode::Default, 0);
}


DEF_SMALL_BUTTON(sbExit, PageService::PageCalibrator::self,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, OnPress_Exit, DrawSB_Exit, nullptr
)


static void Func_Draw()
{
    Region(Display::WIDTH, Display::HEIGHT).Fill(0, 0, Color::BACK);

    int x = 100;
    int dY = 20;
    int y = 20;
    Rectangle(319, 239).Draw(0, 0, Color::FILL);
    y += dY;

    Text(LANG_RU ? "����������" : "INFORMATION").Draw(x, y);
    y += dY;

    Text(LANG_RU ? "��� : �����������" : "Type : oscilloscope").Draw(x, y);
    y += dY;

    Text((LANG_RU ? "������ : %s" : "Model : %s"), NAME_MODEL).Draw(x, y);
    y += dY;

    Text(LANG_RU ? "����������� �����������:" : "Software:").Draw(x, y);
    y += dY;
    x += 15;

    Text((pchar)(LANG_RU ? "- ������ : %s" : "- version : %s"), NUMBER_VER).Draw(x, y);
    y += dY;

    Text("- CRC32 : %X", HAL_CRC::Calculate()).Draw(x, y, Color::FILL);

    Text(LANG_RU ? "��� ��������� ������ ������� � ����������� ������ ������" :
                   "For help, press and hold the HELP button").DrawInCenterRect(0, 200, 320, 20);
}


DEF_PAGE_SB(pageInformation, PageService::self, NamePage::ServiceInformation_SB,
    "����������", "INFORMATION",
    "������� �� ����� ����������������� ������ ������������",
    "Displays identification data of the oscilloscope",
    sbExit,
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    nullptr, nullptr, Func_Draw, nullptr
)

const Page *PageService::PageInformation::self = &pageInformation;
