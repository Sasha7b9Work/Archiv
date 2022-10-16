// 18.02.2009
#include "defines.h"
#include "Timer.h"

//#pragma optimize("", on)

//  [11/3/2009 Sasha]
Timer::Timer() {
    QueryPerformanceFrequency(&counter);
    LONGLONG quad = counter.QuadPart;
    ticksInUs = quad * 1e-6;
    Reset();
}

//  [11/4/2009 Sasha]
void Timer::DelayUs(double us) {
    static LONGLONG prevTicks = 0;
    QueryPerformanceCounter(&counter);
    LONGLONG ticks = (LONGLONG)(us * ticksInUs);                // Сколько тиков нужно пропустить
    int temp = 0;
    while ((counter.QuadPart - prevTicks) < ticks){
        QueryPerformanceCounter(&counter);
        temp++;
    }
    prevTicks = counter.QuadPart;
}

//  [11/4/2009 Sasha]
void Timer::Start(double us) {
    QueryPerformanceCounter(&counter);
    startTimerInTicks = counter.QuadPart;
    timeTimerInTicks = (LONGLONG)(us * ticksInUs);
}

//  [11/4/2009 Sasha]
bool Timer::IsRun() {
    QueryPerformanceCounter(&counter);
    currentTicks = counter.QuadPart;
    delta = currentTicks - startTimerInTicks;
    return (delta < timeTimerInTicks);
}

//  [12/11/2009 Sasha]
void Timer::Reset() {
    QueryPerformanceCounter(&counter);
    timeOfResetInTicks = counter.QuadPart;
    isPause = false;
}

//  [11/6/2009 Sasha]
Time Timer::GetTime() {
    if(isPause) {
        return Time((long long)((timeEnterPauseInTicks - timeOfResetInTicks) / ticksInUs));
    }
    QueryPerformanceCounter(&counter);
    long long currentTimeInTicks = counter.QuadPart;
    return Time((long long)((currentTimeInTicks - timeOfResetInTicks) / ticksInUs));
}

//  [4/30/2010 Sasha]
void Timer::Pause() {
    isPause = true;
    QueryPerformanceCounter(&counter);
    timeEnterPauseInTicks = counter.QuadPart;
}

//  [4/30/2010 Sasha]
void Timer::Release() {
    isPause = false;
    QueryPerformanceCounter(&counter);
    timeOfResetInTicks -= (timeEnterPauseInTicks - counter.QuadPart);
}
