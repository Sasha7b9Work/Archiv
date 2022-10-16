// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Utils/Math_.h"
#include "common/Utils/Containers/Queue_.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Pages/HelpContent.h"
#include "Settings/Settings.h"
#include <cstdio>


namespace Menu
{
    static void DrawHintItem(int x, int y, int width);
}


static void Menu::DrawHintItem(int x, int y, int width)
{
    if (!PageHelpContent::itemHint)
    {
        return;
    }

    pchar names[TypeItem::Count][2] =
    {
        {"",            ""},        // TypeItem::None
        {"",            ""},        // TypeItem::Choice
        {"Кнопка",      "Button"},  // TypeItem::Button
        {"Страница",    "Page"},    // TypeItem::Page
        {"",            ""},        // TypeItem::Governor
        {"",            ""},        // TypeItem::Time
        {"",            ""},        // TypeItem::IP
        {"",            ""},        // Item_SwitchButton
        {"",            ""},        // TypeItem::GovernorColor
        {"",            ""},        // Item_Formula
        {"",            ""},        // TypeItem::MAC
        {"",            ""},        // TypeItem::ChoiceReg
        {"Кнопка",      "Button"}   // TypeItem::SmallButton
    };
    TypeItem::E type = PageHelpContent::itemHint->Type();
    Page *item = (Page*)PageHelpContent::itemHint;

    const int SIZE = 100;
    char title[SIZE];
    std::snprintf(title, SIZE, "%s \"%s\"", names[type][LANG], TITLE(item));

    if (item->IsSmallButton())
    {
        y -= 9;
    }
    Text(title).DrawInCenterRectAndBoundIt(x, y, width, 15, Color::BACK, Color::FILL);
    y = Text(HINT(item)).DrawInBoundedRectWithTransfers(x, y + 15, width, Color::BACK, Color::FILL);
    if (item->IsSmallButton())
    {
        reinterpret_cast<SmallButton *>(item)->DrawHints(x, y, width);
    }
}


void Menu::Draw()
{
    for (int i = 0; i < 6; i++)
    {
        itemsForButtons[i] = &Item::empty;
    }

    if(MENU_IS_SHOWN || !Item::Opened()->IsPage())
    {
        const Item *item = Item::Opened();

        if(MENU_IS_SHOWN)
        {
            if (item->IsPage())
            {
                ((Page *)item)->DrawOpened(Grid::Top());
            }
            else if(item->Keeper()->IsExist())
            {
                item->Keeper()->DrawOpened(Grid::Top());
            }
        }
        else
        {
            if(item->IsChoice())
            {
                ((Choice *)item)->Draw(item->Keeper()->X(), Grid::Top(), true);
            }
            else if(item->IsGovernor())
            {
                ((Governor *)item)->Draw(item->Keeper()->X(), Grid::Top(), true);
            }
        }
    }

    if (Menu::showHelpHints)
    {
        int x = 1;
        int y = 0;
        int width = Menu::IsMinimize() ? 289 : 220;

        Text(LANG_RU ? "Включён режим подсказок. В этом режиме при нажатии на кнопку на экран выводится информация о "
            "её назначении. Чтобы выключить этот режим, нажмите кнопку ПОМОЩЬ и удерживайте её в течение 0.5с."
            :
            "Mode is activated hints. In this mode, pressing the key displays the information on its purpose. "
            "To disable this mode, press the key HELP and hold it for 0.5s.").
                                              DrawInBoundedRectWithTransfers(x, y, width - 1, Color::BACK, Color::FILL);

        y += LANG_RU ? 49 : 40;
        if (PageHelpContent::stringForHint)
        {
            Text(PageHelpContent::stringForHint).
                                              DrawInBoundedRectWithTransfers(x, y, width - 1, Color::BACK, Color::FILL);
        }
        else if (PageHelpContent::itemHint)
        {
            DrawHintItem(x, y, width - 1);
        }
    }
}


int Item::OpenedPosY() const
{
    Page *page = Keeper();

    int8 posCurItem = page->CurrentItem()->GetPosition();

    int y = Grid::Top() + (posCurItem % Page::ITEMS_ON_SUBPAGE) * Item::HEIGHT + Item::TITLE_HEIGHT;

    int heightOpened = HeightOpened();
    
    if(y + heightOpened > Grid::Bottom())
    {
        y = Grid::Bottom() - heightOpened - 2;
    }

    return y + 1;
}


void Menu::DrawStringNavigation()
{
    Text string = Menu::StringNavigation();

    if (string.Size() != 0)
    {
        int length = Font::GetLengthText(string.c_str());
        int height = 11;

        Rectangle(length + 3, height).Draw(Grid::Left(), Grid::Top(), Color::FILL);
        Region(length + 1, height - 2).Fill(Grid::Left() + 1, Grid::Top() + 1, Color::BACK);
        string.Draw(Grid::Left() + 2, Grid::Top() + 1, Color::FILL);
    }
}
