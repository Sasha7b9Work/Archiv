// 18.02.2009
#pragma once
#include "Time.h"

class Timer
{
public:
    Timer();
    void DelayUs(double us);    // Вызов этой функции вызывает задержку на заданное значение микросекунд после предыдущего вызова этой функции
    void Start(double us);      // Можно запустить таймер на us миллисекунд 
    bool IsRun();               // и потом проверять, работает ли он ещё или уже вышел
    void Reset();               // Сбрасывает показания функции GetTime() в ноль
    Time GetTime();             // Возвращает время от последнего вызова Reset();
    void Pause();               // Приостанавливает таймер. Действует на Reset, GetTime;
    void Release();             // Запускает таймер после Pause
    LONGLONG delta;
    LONGLONG startTimerInTicks;     // Время запуска таймера в тиках
    LONGLONG timeTimerInTicks;      // Количество тиков, через которое таймер должен закончить работу
    LONGLONG currentTicks;
private:
    LARGE_INTEGER counter;
    double ticksInUs;
    LONGLONG timeOfResetInTicks;    // Время в тиках, когда выполнялся последний резет
    bool isPause;
    LONGLONG timeEnterPauseInTicks; // Сколько тиков было на момент установки паузы
};
