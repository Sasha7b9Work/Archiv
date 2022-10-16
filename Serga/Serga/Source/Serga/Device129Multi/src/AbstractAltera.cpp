#include "AbstractAltera.h"

#pragma warning(disable:4127)
#include <QObject>
#include <QMessageBox>
#pragma warning(default:4127)
#include "../../common/Timer.h"
#include "../../common/GlobalFunctions.h"
#include "../../common/BinaryMathematics.h"

#include <iostream>

using namespace std;
using std::vector;
using namespace NS_AlteraAddressSpace;
using namespace NS_TypesAltera;
using namespace NS_BinaryMathematics;
using namespace NS_GlobalFunctions;

#define tr QObject::tr

AbstractAltera::AbstractAltera():
    port(NULL),
    running(false),
    numReadBytes(-1),
    shift(0) 
{
    port = new ComPort;
}

AbstractAltera::~AbstractAltera() 
{
    SAFE_DELETE(port);
}

int AbstractAltera::GetPorts(short *_ports, int _size) 
{
    vector<short> ports;
    port->Close();
    ports = port->GetComPorts();
    for(int i = 0; i < static_cast<int>(ports.size()); i++)
        if(i < _size)
            _ports[i] = ports[i];

    return ports.size();
}

bool AbstractAltera::CreateDevice(int _numPort) 
{
    port->Close();
    if(port->Open(_numPort))
    {
        port->SetByteSize(8);
        port->SetBaudRate(500000);
        port->SetParity(NOPARITY);
        port->SetStopBits(ONESTOPBIT);
        Write(WRITE_START, 0);
        BYTE buffer[8];
        return Read(READ_FL, 8, buffer); 
    }
    return false;
}

bool AbstractAltera::IsConnect()
{
    static BYTE rezult[8];
    int counter = 0;
    while((counter < 5) && (!Read(READ_FL, 8, rezult)))
        counter++;

    return (counter < 5);
}

bool AbstractAltera::Start(bool _first) 
{
    Write(WRITE_UPR, 0);
    Write(WRITE_START2, 0x12);
    first = _first;
    running = true;
    return true;
}

void AbstractAltera::Stop() 
{
    InternalStop();
    running = false;
}

void AbstractAltera::InternalStop() 
{
    Write(WRITE_STOP2, 255);
}

bool AbstractAltera::IsRunning() 
{
    return running;
}

bool AbstractAltera::DataReady(BYTE &flag) 
{
    if(!running) 
        return false;

    if(!Read(READ_FL, 1, &flag)) 
        return false;
    
    switch (settings.modeADC) 
    {
        case ModeADC_RealTime:
            return ((flag & (BYTE)3) == 3);
        case ModeADC_Randomize:
            return ((flag & (BYTE)0x06) == 6);
        case ModeADC_PointForPoint:
            return GetBit(flag, 4);
        default:
            //LOG("Error int operator switch");
            break;
    }
    return false;
}

void AbstractAltera::SetKoeffCalibrate(int _channel, double _koeff) 
{
    settings.SetK(_channel, _koeff);
}

double AbstractAltera::GetKoeffCalibrate(int _channel) 
{
    return settings.GetK(_channel);
}

bool AbstractAltera::Calibrate(int _channel, uchar _buffer[512]) 
{
    SettingsAltera tempSettings = settings;
    SetTBase(TBase_50us);
    SetRange(_channel, Range_500mv);
    SetModeInput(_channel, ModeAC);
    SetRShift(_channel, 2000);
    SetSinchroChannel(_channel);
    SetTrigLev(_channel, 2000);
    SetStartMode(StartMode_Wait);
    settings.SetK(_channel, 1.0);
    Sleep(1000);
    Start(true);
    Timer timer;
    timer.Start(10000);
    BYTE flag = 0;
    while(timer.IsRun() && ((flag & (BYTE)0x03) != 3)) 
    {
        DataReady(flag);
    }
    bool retValue = false;
    double k = 1.0;
    if((flag & (BYTE)0x03) == 3) 
    {
        if(0 == _channel)
        {
            Read(READ_CONTR2, 512, _buffer);
            Read(READ_CONTR1, 512, _buffer);
        }
        else
        {
            Read(READ_CONTR1, 512, _buffer);
            Read(READ_CONTR2, 512, _buffer);
        }
        Stop();

        int top = 0;        // Здесь количество точек, попадающих в верхние границы
        int sumTop = 0;
        int bottom = 0;
        int sumBottom = 0;
        for(int i = 0; i < DB_SIGNED_LENGTH; i++) 
        {
            if((_buffer[i] > 175) /* && (buffer[_channel][i] < 250) */ ) 
            {
                sumTop += _buffer[i];
                top++;
            }
            if((_buffer[i] < 75)) 
            {
                sumBottom += _buffer[i];
                bottom++;
            }
        }
        if((top >= 200) && (bottom >= 200)) 
        {
            double topLine = (double)sumTop / top;
            double bottomLine = (double)sumBottom / bottom;
            k = 200.0 / (topLine - bottomLine);
            retValue = true;
        }
    }
    settings = tempSettings;
    settings.SetK(_channel, k);
    SetSettings();
    return retValue;
}

bool AbstractAltera::CalibrateZero() 
{

    bool retValue = false;

    if(/* (!settings.GetAutoCalibrate()) || */ (!IsConnect())) {
        return false;
    }

    ModeInput mInput0 = settings.GetModeInput(0);
    ModeInput mInput1 = settings.GetModeInput(1);
    SetModeInput(0, ModeGRND);
    SetModeInput(1, ModeGRND);

    int tBase = settings.GetTBase();
    SetTBase(TBase_1us);

    StartMode startMode = settings.GetStartMode();
    SetStartMode(StartMode_Auto);

    settings.SetAdditionRShift(0, 0);
    int rShift0 = settings.GetRShift(0);
    SetRShift(0, RShiftZero);
    settings.SetAdditionRShift(1, 0);
    int rShift1 = settings.GetRShift(1);
    SetRShift(1, RShiftZero);

    Timer timer;
    timer.Start(200000);
    Start(true);
    BYTE flag = 0;
    while(timer.IsRun() && ((flag & (BYTE)0x03) != 3)) {
        DataReady(flag);
    }
    BYTE buffer[2][DB_LENGTH];
    if((flag & (BYTE)0x03) == 3) {
        retValue = true;
        Read(READ_CONTR1, 512, buffer[0]);
        Read(READ_CONTR2, 512, buffer[1]);
        // Теперь находим среднее значение
        int sum0 = 0;
        int sum1 = 0;
        for(int i = 0; i < DB_LENGTH; i++) {
            sum0 += buffer[0][i];
            sum1 += buffer[1][i];
        }
        double addRShift0 = (double)sum0 / (double)DB_LENGTH;
        double addRshift1 = (double)sum1 / (double)DB_LENGTH;
        cout << "add rShifts = " << addRShift0 << " " << addRshift1 << endl;
        settings.SetAdditionRShift(0, (AVE_VALUE - addRShift0) * 8.0);
        settings.SetAdditionRShift(1, (AVE_VALUE - addRshift1) * 8.0);
    }

    SetModeInput(0, mInput0);
    SetModeInput(1, mInput1);
    SetTBase(tBase);
    SetStartMode(startMode);
    SetRShift(0, rShift0);
    SetRShift(1, rShift1);

    return retValue;
}

void AbstractAltera::SetSettings() 
{
    WriteVoltages(0);
    WriteVoltages(1);
    WriteVoltagesAdditions();
    WriteShiftVoltage(0);
    WriteShiftVoltage(1);
    WriteTimings();
    WriteTrigLevel();
    WriteTrigF();
    WritePredAndPost();
}

void AbstractAltera::SetModeInput(int _channel, ModeInput _modeInput)
{
    if((_channel == 0) || (_channel == 1)) 
    {
        settings.modeInput[_channel] = _modeInput;
        WriteVoltages(0);
        WriteVoltages(1);
    }
    else 
    {
        //LOG("ERROR!!! Invalid input parameter: channel = %d", _channel);
    }
}

void AbstractAltera::SetFiltr(bool _filtr) 
{
    settings.filtr = _filtr;
    WriteVoltagesAdditions();
}

void AbstractAltera::SetTPos(TPos _tPos) 
{
    static TPos prevTPos = settings.tPos;
    if(prevTPos != _tPos) 
    {
        prevTPos = _tPos;
        settings.tPos = _tPos;
        WritePredAndPost();
    }
}

void AbstractAltera::SetRange(int _channel, int _range) 
{
    static int prevRange[2];
    static bool first = true;
    if(first) 
    {
        prevRange[0] = prevRange[1] = -1;
        first = false;
    }
    if((_channel == 0) || (_channel == 1)) 
    {
        if(prevRange[_channel] != _range) 
        {
            if(settings.SetRange(_channel, _range)) 
            {
                WriteVoltages(_channel);
                //CalibrateZero();
            }
        }
    }
    else 
    {
    }
}

void AbstractAltera::SetRShift(int _channel, int _rShift) 
{
    if(settings.SetRShift(_channel, _rShift)) 
        WriteShiftVoltage(_channel);
}

bool AbstractAltera::SetTBase(int _tBase) 
{
    if(settings.SetTBase(_tBase)) 
    {
        bool retValue = WriteTimings();
		WritePredAndPost();
        return retValue;
    }
    else 
    {
        return false;
    }
}

//  [12/9/2009 Sasha]
void AbstractAltera::SetTShift(int _tShift) {
	static int prevTShift = settings.GetTShift();
	if(prevTShift != _tShift) {
		prevTShift = _tShift;
		settings.SetTShift(_tShift);
		WritePredAndPost();
	}
}

//  [12/9/2009 Sasha]
void AbstractAltera::SetTrigLev(int _channel, int _trigLev) {
	static int prevTrigLev0 = settings.GetTrigLev(0);
	static int prevTrigLev1 = settings.GetTrigLev(1);
	if(_channel == 0) {
		if(prevTrigLev0 != _trigLev) {
			prevTrigLev0 = _trigLev;
			settings.SetTrigLev(0, _trigLev);
			if(settings.GetSinchroChannel() == 0) {
				WriteTrigLevel();
			}
		}
	}
	else {
		if(prevTrigLev1 != _trigLev) {
			prevTrigLev1 = _trigLev;
			settings.SetTrigLev(1, _trigLev);
			if(settings.GetSinchroChannel() == 1) {
				WriteTrigLevel();
			}
		}
	}
}

void AbstractAltera::SetSinchroChannel(int _channel) 
{
    if((_channel == 0) || (_channel == 1) || (_channel == -1 )) 
    {
		settings.sinchroChannel = _channel;
		WriteTrigLevel();
		// WriteVoltages(0);
		// WriteVoltages(1);
        WriteVoltagesAdditions();
        //CalibrateZero();
    }
    else 
    {
        //LOG("ERROR!!! Invalid input parameter: channel = %d", _channel);
    }
}

void AbstractAltera::SetSinchroType(Sinchro _sinchroType) 
{
    settings.sinchro = _sinchroType;
    WriteTrigF();
}

void AbstractAltera::SetStartMode(StartMode _startMode) 
{
    settings.startMode = _startMode;
    WriteTrigF();
}

void AbstractAltera::SetCalibrator(bool _calibrator) 
{
    settings.calibrator = _calibrator;
    WriteVoltagesAdditions();
}

void AbstractAltera::WriteVoltages(int _channel) 
{
    // Таблица для записи битов коэффициента отклонения
    static BYTE tableMasks[12][2] = {
        {2, 128},   // 5mv      00000010 10000000
        {4, 64},    // 10mv     00000100 01000000
        {8, 32},    // 20mv     00001000 00100000
        {18, 144},  // 50mv     00010010 10010000
        {20, 80},   // 100mv    00010100 01010000
        {24, 48},   // 200mv    00011000 00110000

        {34, 136},  // 500mv    00100010 10001000
        {36, 72},   // 1v       00100100 01001000
        {40, 40},   // 2v       00101000 00101000
        {50, 152},  // 5v       00110010 10011000
        {52, 88},   // 10v      00110100 01011000
        {56, 56}    // 20v      00111000 00111000
    };

    BYTE dataChannel = 0;

    if(_channel == 0) {
        if(settings.modeInput[0] == ModeAC)
            SetBit(dataChannel, 7);
        if(settings.modeInput[0] == ModeGRND) 
            SetBit(dataChannel, 6);
        dataChannel |= tableMasks[settings.GetRange(0)][0];
        COND_WRITE(Write(WRITE_ATTR_RG1, dataChannel));
    }
    else 
    {
        if(settings.modeInput[1] == ModeAC)
            SetBit(dataChannel, 2);
        if(settings.modeInput[1] == ModeGRND)
            SetBit(dataChannel, 1);
        dataChannel |= tableMasks[settings.GetRange(1)][1];
        COND_WRITE(Write(WRITE_ATTR_RG2, dataChannel));
    }
}

void AbstractAltera::WriteVoltagesAdditions()
{
    BYTE dataChannel = 0;

    if(!settings.calibrator)
        SetBit(dataChannel, 0);

    // 19.04.2012 изменена полярность условие записывается вместо !settings.filtr settings.filtr
    if(settings.filtr)
        SetBit(dataChannel, 7);

    if(1 == settings.sinchroChannel)
        SetBit(dataChannel, 6);

    if(-1 == settings.sinchroChannel)
        SetBit(dataChannel, 1);

    COND_WRITE(Write(WRITE_ATTR_RG4, dataChannel));
}

void AbstractAltera::WriteShiftVoltage(int _channel) 
{
    if(running) 
        InternalStop();
    
    WORD rShift = (WORD)settings.GetRShift(_channel) + (WORD)settings.GetAdditionRShift(_channel);
    rShift &= 0x0fff;
    BYTE loRShift = (BYTE)rShift;
    BYTE hiRShift = (BYTE)(rShift>>8);
    if(_channel == 1)
        SetBit(hiRShift, 7);
    
    Write(WRITE_RSHIFT_LO, loRShift);
    Write(WRITE_RSHIFT_HI, hiRShift);
    Write(WRITE_SHIFT_RGB, 0);
    if(running) 
        Start();
}

//  [12/10/2009 Sasha]
bool AbstractAltera::WriteTimings() {

    // Устанавливаем режим запуска АЦП

    static BYTE table[0x20] = { 0, 0, 0, 0, 0, 0, 0,
        0x23, // 2us
        0x22, // 5us
        0x21, // 10us
        0x63,// 20us
        0x62,// 50us
        0x61,// 100us
        0x67,// 200us
        0x66,// 500us
        0x65,// 1ms
        0x6b,// 2ms
        0x6a,// 5ms
        0x69,// 10ms
        0x6f,// 20ms
        0x6e,// 50ms
        0x6d,// 100ms
        0x73,// 200ms
        0x72,// 500ms
        0x71,// 1s
        0x77,// 2s
    };

    if(running) {
        InternalStop();
    }
    BYTE recValue = table[settings.GetTBase()];
    BYTE readValue = ~recValue;
    int counter = 0;
    while((readValue != recValue) && (counter < 5)) {
        Write(WRITE_RAZV_RG5, recValue);
        Read(READ_RAZV_RG5, 1, &readValue);
        counter++;
    }

    if(running) {
        Start();
    }

    return (counter < 5);
}

//  [12/21/2009 Sasha]
void AbstractAltera::WriteTrigLevel() {
    if(running) {
        InternalStop();
    }
    WORD trigLev = (WORD)settings.GetTrigLev(settings.sinchroChannel);
    // Чтобы изображение курсора соответствовало уровню синхронизации, обратим его относительно TrigLevZero
    //trigLev = (WORD)TrigLevMax - trigLev;
    trigLev &= 0x0fff;
    BYTE loRShift = (BYTE)trigLev;
    BYTE hiRShift = (BYTE)(trigLev>>8);
    SetBit(hiRShift, 6);
    Write(WRITE_RSHIFT_LO, loRShift);
    Write(WRITE_RSHIFT_HI, hiRShift);
    Write(WRITE_SHIFT_RGB, 0);
    if(running) {
        Start();
    }
}

//  [12/21/2009 Sasha]
void AbstractAltera::WriteTrigF() {
    BYTE data = 0;
    if(Sinchro_Back == settings.sinchro) {
        SetBit(data, 0);
    }
    if(StartMode_Auto == settings.startMode) {
        SetBit(data, 1);
    }
    COND_WRITE(Write(WRITE_TRIG_F, data));
}

//  [12/21/2009 Sasha]
void AbstractAltera::WritePredAndPost() {
    int K = 0;
    int M = 0;
    int N = 0;

    static int tableKReal[3] = {0, 250, 500};
    static int tableShiftMul[6] = {1, 1, 1, 1, 5, 5};
    static int tableShiftDiv[6] = {10, 4, 2, 1, 2, 1};

    int tShift = settings.GetTShift();

    if((ModeADC_RealTime == settings.modeADC) ||(ModeADC_PointForPoint == settings.modeADC)) {
        K = tableKReal[settings.tPos];
        M = K - tShift + 0x1ff;
        Limitation(M, 1, 0x1f4);
        N = tShift - K + 10;
        Limitation(N, 10, 0x3fff);
        N = DWORD(0x3fff) - DWORD(N);
        if(N <= 0) {
            N = 1;
        }
    }
    else {
        static int tableId[] = {100, 50, 20, 10, 5, 2};
        static int tableKRandom[6][3] = {
            {0, 3,    0x5},
            {0, 5,    0xa},
            {0, 0xd,  0x19},
            {0, 0x19, 0x32},
            {0, 0x32, 0x64},
            {0, 0x7d, 0xfa}
        };
        int tBase = settings.GetTBase();
        WORD P = (WORD)(tShift / tableId[tBase]);

        shift = tShift - P * tableId[tBase];
        /*
        if((tBase == 0) && (settings.tPos == 1))
            shift = 50;
        else if((tBase == 2) && (settings.tPos == 1))
            shift = 10;
        else
            shift = 0;
            */

        K = tableKRandom[tBase][settings.tPos];
        M = K - P + 512 / tableId[tBase];
        N = 0x1ff - M;
        Limitation(N, 10, 0x3fff);
        Limitation(M, 1, 512);
        N = DWORD(0x3fff) - DWORD(N) - 2;
        if(N <= 0) {
            N = 1;
        }
    }
    if(running) {
        InternalStop();
    }
    Write(WRITE_POST_RG6, (BYTE)N);
    Write(WRITE_POST_RG7, (BYTE)(N>>8));
    Write(WRITE_PRED_RG3, (BYTE)M);
    Write(WRITE_PRED_RG8, (BYTE)(M>>8));
    if(running) {
        Start();
    }
}

bool AbstractAltera::Write(WRITE _address, BYTE _data) 
{
    WriteNumBytes(1);
    BYTE buffer[2] = {~_address, ~_data};
    DWORD writtingBytes = port->WriteBuffer(buffer, 2);
    if(writtingBytes != 2) 
    {
        // LOG("ERROR!!! Failed write in address 0x%0x", _address);
    }
    return (writtingBytes == 2);
}

bool AbstractAltera::Read(READ _address, DWORD _numBytes, BYTE *_buffer) 
{
    //LOG("Enter Read()");
    static Timer timer;
    WriteNumBytes(_numBytes);
    DWORD bytes = 0;
    port->ClearBuffer();
    port->WriteByte(~_address);
    timer.Start(TIME_ONE_BYTE * _numBytes);  // Устанавливаем максимальное время ожидания c учётом времени передачи одного байта
    while(((bytes = port->GetBytesInQue()) != _numBytes) && timer.IsRun()) 
    {

    }

    bytes = port->GetBytesInQue();
	bool condition = (bytes == _numBytes);
    if(condition) 
    {
        port->ReadBuffer(_buffer, _numBytes);
        for(DWORD i = 0; i < _numBytes; i++) 
            _buffer[i] = ~_buffer[i];
    }
    else
    {
        // cout << "ERROR!!! bytes request = " << (int)_numBytes << ", bytes recieve = " << (int)bytes << endl;
    }
    return condition;
}

bool AbstractAltera::WriteNumBytes(DWORD _numBytes) 
{
// Выключаем  warning C4245: 'initializing' : conversion from 'int' to 'BYTE', signed/unsigned mismatch
#pragma warning(disable:4245)
    static BYTE buffer[2] = {~((BYTE)WRITE_N_BYTES), 0};
#pragma warning(default:4245)
    buffer[1] = (1 == _numBytes) ? ~((BYTE)1) : ~((BYTE)(_numBytes>>2));
    return (port->WriteBuffer(buffer, 2) == _numBytes);
}

#undef tr