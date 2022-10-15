#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Object.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Menu
 *  @{
 *  @defgroup Pages
 *  @{
 *  @}
 */

#define PAGE_IS_MAIN                   (name == Page::Name::Main)
#define MENU_TEMP_ENABLE_STRING_NAVI() Menu::TemporaryEnableStrNavi()


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Menu
{
friend class Page;

public:
    
    static PageBase *pageMain;

    /// �������������
    static void Init();
    /// ���������� true, ���� item - ����� ������� �������� ����.
    static bool IsMainPage(const void *item);

    static void ChangeStateFlashDrive();
    /// ������� ������ ���������� � ������� �����.
    static void Update();
    /// ������� ��������� ��������� ������� ������ (����� 0.5 ���.).
    static void ShortPressureButton(Key button);
    /// ������� ��������� �������� ������� ������ (����� 0.5 ���.).
    static void LongPressureButton(Key button);
    /// ������� ���������� ��� �������, ������� � ���������� ������
    static void ButtonEvent(KeyEvent event);
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);
    /// ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    static char *StringNavigation(char buffer[100]);
    /// ��������/������ ����.
    static void Show(bool show);
    /// ���������� true, ���� ���� ������������ �� ������
    static bool IsShown();

    static void OpenItemTime();
    /// ������� �������� �������� ����������� ������ ��������� ����, ���� ������ �����������.
    static void TemporaryEnableStrNavi();
    /// ���������� ��� ������� ����� Menu_UpdateInput().
    static void RunAfterUpdate(pFuncVV func);
    /// ���������� ����� ��������� �������� ����
    static Control *OpenedItem();
    /// ���� true - ���� ��������� � �������������� ������.
    static bool IsMinimize();

    static Page::Name GetNameOpenedPage();

    static Page *PagePointerFromName(Page::Name name);
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Control *CurrentItem();
    /// ������� �������� ������� ����
    static void CloseOpenedItem();
    /// ��������� ��� ����������� �������� Governor, GovernorColor ��� Choice �� ������ item � ����������� �� ����� delta
    static void ChangeItem(Control *item, int delta);
    /// ������� ��������� �� ����� ������, ��������������� ������ ������ ������.
    static const SButton* GetDrawButton(Key button);

    static void SetItemForHint(void *item);

    static void SaveSettings();
    /// ����� �������� ��� ���������� �������������� ������
    static void ReleaseFunctionalButton(Key key);
    /// ����� �������� ��� "�������" ������� �������������� ������
    static void LongFunctionalButton(Key key);
    ///\brief  ����� �������� ����� �������� ����, ���������������� �������������� ������� [1..5], ���� ��� ��������� � ������ ���������, � 0, 
    /// ���� �� ���� ������ �� ������.
    static Control *itemUnderKey;
    /// ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
    static const char *stringForHint;
    /// Item, ��������� ��� �������� ����� �������� � ������ ����������� ������ ���������.
    static Control *itemHint;

    class Graphics
    {
    public:
        /// ���������� ����
        static void Draw();
        /// � ������ ������� Y �������� ����. ������� ������ ��� ����� ���������
        static int Y();
    };

private:

    static void *RetLastOpened(Page *_page, Control::Type *_type);
    /// ��������� ��������� ������� ������
    static void ProcessingShortPressureButton();
    /// ��������� �������� ������� ������
    static void ProcessingLongPressureButton();
    /// ��������� ��������� ������ ����
    static void ProcessingPressButton();
    /// ��������� �������� ������ �����
    static void ProcessingReleaseButton();
    /// ��������� ������� ������� ��������������� �������� ����
    static void OnTimerAutoHide();
    /// �������, ������� �������� ����� ������ ��������� ����
    static void OnTimerStrNaviAutoHide();
    /// ���������� true, ���� �������� ��������� ������ ������
    static bool NeedForFireSetLED();
    
    static void ProcessButtonForHint(Key button);
    
    static void ShortPress_ChoiceReg(void *choice);
    
    static void ResetItemsUnderButton();
    /// ���������� true, ���� ������ ������ ������������� � ������ ������
    static bool IsProcessed(KeyEvent *event);
    /// ���������� �������� ����, ������� ������ ����������� �� ������� ������ button.
    static const void *PageForButton(Key button);
    /// ���� ��������� �������� ������� ������, �� ����� �������� ��� ���� ������ �� ���������  ����� �������.
    static Key shortPressureButton;
    /// ���� ��������� ������� ������� ������, �� ����� �������� ��� ���� ������ �� ��������� ����� �������.
    static Key longPressureButton;
    /// ��� ������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� �������� ������� ������� ������.
    static Key pressButton;
    /// ��� ���������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� ��������� ������� ���������� ������.
    static Key releaseButton;
    /// ��� ������� ����� �������� ���� ��� ����� Menu::Update().
    static pFuncVV funcAterUpdate;
    /// ����� ���������� ������� ������. ����� ��� ����, ����� ������������ ��������� ���������
    static uint timeLastPressedButton;
    /// �������� ����������, ����������� � ������ ������ ��������������� �������
    static Control *itemUnderButton[Key::Number];
    /// ��� ������� ����� ���������� ������ ����
    static pFuncVV funcUpdate;

public:

    class Title
    {
    public:
        static const int HEIGHT = 9;
        static const int WIDTH = 320 / 5 * 2 - 2;
    };

    class Item
    {
    public:
        static const int HEIGHT = 23;
        static const int WIDTH = 320 / 5;
        static const int WIDTH_OPENED = WIDTH;

        class Value
        {
        public:
            static const int HEIGHT = 13;
            static const int WIDTH = Menu::Item::WIDTH - 4;
        };
    };

    /// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
    struct AutoHide
    {
        enum E
        {
            None = 0,   ///< �������.
            _5 = 5,   ///< ����� 5 ������.
            _10 = 10,  ///< ����� 10 ������.
            _15 = 15,  ///< ����� 15 ������.
            _30 = 30,  ///< ����� 30 ������.
            _60 = 60   ///< ����� 60 ������.
        } value;
        operator uint8() const { return (uint8)value; };
    };
};


/** @}
 */
