// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Painter/Text_.h"
#include "common/Panel/Controls_.h"
#include "common/Utils/Containers/String_.h"
#include "Menu/Items/MenuItems.h"


namespace Menu
{
    void Init();

    void Draw();

    // ������� ������ ���������� � ������� �����.
    void Update();

    // ���������� ����� �������� ����, ���������������� ������ key.
    Item *ItemUnderKey(Key::E key);

    // ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    void SetAutoHide(bool active);

    // ���������� ����� ������ ����, ������������ ��� ������� � ������ ������ �������.
    Item *ItemUnderKey();

    // ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    Text StringNavigation();

    void OpenItemTime();

    // ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������
    // ���������.
    const Item *CurrentItem();

    NamePage::E GetNameOpenedPage();

    // ������� �������� ������� ����.
    void CloseOpenedItem();

    // ������ ����
    void Hide();

    // �������� ����
    void Show();

    // �������� "������������" - ������ ���� ������� � ��������
    void ChangeShowing();

    // ���� true - ���� ��������� � �������������� ������.
    bool IsMinimize();

    // ��������� ����� ��������� �� ������� �������� ����� ������.
    void CurrentPageSBregSet(int angle);

    void DrawStringNavigation();

    // true, ���� ����� �������� ������ ��������� ����
    bool NeedDrawStringNavigation();

    extern bool showHelpHints;      // ���� 1, �� ��� ������� ������ ������ ���������� � ������ ��������� ��������� �
                                    // � ����������
    extern bool needClosePageSB;    // ���� 1, ����� ��������� �������� ����� ������

    extern Item* itemsForButtons[6];    // ����� ����� ��������� ���� ����������� ��������� �� ������������ �����
                                        // ��� ������ MENU (0), F1-F5 (1-5)
                                        // ����������� ����� ������ ��������� ����

    namespace Handlers
    {
        // ������� ��������� ��������� ������� ������ (����� 0.5 ���.).
        void ShortPressureKey(Key::E key);

        // ��������� �������� ������� ������.
        void LongPressureKey(Key::E key);

        // ��������� ��������� ������ ����.
        void PressKey(Key::E key);

        // ��������� �������� ������ �����.
        void ReleaseKey(Key::E key);

        // ��������� �������� ����� ���������.
        void RegulatorSet(Action::E action);

        void KeyForHint(Key::E key);
    };
};
