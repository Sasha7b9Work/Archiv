#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Timer
{
public:

    void Init();

private:
};


extern Timer timer;

/// ��������� ������� ��� ��������� ����� �������� ������� (����� � �����������). �������� ������� ���-�� �� 30 ������.
#define START_MULTI_MEASUREMENT()   { TIM2->CR1 &= (uint)~TIM_CR1_CEN; TIM2->CNT = 0; TIM2->CR1 |= TIM_CR1_CEN; }
/// ���������� ��������� ������ � ���������� ������ START_MULTI_MEASUREMENT
#define gTimeTicks  (TIM2->CNT)
/// ���������� ��������� ����������� � ���������� ������� START_MULTI_MEASUREMENT
#define gTimeUS     (gTimeTicks / 90)
/// ���������� �����������, ��������� � ������ ������ ���������
#define gTimeMS     HAL_GetTick()

#define PAUSE_ON_TICKS(x)   { uint time = gTimeTicks; while(time + (x) > gTimeTicks) {}; }
#define PAUSE_ON_US(x)      { uint time = gTimeUS; while (gTimeUS - time < (x)) {}; }
#define PAUSE_ON_MS(x)      { uint time = gTimeMS; while (gTimeMS - time < (x)) {}; }
