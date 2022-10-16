// 18.02.2009
#include "defines.h"
#include "Timer.h"

//#pragma optimize("", on)

Timer::Timer() 
{
    QueryPerformanceFrequency(&counter);
    LONGLONG quad = counter.QuadPart;
    ticksInUs = quad * 1e-6;
    Reset();
}

void Timer::DelayUs(double us) 
{
    static LONGLONG prevTicks = 0;

    do 
    {
        QueryPerformanceCounter(&counter);
    } while ((counter.QuadPart - prevTicks) < (LONGLONG)(us * ticksInUs));

    prevTicks = counter.QuadPart;
}

void Timer::Start(double us) 
{
    QueryPerformanceCounter(&counter);
    startTimerInTicks = counter.QuadPart;
    timeTimerInTicks = (LONGLONG)(us * ticksInUs);
}

bool Timer::IsRun() 
{
    QueryPerformanceCounter(&counter);
    currentTicks = counter.QuadPart;
    delta = currentTicks - startTimerInTicks;
    return (delta < timeTimerInTicks);
}

void Timer::Reset() 
{
    QueryPerformanceCounter(&counter);
    timeOfResetInTicks = counter.QuadPart;
}

Time Timer::GetTime() 
{
    QueryPerformanceCounter(&counter);
    long long currentTimeInTicks = counter.QuadPart;
    double timeInUs = (currentTimeInTicks - timeOfResetInTicks) / ticksInUs;
    return Time((long long)timeInUs);
}