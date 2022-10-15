#pragma once
#include "Controls.h"
#include "defines.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
public:

    static void Init();

    static void Update();
    /// ���������� ������� RL ��� ������ ����������
    static const int NUM_RL = 6;
    /// ���������� ������� SL ��� ������ ����������
    static const int NUM_SL = 8;
    /// ��������� ��� ������ ����������
    static const char *ControlName(Control control);

private:

    static void FillCommand(Control control, Control::Action::E action);

    static uint TimeBetweenRepeats(uint time);

    static Control commands[10];

    static int pointer;
    /// ��� ����������� ������� ������ ���� ������������ ����� �������
    static uint timePress[NUM_RL][NUM_SL];
    /// ������������� � true �������� ��������, ��� ��������� ����-���� ���������� � � ��������� ������ �� ���������� (����� �� ���� ������
    /// ������������ ���� Long
    static bool alreadyLong[NUM_RL][NUM_SL];

    static bool init;
};
