#include "Panel.h"
#include "Controls.h"
#include "FPGA/FPGA.h"
#include "Utils/Math.h"
#include "Utils/GlobalFunctions.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Log.h"
#include "Hardware/Sound.h"


#include <stm32f2xx_hal.h>
#include <stm32f2xx_hal_gpio.h>


#include <stdio.h>
#include <string.h>


#include "PanelFunctions.c"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_DATA            20

#define LED_CHAN0_ENABLE    129
#define LED_CHAN0_DISABLE   1
#define LED_CHAN1_ENABLE    130
#define LED_CHAN1_DISABLE   2
#define LED_TRIG_ENABLE     131
#define LED_TRIG_DISABLE    3
#define POWER_OFF           4

static PanelButton pressedKey = B_Empty;
static PanelButton pressedButton = B_Empty;         // ��� ������������ ��� ������������ ������� ������ ��� ����������� ������
static uint16 dataTransmitted[MAX_DATA] = {0x00};
static uint16 numDataForTransmitted = 0;
static uint timePrevPressButton = 0;
static uint timePrevReleaseButton = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void(*funcOnKeyDown[B_NumButtons])(void)    =
{    
                                                    0,
                                                    EmptyFuncVV,    // B_Channel0
                                                    EmptyFuncVV,    // B_Service
                                                    EmptyFuncVV,    // B_Channel1
                                                    EmptyFuncVV,    // B_Display
                                                    EmptyFuncVV,    // B_Time
                                                    EmptyFuncVV,    // B_Memory                                         
                                                    EmptyFuncVV,    // B_Sinchro
                                                    StartDown,      // B_Start
                                                    EmptyFuncVV,    // B_Cursors
                                                    EmptyFuncVV,    // B_Measures
                                                    PowerDown,      // B_Power
                                                    EmptyFuncVV,    // B_Help
                                                    EmptyFuncVV,    // B_Menu
                                                    EmptyFuncVV,    // B_F1
                                                    EmptyFuncVV,    // B_F2
                                                    EmptyFuncVV,    // B_F3
                                                    EmptyFuncVV,    // B_F4
                                                    EmptyFuncVV     // B_F5
                                                    };
static void (*funcOnKeyUp[B_NumButtons])(void)    =
{
                                                    0,
                                                    EmptyFuncVV,    // B_Channel0
                                                    EmptyFuncVV,    // B_Service
                                                    EmptyFuncVV,    // B_Channel1
                                                    EmptyFuncVV,    // B_Display
                                                    EmptyFuncVV,    // B_Time
                                                    EmptyFuncVV,    // B_Memory
                                                    EmptyFuncVV,    // B_Sinchro
                                                    EmptyFuncVV,    // B_Start
                                                    EmptyFuncVV,    // B_Cursors
                                                    EmptyFuncVV,    // B_Measures
                                                    EmptyFuncVV,    // B_Power
                                                    EmptyFuncVV,    // B_Help
                                                    EmptyFuncVV,    // B_Menu
                                                    EmptyFuncVV,    // B_F1
                                                    EmptyFuncVV,    // B_F2
                                                    EmptyFuncVV,    // B_F3
                                                    EmptyFuncVV,    // B_F4
                                                    EmptyFuncVV     // B_F5
                                                    };
static void (*funcOnLongPressure[B_NumButtons])(void)    =
{
                                                            0,
                                                            Channel0Long,   // B_Channel0
                                                            EmptyFuncVV,    // B_Service
                                                            Channel1Long,   // B_Channel1
                                                            EmptyFuncVV,    // B_Display
                                                            TimeLong,       // B_Time
                                                            EmptyFuncVV,    // B_Memory
                                                            TrigLong,       // B_Sinchro
                                                            EmptyFuncVV,    // B_Start
                                                            EmptyFuncVV,    // B_Cursors
                                                            EmptyFuncVV,    // B_Measures
                                                            EmptyFuncVV,    // B_Power
                                                            HelpLong,       // B_Help
                                                            MenuLong,       // B_Menu
                                                            F1Long,         // B_F1
                                                            F2Long,         // B_F2
                                                            F3Long,         // B_F3
                                                            F4Long,         // B_F4
                                                            F5Long          // B_F5
                                                            };

static void (*funculatorLeft[R_Set + 1])(void)    =
{
                                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                    Range0Left,     // R_Range0
                                                    RShift0Left,    // R_RShift0
                                                    Range1Left,     // R_Range1
                                                    RShift1Left,    // R_RShift1
                                                    TBaseLeft,      // R_TBase
                                                    TShiftLeft,     // R_TShift
                                                    TrigLevLeft,    // R_TrigLev
                                                    SetLeft         // R_Set
};
static void (*funculatorRight[R_Set + 1])(void) =
{
                                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                    Range0Right,    // R_Range0
                                                    RShift0Right,   // R_RShift0
                                                    Range1Right,    // R_Range1
                                                    RShift1Right,   // R_RShift1
                                                    TBaseRight,     // R_TBase
                                                    TShiftRight,    // R_TShift
                                                    TrigLevRight,   // R_TrigLev
                                                    SetRight        // R_Set
};


uint16 RotateRegRight(Regulator reg);
uint16 ButtonPress(PanelButton button);


PanelButton ButtonIsRelease(uint16 command)
{
    PanelButton button = B_Empty;
    if(command < B_NumButtons && command > B_Empty)
    {
        if(gTimerMS - timePrevReleaseButton > 100)
        {
            button = (PanelButton)command;
            timePrevReleaseButton = gTimerMS;
        }
    }
    return button;
}

PanelButton ButtonIsPress(uint16 command)
{
    PanelButton button = B_Empty;
    if (((command & 0x7f) < B_NumButtons) && ((command & 0x7f) > B_Empty))
    {
        if(gTimerMS - timePrevPressButton > 100)
        {
            button = (PanelButton)(command & 0x7f);
            timePrevPressButton = gTimerMS;
        }
    }
    return button;
}

Regulator RegulatorLeft(uint16 command)
{
    if(command >= 20 && command <= 27)
    {
        return (Regulator)command;
    }
    return R_Empty;
}

Regulator RegulatorRight(uint16 command)
{
    if(((command & 0x7f) >= 20) && ((command & 0x7f) <= 27))
    {
        return (Regulator)(command & 0x7f);
    }
    return R_Empty;
}

void OnTimerPressedKey()
{
    if(pressedKey != B_Empty)
    {
        void (*func)(void) = funcOnLongPressure[pressedKey];
        Menu_ReleaseButton(pressedKey);
        if(func != 0)
        {
            func();
        }
        pressedKey = B_Empty;
    }
    Timer_Disable(kPressKey);
}

bool Panel_ProcessingCommandFromPIC(uint16 command)
{
    static int allRecData = 0;

    PanelButton releaseButton = ButtonIsRelease(command);
    PanelButton pressButton = ButtonIsPress(command);
    Regulator regLeft = RegulatorLeft(command);
    Regulator regRight = RegulatorRight(command);

    if (pressButton)
    {
        pressedButton = pressButton;
    }

    if(gBF.panelIsRunning == 0)
    {
        return true;
    }

    if(command != 0)
    {
        allRecData++;
    }
    else
    {
        allRecData++;
        allRecData--;
    }

    if(releaseButton)
    {
        Menu_ReleaseButton(releaseButton);
        funcOnKeyUp[releaseButton]();
        if(pressedKey != B_Empty)
        {
            Menu_ShortPressureButton(releaseButton);
            pressedKey = B_Empty;
        }
    }
    else if(pressButton)
    {
        funcOnKeyDown[pressButton]();
        Menu_PressButton(pressButton);
        pressedKey = pressButton;
        Timer_Enable(kPressKey, 500, OnTimerPressedKey);
    }
    else if(regLeft)
    {
        /*
        if (set.memory.modeWork == ModeWork_Direct || regLeft == R_Set || regLeft == R_TShift)
        {
        */
            funculatorLeft[regLeft]();
        //}
    }
    else if(regRight)
    {
        /*
        if (set.memory.modeWork == ModeWork_Direct || regRight == R_Set || regRight == R_TShift)
        {
        */
            funculatorRight[regRight]();
        //}
    }

    if ((command > R_Set && command < (B_Empty + 1 + 128)) || (command > (R_Set + 128)))
    {
        if(Settings_DebugModeEnable())
        {
            static int errRecData = 0;
            errRecData++;
            float percent = (float)errRecData / allRecData * 100.0f;
            char buffer[100];
            buffer[0] = 0;
            sprintf(buffer, "%5.3f", percent);
            strcat(buffer, "%");
            LOG_ERROR("������ SPI - %s %d/%d, command = %d", buffer, errRecData, allRecData, (int)command);
        }
        return false;
    }
    return true;
}

void Panel_EnableLEDChannel0(bool enable)
{
    Panel_TransmitData(enable ? LED_CHAN0_ENABLE : LED_CHAN0_DISABLE);
}

void Panel_EnableLEDChannel1(bool enable)
{
    Panel_TransmitData(enable ? LED_CHAN1_ENABLE : LED_CHAN1_DISABLE);
}

void Panel_EnableLEDTrig(bool enable)
{
    static uint timeEnable = 0;
    static bool first = true;
    static bool fired = false;
    if(first)
    {
        Panel_TransmitData(LED_TRIG_DISABLE);
        Display_EnableTrigLabel(false);
        timeEnable = gTimerMS;
        first = false;
    }

    if(enable)
    {
        timeEnable = gTimerMS;
    }

    if(enable != fired)
    {
        if(enable)
        {
            Panel_TransmitData(LED_TRIG_ENABLE);
            Display_EnableTrigLabel(true);
            fired = true;
        }
        else if(gTimerMS - timeEnable > 100)
        {
            Panel_TransmitData(LED_TRIG_DISABLE);
            Display_EnableTrigLabel(false);
            fired = false;
        }
    }
}

void Panel_TransmitData(uint16 data)
{
    if(numDataForTransmitted >= MAX_DATA)
    {
        LOG_WRITE("�� ���� ������� � ������ - ���� �����");
    }
    else
    {
        dataTransmitted[numDataForTransmitted] = data;
        numDataForTransmitted++;
    }
}

uint16 Panel_NextData()
{
    if (numDataForTransmitted > 0)
    {
        numDataForTransmitted--;
        return dataTransmitted[numDataForTransmitted];
    }
    return 0;
}

void Panel_Disable()
{
    gBF.panelIsRunning = 0;
}

void Panel_Enable()
{
    gBF.panelIsRunning = 1;
}

/*
    SPI1
    56  - PG0 - ����������� NSS 
    41  - PA5 - SCK
    42  - PA6 - MISO
    135 - PB5 - MOSI
*/



void Panel_Init()
{
    __SPI1_CLK_ENABLE();

    GPIO_InitTypeDef isGPIOA_B =
    {
        GPIO_PIN_5 | GPIO_PIN_6,    // GPIO_Pin
        GPIO_MODE_AF_PP,            // GPIO_Mode
        GPIO_PULLDOWN,
        GPIO_SPEED_FAST,            // GPIO_Speed
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &isGPIOA_B);
    
    isGPIOA_B.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB, &isGPIOA_B);
    
    if (HAL_SPI_Init(&handleSPI) != HAL_OK)
    {
        HARDWARE_ERROR
    }

    HAL_NVIC_SetPriority(SPI1_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    
    // ������ �������� ����������� NSS (PG0).

    GPIO_InitTypeDef isGPIOG =
    {
        GPIO_PIN_0,                 // GPIO_Pin
        GPIO_MODE_IT_RISING,        // GPIO_Mode
        GPIO_NOPULL
    };      
    HAL_GPIO_Init(GPIOG, &isGPIOG);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    // �������� ���������
    isGPIOG.Pin = GPIO_PIN_12;
    isGPIOG.Mode = GPIO_MODE_OUTPUT_PP;
    isGPIOG.Speed = GPIO_SPEED_HIGH;
    isGPIOG.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOG, &isGPIOG);

    Panel_EnableLEDRegSet(false);
}

void Panel_EnableLEDRegSet(bool enable)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

PanelButton Panel_WaitPressingButton()
{
    pressedButton = B_Empty;
    while (pressedButton == B_Empty) {};
    return pressedButton;
}

static uint8 dataSPIfromPanel;

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        HAL_SPI_Receive_IT(&handleSPI, &dataSPIfromPanel, 1);
    }
}


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* handleSPI)
{
    if (!Panel_ProcessingCommandFromPIC(dataSPIfromPanel))
    {
        HAL_SPI_DeInit(handleSPI);
        HAL_SPI_Init(handleSPI);
    }
    uint16 data = Panel_NextData();
    SPI1->DR = data;
}
