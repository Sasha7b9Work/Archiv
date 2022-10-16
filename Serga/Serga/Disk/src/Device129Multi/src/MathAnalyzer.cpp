//  [7/21/2009 Sasha]
#define _USE_MATH_DEFINES
#include <cmath>
#include "MathAnalyzer.h"
#include "../common/defines.h"
#include "SettingsAltera.h"
#define AP_WIN32
#include "../../common/fft/fft.h"
#include "DataAnalyzer.h"
#include "../common/TablesOfConverting.h"

using namespace NS_TypesAltera;

extern TablesOfConverting tables;

MathAnalyzer::MathAnalyzer() : window(2) { }

DataAnalyzer* MathAnalyzer::Processing(DataAnalyzer *_data, int _fBase0, int _fBase1)
{

    Spectr(_data, 0);
    Spectr(_data, 1);

    if(_fBase0 >= 0)
        _data->deltaF[0] = tables.GetStepFrequency(tables.GetTBaseFromFBase(_fBase0));
    else
        _data->deltaF[0] = -1.0;

    if(_fBase1 >= 0)
        _data->deltaF[1] = tables.GetStepFrequency(tables.GetTBaseFromFBase(_fBase1));
    else
        _data->deltaF[1] = -1.0;

    return _data;
}

void MathAnalyzer::Spectr(DataAnalyzer *_data, int _channel) 
{

    static const int LENGTH = DB_SIGNED_LENGTH - 1;

    if(window == 0)
        WindowHamming(_data->dataAbs[_channel]);
    else if(window == 1)
        WindowBlackman(_data->dataAbs[_channel]);

    // Подготавливаем входные данные
    ap::real_1d_array data;

    data.setcontent(0, LENGTH, _data->dataAbs[_channel]);

    // Подготавливаем массивы для результата
    ap::complex_1d_array outData;

    outData.setbounds(0, LENGTH);

    // Расчитываем спектр
    fftr1d(data, LENGTH, outData);

    // Заносим абсолютные расчитанные данные в DataOscillograph 
    for(int i = 0; i < LENGTH; i++)
        _data->dataAbs[_channel][i] = abscomplex(outData(i)) / static_cast<double>(MAX_VALUE - MIN_VALUE);
}

void MathAnalyzer::WindowHamming(double *_data) 
{
    for(int i = 0; i < DB_SIGNED_LENGTH; i++) 
    {
        double k = 0.54 - 0.46 * cos(2 * asin(1.0) * 2.0 * i / DB_SIGNED_LENGTH);
        _data[i] *= k;
    }
}

void MathAnalyzer::WindowBlackman(double *_data) 
{
    for(int i = 0; i < DB_SIGNED_LENGTH; i++) 
    {
        double k = 0.42 - 0.5 * cos(2 * asin(1.0) * 2.0 * i / DB_SIGNED_LENGTH) + 0.08 * cos(4 * asin(1.0) * 2.0 * i / DB_SIGNED_LENGTH);
        _data[i] *= k;
    }
}