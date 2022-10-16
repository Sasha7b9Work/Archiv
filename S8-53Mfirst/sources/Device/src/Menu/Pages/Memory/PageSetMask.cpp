// 2021/05/12 14:19:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/Math_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include <cstring>


static void PressSB_Exit()
{
}


DEF_SMALL_BUTTON(sbExit, PageMemory::PageSetMask::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_Exit, DrawSB_Exit, nullptr
)


static void PressSB_Delete()
{
    FILE_NAME_MASK[0] = '\0';
}


static void DrawSB_Delete(int x, int y)
{
    Char(Symbol::UGO2::DEL).Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


DEF_SMALL_BUTTON(sbDelete, PageMemory::PageSetMask::self,
    "Удалить", "Delete",
    "Удаляет все введённые символы",
    "Deletes all entered symbols",
    nullptr, PressSB_Delete, DrawSB_Delete, nullptr
)


static void PressSB_Backspace()
{
    int size = (int)(std::strlen(FILE_NAME_MASK));
    if (size > 0)
    {
        if (size > 1 && FILE_NAME_MASK[size - 2] == 0x07)
        {
            FILE_NAME_MASK[size - 2] = '\0';
        }
        else
        {
            FILE_NAME_MASK[size - 1] = '\0';
        }
    }
}


static void DrawSB_Backspace(int x, int y)
{
    Char(Symbol::UGO2::BACKSPACE).Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


DEF_SMALL_BUTTON(sbBackspace, PageMemory::PageSetMask::self,
    "Backspace", "Backspace",
    "Удаляет последний введённый символ",
    "Deletes the last entered symbol",
    nullptr, PressSB_Backspace, DrawSB_Backspace, nullptr
)


static void PressSB_Insert()
{
    int index = INDEX_SYMBOL_NAME_MASK;
    int size = (int)(std::strlen(FILE_NAME_MASK));

    if (size == MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        return;
    }

    if (index < 0x41)
    {
        FILE_NAME_MASK[size] = Tables::symbolsAlphaBet[index][0];
        FILE_NAME_MASK[size + 1] = '\0';
    }
    else
    {
        index -= 0x40;

        if (index == 0x07)  // Для %nN - отдельный случай
        {
            if (size < MAX_SYMBOLS_IN_FILE_NAME - 2 && size > 0)
            {
                if (FILE_NAME_MASK[size - 1] >= 0x30 &&
                    FILE_NAME_MASK[size - 1] <= 0x39) // Если ранее введено число
                {
                    FILE_NAME_MASK[size] = FILE_NAME_MASK[size - 1] - 0x30;
                    FILE_NAME_MASK[size - 1] = 0x07;
                    FILE_NAME_MASK[size + 1] = '\0';
                }
            }
        }
        else
        {
            FILE_NAME_MASK[size] = (int8)index;
            FILE_NAME_MASK[size + 1] = '\0';
        }
    }
}


static void DrawSB_Insert(int x, int y)
{
    Char(Symbol::UGO2::INSERT).Draw4SymbolsInRect(x + 2, y + 2, TypeFont::UGO2);
}


DEF_SMALL_BUTTON(sbInsert, PageMemory::PageSetMask::self,
    "Вставить", "Insert",
    "Вставляет выбранный символ",
    "Inserts the chosen symbol",
    nullptr, PressSB_Insert, DrawSB_Insert, nullptr
)


static void OnPress()
{
    PageMemory::PageSetMask::self->OpenAndSetItCurrent();
}


static void OnRegSet(int angle)
{
    PageMemory::PageSetMask::OnRegSet(angle, sizeof(Tables::symbolsAlphaBet) / 4);
}


static bool IsActive()
{
    return FILE_NAMING_MODE_IS_MASK;
}


DEF_PAGE_SB(pageSetMask, PageMemory::PageExternal::self, NamePage::MemoryExtSetMask_SB,
    "МАСКА", "MASK",
    "Режим ввода маски для автоматического именования файлов",
    "Input mode mask for automatic file naming",
    sbExit,
    sbDelete,
    Item::empty,
    Item::empty,
    sbBackspace,
    sbInsert,
    IsActive, OnPress, nullptr, OnRegSet
)

const Page *PageMemory::PageSetMask::self = &pageSetMask;


void PageMemory::PageSetMask::OnRegSet(int angle, int maxIndex)
{
    int8(*func[3])(int8 *, int8, int8) =
    {
        Math::CircleDecrease<int8>,
        Math::CircleDecrease<int8>,
        Math::CircleIncrease<int8>
    };

    Color::ResetFlash();

    if (INDEX_SYMBOL_NAME_MASK > maxIndex)
    {
        INDEX_SYMBOL_NAME_MASK = (int8)(maxIndex - 1);
    }

    func[Math::Sign(angle) + 1](&INDEX_SYMBOL_NAME_MASK, 0, (int8)(maxIndex - 1));

    Holder::RotateSwitch(angle);
}
