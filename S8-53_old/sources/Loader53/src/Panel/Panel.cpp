#include "defines.h"
#include "Panel.h"
#include "Hardware/Controls.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Hardware/it.h"
#include <stdio.h>
#include <string.h>
#include <stm32f2xx.h>
#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_DATA            20

#define LED_CHANA_ENABLE    129
#define LED_CHANA_DISABLE   1
#define LED_CHANB_ENABLE    130
#define LED_CHANB_DISABLE   2
#define LED_TRIG_ENABLE     131
#define LED_TRIG_DISABLE    3
#define POWER_OFF           4


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static PanelButton pressedButton = B_Empty;
static uint8 dataSPIfromPanel = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool ProcessingCommandFromPIC(uint16 command);
static PanelButton ButtonIsPress(uint16 command);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Panel::Init()
{
    /*
        SPI1
        56  - PG0 - ����������� NSS
        41  - PA5 - SCK
        42  - PA6 - MISO
        135 - PB5 - MOSI
    */

    __SPI1_CLK_ENABLE();

    GPIO_InitTypeDef isGPIOA_B =
    {
        GPIO_PIN_5 | GPIO_PIN_6,
        GPIO_MODE_AF_PP,
        GPIO_PULLDOWN,
        GPIO_SPEED_FAST,
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &isGPIOA_B);

    isGPIOA_B.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB, &isGPIOA_B);

    HAL_SPI_Init(&handleSPI);

    HAL_NVIC_SetPriority(SPI1_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    // ������ �������� ����������� NSS (PG0)

    GPIO_InitTypeDef isGPIOG =
    {
        GPIO_PIN_0,
        GPIO_MODE_IT_RISING,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF0_RTC_50Hz
    };
    HAL_GPIO_Init(GPIOG, &isGPIOG);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::DeInit()
{
    HAL_NVIC_DisableIRQ(SPI1_IRQn);
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
PanelButton Panel::PressedButton()
{
    return pressedButton;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ProcessingCommandFromPIC(uint16 command)
{
    if(command)
    {
        PanelButton prButton = ButtonIsPress(command);
        if(prButton)
        {
            pressedButton = prButton;
        }
        else
        {
            pressedButton = B_Empty;
        }
    }
    return true;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static PanelButton ButtonIsPress(uint16 command)
{
    PanelButton button = B_Empty;

    static uint timePrevPressButton = 0;

    if(command < (B_NumButtons | 0x80) && command > (B_Empty | 0x80))
    {
        if(TIME_MS - timePrevPressButton > 100)
        {
            button = (PanelButton)(command & 0x7f);
            timePrevPressButton = TIME_MS;
        }
    }

    return button;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16 pin)
{
    // ���������� �� SPI �� ������ ����������
    if(pin == GPIO_PIN_0)
    {
        HAL_SPI_Receive_IT(&handleSPI, &dataSPIfromPanel, 1);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hSPI)
{
    if(!ProcessingCommandFromPIC(dataSPIfromPanel))
    {
        HAL_SPI_DeInit(hSPI);
        HAL_SPI_Init(hSPI);
    }
    SPI1->DR = 0;
}

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
// ����������� NSS �� SPI
void EXTI0_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void SPI1_IRQHandler()
{
    HAL_SPI_IRQHandler(&handleSPI);
}

#ifdef __cplusplus
}
#endif
