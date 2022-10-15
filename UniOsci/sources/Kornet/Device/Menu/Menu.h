#pragma once
#include "MenuControls.h"
#include "Keyboard/Buttons.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Menu
{
public:
    Menu();
    void Init();
    void Update();
    void ButtonPress(Key button, TypePress typePress);
    void Draw();
    void SaveSettings();

private:
    void OnPressNone();
    void OnPressRangeLessA();
    void OnPressRangeMoreA();
    void OnPressRangeLessB();
    void OnPressRangeMoreB();
    void OnPressTBaseMore();
    void OnPressTBaseLess();
    void OnPressRShiftLessA();
    void OnPressRShiftMoreA();
    void OnPressRShiftLessB();
    void OnPressRShiftMoreB();
    void OnPressTShiftLess();
    void OnPressTShiftMore();
    void OnPressTrigLevLess();
    void OnPressTrigLevMore();
    void OnPressChannelA();
    void OnPressChannelB();
    void OnPressF1();
    void OnPressF2();
    void OnPressF3();
    void OnPressF4();
    void OnPressF5();
    void OnPressService();
    void OnPressStartStop();
    void OnPressTrig();
    void OnPressMenu();
    void OnPressFunction();
    void OnPressEnter();

    void VerifyOnDoubleClick(int src, int dir);

    /// \tode ����� �������� ����� ����������� ������� ������ ��� ������ �������� �������.
    /// 1 ������ 1�����/2�����/�������������/�����, 2 ������ - ����/�����
    uint timePrevPress[4][2];
    /// ����� �������� �������
    const uint timeDoubleClick;
    /// ����� �������� ��� ������� ����� ��� ������� ButtonPress
    TypePress typePress;
    /// ������� �������������� ������
    Key button;
    /// ���� true, �� ������ ������
    bool isPressed[NumButtons];
    /// ����� ���������� ������� ������. ����� ��� ����, ����� ������������ ��������� ���������
    uint timeLastPressedButton;
    /// ���������� ���� � ������������ � ������� �������
    void TuneOnDevice();
};


extern Menu menu;
