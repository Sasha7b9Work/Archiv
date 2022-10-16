// 18.02.2009
#pragma once
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
    void Pause();               // ���������������� ������. ��������� �� Reset, GetTime;
    void Release();             // ��������� ������ ����� Pause
    LONGLONG delta;
    LONGLONG startTimerInTicks;     // ����� ������� ������� � �����
    LONGLONG timeTimerInTicks;      // ���������� �����, ����� ������� ������ ������ ��������� ������
    LONGLONG currentTicks;
private:
    LARGE_INTEGER counter;
    double ticksInUs;
    LONGLONG timeOfResetInTicks;    // ����� � �����, ����� ���������� ��������� �����
    bool isPause;
    LONGLONG timeEnterPauseInTicks; // ������� ����� ���� �� ������ ��������� �����
};
