//  [3/6/2009 Sasha] ----------------------------------------------------
#include "comport.h"
#include "defines.h"
#include "GlobalFunctions.h"

using std::vector;
using std::wstring;

using namespace NS_GlobalFunctions;

//  [3/11/2009 Sasha] ---------------------------------------------------
ComPort::ComPort() : numPort(0) {
    handle = INVALID_HANDLE_VALUE;
    baudRate = 115200;
    byteSize = 8;
    parity = NOPARITY;
    stopBits = ONESTOPBIT;
}

//  [3/11/2009 Sasha] ---------------------------------------------------
ComPort::~ComPort() {
    Close();
}

//  [9/18/2009 Sasha]
vector <short>& ComPort::GetComPorts() {
    static vector<short> ports;
    ports.clear();
    if(!IsOpen()) {
        for(short i = 1; i < 32; i++) {
            if(Open(i)) {
                Close();
                ports.push_back(i);
            }
        }
    }
    return ports;
}

//  [3/11/2009 Sasha] ---------------------------------------------------
bool ComPort::Open(int _num) {
    if(IsOpen()) {
        return false;
    }
    wchar_t buffer[100];
    _itow_s(_num, buffer, 50, 10);
	wstring comName = (L"\\\\.\\COM");
    comName += wstring(buffer);
    handle = CreateFile(comName.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if(!IsOpen()) {
        numPort = 0;
        return false;
    }
    else {
        numPort = _num;
    }
    ApplyComSettings();
    return IsOpen();
}

//  [12/12/2009 Sasha]
int ComPort::GetNumberPort() {
    return (IsOpen() ? numPort : 0);
}

//  [3/11/2009 Sasha] ---------------------------------------------------
bool ComPort::IsOpen() {
    return handle != INVALID_HANDLE_VALUE;
}

//  [3/11/2009 Sasha] ---------------------------------------------------
void ComPort::Close() {
    if(!IsOpen()) {
        return;
    }
    CloseHandle(handle);
    handle = INVALID_HANDLE_VALUE;
}

//  [3/11/2009 Sasha] ---------------------------------------------------
void ComPort::ApplyComSettings() {
    DCB dcb;
    if(!IsOpen()) {
        return;
    }
    if(!GetCommState(handle, &dcb)) {
        return;
    }
    dcb.BaudRate = baudRate;
    dcb.ByteSize = byteSize;
    dcb.Parity = parity;
    dcb.StopBits = stopBits;
    if(!SetCommState(handle, &dcb)) {
        ERROR_LOG("Error set settings");
        return;
    }
    else {
        //ERROR_LOG("Set settings is ok");
    }
}

//  [3/11/2009 Sasha] ------------------------------------------------
void ComPort::SetBaudRate(const DWORD _value) {
    if(baudRate == _value) {
        return;
    }
    baudRate = _value;
    ApplyComSettings();
}

//  [3/11/2009 Sasha] -------------------------------------------------
void ComPort::SetByteSize(const BYTE _value) {
    if(byteSize == _value) {
        return;
    }
    byteSize = _value;
    ApplyComSettings();
}

//  [3/11/2009 Sasha] --------------------------------------------------
void ComPort::SetParity(const BYTE _value) {
    if(parity == _value) {
        return;
    }
    parity = _value;
    ApplyComSettings();
}

//  [3/11/2009 Sasha] --------------------------------------------------
void ComPort::SetStopBits(const BYTE _value) {
    if(stopBits == _value) {
        return;
    }
    stopBits = _value;
    ApplyComSettings();
}

//  [3/11/2009 Sasha] --------------------------------------------------
COMSTAT* ComPort::GetState(UINT &/*codeError*/) {
    DWORD _error;
    static COMSTAT comStat;
    ClearCommError(handle, &_error, &comStat);
    return &comStat;
}

//  [3/12/2009 Sasha] --------------------------------------------------
DWORD ComPort::GetBytesInQue() {
    COMSTAT comStat;
    DWORD _error;
    ClearCommError(handle, &_error, &comStat);
    return comStat.cbInQue;
}

//  [3/12/2009 Sasha] --------------------------------------------------
DWORD ComPort::ReadBuffer(void* _buffer, DWORD _sizeBuffer) {
    DWORD realRead = GetBytesInQue();
    if(realRead == 0) {
        return 0;
    }
    if(realRead > _sizeBuffer) {
        realRead = _sizeBuffer;
    }
    ReadFile(handle, _buffer, realRead, &realRead, NULL);
    return realRead;
}

//  [3/11/2009 Sasha] --------------------------------------------------
bool ComPort::WriteByte(const BYTE& _B) {
    DWORD realWrite = 0;
    if(WriteFile(handle, (void*)&_B, 1, &realWrite, NULL)) {
        return (realWrite == 1);
    }
    return false;
}

//  [3/12/2009 Sasha] --------------------------------------------------
DWORD ComPort::WriteBuffer(void* _buffer, DWORD _sizeBuffer) {
    DWORD realWrite = 0;
    WriteFile(handle, _buffer, _sizeBuffer, &realWrite, NULL);
    /*
    char *buffer = new char[_sizeBuffer * 4 + 1];
    SequenceBytesToASCII((BYTE*)_buffer, buffer, _sizeBuffer, '.');
    LOG("Write sequence bytes - %s", buffer);
    */
    return realWrite;
}

//----------------------------------------------------------------------
void ComPort::ClearBuffer() {
    PurgeComm(handle, PURGE_TXCLEAR | PURGE_RXCLEAR);
}
