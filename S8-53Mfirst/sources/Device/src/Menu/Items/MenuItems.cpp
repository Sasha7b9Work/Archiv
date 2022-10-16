// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Menu/Menu.h"
#include "Menu/Pages/HelpContent.h"
#include "Settings/Settings.h"


// ������������ ��� �������� ��������� �������� Choice
struct TimeStructChoice
{
    const Choice* choice;               // ����� Choice, ������� ��������� � ������ ������ � ��������. ���� 0 - ��� ��������.
    uint          timeStartMS;          // ����� ������ �������� choice.
    uint          inStateIncrease : 1;
    uint          inStateDecrease : 1;
};

static TimeStructChoice tsChoice;


uint Page::timeLastChangedSubpage = 0;
DataItem Item::emptyData = { TypeItem::None, nullptr, nullptr, nullptr, nullptr };
Item Item::empty;
static const Item *itemsPage[] = { &Item::empty, nullptr };
static const DataPage emptyDataPage = { false, NamePage::Empty, &itemsPage[0], nullptr, nullptr };
pchar titleHint[4] = { "", "", "", "" };
static const DataItem emptyDataItem = { TypeItem::None, nullptr, nullptr, titleHint, &emptyDataPage };
static int8 subPageEmpty = 0;
static int8 actItemEmpty = -1;
static bool actItemIsOpenedEmpty = false;
Page Page::empty(&emptyDataItem, &subPageEmpty, &actItemEmpty, &actItemIsOpenedEmpty);


Item::Item(const DataItem *const _data) : data(_data)
{
    if (data == nullptr)
    {
        data = &emptyData;
    }
}


bool Item::IsPressed() const
{
    return this == Menu::ItemUnderKey();
}


pchar Choice::NameSubItem(int i) const
{
    return OwnData()->names[(i * 2) + LANG];
}


pchar Choice::NameCurrentSubItem() const
{
    const DataChoice *own = OwnData();

    return (own->cell == 0) ? "" : NameSubItem(*own->cell);
}


pchar Choice::NameNextSubItem() const
{
    const DataChoice *own = OwnData();

    if (own->cell == 0)
    {
        return "";
    }
    int index = *own->cell + 1;
    if (index == NumSubItems())
    {
        index = 0;
    }
    return NameSubItem(index);
}


pchar Choice::NamePrevSubItem() const
{
    const DataChoice *own = OwnData();

    if (own->cell == 0)
    {
        return "";
    }
    int index = *own->cell - 1;
    if (index < 0)
    {
        index = NumSubItems() - 1;
    }
    return NameSubItem(index);
}


int Choice::NumSubItems() const
{
    int i = 0;
    while (OwnData()->names[i] != nullptr)
    {
        i++;
    }
    return i / 2;
}


void Choice::StartChange(int delta) const
{
    if (tsChoice.choice != 0)
    {
        return;
    }

    Holder::RotateGovernor(delta);

    if (Menu::showHelpHints)
    {
        PageHelpContent::SetItemForHint(this);
    }
    else if (!IsActive())
    {
        FuncOnChanged(false);
    }
    else
    {
        tsChoice.choice = this;
        tsChoice.timeStartMS = TIMER_MS;
        if (delta > 0)
        {
            tsChoice.inStateIncrease = 1;
        }
        else if (delta < 0)
        {
            tsChoice.inStateDecrease = 1;
        }
    }
}


void Choice::FuncOnChanged(bool active) const
{
    if (OwnData()->funcOnChanged)
    {
        OwnData()->funcOnChanged(active);
    }
}


void Choice::FuncForDraw(int x, int y) const
{
    if (OwnData()->funcForDraw)
    {
        OwnData()->funcForDraw(x, y);
    }
}


float Choice::Step() const
{
    const DataChoice *own = OwnData();

    static const float speed = 0.1F;
    static const int numLines = 12;

    if (tsChoice.choice == this)
    {
        float delta = speed * (TIMER_MS - tsChoice.timeStartMS);

        if (delta == 0.0F) //-V550
        {
            delta = 0.001F; // ������ � ��������� ������ ������ ����� ��������, ��� ������ 0 ��, �� �� ����������
                            // ������� �����, ������ ��� ���� ����� �������� � ���, ��� �������� ���
        }

        if (tsChoice.inStateIncrease == 1)
        {
            if (delta <= numLines)
            {
                return delta;
            }

            Math::CircleIncrease<int8>(own->cell, 0, (int8)(NumSubItems() - 1));
        }
        else if (tsChoice.inStateDecrease == 1)
        {
            delta = -delta;

            if (delta >= -numLines)
            {
                return delta;
            }

            Math::CircleDecrease<int8>(own->cell, 0, (int8)(NumSubItems() - 1));
        }

        tsChoice.choice = 0;
        FuncOnChanged(IsActive());
        Display::Redraw();
        tsChoice.inStateDecrease = 0;
        tsChoice.inStateIncrease = 0;
        return 0.0F;
    }

    return 0.0F;
}


void Choice::OnRegSet(int delta) const
{
    GraphicGovernor::CalculatePosition(this);

    const DataChoice *own = OwnData();

    if (delta < 0)
    {
        int8 value = (*own->cell == NumSubItems() - 1) ? 0 : (*own->cell + 1);
        *own->cell = value;
    }
    else
    {
        int8 value = (int8)((*own->cell == 0) ? (NumSubItems() - 1) : (*own->cell - 1));
        *own->cell = value;
    }

    FuncOnChanged(IsActive());
    Holder::RotateGovernor(delta);
    Display::Redraw();
}


NamePage::E Page::GetName() const
{
    if (Type() != TypeItem::Page)
    {
        return NamePage::NoPage;
    }
    return OwnData()->name;
}


TypeItem::E Item::Type() const
{
    return data->type;
}


int Page::NumItems() const
{
    if (IsPageSB())
    {
        return 5;
    }
    else
    {
        for (int i = 0; ; i++)
        {
            if (GetItem(i) == nullptr)
            {
                return i;
            }
        }
    }
}


bool Item::IsActive() const
{
    if (IsChoice() || IsPage() || IsButton() || IsGovernor() || IsSmallButton())
    {
        pFuncBV func = ((Page *)(this))->data->funcOfActive;

        return func ? func() : true;
    }

    return true;
}


bool Item::IsCurrent() const
{
    return (Menu::CurrentItem() == this);
}


bool Item::IsMayCurrent() const
{
    static bool may[TypeItem::Count] =
    {
        false,  // None
        false,  // Choice
        false,  // Button
        false,  // Page
        true,   // Governor
        false,  // Time
        false,  // IP
        false,  // SwitchButton
        false,  // GovernorColor
        false,  // Formula
        false,  // MAC
        true,   // ChoiceReg
        false   // SmallButton
    };

    return may[data->type];
}


Item *Page::GetItem(int numElement) const
{
    return (Item *)(OwnData()->items[numElement + (IsPageSB() ? 1 : 0)]);
}


int Page::NumSubPages() const
{
    return (NumItems() - 1) / Page::ITEMS_ON_SUBPAGE + 1;
}


bool Page::IsPageSB() const
{
    return OwnData()->isPageSB;
}


Page *Item::Keeper() const
{
    const Page *page = ((Page *)(this))->data->keeper;
    return (Page *)page;
}


void Item::Open() const
{
    Page *keeper = Keeper();

    if (keeper)
    {
        keeper->SetCurrentItem(this);

        int8 position = *keeper->posCurrentItem;

        if (position < 0)
        {

        }
        else
        {
            *keeper->currentItemIsOpened = true;
        }
    }
}


void Item::Close() const
{
    Keeper()->CloseOpenedItem();

    if (!IsMayCurrent())
    {
        Keeper()->SetCurrentItem(nullptr);
    }
}


void Item::ChangeOpenness() const
{
    if (IsOpened())
    {
        Close();
    }
    else
    {
        Open();
    }
}


void Item::SetCurrent(bool current) const
{
    Page* page = Keeper();

    if (!current)
    {
        page->SetCurrentItem(nullptr);
    }
    else
    {
        page->SetCurrentItem(this);

        Page::timeLastChangedSubpage = TIMER_MS;
    }
}


void Page::SetCurrentItem(const Item *item)
{
    *posCurrentItem = -1;

    for (int8 i = 0; i < NumItems(); i++)
    {
        if (GetItem(i) == item)
        {
            *posCurrentItem = i;
            return;
        }
    }
}


void Page::OnRegSet(int delta) const
{
    if (delta > 0 && GetCurrentSubPage() < NumSubPages() - 1)
    {
        Holder::RotateSwitch(delta);
        SetCurrentSubPage(GetCurrentSubPage() + 1);
    }
    else if (delta < 0 && GetCurrentSubPage() > 0)
    {
        Holder::RotateSwitch(delta);
        SetCurrentSubPage(GetCurrentSubPage() - 1);
    }
}


void Page::ShortPress()
{
    if (OwnData()->funcOnPress)
    {
        OwnData()->funcOnPress();
    }

    if (!IsActive())
    {
        return;
    }

    SetCurrent(true);

    ChangeOpenness();
}


void Choice::ShortPress()
{
    if (IsChoice())
    {
        if (!IsActive())
        {
            FuncOnChanged(false);
        }
        else if (!IsOpened())
        {
            StartChange(1);
        }
        else
        {
            OnRegSet(-1);
        }
    }
    else
    {
        if (!IsActive())
        {
            FuncOnChanged(false);
        }
        else if (!IsOpened())
        {
            SetCurrent(!IsCurrent());
        }
    }
}


void Button::ShortPress()
{
    if (!IsActive())
    {
        return;
    }
    SetCurrent(true);
    OwnData()->funcOnPress();
}


void TimeItem::ShortPress()
{
    if (!IsOpened())
    {
        SetCurrent(true);
        SetOpened();
        Open();
    }
    else
    {
        SelectNextPosition();
    }
}


void IPaddress::ShortPress()
{
    if (IsOpened())
    {
        NextPosition();
    }
}


void GovernorColor::ShortPress()
{
    if (!IsActive())
    {
        return;
    }
    if (IsOpened())
    {
        Math::CircleIncrease<int8>(&(OwnData()->colorType->currentField), 0, 3);
    }
    else
    {
        LongPress();
    }
}


void MACaddress::ShortPress()
{
    if (IsOpened())
    {
        Math::CircleIncrease<int8>(&gCurDigit, 0, 5);
    }
}


void SmallButton::ShortPress()
{
    pFuncVV func = OwnData()->funcOnPress;
    if (func)
    {
        func();
    }
}


void Item::LongPress()
{
    if (!IsCurrent())
    {
        SetCurrent(true);
    }
    
    ChangeOpenness();
}


void Button::LongPress()
{
    ShortPress();
}


void TimeItem::LongPress()
{
    if (!IsCurrent())
    {
        SetCurrent(true);
    }

    if (IsOpened() && *OwnData()->curField == iSET)
    {
        SetNewTime();
    }

    ChangeOpenness();

    SetOpened();
}


const Item *Page::RetLastOpened() const
{
    if (CurrentItem()->IsOpened())
    {
        int8 position = CurrentItem()->GetPosition();
        const Item *item = GetItem(position);

        if (GetItem(position)->IsPage())
        {
            return ((Page *)item)->RetLastOpened();
        }
        else
        {
            return item;
        }
    }

    return this;
}


void Item::Change(int delta) const
{
    if (IsChoice() || IsChoiceReg())
    {
        ((Choice *)this)->StartChange(delta);
    }
    else if (IsGovernor())
    {
        Governor *governor = (Governor *)this;
        if (!governor->IsOpened())
        {
            governor->StartChange(delta);
        }
        else
        {
            governor->OnRegSet(delta);
        }
    }
    else if (IsGovernorColor())
    {
        ((GovernorColor *)this)->ChangeValue(delta);
    }
}


void Page::ShortPressOnItem(int numItem) const
{
    const DataPage *own = OwnData();

    if (Type() != TypeItem::Page)
    {
        return;
    }

    if (IsPageSB())
    {
        SmallButton *sb = (SmallButton *)(own->items)[numItem];
        if (sb && own->funcOnPress)
        {
            own->funcOnPress();
        }
    }
}


void Page::OpenAndSetItCurrent() const
{
    SetCurrent(true);

    ChangeOpenness();
}


int Item::HeightOpened() const
{
    if (IsPage())
    {
        int numItems = ((const Page *)this)->NumItems() - ((Page *)this)->GetCurrentSubPage() * Page::ITEMS_ON_SUBPAGE;

        Math::Limitation(&numItems, 0, Page::ITEMS_ON_SUBPAGE);

        return Item::TITLE_HEIGHT + Item::HEIGHT * numItems;
    }
    else if (IsChoice() || IsChoiceReg())
    { 
        return Item::OPENED_HEIGHT_TITLE + ((Choice *)this)->NumSubItems() * Item::OPENED_HEIGHT - 1;
    }
    return Item::HEIGHT;
}


pstring Item::Title() const
{
    return TITLE((Page *)this);
}


SmallButton *Page::SmallButonFrom(int numButton) const
{
    const DataPage *own = OwnData();

    return (SmallButton *)(own->items)[numButton];
}


int Page::GetCurrentSubPage() const
{
    return *currentSubPage;
}


void Page::SetCurrentSubPage(int posSubPage) const
{
    *currentSubPage = (int8)posSubPage;
}


bool Item::IsOpened() const
{
    Page *keeper = Keeper();

    if (keeper)
    {
        return *keeper->currentItemIsOpened;
    }

    return false;
}


void Page::CloseOpenedItem()
{
    *currentItemIsOpened = false;
}


int Page::PosItemOnTop() const
{
    return GetCurrentSubPage() * Page::ITEMS_ON_SUBPAGE;
}


const Item *Page::CurrentItem() const
{
    int position = *posCurrentItem;

    if (position < 0)
    {
        return &empty;
    }

    return OwnData()->items[position];
}


int8 Item::GetPosition() const
{
    Page *keeper = Keeper();

    if (keeper == nullptr)
    {
        return -1;
    }

    int8 result = -1;

    for (int i = 0; i < keeper->NumItems(); i++)
    {
        if (Keeper()->GetItem(i) == this)
        {
            result = (int8)i;
            break;
        }
    }

    return result;
}
