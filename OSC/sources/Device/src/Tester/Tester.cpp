#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Device.h"
#include "Tester.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "Hardware/FSMC.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static DAC_HandleTypeDef hDAC = {DAC};

#define Port_TEST_ON    GPIOF
#define Pin_TEST_ON     GPIO_PIN_13

#define Port_PNP        GPIOF
#define Pin_PNP         GPIO_PIN_14

#define Port_U          GPIOF
#define Pin_U           GPIO_PIN_15

#define Port_I          GPIOG
#define Pin_I           GPIO_PIN_0

#define Port_TEST_STR   GPIOC
#define Pin_TEST_STR    GPIO_PIN_9


uint8 Tester::data[Chan::Number][NUM_STEPS][TESTER_NUM_POINTS];

static Settings oldSet = Settings::GetDefault();

int   Tester::step = 0;
float Tester::stepU = 0.0f;
bool  Tester::enabled = false;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tester::Init()
{
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;      // �������� ���

    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_5,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };

    HAL_GPIO_Init(GPIOA, &isGPIO);

    hDAC.Instance = DAC;

    if (HAL_DAC_Init(&hDAC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    //             TEST_ON       PNP       U
    isGPIO.Pin = Pin_TEST_ON | Pin_PNP | Pin_U;
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    isGPIO.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(Port_TEST_ON, &isGPIO);

    //               I
    isGPIO.Pin = Pin_I;
    HAL_GPIO_Init(Port_I, &isGPIO);

    //             TEST_STR - EXTI9
    isGPIO.Pin = Pin_TEST_STR;
    isGPIO.Mode = GPIO_MODE_IT_RISING;
    isGPIO.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(Port_TEST_STR, &isGPIO);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);

    HAL_GPIO_WritePin(Port_TEST_ON, Pin_TEST_ON, GPIO_PIN_SET);    // ��������� ������-��������

    for (int i = 0; i < Chan::Number; i++)
    {
        for (int j = 0; j < NUM_STEPS; j++)
        {
            for (int k = 0; k < TESTER_NUM_POINTS; k++)
            {
                Tester::data[i][j][k] = 0;
            }
        }
    }

    // �������������� ���
    GPIO_InitTypeDef _gpio =
    {
        GPIO_PIN_5,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOA, &_gpio);

    if (HAL_DAC_Init(&hDAC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    DAC_ChannelConfTypeDef configDAC;
    configDAC.DAC_Trigger = DAC_TRIGGER_NONE;
    configDAC.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;

    if (HAL_DAC_ConfigChannel(&hDAC, &configDAC, DAC1_CHANNEL_2) != HAL_OK)
    {
        ERROR_HANDLER();
    }
    HAL_DAC_SetValue(&hDAC, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, 0);
    HAL_DAC_Start(&hDAC, DAC1_CHANNEL_2);

    Disable();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Update()
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Enable()
{
    if(enabled)
    {
        return;
    }
    
    enabled = true;

    oldSet = set;

    SET_COUPLE_A = SET_COUPLE_B = ModeCouple::GND;

    FPGA::LoadRanges();

    HAL_GPIO_WritePin(Port_TEST_ON, Pin_TEST_ON, GPIO_PIN_RESET);  // �������� ������-���������

    LoadFPGA();

    FPGA::Stop();

    step = 0;                               // ������ ��������� ������� ����� �������� ������� ���

    LoadPolarity();

    LoadStep();

    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);       // �������� ���������� �� �������� ���������
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Disable()
{
    if(!enabled)
    {
        return;
    }

    enabled = false;

    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);      // ��������� ���������� �� �������� ���������

    HAL_GPIO_WritePin(Port_TEST_ON, Pin_TEST_ON, GPIO_PIN_SET);

    oldSet.test_control = TESTER_CONTROL;
    oldSet.test_polarity = TESTER_POLARITY;
    oldSet.test_stepU = TESTER_STEP_U;
    oldSet.test_stepI = TESTER_STEP_I;

    set = oldSet;
    //set.Save();

    FPGA::LoadSettings();

    FPGA::OnPressStart();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadFPGA()
{

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::ProcessStep()
{
                                                                                                                                                  /*
       |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |---
       |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
    ___|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|

       | step == 0 |  2.5 ��   | step == 2 |  2.5 ��   | step == 4 |  2.5 ��   | step == 6 |  2.5 ��   | step == 8 |  2.5 ��   |
       |     0V    |  ������   |   1 * dU  |  ������   |   2 * dU  |  ������   |   3 * dU  |  ������   |  4 * dU   |  ������   |
       |<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|                  */


    static int success = 0;
    static int fail = 0;

    if (!FSMC::InterchangeWithPanel())   // ���� ��� ������� ������ � �������, ������� �� ��������� ��� �� ��������� ���������� � ���������.
    {
        if ((step % 2) == 0)        // ���� ��� ������ ����, �� ����� ������������� ����������
        {
            HAL_DAC_SetValue(&hDAC, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, (uint)(stepU * step / 2));
            StartFPGA();
        }
        else
        {
            ReadData();
        }

        ++step;

        if (step == 10)
        {
            step = 0;
        }
        ++success;
    }
    else
    {
        ++fail;
        FSMC::RunFunctionAfterInteractionWitchPanel(&Tester::ProcessStep);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::StartFPGA()
{
    // ��������� ���� ��� ������ ������������ ���������� �����. ����� ����� ������������� � ������� 2.5 �� (������������� ������ �����)
    FPGA::StartForTester(TESTER_NUM_POINTS);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::ReadData()
{
    uint8 *x = &Tester::data[Chan::A][step / 2][0];
    uint8 *y = &Tester::data[Chan::B][step / 2][0];

    if(FPGA::ReadForTester(x, y))
    {

        if(x[0] == x[1] == x[2])
        {
            //LOG_WRITE("%d, %d, %d", x[0], x[1], x[2]);
        }

        Graphics::SetPoints(step / 2, x, y);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_9)
    {
        Tester::ProcessStep();
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadPolarity()
{
    // ������������� ����������
    HAL_GPIO_WritePin(Port_PNP, Pin_PNP, TESTER_POLARITY_IS_POSITITVE ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadStep()
{
    // ������������� ���������� ����������� ��� �����
    HAL_GPIO_WritePin(Port_U, Pin_U, TESTER_CONTROL_IS_U ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Port_I, Pin_I, TESTER_CONTROL_IS_U ? GPIO_PIN_RESET : GPIO_PIN_SET);

    if (TESTER_CONTROL_IS_U)
    {
        stepU =  255.0f / 3 * ((TESTER_STEP_U == Tester::StepU::_500mV) ? 2 : 0.4f) / 5;
    }
    else
    {
        stepU = 255.0f / 3 * ((TESTER_STEP_I == Tester::StepI::_20mA) ? 2 : 0.4f) / 5;
    }
}
