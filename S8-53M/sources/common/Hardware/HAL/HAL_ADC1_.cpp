// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Settings/SettingsTypes_.h"
#include "Settings/Settings.h"
#include <stm32f4xx_hal.h>


namespace HAL_ADC1
{
    static void StartConversion();

    static Uint16 adc_value = Uint16(0);

    static ADC_HandleTypeDef handle;
}


void HAL_ADC1::Init()
{
    adc_value.SetInvalid();

    HAL_PINS::ADC1_::Init();

    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
    
    handle.Instance = ADC1;
    handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handle.Init.Resolution = ADC_RESOLUTION12b;
    handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handle.Init.ScanConvMode = DISABLE;
    handle.Init.EOCSelection = ENABLE;
    handle.Init.ContinuousConvMode = DISABLE;
    handle.Init.DMAContinuousRequests = DISABLE;
    handle.Init.NbrOfConversion = 1;
    handle.Init.DiscontinuousConvMode = DISABLE;
    handle.Init.NbrOfDiscConversion = 0;
    handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    handle.Init.ExternalTrigConv = ADC_EXTERNALTRIGINJECCONV_T1_CC4;

    if (HAL_ADC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    ADC_ChannelConfTypeDef sConfig;

    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&handle, &sConfig) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 1);         // �� ����� ���������� ����� ��������� �������� �������������
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}


Uint16 HAL_ADC1::GetValue()
{
    if (__HAL_ADC_GET_FLAG(&handle, ADC_FLAG_EOC))
    {
        __HAL_ADC_CLEAR_FLAG(&handle, ADC_FLAG_EOC);

        adc_value = Uint16((uint16)handle.Instance->DR);
    }

    return adc_value;
}


void HAL_ADC1::ResetValue()
{
    adc_value.SetInvalid();
}


void HAL_ADC1::StartConversion()
{
    if ((handle.Instance->CR2 & ADC_CR2_ADON) != ADC_CR2_ADON)
    {
        __HAL_ADC_ENABLE(&handle);

        __IO uint counter = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000U));

        while (counter != 0U)
        {
            counter--;
        }
    }

    ADC_STATE_CLR_SET(handle.State,
        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR, HAL_ADC_STATE_REG_BUSY);

    ADC_CLEAR_ERRORCODE(&handle);

    __HAL_ADC_CLEAR_FLAG(&handle, ADC_FLAG_EOC | ADC_FLAG_OVR);

    if ((handle.Instance->CR2 & ADC_CR2_EXTEN) == RESET)
    {
        handle.Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
    }
}


bool HAL_ADC1::CallbackOnIRQ()
{
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);

        if (TBASE_IS_RAND)
        {
            StartConversion();
        }

        return true;
    }

    return false;
}
