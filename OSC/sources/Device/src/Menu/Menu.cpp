#include "defines.h"
#include "Menu.h"
#include "BufferButtons.h"
#include "Hardware/Timer.h"
#include "MenuTriggers.h"
#include "HandlersKeys.h"
#include "Device.h"
#include "FlashDrive/FlashDrive.h"
#include "Settings/SettingsMemory.h"
#include "Settings/Settings.h"
#include "Pages/Include/PageMemory.h"
#include "Pages/Include/PageChannels.h"
#include "Pages/Include/PageHelp.h"
#include "Pages/Include/PageMeasures.h"
#include "Pages/Include/PageDisplay.h"
#include "Pages/Include/PageFunction.h"
#include "Hardware/Sound.h"
#include "FPGA/FPGA.h"
#include "Utils/Math.h"
#include "Display/Painter.h"

#ifdef WIN32
#include <cstdio>
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       uint Menu::timeLastPressedButton = MAX_UINT;
        Key Menu::shortPressureButton = Key::None;
        Key Menu::longPressureButton = Key::None;
        Key Menu::pressButton = Key::None;
        Key Menu::releaseButton = Key::None;
   Control *Menu::itemUnderKey = 0;
    pFuncVV Menu::funcAterUpdate = 0;
   Control *Menu::itemUnderButton[Key::Number] = {0};
const char *Menu::stringForHint = 0;
   Control *Menu::itemHint = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Update()
{
    while(!BufferButtons::IsEmpty())
    {
        timeLastPressedButton = TIME_MS;

        KeyEvent event = BufferButtons::Extract();

        if (HINT_MODE_ENABLED)
        {
            ProcessButtonForHint(event.key);
            continue;
        }

        if (!Menu::IsShown())
        {
            if(event.type == TypePress::Release && TriggerDebugConsole::Update(event.key))
            {
                continue;
            }
        }
        if(IsProcessed(&event))
        {
            Handlers::Process(event);
        }
    }

    if (funcAterUpdate)
    {
        funcAterUpdate();
        funcAterUpdate = 0;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::IsProcessed(KeyEvent *event)
{
    Key key = event->key;
    TypePress type = event->type;

    switch(Device::CurrentMode())
    {
        case Device::Mode::Tester:
            if(key.IsFunctional() || (key == Key::Enter && !type.IsLong()))
            {
                return true;
            }
            return false;
        case Device::Mode::Multimeter:
            if(key.IsFunctional() || (key == Key::Enter && !type.IsLong()))
            {
                return true;
            }
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ShortPressureButton(Key button)
{
    if (!HINT_MODE_ENABLED)
    {
        if (button == Key::Memory && FDrive::IsConnected() && MODE_BTN_MEMORY_IS_SAVE)
        {
            NEED_SAVE_TO_FLASHDRIVE = 1;
        }

        shortPressureButton = button;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::LongPressureButton(Key button)
{
    if (!HINT_MODE_ENABLED)
    {
        longPressureButton = button;
        NEED_FINISH_DRAW = 1;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessButtonForHint(Key button)
{
    if (button == Key::Enter)
    {
        Menu::stringForHint = LANG_RU ?
            "������ ���� ��������� ��������� �������:\n"
            "1. ��� �������� ���� ������� ���� ������� � ���������� � ������� 0.5� ��������� ����.\n"
            "2. ��� �������� ���� ��������� ������ � ������� 0.5� ��������� ����.\n"
            "3. ��� ��������� \"������\x99����� �� ����\x99���������\" ������� ���������� �������� ����������� ������ ����. ���� ������� �������� "
            "�������� ��������, ���� �����������.\n"
            "4. ��� ��������� \"������\x99����� �� ����\x99�����������\" ������� ���������� �������� �������� ������ ����. ���� ������� �������� "
            "��������� � ������� ������, ���������� ������� �� ���������� ������� ����.\n"
            "5. ���� ���� ��������� � ������ ����� ������, �� ������� ��������� ��������."
            :
        "���� button performs the following functions:\n"
            "1. At the closed menu pressing or pressing with deduction during 0.5s opens the Menu::\n"
            "2. At the open menu deduction of the button during 0.5s closes the Menu::\n"
            "3. At control \"SERVICE\x99Mode btn MENU\x99\x43lose\" current becomes the page of the previous level of the Menu:: If the root page is "
            "current, the menu is closed.\n"
            "4. At control \"SERVICE\x99Mode btn MENU\x99Toggle\" current becomes the page of the current level of the Menu:: If the current page the "
            "last in the current level, happens transition to the previous level of the Menu::\n"
            "5. If the menu is in the mode of small buttons, pressing closes the page.";

    } 
    else if (button == Key::Display)
    {
        Menu::stringForHint = LANG_RU ?
            "������ ������� ��������� ���� �������� �������."
            :
            "������� button opens the display settings Menu::";
    }
    else if (button == Key::Memory)
    {
        Menu::stringForHint = LANG_RU ?
            "1. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����\" ��������� ���� ������ � �������.\n"
            "2. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����������\" ��������� ������ �� ����-����."
            :
            "1. When setting \"������-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"������-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (button == Key::Measure)
    {
        Menu::stringForHint = LANG_RU ?
            "������ ����� ��������� ���� �������������� ���������."
            :
            "����� button opens a menu of automatic measurements.";
    }
    else if (button == Key::Service)
    {
        Menu::stringForHint = LANG_RU ?
            "������ ������ ��������� ���� ��������� ������������."
            :
            "������ button opens a menu of service options.";
    }
    else if (button == Key::Start)
    {
        Menu::stringForHint = LANG_RU ?
            "������ ����/�TO� ��������� � ������������� ������� ����� ����������."
            :
            "����/�TO� button starts and stops the process of gathering information.";
    }
    else if (button == Key::ChannelA)
    {
        Menu::stringForHint = LANG_RU ?
            "1. ������ �����1 ��������� ���� �������� ������ 1.\n"
            "2. ������� � ��������� ������ �����1 � ������� 0.5� ������������� �������� ������ 1 �� ��������� 0�."
            :
            "1. �����1 button opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the button �����1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (button == Key::ChannelB)
    {
        Menu::stringForHint = LANG_RU ?
            "1. ������ �����2 ��������� ���� �������� ������ 2.\n"
            "2. ������� � ��������� ������ �����2 � ������� 0.5� ������������� �������� ������ 2 �� ��������� 0�."
            :
            "1. �����2 button opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the button �����2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (button == Key::Time)
    {
        Menu::stringForHint = LANG_RU ?
            "1. ������ ���� ��������� ���� �������� ���������.\n"
            "2. ������� � ��������� ������ ���� � ������� 0.5� ������������� �������� �� ����������� 0�."
            :
            "1. ���� button open the settings menu sweep.\n"
            "2. Pressing and holding the button ���� for 0.5s Is the offset horizontal 0s.";
    }
    else if (button == Key::Trig)
    {
        Menu::stringForHint = LANG_RU ?
            "1. ������ ����� ��������� ���� �������� �������������.\n"
            "2. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99�����������\" ���������� �������������� "
            "��������� ������ �������������.\n"
            "3. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99����� ������\" ������������� ������� "
            "������������� 0�."
            :
            "1. ����� button opens a menu settings synchronization.\n"
            "2. Pressing and holding the button ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99\x41utolevel\" automatically adjust the "
            "trigger level.\n"
            "3. Pressing and holding the button ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig level\" sets the trigger "
            "level 0V.";
    }
    else
    {
        shortPressureButton = button;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetAutoHide(bool)
{
    if(!Menu::IsShown())
    {
        return;
    }
    if(Display::TimeMenuAutoHide() == 0)
    {
        Timer::Disable(Timer::Type::MenuAutoHide);
    }
    else
    {
        Timer::SetAndStartOnce(Timer::Type::MenuAutoHide, OnTimerAutoHide, (uint)Display::TimeMenuAutoHide());
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Menu::StringNavigation(char buffer[100])
{
    buffer[0] = 0;
    const char * titles[10] = {0};
    int numTitle = 0;
    Control *item = OpenedItem();
    if(IsMainPage(item))
    {
        return 0;
    }
    while(!IsMainPage(item))
    {
        titles[numTitle++] = item->Title();
        item = (Control *)item->keeper;
    }
    for(int i = 9; i >= 0; i--)
    {
        if(titles[i])
        {
            strcat(buffer, titles[i]);
            if(i != 0)
            {
                strcat(buffer, " - ");
            }
        }
    }
    return buffer;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::OnTimerAutoHide()
{
    Menu::Show(false);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessingShortPressureButton()
{
    if(shortPressureButton != Key::None)
    {
        if (shortPressureButton == Key::Memory && MODE_BTN_MEMORY_IS_SAVE && FDrive::IsConnected())
        {
            EXIT_FROM_SETNAME_TO = (uint)(Menu::IsShown() ? RETURN_TO_MAIN_MENU : RETURN_TO_DISABLE_MENU);
            PageMemory::SaveSignalToFlashDrive();
            shortPressureButton = Key::None;
            return;
        }
        NEED_FINISH_DRAW = 1;
        Menu::SetAutoHide(true);

        Key button = shortPressureButton;

        do
        {
            if(button == Key::Enter)                                   // ���� ������ ������ ���� � �� �� ��������� � ����� ��������� ���������.
            {
                if(!Menu::IsShown())
                {
                    Menu::Show(true);
                }
                else
                {
                    if (IS_PAGE(OpenedItem()))
                    {
                        Menu::TemporaryEnableStrNavi();
                    }
                    Menu::CloseOpenedItem();
                }
            }
            else if (Menu::IsShown() && button.IsFunctional())       // ���� ���� �������� � ������ �������������� �������
            {
                void *item = itemUnderButton[button];
                if (HINT_MODE_ENABLED)
                {
                    SetItemForHint(item);
                }
                else
                {
                    if(item)
                    {
                        ((Control *)item)->ShortPress();
                    }
                }
            }
            else                                                        // ���� ���� �� ��������.
            {
                Page::Name name = ((const Page *)OpenedItem())->GetName();
                if(button == Key::ChannelA && name == Page::Name::ChannelA && Menu::IsShown())
                {
                    SET_ENABLED_A = !SET_ENABLED_A;
                    PageChannelA::OnChanged_Input(true);
                    break;
                }
                if(button == Key::ChannelB && name == Page::Name::ChannelB && Menu::IsShown())
                {
                    SET_ENABLED_B = !SET_ENABLED_B;
                    PageChannelB::OnChanged_Input(true);
                    break;
                }

                Page *page = (Page *)PageForButton(button);
                if(page && page != (Page *)PageHelp::pointer)
                {
                    page->SetCurrent(true);
                    page->Open(true);
                    Menu::TemporaryEnableStrNavi();
                    Menu::Show(true);
                }
            }
        } while(false);

        shortPressureButton = Key::None;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessingLongPressureButton()
{
    Key button = longPressureButton;

    if(button != Key::None)
    {
        Control *item = OpenedItem();
        
        Sound::ButtonRelease();
        NEED_FINISH_DRAW = 1;
        SetAutoHide(true);

        if(button == Key::Time)
        {
            FPGA::SetTShift(0);
        }
        else if(button == Key::Trig)
        {
            FPGA::SetTrigLev(TRIG_SOURCE, Trig::ZERO);
        }
        else if(button == Key::ChannelA)
        {
            FPGA::SetRShift(Chan::A, RShift::ZERO);
        }
        else if(button == Key::ChannelB)
        {
            FPGA::SetRShift(Chan::B, RShift::ZERO);
        }
        else if(button == (Key::Enter))
        {
            if (IS_PAGE_SB(OpenedItem()))
            {
                CloseOpenedItem();
            }
            else
            {
                Show(!Menu::IsShown());
                if (NOT_PAGE(item))
                {
                    TemporaryEnableStrNavi();
                }
            }
        }
        else if(Menu::IsShown() && button.IsFunctional())
        {
            item = (Control *)itemUnderButton[button];
            if(item)
            {
                item->LongPress();
            }
            if (NOT_PAGE(item))
            {
                TemporaryEnableStrNavi();
            }
        }
        longPressureButton = Key::None;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessingPressButton()
{
    if ((pressButton >= Key::F1 && pressButton <= Key::F5) || pressButton == Key::Enter)
    {
        if (pressButton != Key::Enter)
        {
            itemUnderKey = itemUnderButton[pressButton];
        }
    }
    if (pressButton == Key::Start && !MODE_WORK_IS_RAM)
    {
        FPGA::OnPressStart();
    }
    pressButton = Key::None;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessingReleaseButton()
{
    if((releaseButton >= Key::F1 && releaseButton <= Key::F5) || pressButton == Key::Enter)
    {
        itemUnderKey = 0;
        releaseButton = Key::None;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::TemporaryEnableStrNavi()
{
    if (SHOW_STRING_NAVI_TEMP)
    {
        SHOW_STRING_NAVIGATION = 1;                                            // ������������� ������� ����, ��� ���� �������� ������ ��������� ����
        Timer::SetAndStartOnce(Timer::Type::StrNaviAutoHide, OnTimerStrNaviAutoHide, 3000); // � ��������� ������, ������� ��� ��������
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::OnTimerStrNaviAutoHide()
{
    SHOW_STRING_NAVIGATION = 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ShortPress_ChoiceReg(void *choice_)
{
    Choice *choice = (Choice*)choice_;

    if(!choice->IsAcitve()) 
    {
        CHOICE_RUN_FUNC_CHANGED(choice, false);
    } 
    else if(OpenedItem() != choice) 
    {
        choice->SetCurrent(CurrentItem() != choice);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ChangeStateFlashDrive()
{
    if(!FDrive::IsConnected())
    {
        if(GetNameOpenedPage() == Page::Name::SB_Memory_Drive_Manager)
        {
            ((Page *)OpenedItem())->ShortPressOnItem(0);
        }
    }
    else if(FLASH_AUTOCONNECT)
    {
        PageMemory::OnPress_Drive_Manager();
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::OpenItemTime()
{
    Display::ShowWarning(Warning::TimeNotSet);
    ShortPressureButton(Key::Service);
    Update();
    Display::Update();
    for (int i = 0; i < 2; i++)
    {
        Update();
        Display::Update();
    }
    ShortPressureButton(Key::F4);
    Update();
    Display::Update();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::NeedForFireSetLED()
{
    Control *item = OpenedItem();
    
    if (!Menu::IsShown())
    {
        return IS_CHOICE_REG(item) || IS_CHOICE(item) || IS_GOVERNOR(item);
    }

    Page::Name name = GetNameOpenedPage();
    if (
            name == Page::Name::SB_Debug_SerialNumber   ||
            name == Page::Name::SB_Service_FFT_Cursors  || 
            name == Page::Name::Measures_Auto_Tune      || 
            name == Page::Name::SB_Memory_Last          || 
            name == Page::Name::SB_Memory_Internal      ||
            PageMeasures::PageCursors::PageSet::IsRegSetActiveOnCursors() ||
            (name == Page::Name::SB_Service_Function && FUNC_MODE_DRAW_IS_ENABLED)
        )
    {
        return true;
    }
    
    item = CurrentItem();
    
    if (IS_GOVERNOR(item)       ||
        IS_CHOICE_REG(item)     ||
        IS_GOVERNOR_COLOR(item))
    {
        return true;
    }
    
    item = OpenedItem();

    if (IS_CHOICE(item)  ||
        (IS_PAGE(item) && ((const Page *)OpenedItem())->NumSubPages() > 1)
        )
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Show(bool show)
{
    set.menu_show = show;
    if (show)
    {
        Menu::TemporaryEnableStrNavi();
    }
    Menu::SetAutoHide(true);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::IsShown()
{
    return set.menu_show;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Init()
{
    PageDisplay::Init();
    PageFunction::PageMultimeter::Init();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::RunAfterUpdate(pFuncVV func)
{
    funcAterUpdate = func;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::OpenedItem()
{
    Control::Type type = Control::Type::None;
    return (Control *)RetLastOpened((Page *)pageMain, &type);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Page::Name Menu::GetNameOpenedPage()
{
    return ((const Page *)OpenedItem())->GetName();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void *Menu::RetLastOpened(Page *page, Control::Type *type)
{
    if (page->CurrentItemIsOpened())
    {
        int8 posActItem = page->PosCurrentItem();
        void *item = page->Item(posActItem);
        if (IS_PAGE(page->Item(posActItem)))
        {
            return RetLastOpened((Page *)item, type);
        }
        else
        {
            return item;
        }
    }
    *type = Control::Type::Page;
    return page;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::CurrentItem()
{
    Control::Type type = Control::Type::None;
    void *lastOpened = RetLastOpened((Page *)pageMain, &type);
    int8 pos = ((const Page *)lastOpened)->PosCurrentItem();
    if (type == Control::Type::Page && pos != 0x7f)
    {
        return ((const Page *)lastOpened)->Item(pos);
    }
    return (Control *)lastOpened;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::CloseOpenedItem()
{
    Control *item = OpenedItem();
    if (IS_PAGE(item))
    {
        if (IS_PAGE_SB(item))
        {
            //((Page *)item)->SmallButonFromPage(0)->funcOnPress();
            SMALL_BUTTON_FROM_PAGE(item, 0)->funcOnPress();
        }
        else
        {
            ((Page *)item)->funcOnEnterExit(false);
        }
        Page::Name name = KEEPER(item)->name;
        ((Page *)KEEPER(item))->SetPosActItem(MENU_POS_ACT_ITEM(name) & 0x7f);
        if (item == (Control *)pageMain)
        {
            Menu::Show(false);
        }
    }
    else
    {
        item->Open(false);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ChangeItem(Control *item, int delta)
{
    if (IS_CHOICE(item) || IS_CHOICE_REG(item))
    {
        ((Choice *)item)->StartChange(delta);
    }
    else if (IS_GOVERNOR(item))
    {
        Governor *governor = (Governor*)item;
        if (OpenedItem() != governor)
        {
            governor->StartChange(delta);
        }
        else
        {
            governor->ChangeValue(delta);
        }
    }
    else if (IS_GOVERNOR_COLOR(item))
    {
        ((GovernorColor *)item)->ChangeValue(delta);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Page *Menu::PagePointerFromName(Page::Name)
{
    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHintItem(int x, int y, int width)
{
    if (!Menu::itemHint)
    {
        return;
    }

    static const struct StructName
    {
        pString name;
        StructName(pString n) : name(n) {};
    }
    names[Control::Type::Number][2] =
    {
        {"",            ""},                    // Item_None
        {"",            ""},                    // Control::Type::Choice
        {"������",      "Button"},              // Control::Type::Button
        {"��������",    "Page"},                // Control::Type::Page
        {"",            ""},                    // Control::Type::Governor
        {"",            ""},                    // Control::Type::Time
        {"",            ""},                    // Control::Type::GovernorColor
        {"",            ""},                    // Control::Type::ChoiceReg
        {"������",      "Button"},              // Control::Type::DrawButton
        {"����� ���������", "Choice parameter"} // Control::Type::ChoiceParameter
    };
    Language lang = LANG;
    Page *item = (Page *)Menu::itemHint;

    const int SIZE = 100;
    char title[SIZE];
    snprintf(title, SIZE, "%s \"%s\"", names[Menu::itemHint->type][lang].name, item->titleHint[lang]);

    if (item->type == Control::Type::DrawButton)
    {
        y -= 9;
    }
    Painter::DrawStringInCenterRectAndBoundItC(x, y, width, 15, title, Color::BACK, Color::FILL);
    y = Painter::DrawTextInBoundedRectWithTransfers(x, y + 15, width, item->titleHint[2 + lang], Color::BACK, Color::FILL);
    if (item->type == Control::Type::DrawButton)
    {
        ((SButton*)item)->DrawHints(x, y, width);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Menu::Graphics::Y()
{
    return Display::HEIGHT - Item::HEIGHT - 2;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Graphics::Draw()
{
    if (Menu::IsShown() || NOT_PAGE(OpenedItem()))
    {
        ResetItemsUnderButton();
        Control *item = OpenedItem();
        if (Menu::IsShown())
        {
            if (IS_PAGE(item))
            {
                item->Draw(0, Y(), true);
            }
            else
            {
                ((Page *)KEEPER(item))->Draw(0, Y(), true);
            }
        }
        else
        {
            if (IS_CHOICE(item) || IS_CHOICE_REG(item))
            {
                ((Choice *)item)->Draw(0, Grid::Top(), false);
                Painter::DrawVLine(0, Grid::Top() + 1, Grid::Top() + 34, Color::BorderMenu(false));
                Painter::DrawVLine(0 + 1, Grid::Top() + 1, Grid::Top() + 34);
                Painter::DrawVLine(Grid::Right(), Grid::Top() + 30, Grid::Top() + 40, Color::FILL);
                Painter::DrawVLine(0 - 1, Grid::Top() + 1, Grid::Top() + 35, Color::BACK);
                Painter::DrawHLine(Grid::Top() + 35, 0 - 1, Grid::Right() - 1);
            }
            else if (IS_GOVERNOR(item))
            {
                ((Governor *)item)->Draw(0, Grid::Top(), true);
                Painter::DrawHLine(Grid::Top(), 0 - 2, Grid::Right(), Color::FILL);
                Painter::DrawVLine(Grid::Right(), Grid::Top(), Grid::Top() + 40);
            }
        }
    }

    if (HINT_MODE_ENABLED)
    {
        int x = 1;
        int y = 0;
        int width = 318;
        if (Menu::IsShown())
        {
            width = Menu::IsMinimize() ? 289 : 220;
        }
        Painter::DrawTextInBoundedRectWithTransfers(x, y, width,
               LANG_RU ? "������� ����� ���������. � ���� ������ ��� ������� �� ������ �� ����� ��������� ���������� � � ����������. "
               "����� ��������� ���� �����, ������� ������ ������ � ����������� � � ������� 0.5�." :
               "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
               "To disable this mode, press the button HELP and hold it for 0.5s.",
               Color::BACK, Color::FILL);
        y += LANG_RU ? 49 : 40;
        if (Menu::stringForHint)
        {
            Painter::DrawTextInBoundedRectWithTransfers(x, y, width, Menu::stringForHint, Color::BACK, Color::WHITE);
        }
        else if (Menu::itemHint)
        {
            DrawHintItem(x, y, width);
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ResetItemsUnderButton()
{
    for (int i = 0; i < Key::Number; i++)
    {
        itemUnderButton[i] = 0;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::IsMinimize()
{
    return IS_PAGE_SB(Menu::OpenedItem());
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const SButton *Menu::GetDrawButton(Key button)
{
    if (Menu::IsMinimize() && button >= Key::Enter && button <= Key::F5)
    {
        Page *page = (Page *)OpenedItem();
        SButton *sb = (SButton *)page->items[button - Key::Enter];
        return sb;
    }
    return NULL;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetItemForHint(void *item)
{
    Menu::stringForHint = 0;
    Menu::itemHint = (Control *)item;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ButtonEvent(KeyEvent event)
{
    /// \todo �� ����� ������ ������ ��� ���������� ��������� ������ ��������
    //Sound::Beep(event.type);
    BufferButtons::Push(event);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SaveSettings()
{
    if((timeLastPressedButton != MAX_UINT) && (TIME_MS - timeLastPressedButton > 5000))
    {
        timeLastPressedButton = MAX_UINT;
        if(Device::CurrentMode() != Device::Mode::Tester)
        {
            Settings::Save();
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ReleaseFunctionalButton(Key key)
{
    if(Menu::IsShown())
    {
        Control *control = (Control *)itemUnderButton[key];
        if(control)
        {
            control->ShortPress();
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::LongFunctionalButton(Key key)
{
    if(Menu::IsShown())
    {
        Control *control = (Control *)itemUnderButton[key];
        if(control)
        {
            control->LongPress();
        }
    }
}
