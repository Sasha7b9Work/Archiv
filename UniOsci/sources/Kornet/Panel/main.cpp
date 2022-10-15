#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/Decoder.h"
#include "Hardware/FSMC.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Keyboard/Keyboard.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    hardware.Init();

    while(1)
    {
        uint8 data = fsmc.ReadByte();
        decoder.AddData(data);
        fsmc.WriteBuffer();
    }
}
