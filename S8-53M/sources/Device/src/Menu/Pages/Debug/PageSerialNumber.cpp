// 2021/05/12 11:01:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"


// В этой структуре будут храниться данные серийного номера при открытой странице ppSerialNumer
struct StructForSN
{
    int number;     // Соответственно, порядковый номер.
    int year;       // Соответственно, год.
    int curDigt;    // Соответственно, номером (0) или годом (1) управляет ручка УСТАНОВКА.
};


static void OnPress_Exit()
{
}

DEF_SMALL_BUTTON(bExit, PageDebug::PageSerialNumber::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, OnPress_Exit, DrawSB_Exit, nullptr
)


static void OnPress_Change()
{
}

static void Draw_Change(int x, int y)
{
    Char(Symbol::UGO2::TAB).Draw4SymbolsInRect(x + 2, y + 2, TypeFont::UGO2);
}

DEF_SMALL_BUTTON(bChange, PageDebug::PageSerialNumber::self,
    "Вставить", "Insert",
    "Вставляет выбраный символ",
    "Inserts the chosen symbol",
    nullptr, OnPress_Change, Draw_Change, nullptr
)


static void OnPress_Save()
{
}


static void Draw_Save(int x, int y)
{
    Char(Symbol::UGO2::SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


DEF_SMALL_BUTTON(bSave, PageDebug::PageSerialNumber::self,
    "Сохранить", "Save",
    "Записывает серийный номер в OTP",
    "Records the serial number in OTP",
    nullptr, OnPress_Save, Draw_Save, nullptr
)


static void OnPress()
{
    PageDebug::PageSerialNumber::self->OpenAndSetItCurrent();
}


static void OnRegSet(int)
{
}


DEF_PAGE_SB(pageSerialNumber, PageDebug::self, NamePage::DebugSerialNumber_SB,
    "С/Н", "S/N",
    "Запись серийного номера в OTP-память. ВНИМАНИЕ!!! ОТP-память - память с однократной записью.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    bExit,
    bChange,
    Item::empty,
    Item::empty,
    Item::empty,
    bSave,
    nullptr, OnPress, nullptr, OnRegSet
)


const Page *PageDebug::PageSerialNumber::self = &pageSerialNumber;
