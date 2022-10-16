//  [3/2/2010 Sasha]
#include "Device.h"
#include "FastLog.h"
#include "Timer.h"

using std::vector;

extern bool debugMode;

//  [3/2/2010 Sasha]
Device::Device() {
    writeBuffer[0] = writeBuffer[1] = 255;
    ClearReadBuffer();
}

//  [3/2/2010 Sasha]
vector<QString>& Device::GetPorts() {
    ports.clear();
    port.Close();
    ports = port.GetComPorts();
    static vector<QString> stringPorts;
    stringPorts.clear();
    QString str;
    size_t maxI = ports.size();
    for(size_t i = 0; i < maxI; i++) {
        stringPorts.push_back(QString(("COM")) + str.setNum(ports[(int)i], 16));
    }
    return stringPorts;
}

//  [3/2/2010 Sasha]
bool Device::CreateDevice(int _indexPort, int &_model, bool _create) {

    if(debugMode) {
        _model = 0;
        return true;
    }

    if((_indexPort < 0) || (_indexPort >= (int)ports.size())) {
        _model = 100;
        return false;
    }
    port.Close();
    if(_create) {
        if(port.Open(ports[_indexPort])) {
            port.SetByteSize(8);
            port.SetBaudRate(115200);
            port.SetParity(NOPARITY);
            port.SetStopBits(ONESTOPBIT);
            bool connect = false;
            int counter = 0;
            while((!connect) && (counter < 5)) {
                connect = IsConnectToComputer(_model);
                counter++;
            }
            return (counter < 5);
        }
    }
    return true;
}

//  [3/2/2010 Sasha]
bool Device::Start() {
    port.WriteBuffer(FillWriteBuffer(ADDR_Start), SIZE_COM);
    return ReadAnswer();
}

//  [3/2/2010 Sasha]
bool Device::Stop() {
    port.WriteBuffer(FillWriteBuffer(ADDR_Stop), SIZE_COM);
    return ReadAnswer();
}

//  [3/2/2010 Sasha]
bool Device::WriteU(int _u) {
    FillWriteBuffer(ADDR_WriteU);
    WriteIntInBytes(_u, &writeBuffer[3]);
    port.WriteBuffer(writeBuffer, SIZE_COM);
    return ReadAnswer();
}

//  [3/2/2010 Sasha]
bool Device::WriteI(int _i) {
    FillWriteBuffer(ADDR_WriteI);
    WriteIntInBytes(_i, &writeBuffer[3]);
    port.WriteBuffer(writeBuffer, SIZE_COM);
    return ReadAnswer();
}

//  [3/2/2010 Sasha]
bool Device::ReadU(int &_uMeas) {
    if(debugMode) {
        _uMeas = 500;
        return true;
    }
    port.WriteBuffer(FillWriteBuffer(ADDR_ReadU), SIZE_COM);
    ClearReadBuffer();
    bool retValue = ReadAnswer();
    _uMeas = BytesInInt(&readBuffer[3]);
    return retValue;
}

//  [3/2/2010 Sasha]
bool Device::ReadI(int &_iMeas) {
	if(debugMode) {
		_iMeas = 500;
		return true;
	}
	port.WriteBuffer(FillWriteBuffer(ADDR_ReadI), SIZE_COM);
	ClearReadBuffer();
	bool retValue = ReadAnswer();
	_iMeas = BytesInInt(&readBuffer[3]);
	return retValue;
}

//  [5/31/2010 Sasha]
bool Device::ReadSetU(int &_uSet) {
	if(debugMode) {
		_uSet = 500;
		return true;
	}
	port.WriteBuffer(FillWriteBuffer(ADDR_ReadSetU), SIZE_COM);
	ClearReadBuffer();
	bool retValue = ReadAnswer();
	_uSet = BytesInInt(&readBuffer[3]);
	return retValue;
}

//  [5/31/2010 Sasha]
bool Device::ReadSetI(int &_iSet) {
	if(debugMode) {
		_iSet = 500;
		return true;
	}
	port.WriteBuffer(FillWriteBuffer(ADDR_ReadSetI), SIZE_COM);
	ClearReadBuffer();
	bool retValue = ReadAnswer();
	_iSet = BytesInInt(&readBuffer[3]);
	return retValue;
}

//  [3/5/2010 Sasha]
bool Device::Enable(bool _enable) {
    BYTE bEnable = _enable ? 1 : 0;
    port.WriteBuffer(FillWriteBuffer(ADDR_Other, bEnable), SIZE_COM);
    ClearReadBuffer();
    return ReadAnswer();
}

//  [3/5/2010 Sasha]
bool Device::IsConnectToComputer(int &_model) {
	port.WriteBuffer(FillWriteBuffer(ADDR_Other, 2), SIZE_COM);
	ClearReadBuffer();
    if(ReadAnswer()) {
        _model = (int)readBuffer[4];
        if((0 == _model) || (1 == _model)) {
            return true;
        }
    }
    _model = 2;
    return false;
}

//  [3/12/2010 Sasha]
bool Device::ReadAnswer() {
    Timer timer;
    timer.Start(TIME_WAIT1);
    DWORD bytes = 0;
    while(((bytes = port.GetBytesInQue()) < SIZE_COM) && timer.IsRun()) {
    }
    DWORD readBytes = 0;
	if((SIZE_COM <= bytes) && (SIZE_COM * 2 > bytes)) {
        readBytes = port.ReadBuffer(readBuffer, SIZE_COM);
        if(IsWaitDevice()) {
            timer.Start(TIME_WAIT2);
            while(((bytes = port.GetBytesInQue()) < SIZE_COM) && timer.IsRun()) {
            }
            if(SIZE_COM == bytes) {
                readBytes = port.ReadBuffer(readBuffer, SIZE_COM);
            }
        }
        else {
            ERROR_LOG("Error wait read");
        }
    }
	else if(SIZE_COM * 2 == bytes) {
		readBytes = port.ReadBuffer(readBuffer, SIZE_COM);
		readBytes = port.ReadBuffer(readBuffer, SIZE_COM);
	}
    else {
        ERROR_LOG("Error read");
    }
    port.ClearBuffer();
    return (SIZE_COM == readBytes);
}

//  [3/2/2010 Sasha]
void Device::WriteIntInBytes(int _value, BYTE *_buffer) {
    *(_buffer  + 1)= (BYTE)_value;
    *(_buffer) = (BYTE)(_value>>8);
}

//  [3/2/2010 Sasha]
int Device::BytesInInt(BYTE *_buffer) {
    return ((int)(*(_buffer + 1)) + (int)(*_buffer) * 256);
}

//  [3/10/2010 Sasha]
void Device::ClearReadBuffer() {
    ZeroMemory(readBuffer, SIZE_COM);
}

//  [3/10/2010 Sasha]
BYTE* Device::FillWriteBuffer(BYTE _byte2, BYTE _byte3, BYTE _byte4) {
    writeBuffer[2] = _byte2;
    writeBuffer[3] = _byte3;
    writeBuffer[4] = _byte4;
    return writeBuffer;
}

//  [3/12/2010 Sasha]
bool Device::IsWaitDevice() {
    for(int i = 0; i < SIZE_COM; i++) {
        if(readBuffer[i] != 255) {
            return false;
        }
    }
    return true;
}