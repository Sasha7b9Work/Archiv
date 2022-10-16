#include "comport.h"
#include "../../common/defines.h"

using std::vector;
using std::wstring;

ComPort::ComPort() : timer(NULL), delayUs(10.0), numPort(0) 
{
    timer = new Timer;
    handle = INVALID_HANDLE_VALUE;
    baudRate = 115200;
    byteSize = 8;
    parity = NOPARITY;
    stopBits = ONESTOPBIT;
}

ComPort::~ComPort() 
{
    SAFE_DELETE(timer);
    Close();
}

vector <short>& ComPort::GetComPorts() 
{
    static std::vector<short> ports;
    ports.clear();

    for(int port = 1; port < 256; port++)
        if(IsExistPort(port))
            ports.push_back(static_cast<short>(port));

    return ports;
}

bool ComPort::Open(int _numPort) 
{
    if(IsOpen()) 
        return false;

    handle = CreateFile(
                        NumberPortInString(_numPort).c_str(),   // 1
                        GENERIC_READ | GENERIC_WRITE,           // 2
                        FILE_SHARE_READ | FILE_SHARE_WRITE,     // 3
                        NULL,                                   // 4
                        OPEN_EXISTING,                          // 5
                        FILE_ATTRIBUTE_NORMAL,                  // 6
                        0                                       // 7
                       );
    if(!IsOpen()) 
    {
        numPort = 0;
        return false;
    }
    else 
    {
        numPort = _numPort;
    }
    ApplyComSettings();
    return IsOpen();
}

bool ComPort::IsExistPort(int _numPort)
{
    wstring namePort = NumberPortInString(_numPort);
    HANDLE handle = CreateFile(
                                namePort.c_str(),
                                GENERIC_READ | GENERIC_EXECUTE,
                                FILE_SHARE_READ | FILE_SHARE_DELETE,
                                NULL,
                                OPEN_EXISTING | CREATE_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                0
                              );
  
    return ERROR_ACCESS_DENIED == GetLastError() || CloseHandle(handle);
}

wstring& ComPort::NumberPortInString(int _numPort)
{
    wchar_t buffer[100];
    _itow_s(_numPort, buffer, 50, 10);
    static wstring comName;
    comName.clear();
    comName = L"\\\\.\\COM" + wstring(buffer);
    return comName;
}

int ComPort::GetNumberPort() 
{
    return (IsOpen() ? numPort : 0);
}

bool ComPort::IsOpen() 
{
    return handle != INVALID_HANDLE_VALUE;
}

void ComPort::Close() 
{
    if(!IsOpen()) 
        return;
    
    CloseHandle(handle);
    handle = INVALID_HANDLE_VALUE;
}

void ComPort::ApplyComSettings() 
{
    DCB dcb;
    if(!IsOpen()) 
        return;
    
    if(!GetCommState(handle, &dcb)) 
        return;
    
    dcb.BaudRate = baudRate;
    dcb.ByteSize = byteSize;
    dcb.Parity = parity;
    dcb.StopBits = stopBits;
    if(!SetCommState(handle, &dcb)) 
        return;
}

void ComPort::SetBaudRate(const DWORD _value) 
{
    if(baudRate == _value) 
        return;
    
    baudRate = _value;
    ApplyComSettings();
}

void ComPort::SetByteSize(const BYTE _value) 
{
    if(byteSize == _value) 
        return;
    
    byteSize = _value;
    ApplyComSettings();
}

void ComPort::SetParity(const BYTE _value) 
{
    if(parity == _value) 
        return;
    
    parity = _value;
    ApplyComSettings();
}

void ComPort::SetStopBits(const BYTE _value) 
{
    if(stopBits == _value) 
        return;
    
    stopBits = _value;
    ApplyComSettings();
}


COMSTAT* ComPort::GetState(UINT &/*codeError*/) 
{
    DWORD _error;
    static COMSTAT comStat;
    ClearCommError(handle, &_error, &comStat);
    return &comStat;
}

DWORD ComPort::GetBytesInQue() 
{
    COMSTAT comStat;
    DWORD _error;
    ClearCommError(handle, &_error, &comStat);
    return comStat.cbInQue;
}

DWORD ComPort::ReadBuffer(void* _buffer, DWORD _sizeBuffer) 
{
    DWORD realRead = GetBytesInQue();
    if(realRead == 0) 
        return 0;
    
    if(realRead > _sizeBuffer) 
        realRead = _sizeBuffer;
    
    ReadFile(handle, _buffer, realRead, &realRead, NULL);
    return realRead;
}

bool ComPort::WriteByte(const BYTE& _B) 
{
    DWORD realWrite = 0;
    timer->DelayUs(delayUs);
    if(WriteFile(handle, (void*)&_B, 1, &realWrite, NULL)) 
        return (realWrite == 1);
    
    return false;
}

DWORD ComPort::WriteBuffer(void* _buffer, DWORD _sizeBuffer) 
{
    DWORD realWrite = 0;
    timer->DelayUs(delayUs);
    WriteFile(handle, _buffer, _sizeBuffer, &realWrite, NULL);
    return realWrite;
}

void ComPort::ClearBuffer() 
{
    PurgeComm(handle, PURGE_TXCLEAR | PURGE_RXCLEAR);
}
