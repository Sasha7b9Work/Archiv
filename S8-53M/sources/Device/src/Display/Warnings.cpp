// 2021/03/30 13:42:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Display/Warnings.h"
#include "Display/Screen/Grid.h"
#include "Settings/Settings.h"


namespace Warnings
{
    static void DrawStringInRectangle(int x, int y, pchar text);

    static void ShowWarn(pchar message);

    static void OnTimerShowWarning();

    static const int NUM_WARNINGS = 10;

    static pchar warnings[NUM_WARNINGS] = {0};          // Здесь предупреждающие сообщения.
    static uint  timeWarnings[NUM_WARNINGS] = {0};      // Здесь время, когда предупреждающее сообщение
                                                        // поступило на экран.
}


void Warnings::Draw()
{
    int delta = 12;
    int y = BottomY();

    for (int i = 0; i < 10; i++)
    {
        if (warnings[i] != 0)
        {
            DrawStringInRectangle(Grid::Left(), y, warnings[i]);
            y -= delta;
        }
    }
}


void Warnings::ShowWarn(pchar message)
{
    if (warnings[0] == 0)
    {
        Timer::Enable(TypeTimer::ShowMessages, 100, OnTimerShowWarning);
    }
    bool alreadyStored = false;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] == 0 && !alreadyStored)
        {
            warnings[i] = message;
            timeWarnings[i] = TIMER_MS;
            alreadyStored = true;
        }
        else if (warnings[i] == message)
        {
            timeWarnings[i] = TIMER_MS;
            return;
        }
    }
}


void Warnings::OnTimerShowWarning()
{
    uint time = TIMER_MS;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (time - timeWarnings[i] > (uint)(TIME_MESSAGES * 1000))
        {
            timeWarnings[i] = 0;
            warnings[i] = 0;
        }
    }

    int pointer = 0;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] != 0)
        {
            warnings[pointer] = warnings[i];
            timeWarnings[pointer] = timeWarnings[i];
            if (pointer != i)
            {
                timeWarnings[i] = 0;
                warnings[i] = 0;
            }
            pointer++;
        }
    }

    if (pointer == 0)
    {
        Timer::Disable(TypeTimer::ShowMessages);
    }
}


void Warnings::ClearFromWarnings()
{
    Timer::Disable(TypeTimer::ShowMessages);

    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        warnings[i] = 0;
        timeWarnings[i] = 0;
    }
}


void Warnings::ShowWarningBad(pchar textRu, pchar textEn)
{
    Color::ResetFlash();
    ShowWarn(LANG_RU ? textRu : textEn);
    Sound::WarnBeepBad();
}


void Warnings::ShowWarningGood(pchar textRu, pchar textEn)
{
    Color::ResetFlash();
    ShowWarn(LANG_RU ? textRu : textEn);
    Sound::WarnBeepGood();
}


void Warnings::DrawStringInRectangle(int, int y, pchar text)
{
    int width = Font::GetLengthText(text);
    int height = 8;
    Rectangle(width + 5, height + 5).Draw(Grid::Left(), y, Color::FILL);
    Rectangle(width + 3, height + 3).Draw(Grid::Left() + 1, y + 1, Color::BACK);
    Region(width + 1, height + 1).Fill(Grid::Left() + 2, y + 2, Color::BLACK_WHITE_1);
    Text(text).Draw(Grid::Left() + 3, y + 2, Color::BLACK_WHITE_0);
}


int Warnings::BottomY()
{
    int result = Grid::FullBottom();

    if (set.memory.mode_work.IsMemInt())
    {
        result -= 13;
    }
    else if (SHOW_MEASURES)
    {
        result = Grid::Bottom();

        if (MEASURES_NUMBER_IS_6_1 ||
            MEASURES_NUMBER_IS_6_2 ||
            MEASURES_NUMBER_IS_1)
        {
            result = Grid::FullBottom();
        }
    }

    return result - 13;
}


void Warnings::ShowWarningBadLimitRange(const Chan &ch)
{
    ShowWarningBad(ch.IsA() ? "ПРЕДЕЛ КАНАЛ 1 - ВОЛЬТ/ДЕЛ" : "ПРЕДЕЛ КАНАЛ 2 - ВОЛЬТ/ДЕЛ",
                   ch.IsA() ? "LIMIT CHANNEL 1 - VOLTS/DIV" : "LIMIT CHANNEL 2 - VOLTS/DIV");
}


void Warnings::ShowWarningBadLimitRShift(const Chan &ch)
{
    ShowWarningBad(ch.IsA() ? "ПРЕДЕЛ КАНАЛ 1 - \x0d" : "ПРЕДЕЛ КАНАЛ 2 - \x0d",
                   ch.IsB() ? "LIMIT CHANNEL 1 - \x0d" : "LIMIT CHANNEL 2 - \x0d");
}
