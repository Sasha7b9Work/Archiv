#pragma once
#include "Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
public:

    Keyboard();

    void Init();

    bool BufferIsEmpty();

    StructControl GetNextControl();

    void Update();
    /// Количество выводов RL для опроса клавиатуры
    static const int NUM_RL = 6;
    /// Количество выводов SL для опроса клавиатуры
    static const int NUM_SL = 8;
    /// Возращает имя органа управления
    const char *ControlName(Control control);

private:

    void FillCommand(Control control, TypePress typePress);

    uint TimeBetweenRepeats();

    StructControl commands[10];

    int pointer;
    /// При обнаружении нажатия кнопки сюда записывается время нажатия
    uint timePress[NUM_RL][NUM_SL];

    bool init;
};


extern Keyboard keyboard;
