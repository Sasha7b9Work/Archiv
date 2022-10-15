// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "defines.h"
#include "Panel/Controls.h"
#include "Menu.h" 
#include "Settings/Settings.h"
#include "Pages/Definition.h"
#include "Pages/PageMemory.h"
#include "Display/DisplayTypes.h"
#include "MenuDrawing.h"
#include "MenuItems.h"
#include "defines.h"
#include "MenuFunctions.h"
#include "MenuItemsLogic.h"
#include "Tables.h"
#include "Display/Display.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Measures.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "FPGA/DataStorage.h"
#include "Hardware/Timer.h"
#include "Log.h"
#include "FPGA/FPGA.h"
#include "Hardware/Hardware.h"
#include "Hardware/Sound.h"
#include "Hardware/RTC.h"
#include "Panel/Panel.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void OnChanged_InputA(bool active);
extern void OnChanged_InputB(bool active);
                                                
static PanelButton shortPressureButton = B_Empty;   ///< ���� ��������� �������� ������� ������, �� ����� �������� ��� ���� ������ �� ��������� ����� �������.
static PanelButton longPressureButton = B_Empty;    ///< ���� ��������� ������� ������� ������, �� ����� �������� ��� ���� ������ �� ��������� ����� �������.
static PanelButton pressButton = B_Empty;           ///< ��� ������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� �������� ������� ������� ������.
static PanelButton releaseButton = B_Empty;         ///< ��� ���������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� ��������� ������� ���������� ������.
static int angleRegSet = 0;                         ///< ����, �� ������� ����� ��������� ����� ��������� - �������� �������� ���������� �������, ���� - ����������� - "-" - �����, "+" - ������
static void* itemUnderKey = 0;                      ///< ����� �������� ����� �������� ����, ���������������� �������������� ������� [1..5], ���� ��� ��������� � ������ ���������, � 0, ���� �� ���� ������ �� ������.

                                                
static void ProcessingShortPressureButton(void);    ///< ��������� ��������� ������� ������.
static void ProcessingLongPressureButton(void);     ///< ��������� �������� ������� ������.
static void ProcessingPressButton(void);            ///< ��������� ��������� ������ ����.
static void ProcessingReleaseButton(void);          ///< ��������� �������� ������ �����.
static void ProcessingRegulatorSet(void);           ///< ��������� �������� ����� ���������.
static void OnTimerAutoHide(void);                  ///< ��������� ������� ������� ��������������� �������� ����.

static void SwitchSetLED(void);                     ///< ��������/��������� ��������� ����� ���������, ���� ����������.

static void ShortPress_Page(void *page);                    ///< ��������� ��������� ������� �� ������� NamePage � ������� page.
static void ShortPress_Choice(void *choice);                ///< ��������� ��������� ������� �� ������� Choice � ������� choice.
static void ShortPress_Time(void *time);                
static void ShortPress_Button(void *button);                ///< ��������� ��������� ������� �� ������� Button � ������� button.
static void ShortPress_Governor(void *governor);            ///< ��������� ��������� ������� �� ������� Governor � ������� governor.
static void ShortPress_GovernorColor(void *governorColor);  ///< ��������� ��������� ������� �� ������� GovernorColor � ������� governorColor.

static void FuncOnLongPressItem(void *item);                ///< ��������� �������� ������� �� ������� ���� item.
static void FuncOnLongPressItemTime(void *item);            
static void FuncOnLongPressItemButton(void *button);        ///< ��������� �������� ������� �� ������� Button � ������� button.

static  pFuncVpV    FuncForShortPressOnItem(void *item);    ///< ���������� ������� ��������� ��������� ������� �� ������� ���� item.
static  pFuncVpV    FuncForLongPressureOnItem(void *item);  ///< ���������� ������� ��������� �������� ������� �� ������� ���� item.

#define SIZE_BUFFER_FOR_BUTTONS 5
static PanelButton bufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {B_Empty, B_Empty, B_Empty, B_Empty, B_Empty};
static const PanelButton sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {B_F5, B_F4, B_F3, B_F2, B_F1};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu_UpdateInput(void)
{
    ProcessingShortPressureButton();
    ProcessingLongPressureButton();
    ProcessingRegulatorSet();
    ProcessingPressButton();
    ProcessingReleaseButton();
    SwitchSetLED();
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu_ShortPressureButton(PanelButton button)
{
    if (gBF.showHelpHints == 0)
    {
        shortPressureButton = button;
    }
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu_LongPressureButton(PanelButton button)
{
    if (gBF.showHelpHints == 0)
    {
        longPressureButton = button;
        Display_Redraw();
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessButtonForHint(PanelButton button)
{
    if (button == B_Menu)
    {
        gStringForHint = set.common.lang == Russian ?
            "������ ���� ��������� ��������� �������:\n"
            "1. ��� �������� ���� ������� ���� ������� � ���������� � ������� 0.5� ��������� ����.\n"
            "2. ��� �������� ���� ��������� ������ � ������� 0.5� ��������� ����.\n"
            "3. ��� ��������� \"������\x99����� �� ����\x99���������\" ������� ���������� �������� ����������� ������ ����. ���� ������� �������� �������� ��������, ���� �����������.\n"
            "4. ��� ��������� \"������\x99����� �� ����\x99�����������\" ������� ���������� �������� �������� ������ ����. ���� ������� �������� ��������� � ������� ������, ���������� ������� �� ���������� ������� ����.\n"
            "5. ���� ���� ��������� � ������ ����� ������, �� ������� ��������� ��������."
            :
        "MENU button performs the following functions:\n"
            "1. At the closed menu pressing or pressing with deduction during 0.5s opens the menu.\n"
            "2. At the open menu deduction of the button during 0.5s closes the menu.\n"
#pragma push
#pragma diag_suppress 192
            "3. At control \"SERVICE\x99Mode btn MENU\x99\Close\" current becomes the page of the previous level of the menu. If the root page is current, the menu is closed.\n"
#pragma pop
            "4. At control \"SERVICE\x99Mode btn MENU\x99Toggle\" current becomes the page of the current level of the menu. If the current page the last in the current level, happens transition to the previous level of the menu.\n"
            "5. If the menu is in the mode of small buttons, pressing closes the page.";

    } else if (button == B_Cursors)
    {
        gStringForHint = set.common.lang == Russian ? 
            "������ ������� ��������� ���� ��������� ���������."
            :
            "������� button to open the menu cursor measurements.";
    }
    else if (button == B_Display)
    {
        gStringForHint = set.common.lang == Russian ?
            "������ ������� ��������� ���� �������� �������."
            :
            "DISPLAY button opens the display settings menu.";
    }
    else if (button == B_Memory)
    {
        gStringForHint = set.common.lang == Russian ?
            "1. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����\" ��������� ���� ������ � �������.\n"
            "2. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����������\" ��������� ������ �� ����-����."
            :
            "1. When setting \"MEMORY-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"MEMORY-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (button == B_Measures)
    {
        gStringForHint = set.common.lang == Russian ?
            "������ ����� ��������� ���� �������������� ���������."
            :
            "����� button opens a menu of automatic measurements.";
    }
    else if (button == B_Help)
    {
        gStringForHint = set.common.lang == Russian ?
            "1. ������ ������ ��������� ���� ������.\n"
            "2. ������� � ��������� ������ ������ � ������� 0.5� �������� � ��������� ����� ������ ���������."
            :
            "1. ������ button opens the help menu.\n"
            "2. Pressing and holding the ������ for 0.5s enables and disables output mode hints.";
    }
    else if (button == B_Service)
    {
        gStringForHint = set.common.lang == Russian ?
            "������ ������ ��������� ���� ��������� ������������."
            :
            "������ button opens a menu of service options.";
    }
    else if (button == B_Start)
    {
        gStringForHint = set.common.lang == Russian ?
            "������ ����/�TO� ��������� � ������������� ������� ����� ����������."
            :
            "����/�TO� button starts and stops the process of gathering information.";
    }
    else if (button == B_Channel0)
    {
        gStringForHint = set.common.lang == Russian ?
            "1. ������ �����1 ��������� ���� �������� ������ 1.\n"
            "2. ������� � ��������� ������ �����1 � ������� 0.5� ������������� �������� ������ 1 �� ��������� 0�."
            :
            "1. �����1 button opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the button �����1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (button == B_Channel1)
    {
        gStringForHint = set.common.lang == Russian ?
            "1. ������ �����2 ��������� ���� �������� ������ 2.\n"
            "2. ������� � ��������� ������ �����2 � ������� 0.5� ������������� �������� ������ 2 �� ��������� 0�."
            :
            "1. �����2 button opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the button �����2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (button == B_Time)
    {
        gStringForHint = set.common.lang == Russian ?
            "1. ������ ���� ��������� ���� �������� ���������.\n"
            "2. ������� � ��������� ������ ���� � ������� 0.5� ������������� �������� �� ����������� 0�."
            :
            "1. ���� button open the settings menu sweep.\n"
            "2. Pressing and holding the button ���� for 0.5s Is the offset horizontal 0s.";
    }
    else if (button == B_Trig)
    {
        gStringForHint = set.common.lang == Russian ?
            "1. ������ ����� ��������� ���� �������� �������������.\n"
            "2. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99�����������\" ���������� �������������� ��������� ������ �������������.\n"
            "3. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99����� ������\" ������������� ������� ������������� 0�."
            :
            "1. ����� button opens a menu settings synchronization.\n"
#pragma push
#pragma diag_suppress 192
            "2. Pressing and holding the button ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99\Autolevel\" automatically adjust the trigger level.\n"
#pragma pop
            "3. Pressing and holding the button ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig level\" sets the trigger level 0V.";
    }
    else
    {
        shortPressureButton = button;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu_PressButton(PanelButton button)
{
    Sound_ButtonPress();
    if (gBF.showHelpHints)
    {
        ProcessButtonForHint(button);
        return;
    }

    if (pressButton == B_Start)
    {
        pressButton = button;
    } 
    else if (!MenuIsShown())
    {
        for (int i = SIZE_BUFFER_FOR_BUTTONS - 1; i > 0; i--)
        {
            bufferForButtons[i] = bufferForButtons[i - 1];
        }
        bufferForButtons[0] = button;
      
        if (memcmp(bufferForButtons, sampleBufferForButtons, SIZE_BUFFER_FOR_BUTTONS) == 0)
        {
            gBF.showDebugMenu = 1;
            Display_ShowWarningGood(MenuDebugEnabled);
        }
    }
    pressButton = button;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu_ReleaseButton(PanelButton button)
{
    Sound_ButtonRelease();
    if (gBF.showHelpHints == 0)
    {
        releaseButton = button;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu_RotateRegSetRight(void)
{   
    if (gBF.showHelpHints == 0)
    {
        angleRegSet++;
        Display_Redraw();
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu_RotateRegSetLeft(void)
{
    if (gBF.showHelpHints == 0)
    {
        angleRegSet--;
        Display_Redraw();
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void* Menu_ItemUnderKey(void)
{
    return itemUnderKey;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu_SetAutoHide(bool active)
{
    if(!MenuIsShown())
    {
        return;
    }
    if(sDisplay_TimeMenuAutoHide() == 0)
    {
        Timer_Disable(kMenuAutoHide);
    }
    else
    {
        Timer_Enable(kMenuAutoHide, sDisplay_TimeMenuAutoHide(), OnTimerAutoHide);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char* Menu_StringNavigation(char buffer[100])
{
    if(SHOW_STRING_NAVIGATION && MenuIsShown())
    {
        buffer[0] = 0;
        const char *titles[10] = {0};
        int numTitle = 0;
        void *item = OpenedItem();
        if(IsMainPage(item))
        {
            return 0;
        }
        while(!IsMainPage(item))
        {
            titles[numTitle++] = TitleItem(item);
            item = Keeper(item);
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
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnTimerAutoHide(void)
{
    ShowMenu(false);
    Timer_Disable(kMenuAutoHide);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingShortPressureButton()
{
    if(shortPressureButton != B_Empty)
    {
        if (shortPressureButton == B_Memory && MODE_BTN_MEMORY_IS_SAVE && gBF.flashDriveIsConnected == 1)
        {
            gMemory.exitFromModeSetNameTo = MenuIsShown() ? RETURN_TO_MAIN_MENU : RETURN_TO_DISABLE_MENU;
            Memory_SaveSignalToFlashDrive();
            shortPressureButton = B_Empty;
            return;
        }
        Display_Redraw();
        Menu_SetAutoHide(true);

        PanelButton button = shortPressureButton;

        do
        {
            if(button == B_Menu)                                   // ���� ������ ������ ���� � �� �� ��������� � ����� ��������� ���������.
            {
                if(!MenuIsShown())
                {
                    ShowMenu(true);
                }
                else
                {
                    CloseOpenedItem();
                }
            }
            else if (MenuIsShown() && IsFunctionalButton(button))       // ���� ���� �������� � ������ �������������� �������
            {
                void *item = ItemUnderButton(button);
                if (gBF.showHelpHints)
                {
                    SetItemForHint(item);
                }
                else
                {
                    FuncForShortPressOnItem(item)(item);
                }
            }
            else                                                        // ���� ���� �� ��������.
            {
                NamePage name = GetNamePage(OpenedItem());
                if(button == B_Channel0 && name == Page_Channel0)
                {
                    ENABLED_A = !sChannel_Enabled(A);
                    OnChanged_InputA(true);
                    break;
                }
                if(button == B_Channel1 && name == Page_Channel1)
                {
                    ENABLED_B = !sChannel_Enabled(B);
                    OnChanged_InputB(true);
                    break;
                }

                const void *page = PageForButton(button);
                if(page)
                {
                    SetCurrentItem(page, true);
                    OpenItem(page, true);
                    ShowMenu(true);
                }
            }
        } while(false);

        shortPressureButton = B_Empty;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingLongPressureButton(void)
{
    if(longPressureButton != B_Empty)
    {
        Display_Redraw();
        Menu_SetAutoHide(true);

        if(longPressureButton == B_Time)
        {
            FPGA_SetTShift(0);
        }
        else if(longPressureButton == B_Trig)
        {
            FPGA_SetTrigLev(TRIG_SOURCE, TrigLevZero);
        }
        else if(longPressureButton == B_Channel0)
        {
            FPGA_SetRShift(A, RShiftZero);
        }
        else if(longPressureButton == B_Channel1)
        {
            FPGA_SetRShift(B, RShiftZero);
        }
        else if(longPressureButton == B_Menu)
        {
                ShowMenu(!MenuIsShown());
        }
        else if(MenuIsShown() && IsFunctionalButton(longPressureButton))
        {
            void *item = ItemUnderButton(longPressureButton);
            FuncForLongPressureOnItem(item)(item);
        }
        longPressureButton = B_Empty;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingRegulatorSet(void)
{
    if (angleRegSet == 0)
    {
        return;
    }

    if (MenuIsShown() || TypeMenuItem(OpenedItem()) != Item_Page)
    {
        void *item = CurrentItem();
        TypeItem type = TypeMenuItem(item);
        static const int step = 2;
        if (TypeMenuItem(OpenedItem()) == Item_Page && (type == Item_ChoiceReg || type == Item_Governor || type == Item_IP || type == Item_MAC))
        {
            if (angleRegSet > step || angleRegSet < -step)
            {
                ChangeItem(item, angleRegSet);
                angleRegSet = 0;
            }
            return;
        }
        else
        {
            item = OpenedItem();
            type = TypeMenuItem(item);
            if (MenuIsMinimize())
            {
                CurrentPageSBregSet(angleRegSet);
            }
            else if (type == Item_Page || type == Item_IP || type == Item_MAC || type == Item_Choice || type == Item_ChoiceReg || type == Item_Governor)
            {
                if (ChangeOpenedItem(item, angleRegSet))
                {
                    angleRegSet = 0;
                }
                return;
            }
            else if (type == Item_GovernorColor)
            {
                ChangeItem(item, angleRegSet);
            }
            else if (type == Item_Time)
            {
                angleRegSet > 0 ? ItemTime_IncCurrentPosition(item) : ItemTime_DecCurrentPosition(item);
            }
        }
    }

    angleRegSet = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingPressButton(void)
{
    if (pressButton == B_Start && !MODE_WORK_IS_LATEST)
    {
        FPGA_OnPressStartStop();
    } 
    else if((pressButton >= B_F1 && pressButton <= B_F5) || pressButton == B_Menu)
    {
        if (pressButton != B_Menu)
        {
            itemUnderKey = ItemUnderButton(pressButton);
        }
    }
    pressButton = B_Empty;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingReleaseButton(void)
{
    if(releaseButton >= B_F1 && releaseButton <= B_F5 || pressButton == B_Menu)
    {
        itemUnderKey = 0;
        releaseButton = B_Empty;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Page(void *item)
{
    Page *page = item;
    if (page->funcOnPress)
    {
        page->funcOnPress();
    }
    if (!ItemIsActive(page))
    {
        return;
    }
    SetCurrentItem(page, true);
    OpenItem((Page*)page, !ItemIsOpened((Page*)page));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Choice(void *choice)
{
    if (!ItemIsActive(choice))
    {
        CHOICE_FUNC_ON_CHANGED((Choice*)choice, false);
    }
    else if (!ItemIsOpened(choice))
    {
        SetCurrentItem(choice, CurrentItem() != choice);
        Choice_StartChange(choice, 1);
    }
    else
    {
        Choice_ChangeValue(choice, -1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_ChoiceReg(void *choice) 
{
    if(!ItemIsActive(choice)) 
    {
        CHOICE_FUNC_ON_CHANGED((Choice*)choice, false);
    } 
    else if(OpenedItem() != choice) 
    {
        SetCurrentItem(choice, CurrentItem() != choice);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncOnLongPressItemButton(void *button)
{
    ShortPress_Button(button);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Button(void *button)
{
    if(!ItemIsActive(button))
    {
        return;
    }
    SetCurrentItem(button, true);
    ((Button*)button)->funcOnPress();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncOnLongPressItem(void *item)
{
    if (CurrentItem() != item)
    {
        SetCurrentItem(item, true);
    }
    OpenItem(item, !ItemIsOpened(item));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncOnLongPressItemTime(void *time)
{
    if (CurrentItem() != time)
    {
        SetCurrentItem(time, true);
    }
    if(ItemIsOpened(time) && *((Time*)time)->curField == iSET)
    {
        ItemTime_SetNewTime(time);
    }
    OpenItem(time, !ItemIsOpened(time));
    ItemTime_SetOpened(time);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Time(void *time)
{
    if(!ItemIsOpened(time))
    {
        SetCurrentItem(time, true);
        ItemTime_SetOpened(time);
        OpenItem(time, true);
    }
    else
    {
        ItemTime_SelectNextPosition(time);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Governor(void *governor)
{
    Governor *gov = (Governor*)governor;
    if(!ItemIsActive(governor))
    {
        return;
    }
    if(OpenedItem() == gov)
    {
        Governor_NextPosition(gov);
    }
    else
    {
        SetCurrentItem(gov, CurrentItem() != gov);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ShortPress_IP(void *item)
{
    if (OpenedItem() == item)
    {
        IPaddress_NextPosition((IPaddress*)item);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_MAC(void *item)
{
    if (OpenedItem() == item)
    {
        CircleIncreaseInt8(&gCurDigit, 0, 5);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_GovernorColor(void *governorColor)
{
    if(!ItemIsActive(governorColor))
    {
        return;
    }
    GovernorColor *governor = (GovernorColor*)governorColor;
    if(OpenedItem() == governor)
    {
        CircleIncreaseInt8(&(governor->colorType->currentField), 0, 3);
    }
    else
    {
        FuncOnLongPressItem(governorColor);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ShortPress_SmallButton(void *smallButton)
{
    SmallButton *sb = smallButton;
    if (sb)
    {
        pFuncVV func = sb->funcOnPress;
        if (func)
        {
            func();
            itemUnderKey = smallButton;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pFuncVpV FuncForShortPressOnItem(void *item)
{
    static const pFuncVpV shortFunction[Item_NumberItems] =
    {
        EmptyFuncVpV,               // Item_None
        ShortPress_Choice,          // Item_Choice
        ShortPress_Button,          // Item_Button
        ShortPress_Page,            // Item_Page
        ShortPress_Governor,        // Item_Governor
        ShortPress_Time,            // Item_Time
        ShortPress_IP,              // Item_IP
        EmptyFuncVpV,               // Item_SwitchButton
        ShortPress_GovernorColor,   // Item_GovernorColor
        EmptyFuncVpV,               // Item_Formula
        ShortPress_MAC,             // Item_MAC
        ShortPress_ChoiceReg,       // Item_ChoiceReg
        ShortPress_SmallButton      // Item_SmallButton
    };

    return shortFunction[TypeMenuItem(item)];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pFuncVpV FuncForLongPressureOnItem(void *item)
{
    static const pFuncVpV longFunction[Item_NumberItems] =
    {
        EmptyFuncVpV,               // Item_None
        FuncOnLongPressItem,        // Item_Choice
        FuncOnLongPressItemButton,  // Item_Button
        FuncOnLongPressItem,        // Item_Page
        FuncOnLongPressItem,        // Item_Governor
        FuncOnLongPressItemTime,    // Item_Time
        FuncOnLongPressItem,        // Item_IP
        EmptyFuncVpV,               // Item_SwitchButton
        FuncOnLongPressItem,        // Item_GovernorColor
        EmptyFuncVpV,               // Item_Formula
        FuncOnLongPressItem,        // Item_MAC
        FuncOnLongPressItem,        // Item_ChoiceReg
        ShortPress_SmallButton      // Item_SmallButton
    };

    return ItemIsActive(item) ? longFunction[TypeMenuItem(item)] : EmptyFuncVpV;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ChangeStateFlashDrive(void)
{
    if(gBF.flashDriveIsConnected == 0)
    {
        if(GetNameOpenedPage() == Page_SB_FileManager)
        {
            ShortPressOnPageItem(OpenedItem(), 0);
        }
    }
    else if(FLASH_AUTOCONNECT)
    {
        OnPressMemoryExtFileManager();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
extern const Page mpService;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu_OpenItemTime(void)
{
    Display_ShowWarningGood(TimeNotSet);
    Menu_ShortPressureButton(B_Service);
    Menu_UpdateInput();
    Display_Update();
    for (int i = 0; i < 2; i++)
    {
        Menu_RotateRegSetRight();
        Menu_UpdateInput();
        Display_Update();
    }
    Menu_ShortPressureButton(B_F4);
    Menu_UpdateInput();
    Display_Update();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool NeedForFireSetLED(void)    // ���������� true, ���� �������� ��������� ������ ������
{
    if (!MenuIsShown())
    {
        return false;
    }
    NamePage name = GetNameOpenedPage();
    if (
        name == Page_SB_MathCursorsFFT      || 
        name == Page_SB_MeasTuneMeas        || 
        name == Page_SB_MemLatest           || 
        name == Page_SB_MemInt              ||
        (name == Page_SB_MathFunction && !DISABLED_DRAW_MATH)
        )
    {
        return true;
    }
    
    TypeItem typeCurrentItem = TypeMenuItem(CurrentItem());
    if (typeCurrentItem == Item_Governor    ||
        typeCurrentItem == Item_ChoiceReg)
    {
        return true;
    }

    TypeItem typeOpenedItem = TypeOpenedItem();
    if (typeOpenedItem == Item_Choice       ||
        (typeOpenedItem == Item_Page && NumSubPages(OpenedItem()) > 1)
        )
    {
        return true;
    }
    
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SwitchSetLED(void)
{
    static bool first = true;
    static bool prevState = false;  // true - �����, false - �� �����

    bool state = NeedForFireSetLED();

    if (first)
    {
        first = false;
        Panel_EnableLEDRegSet(state);
        prevState = state;
    }
    else if (prevState != state)
    {
        Panel_EnableLEDRegSet(state);
        prevState = state;
    }
}
