#pragma once
#include "defines.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Tester
{
public:
    void Init();

    void Enable();

    void Disable();

    void Update();

    void ProcessStep();
    /// Загружает полярность из Settings
    void LoadPolarity();
    /// Устанавливает шаг изменения напряжения в соотвествии с настройками Settings
    void LoadStep();

private:
    /// Загрузить FPGA в соответствии с установленными настройками
    void LoadFPGA();
    /// Считать данные очередной ступеньки
    void ReadData();
    /// Запустить процесс чтения очередной ступеньки
    void StartFPGA();
    /// Текущий шаг
    int step;
    /// Шаг изменения напряжения
    float stepU;
};


extern Tester tester;

#define NUM_STEPS       5

extern uint8 dataTester[NumChannels][NUM_STEPS][TESTER_NUM_POINTS];
