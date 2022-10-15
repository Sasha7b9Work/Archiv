#pragma once

enum Key
{
    K_None,
    K_Function,
    K_Measures,
    K_Memory,
    K_Service,
    K_ChannelA,
    K_ChannelB,
    K_Time,         //
    K_Start,
    K_Trig,         //
    K_Menu,
    K_RangeMoreA,
    K_RangeLessA,
    K_RShiftMoreA,
    K_RShiftLessA,
    K_RangeMoreB,
    K_RangeLessB,
    K_RShiftMoreB,
    K_RShiftLessB,
    K_TBaseMore,
    K_TBaseLess,
    K_TShiftMore,   //
    K_TShiftLess,   //
    K_TrigLevMore,     //
    K_TrigLevLess,
    K_Left,
    K_Right,
    K_Up,
    K_Down,
    K_Enter,
    K_1,
    K_2,
    K_3,
    K_4,
    K_5,
    NumButtons
};

enum TypePress
{
    Press,      ///< Нажатие кнопки
    Repeat,     ///< Автоматический повтор
    Release     ///< Отпускание кнопки
};
