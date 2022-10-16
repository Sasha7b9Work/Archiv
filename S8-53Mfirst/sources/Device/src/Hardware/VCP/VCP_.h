// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace VCP
{
    void Init();

    void SendBuffer(const uint8 *buffer, int size);

    // Сообщение передаётся без завершающего нуля
    void SendMessage(pchar message);

    extern bool cableIsConnected;       // true, если подсоединён кабель
    extern bool connectToHost;          // true, если есть подключение
};
