// 2022/01/25 10:14:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace LAN
{
    void Init();

    void SendBuffer(const uint8 *buffer, int size);

    // Передаёт сообщение без завершаюшего нуля
    void SendMessage(pchar message);

    extern bool clientIsConnected;

    extern bool cableIsConnected;
}

