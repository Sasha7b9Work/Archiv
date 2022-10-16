#pragma once

#include <Windows.h>

BOOL WINAPI DllMain(_In_ HANDLE _HDllHandle, _In_ DWORD _Reason, _In_opt_ LPVOID _Reserved);

__declspec(dllexport)int    __stdcall D129_GetPorts(short *_ports, int _size);
__declspec(dllexport)int    __stdcall D129_CreateDevice(int _modeDevice);
__declspec(dllexport)int    __stdcall D129_ConnectDeviceToPort(int _numDevice, short _numPort);         // 0 - error connect, 1 - good connect
__declspec(dllexport)void   __stdcall D129_DestroyDevice(int _numDevice);
__declspec(dllexport)void   __stdcall D129_SetSettings(int _numDevice);

__declspec(dllexport)int    __stdcall D129_IsConnect(int _numDevice);           // 0 - not connect, 1 - exist connect
__declspec(dllexport)int    __stdcall D129_IsRunning(int _numDevice);           // 0 - not running, 1 - running
__declspec(dllexport)int    __stdcall D129_DataReadyForRead(int _numDevice, unsigned char &_flag);   // 0 - data not ready, 1 - data ready
__declspec(dllexport)int    __stdcall D129_DataReadyForDraw(int _numDevice);    // 0 - data not ready, 1 - data ready

__declspec(dllexport)int    __stdcall D129_ReadData(int _numDevice);            // 0 - data not read, 1 - data read
__declspec(dllexport)void   __stdcall D129_GetDataDraw(int _numDevice, int _channel, double _buffer[501], int _size);
__declspec(dllexport)double* __stdcall D129_GetPointerToDataDraw(int _numDevice, int _channel);

//__declspec(dllexport)int    __stdcall D129_GetAutoCalibrate(int _numDevice);
__declspec(dllexport)int    __stdcall D129_GetRange(int _numDevice, int _channel);  // Если _channel == 2 - возвращает масштаб по напряжению для математической функции
__declspec(dllexport)int    __stdcall D129_GetTPos(int _numDevice);
__declspec(dllexport)int    __stdcall D129_GetTBase(int _numDevice);
__declspec(dllexport)double __stdcall D129_GetTShiftAbs(int _numDevice);
__declspec(dllexport)int    __stdcall D129_GetTShiftRel(int _numDevice);
__declspec(dllexport)int    __stdcall D129_GetModeInput(int _numDevice, int _channel);
__declspec(dllexport)int    __stdcall D129_GetSinchroChannel(int _numDevice);
__declspec(dllexport)int    __stdcall D129_GetRShiftRel(int _numDevice, int _channel);
__declspec(dllexport)double __stdcall D129_GetRShiftAbs(int _numDevice, int _channel);
__declspec(dllexport)int    __stdcall D129_GetTrigLevRel(int _numDevice, int _channel);
__declspec(dllexport)double __stdcall D129_GetTrigLevAbs(int _numDevice, int _channel);
__declspec(dllexport)int    __stdcall D129_GetModeADC(int _numDevice);
__declspec(dllexport)int    __stdcall D129_GetStartMode(int _numDevice);
__declspec(dllexport)int    __stdcall D129_GetAdditionRShift(int _numDevice, int _channel);
__declspec(dllexport)double __stdcall D129_GetKoeffCalibrate(int _numDevice, int _channel);
__declspec(dllexport)double __stdcall D129_GetAddr_Min(int _numDevice);
__declspec(dllexport)double __stdcall D129_GetAddr_Max(int _numDevice);

__declspec(dllexport)void   __stdcall D129_SetModeInput(int _numDevice, int _channel, int _modeInput);
__declspec(dllexport)void   __stdcall D129_SetKoeffCalibrate(int _numDevice, int _channel, double _koeff);
__declspec(dllexport)void   __stdcall D129_SetAddr_Min(int _numDevice, double _value);
__declspec(dllexport)void   __stdcall D129_SetAddr_Max(int _numDevice, double _value);
__declspec(dllexport)void   __stdcall D129_SetRShiftRel(int _numDevice, int _channel, int _rShift);
__declspec(dllexport)void   __stdcall D129_SetAdditionRShift(int _numDevice, int _channel, int _additionRShift);
__declspec(dllexport)void   __stdcall D129_SetRange(int _numDevice, int _channel, int _range);
__declspec(dllexport)void   __stdcall D129_SetTrigLevRel(int _numDevice, int _channel, int _trigLev);
__declspec(dllexport)void   __stdcall D129_SetTBase(int _numDevice, int _tBase);
__declspec(dllexport)void   __stdcall D129_SetTShiftRel(int _numDevice, int _tShift);
__declspec(dllexport)void   __stdcall D129_SetTPos(int _numDevice, int _tPos);
__declspec(dllexport)void   __stdcall D129_SetSinchroChannel(int _numDevice, int _channel);  // 0 - 1й канал, 1 - 2й канал, -1 - синхронизация отключена
__declspec(dllexport)void   __stdcall D129_SetSinchroType(int _numDevice, int _type);
__declspec(dllexport)void   __stdcall D129_SetStartMode(int _numDevice, int _startMode);
__declspec(dllexport)void   __stdcall D129_SetFiltr(int _numDevice, bool _filtr);
__declspec(dllexport)void   __stdcall D129_SetInvert(int _numDevice, int _channel, bool _invert);
__declspec(dllexport)void   __stdcall D129_SetMathFunction(int _numDevice, int _function, int _enable);
__declspec(dllexport)void   __stdcall D129_SetModePalm(int _numDevice, int _channel, bool _palm);
__declspec(dllexport)void   __stdcall D129_SetPointsPalm(int _numDevice, int _numPoints);
//__declspec(dllexport)void   __stdcall D129_SetAutoCalibrate(int _numDevice, int _autoCalibrate);
__declspec(dllexport)void   __stdcall D129_SetCalibrator(int _numDevice, int _calibrator);

__declspec(dllexport)void   __stdcall D129_ResetAddrMinMax(int _numDevice);
__declspec(dllexport)int    __stdcall D129_MathFunctionIsEnable(int _numDevice);
__declspec(dllexport)int    __stdcall D129_IsSpectrView(int _numDevice, int _channel);
__declspec(dllexport)void   __stdcall D129_Stop(int _numDevice);
__declspec(dllexport)void   __stdcall D129_Start(int _numDevice, bool _first);

__declspec(dllexport)int    __stdcall D129_CalibrateZero(int _numDevice);           // 0 - fail, 1 - Ok
__declspec(dllexport)int    __stdcall D129_Calibrate(int _numDevice, int _channel, unsigned char _buffer[501]);
__declspec(dllexport)int   __stdcall D129_Write(int _numDevice, int address, unsigned char data);

///******************************************************************************************
/*
    Функции осциллографа
*/
__declspec(dllexport)double*    __stdcall D129_GetPointerToSpectr(int _numDevice, int _channel);
__declspec(dllexport)double*    __stdcall D129_GetPointerToDataMathFunction(int _numDevice);
__declspec(dllexport)double     __stdcall D129_GetFraction(int _numDevice);
__declspec(dllexport)void       __stdcall D129_EnableSpectr(int _numDevice, int _channel, int _enable);
__declspec(dllexport)void       __stdcall D129_GetDataMathFunction(int _numDevice, double _buffer[501], int _size);
__declspec(dllexport)double     __stdcall D129_GetStepFreq(int _numDevice, char _string[15], int size);         // В осциллографе, режиме спектроанализатора возвращает количество герц в 10 клетках
__declspec(dllexport)void       __stdcall D129_GetDataSpectr(int _numDevice, int _channel, double _buffer[501], int _size);
__declspec(dllexport)double     __stdcall D129_GetMeasure(int _numDevice, int _channel, int _func, char _buffer[15], int _size);
__declspec(dllexport)void       __stdcall D129_SetModeAverage(int _numDevice, int _average);                        // 0 - не усреднять, 1 - усреднять
__declspec(dllexport)void       __stdcall D129_SetNumberAverage(int _numDevice, int _numAverage);

///******************************************************************************************
/*
    Функции спектроанализатора
*/
__declspec(dllexport)void   __stdcall D129_SetWindow(int _numDevice, int _window);   // 0 - Окно Хэмминга, 1 - Окно Блэкмэна
__declspec(dllexport)double __stdcall D129_GetDeltaF(int _numDevice, int _numChannel, char _textBuffer[16], int _size);
__declspec(dllexport)double __stdcall D129_GetU0(int _numDevice, int _numChannel, char _textBuffer[16], int _size);
__declspec(dllexport)double __stdcall D129_GetUmax(int _numDevice, int _numChannel, char _textBuffer[16], int _size);
__declspec(dllexport)double __stdcall D129_GetFUmax(int _numDevice, int _numChannel, char _textBuffer[16], int _size);
__declspec(dllexport)void   __stdcall D129_SetFBase(int _numDevice, int _channel, int _fBase);


///******************************************************************************************
/*
    Функции регистратора
*/
__declspec(dllexport)int    __stdcall D129_DataReadyForReadRegistrator(int _numDevice);     // 0 - data not ready, 1 - data is ready
__declspec(dllexport)double __stdcall D129_GetValue(int _numDevice, int _channel);

///******************************************************************************************
/*
    Функции вольтметра
*/
__declspec(dllexport)double   __stdcall D129_CalculateRMS(int _numDevice, int _channel, char _buffer[15], int _size);
__declspec(dllexport)double   __stdcall D129_CalculatePP(int _numDevice, int _channel, char _buffer[15], int _size);


///******************************************************************************************
/*
    Внутренние функции
*/
double CalculateVoltageMax(int _numDevice, double *_input, int _channel);
double CalculateVoltageMin(int _numDevice, double* _input,int _channel) ;
