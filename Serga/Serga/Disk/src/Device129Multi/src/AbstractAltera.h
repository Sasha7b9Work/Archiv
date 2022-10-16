//  [12/9/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QString>
#pragma warning(default:4127)
#include <vector>
#include "../../common/TypesAltera.h"
#include "../../common/defines.h"
#include "SettingsAltera.h"
#include "ComPort.h"
#include "AddressSpaceAltera.h"

class AbstractAltera {
#define COND_WRITE(x)               \
        if(running) InternalStop(); \
        x;                          \
        if(running) Start();

public:
    AbstractAltera();
    virtual ~AbstractAltera();

    SettingsAltera settings;

    int GetPorts(short *_ports, int _size);        // Получить список портов для заполнения комбобокса
    bool CreateDevice(int _numPort);        // Создать устройство на ком-порте с индексом indexPort из комбобокса портов, заполненного из выхода
                                             // функции GetPorts(). Функция возвращает false, если связь с устройством установить не удалось.
                                             // Загрузка настроек не производится
    bool IsConnect();                        // Возвращает true, если устройство с устройсвом есть связь
    virtual bool Start(bool first = true);			 // first имеет значение для режима рандомизатора
    virtual void Stop();                             // Внешний стоп. Он вызывает внутренний и устанавливает признак того, что альтера в остановленном состоянии
    bool IsRunning();
    virtual bool DataReady(BYTE &flag);

    void SetKoeffCalibrate(int channel, double koeff);
    double GetKoeffCalibrate(int channel);
    virtual bool Calibrate(int channel, uchar _buffer[501]);             // Калибровать. Если калибровка не прошла, вернуть false

    bool CalibrateZero();                               // Установка нуля

    void SetAddr_Min(double addr_Min) { settings.addr_Min = addr_Min; };
    double GetAddr_Min()              { return settings.addr_Min; }
    void SetAddr_Max(double addr_Max) { settings.addr_Max = addr_Max; };
    double GetAddr_Max()              { return settings.addr_Max; }

	// Загрузка настроек в альтеру
    void SetSettings();                      // Загружает ранее установленные настройки
    void SetFiltr(bool filtr);
    void SetTPos(NS_TypesAltera::TPos tPos);

    void SetRange(int channel, int range);
    void SetRShift(int channel, int rShift);
    bool SetTBase(int tBase);
    void SetTShift(int tShift);
    void SetTrigLev(int channel, int trigLev);
    void SetSinchroChannel(int channel);
    void SetSinchroType(NS_TypesAltera::Sinchro sinchroType);
    void SetStartMode(NS_TypesAltera::StartMode startMode);
    void SetCalibrator(bool calibrator);
    void SetModeInput(int _channel, NS_TypesAltera::ModeInput _modeInput);

    bool Write(NS_AlteraAddressSpace::WRITE address, BYTE data);

protected:
    ComPort *port;
    bool running;
    bool first;
    int numReadBytes;            // Здесь хранится число считываемых байт
    DWORD shift;                // Для режима рандомизатора

    void InternalStop();        // "Внутренний стоп". Отличие от внешнего в том, что признак рабочего состояния не сбрасывается.
                                // Его нужно писать перед считыванием данных и изменением настроек
    void WriteVoltages(int channel);
    void WriteVoltagesAdditions();
    void WriteShiftVoltage(int channel);
    bool WriteTimings();
    void WriteTrigLevel();
    void WriteTrigF();
    void WritePredAndPost();

    bool Read(NS_AlteraAddressSpace::READ address, DWORD numBytes, BYTE *buffer);
    bool WriteNumBytes(DWORD numBytes);         // Записать требуемое число байт
};
