#include "defines.h"
#include "Utils/Decoder.h"
#include "FSMC.h"
#include "Timer.h"
#include "Display/Display.h"
#include <string.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PORT_NE4        GPIOC
#define PIN_NE4         GPIO_PIN_13

#define PORT_PAN_0      GPIOC
#define PIN_PAN_0       GPIO_PIN_14

#define PORT_PAN_1      GPIOC
#define PIN_PAN_1       GPIO_PIN_15

#define NE4_IS_HI       ((PORT_NE4->IDR & PIN_NE4) != GPIO_PIN_RESET)
#define NE4_IS_LOW      ((PORT_NE4->IDR & PIN_NE4) == GPIO_PIN_RESET)

#define PAN_0_SET       (PORT_PAN_0->BSRR = PIN_PAN_0)
#define PAN_0_RESET     (PORT_PAN_0->BSRR = (uint)PIN_PAN_0 << 16U)

#define PAN_1_SET       (PORT_PAN_1->BSRR = PIN_PAN_1)
#define PAN_1_RESET     (PORT_PAN_1->BSRR = (uint)PIN_PAN_1 << 16U)

#define SET_BUSY                        PAN_0_RESET; PAN_1_RESET;
#define SET_READY_TRANSMIT              PAN_0_SET;   PAN_1_RESET;
#define SET_READY_RECEIVE               PAN_0_RESET; PAN_1_SET;
#define SET_RECEIVE_TRANSMIT_CONFIRM    PAN_0_SET;   PAN_1_SET;


bool FSMC::inReadState = false;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FSMC::Init()
{
    /// \todo ���� �� �����������, ��� ��������� ������������ ���� ��������. �� � ���� ����� ������ �������

    GPIO_InitTypeDef isGPIO =           //  PAN_0, PAN_1
    {
        PIN_PAN_0 | PIN_PAN_1,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };
    HAL_GPIO_Init(PORT_PAN_0, &isGPIO);

    isGPIO.Pin = PIN_NE4;               // NE4
    isGPIO.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(PORT_NE4, &isGPIO);

    SET_BUSY;

    ConfigToRead();
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::ConfigToRead()
{
    static GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_INPUT,
        GPIO_PULLDOWN
    };
    HAL_GPIO_Init(GPIOE, &isGPIO);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::ConfigToWrite()
{
    static GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };
    HAL_GPIO_Init(GPIOE, &isGPIO);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 FSMC::ReadByte()
{
    inReadState = true;
    
    SET_READY_RECEIVE;                  // ������������� ������� ����, ��� �� ������ � ����� ������

    while (NE4_IS_HI) {};               // ���, ���� NE4 ����������� � ����, ������������ � ���, ��� ������ ���������� �� ����

    uint8 data = (uint8)GPIOE->IDR;     // ������ ������ � ����

    SET_RECEIVE_TRANSMIT_CONFIRM;       // ���������� ������� ����� ����������

    while (NE4_IS_LOW) {};              // � ���, ���� ���������� ��������� ��� ����������

    SET_BUSY;
        
    inReadState = false;
    return data;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::WriteBuffer(uint8 *data, int length)
{
#define SIZE_BUFFER (1024 * 10)
    static uint8 buffer[SIZE_BUFFER];
    static int pointer = 0;

    if (inReadState)                                        // ���� ��� ������� �����
    {
        if (data)
        {
            memcpy(buffer + pointer, data, (uint)length);   // �� ��������� ���������� ��� �������� � �������
            pointer += length;
        }
        return;                                             // � �������
    }
    
    if (data)                                               // ���� ���� ������ ��� ��������
    {
        memcpy(buffer + pointer, data, (uint)length);       // �� ������������ �� � �����
        pointer += length;
    }

    if (pointer)                            // ���� ����� �������� �� ����
    {
        ConfigToWrite();    /// \todo ����� ��-���� �� ������ ���������. �� ����� ���� ������� RD ��������������� � 1, ��� ����� �����������
                            /// ���� ������ �� ��������� ����������, ��� ����� �������� ���� ������.

        for (int i = 0; i < pointer; i++)
        {
            SET_READY_TRANSMIT;             // ������������� ������� ����, ��� ������ ����� ������������ ����������

            while (NE4_IS_HI) {};           // ��� ������� ����, ��� ���������� ������ � ����� ����������

            GPIOE->ODR = buffer[i];         // ���������� ���� � �����

            SET_RECEIVE_TRANSMIT_CONFIRM;   // ������������� ������� ����, ��� ������ �����������

            while (NE4_IS_LOW) {};          // ���, ��� ���������� ������� ����������
        }

        pointer = 0;

        SET_BUSY;

        ConfigToRead();
    }
}
