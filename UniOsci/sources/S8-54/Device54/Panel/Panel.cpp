#include "Log.h"
#include "Panel.h"
#include "PanelFunctions.h"
#include "Display/PainterData.h"
#include "Hardware/Hardware.h"



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void OnPress_ResetSettings();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_DATA            20

#define LED_CHANA_ENABLE    129u
#define LED_CHANA_DISABLE   1u
#define LED_CHANB_ENABLE    130u
#define LED_CHANB_DISABLE   2u
#define LED_TRIG_ENABLE     131u
#define LED_TRIG_DISABLE    3u
#define POWER_OFF           4u


static PanelButton pressedKey = B_Empty;
static volatile PanelButton releasedButton = B_Empty;    ///< ???????????? ??? ???????????? ??????? ?????? ??? ??????????? ??????.
static uint16 dataTransmitted[MAX_DATA] = {0x00};
static uint16 firstPos = 0;
static uint16 lastPos = 0;


// ? ???? ?????????? ????????? ???????? ? ??????????
static PanelButton releaseButton = B_Empty;
static PanelButton pressButton = B_Empty;
static PanelRegulator regLeft = R_Empty;
static PanelRegulator regRight = R_Empty;
static int numReg = 0;                                  ///< ????? ????????? ?????.
static PanelRegulator regPress = R_Empty;
static PanelRegulator regRelease = R_Empty;
static PanelCommand recvCommand = C_None;

static int allRecData = 0;
static bool isRunning = true;

/// ????????/????????? ???????? ?????1
static void EnableLEDChannelA(bool enable);
/// ????????/????????? ???????? ?????2
static void EnableLEDChannelB(bool enable);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    void(*funcOnKey)(int);          ///< ??????? ?????????? ??? ???????(1) / ??????????(-1) ??????.
    void(*funcLongPressure)();  ///< ??????? ????????? ??? ?????????? ????????? ??????.
} StructButton;


static const StructButton funcButton[B_NumButtons] =
{
    {0, 0},
    {EFB,           Long_ChannelA}, ///< B_ChannelA
    {EFB,           EmptyFuncVV},   ///< B_Service
    {EFB,           Long_ChannelB}, ///< B_ChannelB
    {EFB,           EmptyFuncVV},   ///< B_Display
    {EFB,           Long_Time},     ///< B_Time
    {EFB,           EmptyFuncVV},   ///< B_Memory
    {EFB,           Long_Trig},     ///< B_Sinchro
    {Func_Start,    Long_Start},    ///< B_Start
    {EFB,           EmptyFuncVV},   ///< B_Cursors
    {EFB,           EmptyFuncVV},   ///< B_Measures
    {Func_Power,    EmptyFuncVV},   ///< B_Power
    {EFB,           Long_Help},     ///< B_Help
    {EFB,           Long_Menu},     ///< B_Menu
    {EFB,           F1_Long},       ///< B_F1
    {EFB,           F2_Long},       ///< B_F2
    {EFB,           F3_Long},       ///< B_F2
    {EFB,           F4_Long},       ///< B_F3
    {EFB,           F5_Long}        ///< B_F4
};


typedef struct
{
    void(*rotate)(int delta);       ///< ??? ??????? ?????????? ??? ???????? ?????.
    void(*press)(int delta);        ///< ??????? ?????????? ??? ???????/?????????? ?????.
    void(*longPress)();         ///< ??? ??????? ?????????? ??? ?????????? ??????? ?????.
} StructReg;

/** @todo ?????? ????????????*/
static const StructReg funculatorReg[] =
{
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {FuncRangeA,    EFB,                EmptyFuncVV},   ///< R_RangeA
    {FuncRShiftA,   FuncBtnRegChannelA, EmptyFuncVV},   ///< R_RShiftA
    {FuncRangeB,    EFB,                EmptyFuncVV},   ///< R_RangeB
    {FuncRShiftB,   FuncBtnRegChannelB, EmptyFuncVV},   ///< R_RShiftB
    {FuncTBase,     EFB,                EmptyFuncVV},   ///< R_TBase
    {FuncTShift,    FuncBtnRegTime,     EmptyFuncVV},   ///< R_TShift
    {FuncTrigLev,   FuncBtnRegTrig,     EmptyFuncVV},   ///< R_TrigLev
    {FuncRegSet,    FuncBtnRegSet,      EmptyFuncVV},   ///< R_Set
    {FuncRangeA,    EFB,                EmptyFuncVV},   ///< R_RangeA
    {FuncRShiftA,   FuncBtnRegChannelA, EmptyFuncVV},   ///< R_RShiftA
    {FuncRangeB,    EFB,                EmptyFuncVV},   ///< R_RangeB
    {FuncRShiftB,   FuncBtnRegChannelB, EmptyFuncVV},   ///< R_RShiftB
    {FuncTBase,     EFB,                EmptyFuncVV},   ///< R_TBase
    {FuncTShift,    FuncBtnRegTime,     EmptyFuncVV},   ///< R_TShift
    {FuncTrigLev,   FuncBtnRegTrig,     EmptyFuncVV},   ///< R_TrigLev
    {FuncRegSet,    FuncBtnRegSet,      EmptyFuncVV}    ///< R_Set
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static PanelButton ButtonIsRelease(uint16 command)
{
    PanelButton button = B_Empty;

    static uint timePrevReleaseButton = 0;

    if(command < B_NumButtons && command > B_Empty)
    {
        if(gTimeMS - timePrevReleaseButton > 100)
        {
            button = (PanelButton)command;
            timePrevReleaseButton = gTimeMS;
        }
    }

    return button;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static PanelButton ButtonIsPress(uint16 command)
{
    PanelButton button = B_Empty;

    static uint timePrevPressButton = 0;

    if (command < (B_NumButtons | 0x80) && command > (B_Empty | 0x80))
    {
        if(gTimeMS - timePrevPressButton > 100)
        {
            button = (PanelButton)(command & 0x7f);
            timePrevPressButton = gTimeMS;
        }
    }

    return button;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static PanelRegulator RegulatorPress(uint16 command)
{
    if (command >= (0x1c | 0x80) && command <= (0x23 | 0x80))
    {
        return (PanelRegulator)(command & 0x7f);
    }
    return R_Empty;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static PanelRegulator RegulatorRelease(uint16 command)
{
    if (command >= 0x1c && command <= 0x23)
    {
        return (PanelRegulator)command;
    }
    return R_Empty;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static PanelCommand ReceiveCommand(uint16 command)
{
    if (command == C_Reset)
    {
        return C_Reset;
    }
    return C_None;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static PanelRegulator RegulatorLeft(uint16 command)
{
    if(command >= 20 && command <= 27)
    {
        return (PanelRegulator)command;
    }
    return R_Empty;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static PanelRegulator RegulatorRight(uint16 command)
{
    if(command >= (20 | 0x80)  && command <= (27 | 0x80))
    {
        return (PanelRegulator)(command & 0x7f);
    }
    return R_Empty;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnTimerPressedKey()
{
    if(pressedKey != B_Empty)
    {
        void (*func)() = funcButton[pressedKey].funcLongPressure;
        if(func)
        {
            func();
        }
        else
        {
            Menu::ReleaseButton(pressedKey);
        }
        pressedKey = B_Empty;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Panel::ProcessingCommandFromPIC(uint16 command)
{
    if (command != 0)
    {

        if (!isRunning)
        {
            releasedButton = ButtonIsRelease(command);
            return true;
        }

        PainterData::InterruptDrawing();

        PANEL_CONTROL_RECEIVE = 1;

        allRecData++;

        PanelButton relButton = ButtonIsRelease(command);
        if (relButton)
        {
            releaseButton = relButton;
        }
        else
        {
            PanelButton prButton = ButtonIsPress(command);
            if (prButton)
            {
                pressButton = prButton;
            }
            else
            {
                PanelRegulator rLeft = RegulatorLeft(command);
                if (rLeft)
                {
                    regLeft = rLeft;
                    numReg++;
                }
                else
                {
                    PanelRegulator rRight = RegulatorRight(command);
                    if (rRight)
                    {
                        regRight = rRight;
                        numReg++;
                    }
                    else
                    {
                        PanelRegulator rPress = RegulatorPress(command);
                        if (rPress)
                        {
                            regPress = rPress;
                        }
                        else
                        {
                            PanelRegulator rRelease = RegulatorRelease(command);
                            if (rRelease)
                            {
                                regRelease = rRelease;
                            }
                            else
                            {
                                PanelCommand com = ReceiveCommand(command);
                                if (com)
                                {
                                    recvCommand = com;
                                }
                                else
                                {
                                    if (Settings::DebugModeEnable())
                                    {
                                        static int errRecData = 0;
                                        errRecData++;
                                        float percent = (float)errRecData / allRecData * 100.0f;
                                        const int SIZE = 100;
                                        char buffer[SIZE];
                                        buffer[0] = 0;
                                        snprintf(buffer, SIZE, "%5.3f", percent);
                                        strcat(buffer, "%");
                                        LOG_ERROR_TRACE("?????? SPI - %s %d/%d, command = %d", buffer, errRecData, allRecData, (int)command);
                                    }
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessingCommand()
{
    if (recvCommand == C_Reset)
    {
        Settings::Load(true);
        recvCommand = C_None;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::Update()
{
    if (isRunning)
    {
        if (releaseButton)
        {
            Menu::ReleaseButton(releaseButton);
            funcButton[releaseButton].funcOnKey(-1);
            if (pressedKey != B_Empty)
            {
                Menu::ShortPressureButton(releaseButton);
                pressedKey = B_Empty;
            }
            Timer::Disable(kPressKey);
        }
        else if (pressButton)
        {
            funcButton[pressButton].funcOnKey(1);
            Menu::PressButton(pressButton);
            pressedKey = pressButton;
            Timer::SetAndStartOnce(kPressKey, OnTimerPressedKey, 500);
        }
        else if (regLeft)
        {
            for (int i = 0; i < numReg; i++)
            {
                funculatorReg[regLeft].rotate(-1);
            }
        }
        else if (regRight)
        {
            for (int i = 0; i < numReg; i++)
            {
                funculatorReg[regRight].rotate(1);
            }
        }
        else if (regPress)
        {
            Sound::ButtonPress();
            funculatorReg[regPress].press(1);
            regPress = R_Empty;
        }
        else if (regRelease)
        {
            Sound::ButtonRelease();
            funculatorReg[regRelease].press(-1);
            regRelease = R_Empty;
        }
        else
        {
            ProcessingCommand();
        }
    }

    pressButton = B_Empty;
    releaseButton = B_Empty;
    regLeft = R_Empty;
    regRight = R_Empty;
    numReg = 0;

    PANEL_CONTROL_RECEIVE = 0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void EnableLEDChannelA(bool enable)
{
    Panel::TransmitData(enable ? LED_CHANA_ENABLE : LED_CHANA_DISABLE);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void EnableLEDChannelB(bool enable)
{
    Panel::TransmitData(enable ? LED_CHANB_ENABLE : LED_CHANB_DISABLE);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::EnableLEDChannel(Channel ch, bool enable)
{
    if (ch == A)
    {
        EnableLEDChannelA(enable);
    }
    else
    {
        EnableLEDChannelB(enable);
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::EnableLEDTrig(bool enable)
{
    static bool enabled = false;

    if (enable != enabled)
    {
        enabled = enable;
        Panel::TransmitData(enable ? LED_TRIG_ENABLE : LED_TRIG_DISABLE);
        Display::EnableTrigLabel(enable);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::TransmitData(uint16 data)
{
    if(lastPos == MAX_DATA)
    {
        LOG_ERROR_TRACE("?? ???? ??????? ? ?????? - ???? ?????");
    }
    else
    {
        if (data == 0x04)
        {
            uint time = gTimeMS;
            while (gTimeMS - time < 200) {};
        }
        dataTransmitted[lastPos++] = data;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint16 Panel::NextData()
{
    if (lastPos > 0)
    {
        uint16 data = dataTransmitted[firstPos++];
        if(firstPos == lastPos)
        {
            firstPos = lastPos = 0;
        }
        return data;
    }
    return 0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::Disable()
{
    isRunning = false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::Enable()
{
    isRunning = true;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
    SPI1
    56  - PG0 - ??????????? NSS 
    41  - PA5 - SCK
    42  - PA6 - MISO
    135 - PB5 - MOSI
*/


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::Init()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
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
        ERROR_HANDLER();
    }

    HAL_NVIC_SetPriority(SPI1_IRQn, PRIORITY_PANEL_SPI1);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    // ?????? ???????? ??????????? NSS (PB6).

    GPIO_InitTypeDef isGPIOG =
    {
        GPIO_PIN_6,                 // GPIO_Pin
        GPIO_MODE_IT_RISING,        // GPIO_Mode
        GPIO_NOPULL
    };      
    HAL_GPIO_Init(GPIOB, &isGPIOG);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, PRIORITY_PANEL_EXTI9_5);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    // ???????? ?????????
    isGPIOG.Pin = GPIO_PIN_12;
    isGPIOG.Mode = GPIO_MODE_OUTPUT_PP;
    isGPIOG.Speed = GPIO_SPEED_HIGH;
    isGPIOG.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOG, &isGPIOG);

    Panel::EnableLEDRegSet(false);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::EnableLEDRegSet(bool enable)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
PanelButton Panel::WaitPressingButton()
{
    releasedButton = B_Empty;
    while (releasedButton == B_Empty) {};
    return releasedButton;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::DisableIfNessessary()
{
    if (NEED_DISABLE_POWER)
    {
        Settings::Save();
        Log::DisconnectLoggerUSB();
        Panel::TransmitData(0x04);
        while (1)
        {
        };
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 dataSPIfromPanel;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    // ?????????? ?? SPI ?? ?????? ??????????
    if (pin == GPIO_PIN_6)
    {
        HAL_SPI_Receive_IT(&handleSPI, &dataSPIfromPanel, 1);
    }

    // ?????????? ?? ?????? ????? ??????????? ??????
    if (pin == GPIO_PIN_2)
    {
        FPGA::ReadPoint();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hSPI)
{
    if (!Panel::ProcessingCommandFromPIC(dataSPIfromPanel))
    {
        HAL_SPI_DeInit(hSPI);
        HAL_SPI_Init(hSPI);
    }

    SPI1->DR = Panel::NextData();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Panel::IsFunctionalButton(PanelButton button)
{
    return button >= B_F1 && button <= B_F5;
}

