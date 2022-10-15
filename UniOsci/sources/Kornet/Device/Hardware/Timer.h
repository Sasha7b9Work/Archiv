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

/// Запускает счётчик для измерения малых отрезков времени (тиков и микросекунд). Счётчика хватает где-то на 30 секунд.
#define START_MULTI_MEASUREMENT()   { TIM2->CR1 &= (uint)~TIM_CR1_CEN; TIM2->CNT = 0; TIM2->CR1 |= TIM_CR1_CEN; }
/// Количество прошедших тактов с последнего вызова START_MULTI_MEASUREMENT
#define gTimeTicks  (TIM2->CNT)
/// Количество прошедших микросекунд с последнего вызоыва START_MULTI_MEASUREMENT
#define gTimeUS     (gTimeTicks / 90)
/// Количество миллисекунд, прошедших с начала работы программы
#define gTimeMS     HAL_GetTick()

#define PAUSE_ON_TICKS(x)   { uint time = gTimeTicks; while(time + (x) > gTimeTicks) {}; }
#define PAUSE_ON_US(x)      { uint time = gTimeUS; while (gTimeUS - time < (x)) {}; }
#define PAUSE_ON_MS(x)      { uint time = gTimeMS; while (gTimeMS - time < (x)) {}; }
