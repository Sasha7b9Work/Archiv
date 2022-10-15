#include <stm32f4xx_hal.h>
#include "GPIO.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GPIO gpio;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO::SetInputPullDown(GPIO_TypeDef *reg, uint numPin)
{
    reg->MODER &= ~(3U << (numPin * 2));

    reg->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (numPin * 2U));
    reg->PUPDR |= GPIO_PULLDOWN << (numPin * 2U);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void GPIO::SetOutputPP_PullDown(GPIO_TypeDef *reg, uint numPin)
{
    reg->MODER &= ~(GPIO_MODER_MODER0 << (numPin * 2));
    reg->MODER |= (GPIO_MODE_OUTPUT_PP << (numPin * 2));

    reg->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (numPin * 2U));
    reg->PUPDR |= GPIO_PULLDOWN << (numPin * 2U);
}
