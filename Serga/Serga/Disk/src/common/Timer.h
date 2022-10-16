// 18.02.2009
#pragma once
#include <Windows.h>
#include "Time.h"

class Timer
{
public:
    Timer();
    void DelayUs(double us);    // ����� ���� ������� �������� �������� �� �������� �������� ����������� ����� ����������� ������ ���� �������
    void Start(double us);      // ����� ��������� ������ �� us ����������� 
    bool IsRun();               // � ����� ���������, �������� �� �� ��� ��� ��� �����
    void Reset();               // ���������� ��������� ������� GetTime() � ����
    Time GetTime();             // ���������� ����� �� ���������� ������ Reset();
    LONGLONG delta;
    LONGLONG startTimerInTicks;    // ����� ������� ������� � �����
    LONGLONG timeTimerInTicks;     // ���������� �����, ����� ������� ������ ������ ��������� ������
    LONGLONG currentTicks;
private:
    LARGE_INTEGER counter;
    double ticksInUs;
    LONGLONG timeOfResetInTicks;   // ����� � �����, ����� ���������� ��������� �����
};
