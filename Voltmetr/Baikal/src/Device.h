//  [3/2/2010 Sasha]
#pragma once
#include "ComPort.h"

enum ADDR {
    ADDR_WriteU = 1,
    ADDR_WriteI,
    ADDR_Start,
    ADDR_Stop,
    ADDR_ReadU,
    ADDR_ReadI,
    ADDR_Other,
	ADDR_ReadSetU,
	ADDR_ReadSetI
};

#define SIZE_COM 5
// ����� �������� ������� ������
#define TIME_WAIT1 10000
// ����� �������� ������� ������
#define TIME_WAIT2 1000000

class Device {
public:
    Device();
    std::vector<QString> &GetPorts();
    bool CreateDevice(int indexPort, int &model, bool create);

    bool Start();
    bool Stop();
    bool WriteU(int u);
    bool WriteI(int i);
    bool ReadU(int &uMeas);
    bool ReadI(int &uMeas);
	bool ReadSetU(int &uSet);
	bool ReadSetI(int &iSet);
    bool Enable(bool enable);
private:
    ComPort port;
    std::vector<short> ports;
    BYTE writeBuffer[SIZE_COM];
    BYTE readBuffer[SIZE_COM];
    bool ReadAnswer();                              // ������ ����� �� ������ ���������
    void WriteIntInBytes(int value, BYTE *buffer);
    int BytesInInt(BYTE *buffer);
    void ClearReadBuffer();
    BYTE* FillWriteBuffer(BYTE byte2, BYTE byte3 = 0, BYTE byte4 = 0);
    bool IsWaitDevice();        // ���������� true, ���� ��� 5 �������� ���� ����� 255
    bool IsConnectToComputer(int &model);         // ���� ���������� ���������� � �����, ���������� true � ����� ������. 0 - 30�, 1 - 100�
};