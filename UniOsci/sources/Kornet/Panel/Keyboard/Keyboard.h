#pragma once
#include "Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
public:

    Keyboard();

    void Init();

    bool BufferIsEmpty();

    StructControl GetNextControl();

    void Update();
    /// ���������� ������� RL ��� ������ ����������
    static const int NUM_RL = 6;
    /// ���������� ������� SL ��� ������ ����������
    static const int NUM_SL = 8;
    /// ��������� ��� ������ ����������
    const char *ControlName(Control control);

private:

    void FillCommand(Control control, TypePress typePress);

    uint TimeBetweenRepeats();

    StructControl commands[10];

    int pointer;
    /// ��� ����������� ������� ������ ���� ������������ ����� �������
    uint timePress[NUM_RL][NUM_SL];

    bool init;
};


extern Keyboard keyboard;
