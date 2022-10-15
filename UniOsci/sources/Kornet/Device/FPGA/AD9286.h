#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AD9286
{
public:
    void Init();
    uint8 ReadByte(uint8 address);
    void  WriteByte(uint8 address, uint8 byte);

private:
    void FirstConfig();
    void ConfigToWrite();
    void ConfigToRead();
    void SetPin(uint pin);
    void ResetPin(uint pin);
    int ReadPin(uint pin);
};

extern AD9286 ad9286;
