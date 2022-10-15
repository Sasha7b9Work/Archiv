#pragma once

#define KORNET

#include <stm32f4xx.h>
#include "commonDefines.h"

enum Color
{
    COLOR_BACK = 0x00,
    COLOR_FILL = 0x01
};


#define TIME_UPDATE_KEYBOARD 2   ///< Время между опросами клавиатуры
