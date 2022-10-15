#include "defines.h"
#include "Hardware.h"
#include "FLASH.h"
#include "FSMC.h"
#include "Hardware/Timer.h"
#include "Display/Display.h"
#include "FlashDrive/FlashDrive.h"
#include "Panel/Panel.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static TIM_HandleTypeDef handleTIM6forTimer =
{
    TIM6,
    {
        119,                    // Init.Prescaler
        TIM_COUNTERMODE_UP,     // Init.CounterMode
        500,                    // Init.Period
        TIM_CLOCKDIVISION_DIV1  // Init.ClockDivision
    }
};

// ������ ��� �����
TIM_HandleTypeDef tim2handle =
{
    TIM2,
    {
        0,
        TIM_COUNTERMODE_UP,
        0xffffffff,
        TIM_CLOCKDIVISION_DIV1
    }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SystemClock_Config();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hardware_Init() 
{
    HAL_Init();

    SystemClock_Config();

    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();
    __DMA1_CLK_ENABLE();        // ��� DAC1 (�������)
    
    __TIM6_CLK_ENABLE();        // ��� ������� �����������
    __TIM2_CLK_ENABLE();        // ��� �����
    __TIM7_CLK_ENABLE();        // ��� DAC1 (�������)
    __DAC_CLK_ENABLE();         // ��� �������
    __PWR_CLK_ENABLE();

    __SYSCFG_CLK_ENABLE();

    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    // Timer  /////////////////////////////////////////////////////////////////
    //RCC_PCLK1Config(RCC_HCLK_Div1);

    // ������ ��� ��
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

    HAL_TIM_Base_Init(&handleTIM6forTimer);

    HAL_TIM_Base_Start_IT(&handleTIM6forTimer);

    HAL_TIM_Base_Init(&tim2handle);
    HAL_TIM_Base_Start(&tim2handle);

    Panel_Init();

    FSMC_Init();
    
// Analog and DAC programmable SPI ////////////////////////////////////////

    GPIO_InitTypeDef isGPIOG =
    {
        GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_7,     // GPIO_PIN_1 - ��� ������ � ��������
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF0_MCO
    };
    HAL_GPIO_Init(GPIOG, &isGPIOG);

    isGPIOG.Pin = GPIO_PIN_1;
    isGPIOG.Mode = GPIO_MODE_OUTPUT_PP;
    isGPIOG.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOG, &isGPIOG);
    
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_1, GPIO_PIN_RESET);                   // PG1 - ����� ����� 1, ������ �������, � ��������� ������� 0

}

void Hardware_DeInit()
{
    HAL_DeInit();

    __GPIOA_CLK_DISABLE();
    __GPIOB_CLK_DISABLE();
    __GPIOC_CLK_DISABLE();
    __GPIOD_CLK_DISABLE();
    __GPIOE_CLK_DISABLE();
    __GPIOF_CLK_DISABLE();
    __GPIOG_CLK_DISABLE();
    __DMA1_CLK_DISABLE();        // ��� DAC1 (�������)

    __TIM6_CLK_DISABLE();        // ��� ������� �����������
    __TIM2_CLK_DISABLE();        // ��� �����
    __TIM7_CLK_DISABLE();        // ��� DAC1 (�������)
    __DAC_CLK_DISABLE();         // ��� �������
    __PWR_CLK_DISABLE();

    __SYSCFG_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
    HAL_TIM_Base_DeInit(&handleTIM6forTimer);
    HAL_TIM_Base_Stop_IT(&handleTIM6forTimer);

    HAL_TIM_Base_DeInit(&tim2handle);
    HAL_TIM_Base_Stop(&tim2handle);
}

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void TIM6_DAC_IRQHandler()
{
    if (__HAL_TIM_GET_FLAG(&handleTIM6forTimer, TIM_FLAG_UPDATE) == SET && __HAL_TIM_GET_ITSTATUS(&handleTIM6forTimer, TIM_IT_UPDATE))
    {
        Timer_Update1ms();
        __HAL_TIM_CLEAR_FLAG(&handleTIM6forTimer, TIM_FLAG_UPDATE);
        __HAL_TIM_CLEAR_IT(&handleTIM6forTimer, TIM_IT_UPDATE);
    }
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SystemClock_Config()
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    // Enable HSE Oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 240;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 5;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    };
    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        ERROR_HANDLER();
    };
}
