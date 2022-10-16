// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/Warnings.h"
#include "Display/Screen/BottomPart.h"
#include "Display/Screen/TopPart.h"
#include "Display/Painter/PainterData.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Data/ProcessingData.h"
#include "common/Utils/Containers/Buffer_.h"
#include "FPGA/MathFPGA.h"
#include "Data/InputStorage/InputStorage.h"
#include <cstdio>
#include <cstring>


namespace Display
{
    static void FuncOnTimerDisableShowLevelTrigLev();

    static void FuncOnTimerRShiftMarkersAutoHide();

    static bool NeedForClearScreen();

    static void DrawTimeForFrame(uint timeMS);

    // Возвращает true, если надо обновлять дисплей
    static bool NeedUpdate();

    pFuncVV funcDrawTimer = nullptr;        // Эта функция будет отрисовываться в режиме DrawMode::Timer
    static bool needFinishDraw = true;      // Если 1, то дисплей нуждается в перерисовке
    static uint numDrawingSignals = 0;      // Число нарисованных сигналов для режима накопления

    namespace Message
    {
        static void FuncDraw();

        static pchar text = nullptr;    // Выводимое сообщение
        static uint timeStart = 0;      // Время начала вывода сообщения
    }

    namespace Sender
    {
        static bool needSendPalette = false;
        static bool needSendFrame = false;
    }

    namespace Math
    {
        static void Draw();

        static bool NeedDraw();
    }
}


void Display::RotateTrigLev()
{
    if (TIME_SHOW_LEVELS)
    {
        TrigLev::show_level = true;
        Timer::Enable(TypeTimer::ShowLevelTrigLev, TIME_SHOW_LEVELS * 1000,
            FuncOnTimerDisableShowLevelTrigLev);
    }

    Redraw();
}


void Display::FuncOnTimerDisableShowLevelTrigLev()
{
    DisableShowLevelTrigLev();
}


void Display::Redraw()
{
    needFinishDraw = true;
}


bool Display::NeedForClearScreen()
{
    if (TBASE_IS_RAND ||
        ENUM_ACCUM == 1 ||
        MODE_ACCUM_IS_NO_RESET ||
        PageTime::InSelfRecoredMode())
    {
        return true;
    }

    if (needFinishDraw)
    {
        needFinishDraw = false;
        return true;
    }

    if (MODE_ACCUM_IS_RESET && (numDrawingSignals >= (uint)ENUM_ACCUM))
    {
        numDrawingSignals = 0;
        return true;
    }
    return false;
}


bool Display::NeedUpdate()
{
    if (funcDrawTimer != nullptr)
    {
        return false;
    }

    static uint prevTime = 0;       // Время предыдущей отрисовки

    if (TIMER_MS - prevTime >= ENumSignalsInSec::DeltaTime(ENUM_FPS))
    {
        prevTime = TIMER_MS;

        return true;
    }

    return false;
}


void Display::Update()
{
    if (!NeedUpdate())
    {
        return;
    }

    bool need_clear = NeedForClearScreen();

    if (need_clear)
    {
        BeginFrame(Color::BACK);
        Grid::Draw_Fill();
        RShift::Draw();
    }

    PainterData::DrawData();
    Math::Draw();
    Grid::Draw_Border();

    if (need_clear)
    {
        Cursors::Draw();
        TopPart::Draw();
        BottomPart::Draw();
        TrigLev::Draw();
        Menu::DrawStringNavigation();
        TPos::Draw();
        TShift::Draw();
    }

    Menu::Draw();

    if (need_clear)
    {
        Warnings::Draw();
    }

    Console::Draw();

    if (need_clear)
    {
        TrigLev::WriteValue();
    }

    EndFrame();

    Sender::Update();
}


void Display::DrawTimeForFrame(uint timeTicks)
{
    if(!setNRST.show_stats)
    {
        return;
    }
    static char buffer[10];
    static bool first = true;
    static uint timeMSstartCalculation = 0;
    static int numFrames = 0;
    static float numMS = 0.0F;
    if(first)
    {
        timeMSstartCalculation = TIMER_MS;
        first = false;
    }
    numMS += timeTicks / 120000.0F;
    numFrames++;
    
    if((TIMER_MS - timeMSstartCalculation) >= 500)
    {
        std::sprintf(buffer, "%.1fms/%d", numMS / numFrames, numFrames * 2);
        timeMSstartCalculation = TIMER_MS;
        numMS = 0.0F;
        numFrames = 0;
    }

    Rectangle(84, 10).Draw(Grid::Left(), Grid::FullBottom() - 10, Color::FILL);
    Region(82, 8).Fill(Grid::Left() + 1, Grid::FullBottom() - 9, Color::BACK);
    Text(buffer).Draw(Grid::Left() + 2, Grid::FullBottom() - 9, Color::FILL);

//    std::sprintf(message, "%d", Storage::NumElementsWithSameSettings());

    Text message("1");

//    char numAvail[10] = {0};
//    std::sprintf(numAvail, "%d", Storage::NumberAvailableEntries());
//    std::strcat(message, numAvail);
    message.Draw(Grid::Left() + 50, Grid::FullBottom() - 9);
}


void Display::DisableShowLevelTrigLev()
{
    TrigLev::show_level = false;
    Timer::Disable(TypeTimer::ShowLevelTrigLev);
}


void Display::SetDrawMode(DrawMode::E mode, pFuncVV func)
{
    funcDrawTimer = func;

    if (mode == DrawMode::Timer)
    {
        Timer::Enable(TypeTimer::DisplayUpdate, 100, funcDrawTimer);
    }
    else if (mode == DrawMode::Default)
    {
        funcDrawTimer = nullptr;

        Timer::Disable(TypeTimer::DisplayUpdate);
    }
}


void Display::Clear()
{
    Region(WIDTH - 1, HEIGHT - 2).Fill(0, 0, Color::BACK);
}


void Display::ShiftScreen(int delta)
{
    SHIFT_IN_MEMORY_IN_POINTS = ::Math::Limitation<int16>(
        (int16)(SHIFT_IN_MEMORY_IN_POINTS + delta),
        0,
        (int16)(EnumPointsFPGA::PointsInChannel() - 282)
        );
}


void Display::ChangedRShiftMarkers()
{
    Timer::Enable(TypeTimer::RShiftMarkersAutoHide, 5000, FuncOnTimerRShiftMarkersAutoHide);
}

void Display::FuncOnTimerRShiftMarkersAutoHide()
{
    OnRShiftMarkersAutoHide();
}


void Display::OnRShiftMarkersAutoHide()
{
    Timer::Disable(TypeTimer::RShiftMarkersAutoHide);
}


void Display::Message::Show(pchar textRu, pchar textEn)
{
    text = LANG_RU ? textRu : textEn;

    timeStart = TIMER_MS;

    Display::SetDrawMode(DrawMode::Timer, FuncDraw);
}


void Display::Message::Hide()
{
    Display::SetDrawMode(DrawMode::Default);
}


void Display::Message::FuncDraw()
{
    uint time = ((TIMER_MS - timeStart) / 50) % 100;

    if (time > 50)
    {
        time = (100 - time);
    }

    int width = 200;
    int height = 80;
    int x = (Display::WIDTH - width) / 2;
    int y = (Display::HEIGHT - height) / 2;

    Region(width, height).Fill(x, y, Color::BACK);
    Rectangle(width, height).Draw(x, y, Color::FILL);

    int length = Font::GetLengthText(text);

    if (length < width)
    {
        Text(text).DrawInCenterRect(x, y, width, height - 20);
    }
    else
    {
        Text(text).DrawInRectWithTransfers(x + 11, y + 20, width - 22, height - 20);
    }

    char buf[100] = { 0 };

    for (uint i = 0; i < time; i++)
    {
        std::strcat(buf, ".");
    }

    Text(buf).DrawInCenterRect(x, y + 20, width, height - 20);

    Display::EndFrame();
}


void Display::Sender::Update()
{
    if (needSendPalette)
    {
#ifdef DEVICE

        for (uint8 i = 0; i < Color::Count; i++)
        {
            uint8 command[4];
            command[0] = 9;     // Команда установки значения цвета
            *(command + 1) = i;
            *((uint16 *)(&command[0] + 2)) = (uint16)COLOR(i);

            SCPI::SendBuffer(command, 4);
        }

#endif

        needSendPalette = false;
    }

    if (needSendFrame)
    {
        needSendFrame = false;
    }
}


void Display::Sender::SendPalette()
{
    needSendPalette = true;
}


void Display::Sender::SendFrame()
{
    needSendFrame = true;
}


static bool Display::Math::NeedDraw()
{
    if (MODE_DRAW_MATH_IS_DISABLED)
    {
        return false;
    }

    ReadingFrame data;

    if (!InputStorage::ExtractLastFrame(data))
    {
        return false;
    }

    if (data.Data(ChA) == nullptr || data.Data(ChB) == nullptr)
    {
        return false;
    }

    return true;
}


static void Display::Math::Draw()
{
    if (!NeedDraw())
    {
        return;
    }

    uint8 *dataRel0 = 0;
    uint8 *dataRel1 = 0;
    DataSettings *ds = 0;

    Processing::GetData(&dataRel0, &dataRel1, &ds);

    BitSet64 p = SettingsDisplay::BytesOnDisplay();

    int num_bytes = p.second - p.first;

    Buffer<float> absA(num_bytes);
    Buffer<float> absB(num_bytes);

    MathFPGA::PointsRelToVoltage(dataRel0, num_bytes, ds->range[ChA], (int16)ds->r_shift_a, absA.Data());
    MathFPGA::PointsRelToVoltage(dataRel1, num_bytes, ds->range[ChB], (int16)ds->r_shift_b, absB.Data());

    Processing::CalculateMathFunction(absA.Data(), absB.Data(), num_bytes);

    Buffer<uint8> points(num_bytes);

    MathFPGA::PointsVoltageToRel(absA.Data(), num_bytes, set.math.range, set.math.rShift, points.Data());

    DrawingFrame frame(points);

    frame.DrawChannel(Chan::Math);

    static const int width = 71;
    static const int height = 10;
    int delta = (SHOW_STRING_NAVIGATION && MODE_DRAW_MATH_IS_TOGETHER) ? 10 : 0;

    Rectangle(width, height).Draw(Grid::Left(), Grid::Math::Top(), Color::FILL);
    Region(width - 2, height - 2).Fill(Grid::Left(), Grid::Math::Top(), Color::BACK);

    Divider::E multiplier = set.math.multiplier;

    Text(Range::ToString(set.math.range, multiplier)).Draw(Grid::Left() + 2, Grid::Math::Top() + 1 + delta, Color::FILL);
    Text(":").Draw(Grid::Left() + 25, Grid::Math::Top() + 1 + delta);
    Text(RShift::ToString(set.math.rShift, set.math.range, multiplier)).Draw(Grid::Left() + 27, Grid::Math::Top() + 1 + delta);
}
