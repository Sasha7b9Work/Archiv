// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/Buffer_.h"
#include "FPGA/FreqMeter.h"
#include "FPGA/TypesFPGA.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include "FPGA/StructuresFPGA.h"


namespace FPGA
{
    extern FlagFPGA flag;

    void Init();

    // Установить количество считываемых сигналов в секунду.
    void SetNumSignalsInSec(int num_sig_in_sec);

    void Update();

    // Запуск процесса сбора информации.
    void Start();

    // Прерывает процесс сбора информации.
    void Stop();

    // Нажатие на кнопку ПУСК/СТОП
    void OnPressStartStop();

    // Возвращает true, если прибор находится не в процессе сбора информации.
    bool IsRunning();

    // Запустить процесс поиска сигнала.
    void StartAutoFind();

    // Установить временную паузу после изменения ручек - чтобы смещённый сигнал зафиксировать на некоторое время
    void SetTemporaryPause();

    // Загрузить настройки в аппаратную часть из глобальной структуры SSettings.
    void LoadSettings();

    void LoadRegUPR();
};
