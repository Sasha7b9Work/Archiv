// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "common/Utils/Containers/Buffer_.h"
#include "common/Utils/Containers/Queue_.h"
#include "common/Utils/Containers/Values_.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/Painter/DisplayPrimitives.h"
#include "FPGA/FPGA.h"
#include "FPGA/ReaderFPGA.h"
#include "Menu/Pages/Definition.h"
#include "Panel/Panel.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>


namespace Calibrator
{
    static void ReadPoints1024(uint8 buffer[1024], const Chan &ch);

    // Читает 1024 точки и возвращает их среднее значение
    static float ReadPoints1024ave(const Chan &ch);

    // Читает 1024 точки и возвращает минимальное и максимальное значения
    static void ReadPoints1024min_max(const Chan &ch, float *min, float *max);

    static void FuncDraw();

    static void DrawParametersChannel(const Chan &ch, int x, int y);

    // Заполнить буфер значениями смещения
    static void FillBuffer(Buffer<int16> &buffer, int16 *first);

    static uint timeStart = 0;

    namespace Balancer
    {
        static void CalibrateAddRShiftGND(const Chan &ch);
        static int16 CalculateAddRShift(float ave);
    }

    namespace Stretcher
    {
        // Рассчитать коэффициент растяжки исходя из измеренных min и max
        static float Calculate(float min, float max);
    }

    struct StateCalibration {
        enum E {
            None,
            NeedChannel1,       // Приглашение к калибровке 1-го канала
            NeedChannel2,       // Приглашение к калибровке 2-го канала
            ProcessChannel1,    // Идёт калибровка первого канала
            ProcessChannel2,    // Идёт калибровка второго канала
            CompleteNormal,     // Нормальное завершение калибровки
            CompleteFail        // Калибровка не прошла
        };
    };


    static StateCalibration::E stateCalibration = StateCalibration::None;
}


void Calibrator::PerformCalibration()
{
    Panel::DisableInput();

    timeStart = TIMER_MS;

    bool channelAisOK = false;
    bool channelBisOK = false;

    Settings storedSettings = set;                          // Сохранить настройки

    stateCalibration = StateCalibration::None;

    Display::SetDrawMode(Display::DrawMode::Timer, Calibrator::FuncDraw);

    stateCalibration = StateCalibration::NeedChannel1;                              // Калибруем первый канал

    if (Panel::WaitPressingKey() != Key::Start)         { goto ExitCalibration; }

    stateCalibration = StateCalibration::ProcessChannel1;

    channelAisOK = CalibrationChannel(ChA);

    stateCalibration = StateCalibration::NeedChannel2;                              // Калибруем второй канал

    if (Panel::WaitPressingKey() != Key::Start)         { goto ExitCalibration; }

    stateCalibration = StateCalibration::ProcessChannel2;

    channelBisOK = CalibrationChannel(ChB);

    stateCalibration = (channelAisOK && channelBisOK) ? StateCalibration::CompleteNormal : StateCalibration::CompleteFail;

    Panel::WaitPressingKey();

ExitCalibration:

    set = storedSettings;                                                           // Восстановить настройки
    FPGA::Init();

    Display::SetDrawMode(Display::DrawMode::Default);

    Panel::EnableInput();
}


bool Calibrator::CalibrationChannel(const Chan &ch)
{
    /*
    * 1. Провести балансировку всех диапазонов.
    * 2. Провести растяжку диапазонов.
    */

    bool balance = Balancer::PerformNormal(ch);

    bool stretch = Stretcher::Perform(ch);

    return (balance && stretch);
}


void Calibrator::Balancer::PerformOnGround(const Chan &ch)
{
//    *КК -калибровочный коээфициент

    static const pchar messages[Chan::Count][Language::Count] =
    {
        {"Балансирую канал 1", "Balancing the channel 1"},
        {"Балансирую канал 2", "Balancing the channel 2"}
    };

    Panel::DisableInput();

    Settings storedSettings;

    storedSettings = set;
    Display::Message::Show(messages[0][ch], messages[1][ch]);           // Вывести сообщение о балансировке.

    CalibrateAddRShiftGND(ch);                                          // Произвести балансировку канала

    set = storedSettings;
    FPGA::LoadSettings();
    Display::Message::Hide();                                           // Убрать сообщение о балансировке

    Panel::EnableInput();
}


bool Calibrator::Balancer::PerformNormal(const Chan &ch)
{
    FPGA::Stop();

    RShift::Set(ch, RShift::ZERO);                                  // установить общие настройки
    TBase::Set(TBase::_200us);
    TShift::Set(0);
    TrigSource::Set(ch);
    TrigPolarity::Set(TrigPolarity::Front);
    TrigLev::Set(ch, TrigLev::ZERO);
    PageTime::SetPeakDetMode(PeakDetMode::Disable);

    PageService::PageCalibrator::SetCalibratorMode(CalibratorMode::GND);

    bool result = true;

    for (int range = 0; range < Range::Count; range++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            NRST_RSHIFT(ch, range, mode) = 0;
            Range::Set(ch, (Range::E)range);

            float ave = ReadPoints1024ave(ch);

            int16 addShift = CalculateAddRShift(ave);

            if (Math::Abs(addShift) < 40)
            {
                NRST_RSHIFT(ch, range, mode) = addShift;
            }
            else
            {
                result = false;
            }
        }
    }

    return result;
}


bool Calibrator::Stretcher::Perform(const Chan &ch)
{
    FPGA::Stop();

    ModeCouple::Set(ch, ModeCouple::AC);
    Range::Set(ch, Range::_500mV);
    RShift::Set(ch, RShift::ZERO);                                  // установить общие настройки
    TBase::Set(TBase::_200us);
    TShift::Set(0);
    TrigSource::Set(ch);
    TrigPolarity::Set(TrigPolarity::Front);
    TrigLev::Set(ch, TrigLev::ZERO);
    PageTime::SetPeakDetMode(PeakDetMode::Disable);

    PageService::PageCalibrator::SetCalibratorMode(CalibratorMode::Freq);

    bool result = true;

    float min = 0.0f;
    float max = 0.0f;

    NRST_STRETCH(ch) = 1.0f;

    ReadPoints1024min_max(ch, &min, &max);

    float stretch = Calculate(min, max);

    if (stretch > 0.8f && stretch < 1.2f)
    {
        NRST_STRETCH(ch) = stretch;
    }
    else
    {
        result = false;
    }

    return result;
}


void Calibrator::Balancer::CalibrateAddRShiftGND(const Chan &ch)
{
    FPGA::Stop();

    RShift::Set(ch, RShift::ZERO);                                  // установить общие настройки
    ModeCouple::Set(ch, ModeCouple::GND);
    TBase::Set(TBase::_200us);
    TShift::Set(0);
    TrigSource::Set(ch);
    TrigPolarity::Set(TrigPolarity::Front);
    TrigLev::Set(ch, TrigLev::ZERO);
    PageTime::SetPeakDetMode(PeakDetMode::Disable);

    for (int range = 2; range < Range::Count; range++)
    {
        NRST_RSHIFT(ch, range, ModeCouple::DC) = 0;
        NRST_RSHIFT(ch, range, ModeCouple::AC) = 0;

        Range::Set(ch, (Range::E)range);

        float ave = ReadPoints1024ave(ch);

        int16 addShift = CalculateAddRShift(ave);

        NRST_RSHIFT(ch, range, ModeCouple::DC) = addShift;
        NRST_RSHIFT(ch, range, ModeCouple::AC) = addShift;
    }
}


float Calibrator::ReadPoints1024ave(const Chan &ch)
{
    uint8 buffer[1024];

    ReadPoints1024(buffer, ch);

    return Buffer<uint8>::Sum(buffer, 1024) / 1024;
}


void Calibrator::ReadPoints1024min_max(const Chan &ch, float *min, float *max)
{
    uint8 buffer[1024];

    ReadPoints1024(buffer, ch);

    Queue<float> mins;
    Queue<float> maxs;

    for (int i = 0; i < 1024; i++)
    {
        if (buffer[i] > 200)
        {
            maxs.Push(buffer[i]);
        }
        else if (buffer[i] < 50)
        {
            mins.Push(buffer[i]);
        }
    }

    *min = 0.0f;

    for (int i = 0; i < mins.Size(); i++)
    {
        *min += (float)mins[(int)i];
    }

    *min = *min / mins.Size();

    *max = 0.0f;

    for (int i = 0; i < maxs.Size(); i++)
    {
        *max += (float)maxs[(int)i];
    }

    *max = *max / maxs.Size();
}


void Calibrator::ReadPoints1024(uint8 buffer[1024], const Chan &ch)
{
    HAL_TIM2::DelayMS((RANGE(ch) < 2) ? 500U : 100U);

    FPGA::Start();

    std::memset(buffer, 255, 1024);

    HAL_TIM2::DelayMS(8);

    uint16 fl = HAL_FMC::Read(RD_FL);

    while (_GET_BIT(fl, FL_PRED_READY) == 0) { fl = HAL_FMC::Read(RD_FL); }

    TrigPolarity::Switch();

    while (_GET_BIT(fl, FL_TRIG_READY) == 0) { fl = HAL_FMC::Read(RD_FL); }

    HAL_TIM2::DelayMS(8);

    while (_GET_BIT(fl, FL_DATA_READY) == 0) { fl = HAL_FMC::Read(RD_FL); }

    ReaderFPGA::ReadPoints(ch, buffer, &buffer[0] + 1024);
}


int16 Calibrator::Balancer::CalculateAddRShift(float ave)
{
    return (int16)((ValueFPGA::AVE - ave) * 2);
}


float Calibrator::Stretcher::Calculate(float min, float max)
{
    const float K = 200.0f;

    float delta = max - min;

    return K / delta;
}


void Calibrator::FuncDraw()
{
    Display::BeginFrame(Color::BLACK);

    Color::WHITE.SetAsCurrent();

    int y = 10;

    switch (stateCalibration)
    {
    case StateCalibration::None:
        break;

    case StateCalibration::NeedChannel1:
    case StateCalibration::NeedChannel2:

        Text(LANG_RU ? "Подключите ко входу канала %d выход калибратора и нажмите кнопку ПУСК/СТОП."
            " Нажмите любую другую кнопку для выхода из режима калибровки" :
            "Connect the output of the calibrator to the input of channel %d and press the START/STOP button."
            " Press any other button to exit calibration mode",
            stateCalibration == StateCalibration::NeedChannel1 ? 1 : 2).
            DrawInRect(50, y + 25, Display::WIDTH - 100, 100);

        break;

    case StateCalibration::ProcessChannel1:
    case StateCalibration::ProcessChannel2:
        {
            uint time = ((TIMER_MS - timeStart) / 50) % 100;

            if (time > 50)
            {
                time = (100 - time);
            }

            Text(LANG_RU ? "Выполняется калибровка канала %d" : "Channel %d is being calibrated",
                stateCalibration == StateCalibration::ProcessChannel1 ? 1 : 2).
                DrawInRect(50, y + 25, Display::WIDTH - 100, 100);

            char buf[100] = { 0 };

            for (uint i = 0; i < time; i++)
            {
                std::strcat(buf, ".");
            }

            Text(buf).DrawInCenterRect(50, y + 25, Display::WIDTH - 100, 100);
        }

        break;

    case StateCalibration::CompleteNormal:
    case StateCalibration::CompleteFail:

        if (stateCalibration == StateCalibration::CompleteNormal)
        {
            Text(LANG_RU ? "Калибровка завершена успешно" :
                "Calibration completed successfully").Draw(50, 50);
        }
        else
        {
            Text(LANG_RU ? "ОШИБКА !!! Калибровка завершена неуспешно" :
                "ERROR !!! Calibration completed unsuccessfully").Draw(50, 50);
        }

        DrawParametersChannel(ChA, 10, 100);

        DrawParametersChannel(ChB, 10, 130);

        break;
    }

    Console::Draw();

    Display::EndFrame();
}


void Calibrator::DrawParametersChannel(const Chan &ch, int x, int y)
{
    Text(LANG_RU ? "Канал %d" : "Channel %d", (int)ch.value + 1).Draw(x, y);

    Buffer<int16> buffer(Range::Count);

    FillBuffer(buffer, &NRST_RSHIFT(ch, 0, 0));

    Text(buffer.ToString().c_str()).Draw(x + 40, y);
    
    Text("%f", NRST_STRETCH(ch)).Draw(x + 250, y);

    FillBuffer(buffer, &NRST_RSHIFT(ch, 0, 1));

    Text(buffer.ToString().c_str()).Draw(x + 40, y + 10);
}


void Calibrator::FillBuffer(Buffer<int16> &buffer, int16 *first)
{
    for (int i = 0; i < Range::Count; i++)
    {
        buffer[i] = *first;
        first += 2;
    }
}
