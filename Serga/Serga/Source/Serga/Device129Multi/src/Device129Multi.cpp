#include <QMutex>
#include <QFile>
#include "Device129Multi.h"
#include "../../common/defines.h"
#include "Math.h"
#include "AbstractAltera.h"
#include "AlteraOscillograph.h"
#include "AlteraVoltmeter.h"
#include "AlteraRegistrator.h"
#include "AlteraAnalyzer.h"
#include "../../common/TypesAltera.h"
#include "../../common/BinaryMathematics.h"
#include "../../common/TablesOfConverting.h"
#include "MathMeasures.h"
#include "MathAnalyzer.h"
#include "DataAnalyzer.h"
#include "SettingsAltera.h"
#include "../common/MathDisplayOscilloscope.h"

using namespace NS_TypesAltera;
using namespace NS_TypesMath;
using namespace NS_BinaryMathematics;

TablesOfConverting tables;

static const int NUM_DEVICES = 4;

Math math[NUM_DEVICES];
MathMeasures *mathMeasures[NUM_DEVICES];
MathAnalyzer mathAnalyzer[NUM_DEVICES];
MathDisplayOscilloscope mathDisplayOscilloscope;

AbstractAltera* altera[NUM_DEVICES];

AlteraOscillograph* alteraOsci[NUM_DEVICES];
static DataOscillograph*   dataOsci[NUM_DEVICES];
bool dataIsReading[NUM_DEVICES];

AlteraVoltmeter*    alteraVoltmeter[NUM_DEVICES];
AbstractData*       dataVoltmeter[NUM_DEVICES];
static int          numAverage[NUM_DEVICES];        // Для расчёта RMS и PP в вольтметре
static CircularStack<double> stackUpp[NUM_DEVICES][2];
static CircularStack<double> stackUrms[NUM_DEVICES][2];
static bool         firstRMS[NUM_DEVICES];
static bool         firstPP[NUM_DEVICES];

AlteraAnalyzer*     alteraAnalyzer[NUM_DEVICES];
DataAnalyzer*       dataAnalyzer[NUM_DEVICES];
int fBase[NUM_DEVICES][2];

AlteraRegistrator*  alteraRegistrator[NUM_DEVICES];

ModeDevice mode[NUM_DEVICES];
static bool existsDevice[NUM_DEVICES];      // Если true, значит, устройство с этим номером существует (подключено к порту)

AlteraOscillograph alteraPorts;             // Используется для получения списка портов

BOOL WINAPI DllMain(_In_ HANDLE /*_HDllHandle*/, _In_ DWORD _Reason, _In_opt_ LPVOID /*_Reserved*/)
{
    static bool firstEnter = true;

    switch(_Reason)
    {
    case DLL_PROCESS_ATTACH:
        if(firstEnter)                          // This sequence only first enter in DllMain
        {
            for(int i = 0; i < NUM_DEVICES; i++)
            {
                mathMeasures[i] = 0;
                altera[i] = 0;
                alteraOsci[i] = 0;
                dataOsci[i] = 0;
                dataIsReading[i] = false;
                alteraVoltmeter[i] = 0;
                dataVoltmeter[i] = 0;
                numAverage[i] = 32;
                alteraAnalyzer[i] = 0;
                dataAnalyzer[i] = 0;
                alteraRegistrator[i] = 0;
                mode[i] = NO_DEVICE;
                firstPP[i] = true;
                firstRMS[i] = true;
                fBase[i][0] = fBase[i][1] = -1;
                existsDevice[i] = false;
            }
            firstEnter = false;
        }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport) int __stdcall D129_GetRange(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return 0;
    
    if((ModeOscilloscope == mode[_numDevice]) && (_channel == 2))
        return dataOsci[_numDevice]->rangeMath;

    return altera[_numDevice]->settings.GetRange(_channel);
};

__declspec(dllexport)int __stdcall D129_GetTPos(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetTPos();
}

__declspec(dllexport)int __stdcall D129_GetTBase(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetTBase();
}

__declspec(dllexport)double __stdcall D129_GetTShiftAbs(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetTShiftAbs();
}

__declspec(dllexport)int __stdcall D129_GetTShiftRel(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetTShift();
}

__declspec(dllexport)int __stdcall D129_GetModeInput(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetModeInput(_channel);
}

__declspec(dllexport)void __stdcall D129_SetKoeffCalibrate(int _numDevice, int _channel, double _koeff)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetKoeffCalibrate(_channel, _koeff);
}

__declspec(dllexport)void __stdcall D129_SetAddr_Min(int _numDevice, double _value)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetAddr_Min(_value);
}

__declspec(dllexport)void __stdcall D129_SetAddr_Max(int _numDevice, double _value)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetAddr_Max(_value);
}

__declspec(dllexport)void __stdcall D129_SetRShiftRel(int _numDevice, int _channel, int _rShift)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetRShift(_channel, _rShift);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_ResetAddrMinMax(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return;

    alteraOsci[_numDevice]->ResetAddrMinMax();
}

__declspec(dllexport)void __stdcall D129_SetAdditionRShift(int _numDevice, int _channel, int _additionRShift)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->settings.SetAdditionRShift(_channel, _additionRShift);
}

__declspec(dllexport)int __stdcall D129_GetSinchroChannel(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetSinchroChannel();
}

__declspec(dllexport)int __stdcall D129_GetRShiftRel(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetRShift(_channel);
}

__declspec(dllexport)double __stdcall D129_GetRShiftAbs(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetRShiftAbs(_channel);
}

__declspec(dllexport)int __stdcall D129_GetTrigLevRel(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetTrigLev(_channel);
}

__declspec(dllexport)double __stdcall D129_GetTrigLevAbs(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetTrigLevAbs(_channel);
}

__declspec(dllexport)int __stdcall D129_DataReadyForRead(int _numDevice, unsigned char &_flag)
{
    if(!existsDevice[_numDevice])
        return -1;

    int retValue = 0;

    if(ModeOscilloscope == mode[_numDevice])
    {
        retValue = altera[_numDevice]->DataReady(_flag) ? 1 : 0;
    }
    else if(ModeVoltmeter == mode[_numDevice])
    {
        retValue = altera[_numDevice]->DataReady(_flag) ? 1 : 0;
    }
    else if(ModeAnalyzer == mode[_numDevice])
    {
        bool answer = false;
        bool synchro = false;
        retValue = alteraAnalyzer[_numDevice]->DataReady(answer, synchro) ? 1 : 0;
    }

    return retValue;    
}

__declspec(dllexport)int __stdcall D129_DataReadyForReadRegistrator(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return 0;

    if(ModeRegistrator == mode[_numDevice])
    {
        return alteraRegistrator[_numDevice]->DataReady() ? 1  : 0;
    }

    return 0;
}

__declspec(dllexport)double __stdcall D129_GetValue(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return -1;

    if(ModeRegistrator == mode[_numDevice])
    {
        return alteraRegistrator[_numDevice]->GetValue(_channel);
    }

    return 0;
}

__declspec(dllexport)int __stdcall D129_DataReadyForDraw(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    int retValue = 0;

    if(ModeOscilloscope == mode[_numDevice] && dataOsci[_numDevice] && dataIsReading[_numDevice])
        retValue = dataOsci[_numDevice]->isReady ? 1 : 0;
    else if(ModeAnalyzer == mode[_numDevice] && dataAnalyzer[_numDevice])
        retValue = dataAnalyzer[_numDevice]->isReady ? 1 : 0;

    return retValue;
}

__declspec(dllexport)double* _stdcall D129_GetPointerToDataDraw(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return 0;

    double *retValue = 0;

    if(ModeOscilloscope == mode[_numDevice])
        retValue = dataOsci[_numDevice]->channelDraw[_channel];
    else if(ModeVoltmeter == mode[_numDevice])
        retValue = dataVoltmeter[_numDevice]->dataDraw[_channel];
    else if(ModeAnalyzer == mode[_numDevice])
        retValue = dataAnalyzer[_numDevice]->dataAbs[_channel];

    return retValue;
}

__declspec(dllexport)void __stdcall D129_GetDataDraw(int _numDevice, int _channel, double _buffer[501], int _size)
{
    if(!existsDevice[_numDevice])
        return;

    double *data = D129_GetPointerToDataDraw(_numDevice, _channel);
    for(int i = 0; i < DB_SIGNED_LENGTH; i++)
        if(i < _size)
            _buffer[i] = data[i];
}

__declspec(dllexport)int __stdcall D129_MathFunctionIsEnable(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return 0;

    return dataOsci[_numDevice]->mathFunction ? 1 : 0;
}

__declspec(dllexport)void __stdcall D129_GetDataMathFunction(int _numDevice, double _buffer[501])
{
    if(!existsDevice[_numDevice])
        return;

    double *dataBuffer = D129_GetPointerToDataMathFunction(_numDevice);
    for(int i = 0; i < DB_SIGNED_LENGTH; i++)
        _buffer[i] = dataBuffer[i];
}

__declspec(dllexport)double* __stdcall D129_GetPointerToDataMathFunction(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return 0;

    return dataOsci[_numDevice]->channelDraw[2];
}

__declspec(dllexport)int __stdcall D129_IsSpectrView(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return 0;

    if((0 == _channel) && (0 == dataOsci[_numDevice]->viewSpectr))
        return 1;

    if((1 == _channel) && (1 == dataOsci[_numDevice]->viewSpectr))
        return 1;
 
    return 0;
}

__declspec(dllexport)void __stdcall D129_GetDataSpectr(int _numDevice, int _channel, double _buffer[501])
{
    if(!existsDevice[_numDevice])
        return;

    double *dataSpectr = D129_GetPointerToSpectr(_numDevice, _channel);
    for(int i = 0; i < DB_SIGNED_LENGTH; i++)
        _buffer[i] = dataSpectr[i];
}

__declspec(dllexport)double* _stdcall D129_GetPointerToSpectr(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return 0;

    return dataOsci[_numDevice]->spectrDraw[_channel];
}

__declspec(dllexport)int __stdcall D129_GetModeADC(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetModeADC();
}

__declspec(dllexport)int __stdcall D129_GetStartMode(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetStartMode();
}

__declspec(dllexport)double __stdcall D129_GetFraction(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    if(mode[_numDevice] == ModeOscilloscope)
    {
        return dataOsci[_numDevice]->GetFraction();
    }
    
    return INVALID_VALUE_DOUBLE;
}

__declspec(dllexport)int __stdcall D129_GetPorts(short *_ports, int _size)
{
    int retValue = alteraPorts.GetPorts(_ports, _size);
    return retValue;
}

__declspec(dllexport)int __stdcall D129_GetAdditionRShift(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetAdditionRShift(_channel);
}

__declspec(dllexport)double __stdcall D129_GetKoeffCalibrate(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->GetKoeffCalibrate(_channel);
}

__declspec(dllexport)double __stdcall D129_GetAddr_Min(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->GetAddr_Min();
}

__declspec(dllexport)double __stdcall D129_GetAddr_Max(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->GetAddr_Max();
}

__declspec(dllexport)double __stdcall D129_GetMeasure(int _numDevice, int _channel, int _func, char * _buffer, int _size)
{
    if(!existsDevice[_numDevice])
        return -1;

    return mathMeasures[_numDevice]->GetMeasure(_channel, _func, _buffer, _size);
}

__declspec(dllexport)double __stdcall D129_CalculateRMS(int _numDevice, int _channel, char _buffer[15], int _size)
{
    if(!existsDevice[_numDevice])
        return -1;

    int periodic = 0;    

    if(firstRMS[_numDevice]) 
    {
        stackUrms[_numDevice][0].Reset(numAverage[_numDevice]);
        stackUrms[_numDevice][1].Reset(numAverage[_numDevice]);
        firstRMS[_numDevice] = false;
    }

    double *input = dataVoltmeter[_numDevice]->dataAbs[_channel];

    if((CalculateVoltageMax(_numDevice, input, _channel) == INVALID_VALUE_DOUBLE) || (CalculateVoltageMin(_numDevice, input, _channel) == INVALID_VALUE_DOUBLE)) 
    {
        stackUrms[_numDevice][_channel].Reset(numAverage[_numDevice]);
        strcpy_s(_buffer, _size, EMPTY_STRING);
        return INVALID_VALUE_DOUBLE;
    }

    double arraySum[DB_SIGNED_LENGTH] = {0.0};
    arraySum[0] = input[0];
    for(int i = 1; i < DB_SIGNED_LENGTH; i++)
        arraySum[i] = arraySum[i-1] + input[i];

    double deltaSumIMax[DB_SIGNED_LENGTH];   // Здесь будут храниться максимальные разности подсчёта сумм при сдвиге начальной точки на 100 для каждого периода
    for(int i = 0; i < DB_SIGNED_LENGTH; i++)
        deltaSumIMax[i] = 0.0;

    // Предполагаемый период изменяем от 400 до 200
    for(periodic = DB_SIGNED_LENGTH - 100; periodic > 200; periodic--) 
    {
        // Находим первую - образцовую - сумму текущего периода
        double sum0 = arraySum[periodic - 1];
        // Теперь найдём суммы на том же периоде при сдвиге начальной точки на 99 позиций
        for(int start = 1; start < 100; start++) 
        {
            double sum = arraySum[start + periodic - 1] - arraySum[start - 1];
            if(fabs(sum0 - sum) > fabs(deltaSumIMax[periodic]))
                deltaSumIMax[periodic] = sum0 - sum;
        }
    }

    periodic++;

    for(int i = periodic; i < DB_SIGNED_LENGTH - 100; i++)
        if(fabs(deltaSumIMax[i]) < fabs(deltaSumIMax[periodic]))
            periodic = i;

    double RMS = 0.0;
    int counter = 0;
    for(int i = 0; i < periodic; i++) 
    {
        RMS += input[i] * input[i];
        counter++;
    }

    RMS = sqrt(RMS/counter);
    stackUrms[_numDevice][_channel].Push(RMS);
    double retValue = stackUrms[_numDevice][_channel].GetAverage();
    strcpy_s(_buffer, _size, VoltageToString(retValue, 4).toAscii());
    return retValue;
}

__declspec(dllexport)double __stdcall D129_CalculatePP(int _numDevice, int _channel, char _buffer[15], int _size)
{
    if(!existsDevice[_numDevice])
        return -1;

    if(firstPP[_numDevice]) {
        stackUpp[_numDevice][0].Reset(numAverage[_numDevice]);
        stackUpp[_numDevice][1].Reset(numAverage[_numDevice]);
        firstPP[_numDevice] = false;
    }

    double *input = dataVoltmeter[_numDevice]->dataAbs[_channel];

    double max = CalculateVoltageMax(_numDevice, input, _channel);
    double min = CalculateVoltageMin(_numDevice, input, _channel);

    if((max == INVALID_VALUE_DOUBLE) || (min == INVALID_VALUE_DOUBLE)) 
    {
        stackUpp[_numDevice][_channel].Reset(numAverage[_numDevice]);
        strcpy_s(_buffer, _size, EMPTY_STRING);
        return INVALID_VALUE_DOUBLE;
    }

    stackUpp[_numDevice][_channel].Push(fabs(max - min));
    double retValue = stackUpp[_numDevice][_channel].GetAverage();
    strcpy_s(_buffer, _size, VoltageToString(retValue, 4).toAscii());
    return retValue;
}

__declspec(dllexport)void __stdcall D129_Stop(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->Stop();
}

__declspec(dllexport)void __stdcall D129_Start(int _numDevice, bool _first)
{
    if(!existsDevice[_numDevice])
        return;

    dataIsReading[_numDevice] = false;
    altera[_numDevice]->Start(_first);
}

__declspec(dllexport)int __stdcall D129_CreateDevice(int _modeDevice)
{
    // Находим первый свободный номер
    int numDevice = 0;
    for(numDevice = 1; numDevice < NUM_DEVICES; numDevice++)
        if(!existsDevice[numDevice])
            break;

    D129_DestroyDevice(numDevice);

    mode[numDevice] = (ModeDevice)_modeDevice;
    existsDevice[numDevice] = true;

    switch(mode[numDevice])
    {

    case ModeOscilloscope:
        alteraOsci[numDevice] = new AlteraOscillograph;
        mathMeasures[numDevice] = new MathMeasures(alteraOsci[numDevice]);
        altera[numDevice] = alteraOsci[numDevice];
        dataOsci[numDevice] = 0;
        break;

    case ModeVoltmeter:
        alteraVoltmeter[numDevice] = new AlteraVoltmeter;
        altera[numDevice] = alteraVoltmeter[numDevice];
        D129_SetSinchroType(numDevice, Sinchro_Front);
        break;

    case ModeAnalyzer:
        alteraAnalyzer[numDevice] = new AlteraAnalyzer;
        altera[numDevice] = alteraAnalyzer[numDevice];
        break;

    case ModeRegistrator:
        alteraRegistrator[numDevice] = new AlteraRegistrator;
        altera[numDevice] = alteraRegistrator[numDevice];
        break;
    default:

        numDevice = -1;
        break;
    }

    return numDevice;
}

__declspec(dllexport)int __stdcall D129_ConnectDeviceToPort(int _numDevice, short _numPort)
{
    existsDevice[_numDevice] = altera[_numDevice]->CreateDevice(_numPort);
    return existsDevice[_numDevice] ? 1 : 0;
}

__declspec(dllexport)void __stdcall D129_DestroyDevice(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return;

    SAFE_DELETE(altera[_numDevice]);
    alteraOsci[_numDevice] = 0;
    alteraVoltmeter[_numDevice] = 0;
    alteraAnalyzer[_numDevice] = 0;
    alteraRegistrator[_numDevice] = 0;

    dataOsci[_numDevice] = 0;
    dataAnalyzer[_numDevice] = 0;
    dataVoltmeter[_numDevice] = 0;

    SAFE_DELETE(mathMeasures[_numDevice]);

    existsDevice[_numDevice] = false;
}

__declspec(dllexport)void __stdcall D129_SetSettings(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetSettings();
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetRange(int _numDevice, int _channel, int _range)
{
    if(existsDevice[_numDevice])
    {
        altera[_numDevice]->SetRange(_channel, _range);
        math[_numDevice].Reset();
    }
}

__declspec(dllexport)void __stdcall D129_SetTrigLevRel(int _numDevice, int _channel, int _trigLev)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetTrigLev(_channel, _trigLev);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetTBase(int _numDevice, int _tBase)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetTBase(_tBase);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetFBase(int _numDevice, int _channel, int _fBase)
{
    if(!existsDevice[_numDevice])
        return;

    fBase[_numDevice][_channel] = _fBase;
    alteraAnalyzer[_numDevice]->SetFBase(_fBase);
}

__declspec(dllexport)void __stdcall D129_SetTShiftRel(int _numDevice, int _tShift)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetTShift(_tShift);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetTPos(int _numDevice, int _tPos)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetTPos((TPos)_tPos);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetSinchroChannel(int _numDevice, int _channel)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetSinchroChannel(_channel);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetSinchroType(int _numDevice, int _type)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetSinchroType((Sinchro)_type);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetStartMode(int _numDevice, int _startMode)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetStartMode((StartMode)_startMode);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetModeInput(int _numDevice, int _channel, int _modeInput)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetModeInput(_channel, (ModeInput)_modeInput);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetFiltr(int _numDevice, bool _filtr)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetFiltr(_filtr);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetInvert(int _numDevice, int _channel, bool _invert)
{
    if(!existsDevice[_numDevice])
        return;

    math[_numDevice].SetInvert(_channel, _invert);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetMathFunction(int _numDevice, int _function, int _enable)
{
    if(!existsDevice[_numDevice])
        return;

    math[_numDevice].SetMathFunction((Function)_function, _enable);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetModePalm(int _numDevice, int _channel, bool _palm)
{
    if(!existsDevice[_numDevice])
        return;

    math[_numDevice].SetModePalm(_channel, _palm);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetPointsPalm(int _numDevice, int _numPoints)
{
    if(!existsDevice[_numDevice])
        return;

    math[_numDevice].SetPointsPalm(_numPoints);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetModeAverage(int _numDevice, int _average)
{
    if(!existsDevice[_numDevice])
        return;

    math[_numDevice].SetModeAverage((bool)_average);
    math[_numDevice].Reset();
}

__declspec(dllexport)void __stdcall D129_SetNumberAverage(int _numDevice, int _numAverage)
{
    if(!existsDevice[_numDevice])
        return;

    math[_numDevice].SetNumberAverage(_numAverage);
    math[_numDevice].Reset();
}

/*
__declspec(dllexport)void __stdcall D129_SetAutoCalibrate(int _numDevice, int _autoCalibrate)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->settings.SetAutoCalibrate(_autoCalibrate == 1);
}

__declspec(dllexport)int __stdcall D129_GetAutoCalibrate(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->settings.GetAutoCalibrate() ? 1 : 0;
}
*/

__declspec(dllexport)void __stdcall D129_SetCalibrator(int _numDevice, int _calibrator)
{
    if(!existsDevice[_numDevice])
        return;

    altera[_numDevice]->SetCalibrator(_calibrator == 1);
}

__declspec(dllexport)void __stdcall D129_EnableSpectr(int _numDevice, int _channel, int _enable)
{
    if(!existsDevice[_numDevice])
        return;

    math[_numDevice].EnableSpectr(_channel, _enable == 1);
}

__declspec(dllexport)int __stdcall D129_IsConnect(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    int retValue = altera[_numDevice]->IsConnect() ? 1 : 0;
    return retValue;
}

__declspec(dllexport)int __stdcall D129_IsRunning(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->IsRunning() ? 1 : 0;
}

__declspec(dllexport)int __stdcall D129_ReadData(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    unsigned char flag;
    if(D129_DataReadyForRead(_numDevice, flag) == 0)
        return 0;

    switch(mode[_numDevice])
    {
    case ModeOscilloscope:
        {
        dataOsci[_numDevice] = alteraOsci[_numDevice]->GetData();
        math[_numDevice].Processing(dataOsci[_numDevice], altera[_numDevice]->settings.GetStartMode(), 
                                    altera[_numDevice]->settings.GetTBase(), altera[_numDevice]->settings.GetRange(0), 
                                    altera[_numDevice]->settings.GetRange(1), altera[_numDevice]->settings.GetRShiftAbs(0), 
                                    altera[_numDevice]->settings.GetRShiftAbs(1));
        dataIsReading[_numDevice] = dataOsci[_numDevice]->isReady;
        return 1;
        break;
        }

    case ModeVoltmeter:
        dataVoltmeter[_numDevice] = alteraVoltmeter[_numDevice]->GetData();
        return 1;
        break;

    case ModeAnalyzer:
        dataAnalyzer[_numDevice] = alteraAnalyzer[_numDevice]->GetData();
        mathAnalyzer[_numDevice].Processing(dataAnalyzer[_numDevice], fBase[_numDevice][0], fBase[_numDevice][1]);
        return 1;
        break;

    case ModeRegistrator:
        return 0;
        break;
    }

    return 0;
}

__declspec(dllexport)int __stdcall D129_CalibrateZero(int _numDevice)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->CalibrateZero() ? 1 : 0;
}

__declspec(dllexport)int __stdcall D129_Calibrate(int _numDevice, int _channel, uchar _buffer[501])
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->Calibrate(_channel, _buffer);
}

__declspec(dllexport)int __stdcall D129_Write(int _numDevice, int address, unsigned char data)
{
    if(!existsDevice[_numDevice])
        return -1;

    return altera[_numDevice]->Write((NS_AlteraAddressSpace::WRITE)address, data);
}

__declspec(dllexport)void __stdcall D129_SetWindow(int _numDevice, int _window)
{
    if(!existsDevice[_numDevice])
        return;

    mathAnalyzer[_numDevice].SetWindow(_window);
}

__declspec(dllexport)double __stdcall D129_GetDeltaF(int _numDevice, int _numChannel, char _buffer[16], int _size)
{
    if(!existsDevice[_numDevice])
        return -1;

    double deltaF = dataAnalyzer[_numDevice]->deltaF[_numChannel];
    QString text = FrequencyToString(deltaF);
    strcpy_s(_buffer, _size - 1, text.toAscii());
    return deltaF;
}

__declspec(dllexport)double __stdcall D129_GetU0(int _numDevice, int _numChannel, char _textBuffer[16], int _size)
{
    if(ModeAnalyzer != mode[_numDevice])
        return INVALID_VALUE_DOUBLE;

    if(!existsDevice[_numDevice])
        return -1;

    double max = mathDisplayOscilloscope.GetMaxVoltage(alteraAnalyzer[_numDevice]->settings.GetRange(_numChannel));
    double *data = D129_GetPointerToDataDraw(_numDevice, _numChannel);
    double value = data[0];
    QString text = (value <= max) ? VoltageToString(value, 2): EMPTY_STRING;
    strcpy_s(_textBuffer, _size - 1, text.toAscii());
    return (value <= max) ? value : INVALID_VALUE_DOUBLE;
}

__declspec(dllexport)double __stdcall D129_GetUmax(int _numDevice, int _numChannel, char _textBuffer[16], int _size)
{
    if(!existsDevice[_numDevice])
        return -1;

    double max = mathDisplayOscilloscope.GetMaxVoltage(alteraAnalyzer[_numDevice]->settings.GetRange(_numChannel));
    double *data = D129_GetPointerToDataDraw(_numDevice, _numChannel);
    int iMax = 1;
    double maxValue = data[iMax];
    for(int i = 2; i < 250; i++)
    {
        if(data[i] > maxValue)
        {
            maxValue = data[i];
            iMax = i;
        }
    }
    QString text((maxValue <= max) ? VoltageToString(maxValue, 2) : EMPTY_STRING);
    strcpy_s(_textBuffer, _size - 1, text.toAscii());
    
    return (maxValue <= max) ? maxValue : INVALID_VALUE_DOUBLE;
}

__declspec(dllexport)double __stdcall D129_GetFUmax(int _numDevice, int _numChannel, char _textBuffer[16], int _size)
{
    if(!existsDevice[_numDevice])
        return -1;

    double *data = D129_GetPointerToDataDraw(_numDevice, _numChannel);
    int iMax = 1;
    double maxValue = data[iMax];
    for(int i = 2; i < 250; i++)
    {
        if(data[i] > maxValue)
        {
            maxValue = data[i];
            iMax = i;
        }
    }
    double retValue = iMax * dataAnalyzer[_numDevice]->deltaF[_numChannel];
    QString text(FrequencyToString(retValue, 2));
    strcpy_s(_textBuffer, _size - 1, text.toAscii());
    return retValue;
}

double CalculateVoltageMax(int _numDevice, double *_input, int _channel) 
{
    if(!existsDevice[_numDevice])
        return -1;

    double _max = tables.GetRShiftAbs(3000, D129_GetRange(_numDevice, _channel));
    double _min = -_max;
    double retValue = _min;
    for(int i = 0; i < DB_SIGNED_LENGTH; i++)
        retValue = max(retValue, _input[i]);
    if((retValue == _min) || (retValue > _max))
        return INVALID_VALUE_DOUBLE;
    else
        return retValue;
}

double CalculateVoltageMin(int _numDevice, double* _input,int _channel) 
{
    if(!existsDevice)
        return -1;

    double _max = tables.GetRShiftAbs(3000, D129_GetRange(_numDevice, _channel));
    double _min = -_max;
    double retValue = _max;
    for(int i = 0; i < DB_SIGNED_LENGTH; i++)
        retValue = min(retValue, _input[i]);
    if((retValue == _max) || (retValue < _min))
        return INVALID_VALUE_DOUBLE;
    else
        return retValue;
}

__declspec(dllexport) double _stdcall D129_GetStepFreq(int _numDevice, char _string[15], int /*size*/)
{
    if(ModeOscilloscope != mode[_numDevice])
        return 0.0;

    int tBase = D129_GetTBase(_numDevice);
    double stepFreq = tables.GetStepFrequency(tBase) * 250;  // Герц / экран
    QString str = FrequencyToString(stepFreq);
    strcpy_s(_string, 15, str.toAscii());
    return stepFreq;
}