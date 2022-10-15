#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "main.h"
#include "Device.h"
#include "Hardware/AT25160N.h"
#include "Hardware/Timer.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    Device::Init();

    AT25160N::Init();

    while (1)
    {
        Device::Update();
    }
}
