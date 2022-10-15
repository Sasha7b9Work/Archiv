#include "Decoder.h"
#include "FSMC.h"
#include "Menu/Menu.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Decoder decoder;

#define SIZE_BUFFER 1024
static uint8 buffer[SIZE_BUFFER];
static int pointer = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Decoder::Decoder() : curFunc(0), step(0)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::AddData(uint8 data)
{
    if (pointer < SIZE_BUFFER)
    {
        buffer[pointer++] = data;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::Update()
{
    if (pointer)
    {
        if(buffer[0] == 1)
        {
            buffer[0] = 1;
        }
        
        for (int i = 0; i < pointer; i++)
        {
            RunStep(buffer[i]);
        }
        pointer = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::RunStep(uint8 data)
{
    static const pFuncBU8 commands[NUM_COMMANDS] =
    {
        &Decoder::EmptyFunc,
        &Decoder::ButtonPress
    };

    if (step == 0)
    {
        if (data < NUM_COMMANDS)
        {
            curFunc = commands[data];
        }
        else
        {
            FinishCommand();
            return;
        }
    }

    if (curFunc)
    {
        if ((this->*curFunc)(data))
        {
            FinishCommand();
        }
        else
        {
            ++step;
        }
    }
    else
    {
        FinishCommand();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::ButtonPress(uint8 data)
{
    static Key button;
    if (step == 0)
    {
        return false;
    }
    else if (step == 1)
    {
        button = (Key)data;
        return false;
    }
    else
    {
        menu.ButtonPress(button, (TypePress)data);
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::FinishCommand()
{
    step = 0;
    curFunc = 0;
}
