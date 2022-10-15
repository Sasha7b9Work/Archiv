#include <stm32f4xx.h>
#include "AD9286.h"
#include "Hardware/Timer.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AD9286 ad9286;

#define SPI4_SCK GPIO_PIN_12
#define SPI4_DAT GPIO_PIN_14
#define SPI4_CS GPIO_PIN_11

GPIO_InitTypeDef isGPIO =
{
    SPI4_SCK | SPI4_DAT | SPI4_CS,
    GPIO_MODE_OUTPUT_PP,
    GPIO_PULLDOWN
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AD9286::Init()
{
    HAL_GPIO_Init(GPIOE, &isGPIO);

    isGPIO.Pin = SPI4_DAT;

    SetPin(SPI4_CS);
    ResetPin(SPI4_DAT);
    ResetPin(SPI4_SCK);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::ConfigToRead()
{
    isGPIO.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOE, &isGPIO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::ConfigToWrite()
{
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOE, &isGPIO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::WriteByte(uint8 address, uint8 byte)
{
    ResetPin(SPI4_CS);

    uint value = (uint)((address << 8) + byte);

    for (int i = 23; i >= 0; --i)
    {
        if (_GET_BIT(value, i))
        {
            SetPin(SPI4_DAT);
        }
        else
        {
            ResetPin(SPI4_DAT);
        }
        PAUSE_ON_TICKS(100);

        SetPin(SPI4_SCK);
        PAUSE_ON_TICKS(100);

        ResetPin(SPI4_SCK);
    }

    SetPin(SPI4_CS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 AD9286::ReadByte(uint8 address)
{
    uint16 value = (uint16)(0x8000 + address);

    ResetPin(SPI4_CS);

    for (int i = 15; i >= 0; --i)
    {
        if (_GET_BIT(value, i))
        {
            SetPin(SPI4_DAT);
        }
        else
        {
            ResetPin(SPI4_DAT);
        }
        PAUSE_ON_TICKS(100);

        SetPin(SPI4_SCK);
        PAUSE_ON_TICKS(100);

        ResetPin(SPI4_SCK);
    }

    SetPin(SPI4_CS);

    uint8 retValue = 0;
    
    ConfigToRead();

    ResetPin(SPI4_CS);

    for (int i = 7; i >= 0; --i)
    {
        SetPin(SPI4_SCK);
        int bit = ReadPin(SPI4_DAT);
        if (bit)
        {
            _SET_BIT_VALUE(retValue, i, bit);
        }
        ResetPin(SPI4_SCK);
        PAUSE_ON_TICKS(100);
    }

    SetPin(SPI4_CS);

    ConfigToWrite();
    
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::SetPin(uint pin)
{
    HAL_GPIO_WritePin(GPIOE, (uint16)pin, GPIO_PIN_SET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::ResetPin(uint pin)
{
    HAL_GPIO_WritePin(GPIOE, (uint16)pin, GPIO_PIN_RESET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int AD9286::ReadPin(uint pin)
{
    return HAL_GPIO_ReadPin(GPIOE, (uint16)pin);
}
