// 2022/01/31 12:01:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Panel/Panel.h"
#include "common/Utils/Containers/Buffer_.h"
#include "Settings/Settings.h"
#include "FPGA/FreqMeter.h"


struct StateWorkFPGA {
    enum E {
        Stop,    // СТОП - не занимается считыванием информации.
        Wait,    // Ждёт поступления синхроимпульса.
        Work     // Идёт работа.
    };

    static E value;

    static bool IsStop() { return value == Stop; }
    static bool IsWait() { return value == Wait; }
    static bool IsWork() { return value == Work; }

    static void SetWait() { value = Wait; }
    static void SetWork() { value = Work; }
    static void SetStop() { value = Stop; }
};



namespace SyncPulse
{
    void Update(uint flag);

    extern bool is_fired;       // Если true - нужно отображать

    extern uint time_fire;      // Время последнего "поджига"
}


namespace Randomizer
{
    // Установить количество измерений, по которым будут рассчитываться ворота в режиме рандомизатора.
    void SetNumberMeasuresForGates(int number);

    bool CalculateGate(uint16 rand, uint16 *min, uint16 *max);
};


namespace Calibrator
{
    // Запуск функции полной калибровки - смещение и растяжка
    void PerformCalibration();

    bool CalibrationChannel(const Chan &ch);

    namespace Balancer
    {
        // Запуск балансировки из меню КАНАЛ
        void PerformOnGround(const Chan &ch);

        // Запуск балансировки для AC и DC (из процедуры калибровки)
        bool PerformNormal(const Chan &ch);
    };

    namespace Stretcher
    {
        bool Perform(const Chan &ch);
    };
};


// Структура для управления пред/после- запусками
namespace LaunchFPGA
{
    // Загружает в аппаратную часть пред- и после- запуски
    void Load();

    uint16 Pred();

    // Возвращают значения, готовые для записи в ПЛИС
    uint16 PredForWrite();
    uint16 PostForWrite();

    // На сколько менять адрес первого чтения
    int DeltaReadAddress();

    // Дополнительное смещение для первой считываемой точки (нужно для того, чтобы учесть, что в режиме рандомизатора
    // изменение tShift вызывает перемещение по экрану не на 1 пиксель)
    int AdditionalOffsetIndexFirst();
};


namespace BUS_FPGA
{
    void Write(uint16 *const address, uint16 data, bool restart = true);

    // Эта функция используется тогда, когда запись в регистр не требует перезапуска процесса измерения. Например,
    // в регистр, управляющий режимом калибратора
    void WriteWithoutStart(uint16 *const address, uint16 data);

    void WriteToDAC(TypeWriteDAC::E type, uint16 data);

    void WriteToAnalog(TypeWriteAnalog::E type, uint data);

    // Загрузить все параметры напряжения каналов и синхронизации в аппаратную часть.
    void SetAttribChannelsAndTrig(TypeWriteAnalog::E type);
};


struct FlagFPGA
{
    FlagFPGA() : flag(0), time_pred_launch_ready(0) {}

    inline bool IsTrigReady()       const { return _GET_BIT(flag, FL_TRIG_READY) == 1; }
    inline bool IsDataReady()       const { return _GET_BIT(flag, FL_DATA_READY) == 1; };
    inline bool IsPointReady()      const { return _GET_BIT(flag, FL_POINT_READY) == 1; };
    inline bool IsPredLaunchReady() const { return _GET_BIT(flag, FL_PRED_READY) == 1; }
    inline bool IsFirstByte0()      const { return _GET_BIT(flag, FL_FIRST_BYTE) == 0; }

    void Read()
    {
        flag = HAL_FMC::Read(RD_FL);

        // Рассчитываем время, когда был установлен флаг предзапуска

        if (_GET_BIT(flag, FL_PRED_READY) == 1)
        {
            if (time_pred_launch_ready == 0)
            {
                time_pred_launch_ready = TIMER_MS;
            }
        }
        else
        {
            time_pred_launch_ready = 0;
        }


        // Запустить отсчёт послезапуска, если нужно (если у нас включена автоматическая синхронизация, после отсчёта
        // предзапуска прошло много времени, а синхронизация так и не прошла, нужно давать принудительную синхронизацию
        if (START_MODE_IS_AUTO)
        {
            if ((_GET_BIT(flag, FL_PRED_READY) == 1) &&
                (_GET_BIT(flag, FL_TRIG_READY) == 0) &&
                (TIMER_MS - time_pred_launch_ready) > 1000)
            {
                static uint timePrev = 0;

                if (TIMER_MS - timePrev >= 100)
                {
                    timePrev = TIMER_MS;

                    TrigPolarity::Switch();
                }
            }
        }

        FreqMeter::Update(flag);

        SyncPulse::Update(flag);
    }

    uint16 flag;
    uint time_pred_launch_ready;     // Время, когда флаг PredReady установился. Если == 0, не установлен
};
