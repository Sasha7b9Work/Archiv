//  [4/29/2010 Sasha]
#pragma once
#include "defines.h"
#include "Timer.h"

// Типы сигналов, посылаемых основной программе
enum TypeControlSignal {
    TCS_SetValues,        // Установить значение ручек
	TCS_Stop
};

class ProgrammingThread : public QThread {
    Q_OBJECT
public:
    ProgrammingThread(QObject *parent = 0);
    ~ProgrammingThread();

    void StartProgram();                    // Запуск программы
    void PauseProgram(bool pause);          // Постановка на паузу
    void StopProgram();                     // Остановка программы
	Time GetTime();							// Возвращает текущее время работы программы

protected:
    void run();

signals:
    // Сигнал управления. Посылает главной программе 
    // numRow - какую строку подсвечивать
    void ControlSignal(/* TypeControlSignal */ int type, int voltageRel, int amperageRel, int numRow);  

private slots:
    void OnTimer();

private:
    Timer timer;        // Таймер отсчёта времени
    QTimer *clockTimer;  // Это основной таймер
    int currentStep;    // Текущий шаг программы (индекс вектора settingsProg)
};