// 2021/03/01 10:48:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


static DAC_HandleTypeDef handle = { DAC };


void HAL_DAC1::Init()
{
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;      // ???????? ???

    if (HAL_DAC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);

    if (HAL_DAC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    DAC_ChannelConfTypeDef configDAC;
    configDAC.DAC_Trigger = DAC_TRIGGER_NONE;
    configDAC.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;

    if (HAL_DAC_ConfigChannel(&handle, &configDAC, DAC1_CHANNEL_1) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_DAC_Start(&handle, DAC1_CHANNEL_1);

    SetValue(100);
}


void HAL_DAC1::SetValue(int value)
{
    HAL_DAC_SetValue(&handle, DAC1_CHANNEL_1, DAC_ALIGN_8B_R, (uint)(value));
}
