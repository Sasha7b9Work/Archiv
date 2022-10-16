// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "Display/Warnings.h"
#include "Hardware/FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Pages/HelpContent.h"
#include "Panel/Panel.h"
#include <cstring>


namespace Menu
{
    static void OpenFileManager();

    // ��������� ������� ������� ��������������� �������� ����.
    static void OnTimerAutoHide();

    // ���������� ��������� �� �������� � ������ name
    static Page *FindPage(const Page *page, NamePage::E name);

    const int stepAngleRegSet = 2;

    bool showHelpHints = false;
    bool needClosePageSB = true;
    Item *itemsForButtons[6] = {&Item::empty, &Item::empty, &Item::empty, &Item::empty, &Item::empty, &Item::empty};
}


void Menu::Update()
{
    led_RegSet.Switch();
    
    if(FDrive::needOpenFileMananger)
    {
        FDrive::needOpenFileMananger = false;

        OpenFileManager();
    }

    if (Item::Opened()->IsPage())
    {
        set.menu.currentPage_ = ((Page *)Item::Opened())->GetName();
        set.menu.currentSubPage = ((Page *)Item::Opened())->GetCurrentSubPage();
    }
};


void Menu::Handlers::KeyForHint(Key::E key)
{
    if (key == Key::Menu)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ���� ��������� ��������� �������:\n"
            "1. ��� �������� ���� ������� ���� ������� � ���������� � ������� 0.5� ��������� ����.\n"
            "2. ��� �������� ���� ��������� ������ � ������� 0.5� ��������� ����.\n"
            "3. ��� ��������� \"������\x99����� �� ����\x99���������\" ������� ���������� �������� ����������� ������ "
            "����. ���� ������� �������� �������� ��������, ���� �����������.\n"
            "4. ��� ��������� \"������\x99����� �� ����\x99�����������\" ������� ���������� �������� �������� ������ "
            "����. ���� ������� �������� ��������� � ������� ������, ���������� ������� �� ���������� ������� ����.\n"
            "5. ���� ���� ��������� � ������ ����� ������, �� ������� ��������� ��������."
            :
            "MENU key performs the following functions:\n"
            "1. At the closed menu pressing or pressing with deduction during 0.5s opens the menu.\n"
            "2. At the open menu deduction of the key during 0.5s closes the menu.\n"
#ifdef __ARMCC_VERSION
#pragma push
#pragma diag_suppress 192
#endif
            "3. At control \"SERVICE\x99Mode btn MENU\x99\x43lose\" current becomes the page of the previous level of "
                "the menu. If the root page is current, the menu is closed.\n"
#ifdef __ARMCC_VERSION
#pragma pop
#endif
            "4. At control \"SERVICE\x99Mode btn MENU\x99Toggle\" current becomes the page of the current level of the "
            "menu. If the current page the last in the current level, happens transition to the previous level of the "
            "menu.\n"
            "5. If the menu is in the mode of small buttons, pressing closes the page.";

    } else if (key == Key::Cursors)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ������� ��������� ���� ��������� ���������."
            :
            "������� key to open the menu cursor measurements.";
    }
    else if (key == Key::Display)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ������� ��������� ���� �������� �������."
            :
            "DISPLAY key opens the display settings menu.";
    }
    else if (key == Key::Memory)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����\" ��������� ���� ������ � �������.\n"
            "2. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����������\" ��������� ������ �� ����-����."
            :
            "1. When setting \"MEMORY-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"MEMORY-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (key == Key::Measures)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ����� ��������� ���� �������������� ���������."
            :
            "����� key opens a menu of automatic measurements.";
    }
    else if (key == Key::Help)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ������ ������ ��������� ���� ������.\n"
            "2. ������� � ��������� ������ ������ � ������� 0.5� �������� � ��������� ����� ������ ���������."
            :
            "1. ������ key opens the help menu.\n"
            "2. Pressing and holding the ������ for 0.5s enables and disables output mode hints.";
    }
    else if (key == Key::Service)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ������ ��������� ���� ��������� ������������."
            :
            "������ key opens a menu of service options.";
    }
    else if (key == Key::Start)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "������ ����/�TO� ��������� � ������������� ������� ����� ����������."
            :
            "����/�TO� key starts and stops the process of gathering information.";
    }
    else if (key == Key::ChannelA)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ������ �����1 ��������� ���� �������� ������ 1.\n"
            "2. ������� � ��������� ������ �����1 � ������� 0.5� ������������� �������� ������ 1 �� ��������� 0�."
            :
            "1. �����1 key opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the key �����1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (key == Key::ChannelB)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ������ �����2 ��������� ���� �������� ������ 2.\n"
            "2. ������� � ��������� ������ �����2 � ������� 0.5� ������������� �������� ������ 2 �� ��������� 0�."
            :
            "1. �����2 key opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the key �����2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (key == Key::Time)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ������ ���� ��������� ���� �������� ���������.\n"
            "2. ������� � ��������� ������ ���� � ������� 0.5� ������������� �������� �� ����������� 0�."
            :
            "1. ���� key open the settings menu sweep.\n"
            "2. Pressing and holding the key ���� for 0.5s Is the offset horizontal 0s.";
    }
    else if (key == Key::Trig)
    {
        PageHelpContent::stringForHint = LANG_RU ?
            "1. ������ ����� ��������� ���� �������� �������������.\n"
            "2. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� "
            "�����\x99�����������\" ���������� �������������� ��������� ������ �������������.\n"
            "3. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� "
            "�����\x99����� ������\" ������������� ������� ������������� 0�."
            :
            "1. ����� key opens a menu settings synchronization.\n"
#ifdef __ARMCC_VERSION
#pragma push
#pragma diag_suppress 192
#endif
            "2. Pressing and holding the key ����� for 0.5s when setting \"SERVICE\x99Mode long "
            "TRIG\x99\x41utolevel\" automatically adjust the trigger level.\n"
#ifdef __ARMCC_VERSION
#pragma pop
#endif
            "3. Pressing and holding the key ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig "
            "level\" sets the trigger level 0V.";
    }
    else
    {
        Handlers::ShortPressureKey(key);
    }
}


void Menu::Handlers::PressKey(Key::E key) 
{
    Sound::ButtonPress();

    if (showHelpHints)
    {
        KeyForHint(key);
        return;
    }
};


void Menu::Handlers::RegulatorSet(Action::E action)
{
    if (showHelpHints)
    {
        return;
    }

    // ����, �� ������� ����� ��������� ����� ��������� - �������� �������� ���������� �������,
    // ���� - ����������� - "-" - �����, "+" - ������
    int angle = (action == Action::RotateRight) ? 1 : -1;

    Display::Redraw();

    if (MENU_IS_SHOWN || !Item::Opened()->IsPage())
    {
        const Item *item = CurrentItem();

        if (Item::Opened()->IsPage() && (item->IsChoiceReg() || item->IsGovernor() || item->IsIP() || item->IsMAC()))
        {
            item->Change(angle);
        }
        else
        {
            item = Item::Opened();

            if (IsMinimize())
            {
                CurrentPageSBregSet(angle);
            }
            else if (item->IsPage() || item->IsIP() || item->IsMAC() || item->IsChoice() || item->IsChoiceReg() ||
                item->IsGovernor())
            {
                item->OnRegSet(angle);
            }
            else if (item->IsGovernorColor())
            {
                item->Change(angle);
            }
            else if (item->IsTime())
            {
                (angle > 0) ? ((TimeItem *)item)->IncCurrentPosition() : ((TimeItem *)item)->DecCurrentPosition();
            }
        }
    }
}


Item* Menu::ItemUnderKey()
{
    return nullptr;
};


void Menu::SetAutoHide(bool)
{
    if(!MENU_IS_SHOWN)
    {
        return;
    }
    if(MenuAutoHide::Time(MENU_AUTO_HIDE) == 0)
    {
        Timer::Disable(TypeTimer::MenuAutoHide);
    }
    else
    {
        Timer::Enable(TypeTimer::MenuAutoHide, MenuAutoHide::Time(MENU_AUTO_HIDE), OnTimerAutoHide);
    }
}


Text Menu::StringNavigation()
{
    Text result;

    if(Menu::NeedDrawStringNavigation() && MENU_IS_SHOWN)
    {
        pchar titles[10] = {0};
        int numTitle = 0;
        const Item *item = Item::Opened();

        if(IsMainPage(item))
        {
            return result;
        }

        while(!IsMainPage(item))
        {
            titles[numTitle++] = item->Title();
            item = item->Keeper();
        }

        for(int i = 9; i >= 0; i--)
        {
            if(titles[i])
            {
                result.Append(titles[i]);

                if(i != 0)
                {
                    result.Append(" - ");
                }
            }
        }
    }

    return result;
}


void Menu::OnTimerAutoHide()
{
    Hide();
    Timer::Disable(TypeTimer::MenuAutoHide);
}


void Menu::Handlers::ShortPressureKey(Key::E key)
{
    if (!showHelpHints)
    {
        if (key == Key::Help)
        {
            showHelpHints = true;
            PageHelpContent::stringForHint = 0;
            PageHelpContent::itemHint = 0;
        }
    }

    if (key == Key::Memory && MODE_BUTTON_MEMORY_IS_SAVE && FDrive::IsConnected())
    {
        FDrive::SaveCurrentSignal();

        return;
    }

    Display::Redraw();
    SetAutoHide(true);

    do
    {
        if (key == Key::Menu)            // ���� ������ ������ ���� � �� �� ��������� � ����� ��������� ���������.
        {
            if (!MENU_IS_SHOWN)
            {
                Show();
            }
            else
            {
                CloseOpenedItem();
            }
        }
        else if (MENU_IS_SHOWN && Key::IsFunctional(key)) // ���� ���� �������� � ������ �������������� �������
        {
            Item *item = ItemUnderKey(key);

            if (showHelpHints)
            {
                PageHelpContent::SetItemForHint(item);
            }
            else
            {
                item->ShortPress();
            }
        }
        else                                                        // ���� ���� �� ��������.
        {
            NamePage::E name = ((const Page *)Item::Opened())->GetName();

            if (key == Key::ChannelA && name == NamePage::ChannelA)
            {
                ENABLED_A = !ChA.IsEnabled();
                PageChannel::A::OnChanged_Input(true);
            }
            else if (key == Key::ChannelB && name == NamePage::ChannelB)
            {
                ENABLED_B = !ChB.IsEnabled();
                PageChannel::B::OnChanged_Input(true);
            }
            else
            {
                const Item *page = PageForButton(key);

                if (page)
                {
                    page->SetCurrent(true);
                    page->Open();
                    Show();
                }
            }
        }
    } while (false);
}


void Menu::Handlers::LongPressureKey(Key::E key)
{
    if (!showHelpHints)
    {
        Display::Redraw();
    }

    Display::Redraw();
    SetAutoHide(true);

    if (key == Key::Time)
    {
        TShift::Set(0);
    }
    else if (key == Key::Trig)
    {
        TrigLev::Set(TRIG_SOURCE, TrigLev::ZERO);
    }
    else if (key == Key::ChannelA)
    {
        RShift::Set(ChA, RShift::ZERO);
    }
    else if (key == Key::ChannelB)
    {
        RShift::Set(ChB, RShift::ZERO);
    }
    else if (key == Key::Menu)
    {
        ChangeShowing();
    }
    else if (MENU_IS_SHOWN && Key::IsFunctional(key))
    {
        Item *item = ItemUnderKey(key);
        if (item->IsActive())
        {
            item->LongPress();
        }
    }
}


void Menu::Handlers::ReleaseKey(Key::E )
{
    Sound::ButtonRelease();
}


void Menu::OpenItemTime()
{
    Warnings::ShowWarningGood("����� �� �����������. ������ ���������� ��� ������",
                              "TIME IS NOT SET. YOU CAN INSTALL IT NOW");
    Handlers::ShortPressureKey(Key::Service);
    Display::Update();
    for (int i = 0; i < 2; i++)
    {
        Menu::Handlers::RegulatorSet(Action::RotateRight);
        Display::Update();
    }
    Handlers::ShortPressureKey(Key::F4);
    Display::Update();
}


void Menu::OpenFileManager()
{
    for (int i = 0; i < 10; i++)
    {
        Handlers::ShortPressureKey(Key::Menu);
        Display::Update();
    }

    if (!MENU_IS_SHOWN)
    {
        Handlers::ShortPressureKey(Key::Menu);
        Display::Update();
    }

    for (int i = 0; i < 5 * stepAngleRegSet + 1; i++)
    {
        Menu::Handlers::RegulatorSet(Action::RotateLeft);
        Display::Update();
    }

    for (int i = 0; i < 2 * stepAngleRegSet + 1; i++)
    {
        Menu::Handlers::RegulatorSet(Action::RotateRight);
        Display::Update();
    }

    Handlers::ShortPressureKey(Key::F2);
    Display::Update();

    Handlers::ShortPressureKey(Key::F4);
    Display::Update();

    for (int i = 0; i < stepAngleRegSet + 1; i++)
    {
        Menu::Handlers::RegulatorSet(Action::RotateLeft);
        Display::Update();
    }

    for (int i = 0; i < 2; i++)
    {
        Handlers::ShortPressureKey(Key::F1);
        Display::Update();
    }
}


void Menu::ChangeShowing()
{
    if (MENU_IS_SHOWN)
    {
        Hide();
    }
    else
    {
        Show();
    }
}


void Menu::Hide()
{
    MENU_SHOWN = 0U;
}


void Menu::Show()
{
    MENU_SHOWN = 1U;
    SetAutoHide(true);
}


bool Menu::IsMinimize()
{
    return Item::Opened()->IsPage() && ((const Page *)Item::Opened())->IsPageSB();
}


void Menu::CurrentPageSBregSet(int angle)
{
    const Page *page = Item::Opened()->ReinterpretToPage();
    if (page->OwnData()->funcRegSetSB)
    {
        page->OwnData()->funcRegSetSB(angle);
    }
}


bool Menu::NeedDrawStringNavigation()
{
    return (SHOW_STRING_NAVIGATION && (TIMER_MS - Page::timeLastChangedSubpage < 5000));
}


void Menu::Init()
{
    // � �������������� ������ ��������� ������ ��������

    Page *page = FindPage(PageMain::self, set.menu.currentPage_);

    if (page)
    {
        if (page->GetName() == NamePage::MainPage)
        {
            page->SetCurrentSubPage(set.menu.currentSubPage);

            return;
        }

        Queue<Page *> pages;

        pages.Push(page);

        Page *keeper = page->Keeper();

        while (keeper != PageMain::self && keeper != &Page::empty)
        {
            pages.Push(keeper);

            keeper = keeper->Keeper();
        }

        for (int i = 0; i < pages.Size(); i++)
        {
            pages[i]->ShortPress();
        }

        ((Page *)Item::Opened())->SetCurrentSubPage(set.menu.currentSubPage);
    }
}


Page *Menu::FindPage(const Page *parent, NamePage::E name)
{
    if (parent->GetName() == name)
    {
        return (Page *)parent;
    }

    for (int i = 0; i < parent->NumItems(); i++)
    {
        Item *item = parent->GetItem(i);

        if (item->IsPage())
        {
            Page *page = (Page *)item;

            if (page->GetName() == name)
            {
                return page;
            }

            Page *result = FindPage(page, name);

            if (result != nullptr)
            {
                return result;
            }
        }
    }

    return nullptr;
}
