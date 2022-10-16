//  [3/6/2009 Sasha]--------------------------------------------
#pragma once 
#include <Windows.h>
#include <vector>
#include <string>
#include "../../common/Timer.h"

class ComPort {
public:
    ComPort();
    ~ComPort();

    std::vector <short>& GetComPorts();

    bool Open(int num);             // В функцию передаётся номер ком-порта, который нужно открыть
    int GetNumberPort();

    bool IsOpen();

    bool ReadByte(BYTE &B);
    bool WriteByte(const BYTE& B);

    DWORD ReadBuffer(void* buffer, DWORD sizeBuffer);
    DWORD WriteBuffer(void* buffer, DWORD sizeBuffer);

    void ClearBuffer();

    void SetByteSize(const BYTE value);
    void SetBaudRate(const DWORD value);
    void SetParity(const BYTE value);
    void SetStopBits(const BYTE value);

    void Close();

    DWORD GetBytesInQue();
    
private:
    Timer *timer;
    double delayUs;    // Минимальное время в микросекундах между записью в альтеру
    HANDLE handle;
    DWORD baudRate;
    BYTE byteSize;
    BYTE parity;        // EVENPARITY, ODDPARITY or NOPARITY
    BYTE stopBits;      // ONESTOPBIT or TWOSTOPBITS
    int numPort;

    void ApplyComSettings();
    COMSTAT* GetState(UINT &codeError);
    bool IsExistPort(int _numPort);
    std::wstring& NumberPortInString(int _numPort);
};