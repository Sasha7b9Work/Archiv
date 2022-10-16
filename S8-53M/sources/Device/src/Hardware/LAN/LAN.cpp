// 2022/01/25 10:17:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/LAN/LAN.h"


bool LAN::clientIsConnected = false;
bool LAN::cableIsConnected = false;


void LAN::Init()
{

}


void LAN::SendBuffer(const uint8 *, int)
{
    if (!LAN::clientIsConnected)
    {
        return;
    }
}


void LAN::SendMessage(pchar)
{
    if (!LAN::clientIsConnected)
    {
        return;
    }
}
