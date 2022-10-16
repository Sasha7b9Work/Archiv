#include "Math.h"
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#include <QObject>
#pragma warning(default:4244)
#pragma warning(default:4127)
#include <vector>
#include "../../common/defines.h"
#include "../../common/CircularStack.h"
#include "../../common/InterpolationSin.h"
#define AP_WIN32
#pragma warning(disable:4127)
#include "../../common/fft/fft.h"
#pragma warning(default:4127)
#include "DataOscillograph.h"
#include "../common/TablesOfConverting.h"
#include "../common/MathDisplayOscilloscope.h"

using namespace NS_TypesAltera;
using namespace NS_TypesMath;
using std::vector;

extern TablesOfConverting tables;
extern MathDisplayOscilloscope mathDisplay;

Math::Math() :
    pointsPalm(3),
    enableAverage(false),
    numberAverage(1),
    addition(false),
    multiplication(false),
    dataStack(NULL)
{
    palm[0] = palm[1] = false;
    invert[0] = invert[1] = false;
    spectr[0] = spectr[1] = false;
    dataStack = new CircularStack<DataOscillograph>(32);
}

Math::~Math() 
{
    SAFE_DELETE(dataStack);
}

void Math::Reset() 
{
    dataStack->Reset(numberAverage);
}


DataOscillograph* Math::Processing(DataOscillograph *_data, StartMode _startMode, int _tBase, int _range0, int _range1, double _rShiftAbs0, double _rShiftAbs1) 
{
    if((_tBase < TBase_1us) && (StartMode_Single == _startMode))
        // Если включен режим одиночного запуска для малых времён - строим недостающие точки
        Interpolation(_data, _tBase);

    if(enableAverage && (_tBase > (int)TBase_500ns) ) 
        Average(_data);

    Palm(_data, _tBase);        // Сглаживание будем производить, даже если выыключен режим сглаживания - но по трём точкам

    if(invert[0] || invert[1])
        Invert(_data, _rShiftAbs0, _rShiftAbs1); 

    if(addition)
        Addition(_data);
    else if(multiplication)
        Multiplication(_data);
    else
        _data->mathFunction = false;

    if(spectr[0])
        Spectr(_data, 0);
    else if(spectr[1]) 
        Spectr(_data, 1);
    else
        _data->viewSpectr = 2;

    PreparingForDraw(_data, _range0, _range1, _rShiftAbs0, _rShiftAbs1, _tBase);

    _data->isReady = true;
    return _data;
}

void Math::SetModePalm(int _channel, bool _palm) 
{
    palm[_channel] = _palm;
}

void Math::SetPointsPalm(int n) 
{
    pointsPalm = n;
}

void Math::SetInvert(int _channel, bool _inv) 
{
    invert[_channel] = _inv;
    Reset();
}

void Math::SetModeAverage(bool _average) 
{
    enableAverage = _average;
    dataStack->Reset(numberAverage);
}

void Math::SetNumberAverage(int n) 
{
    numberAverage = n;
    dataStack->Reset(numberAverage);
}

void Math::SetMathFunction(Function func, bool enable) 
{
    if((func == Function_Addition) && enable) 
    {
        addition = true;
        multiplication = false;
    }
    else if(func == Function_Multiplication && enable) 
    {
        multiplication = true;
        addition = false;
    }
    else 
    {
        addition = false;
        multiplication = false;
    }
}

void Math::EnableSpectr(int _channel, bool _spectr) 
{
    spectr[0] = spectr[1] = false;
    spectr[_channel] = _spectr;
}

void Math::PreparingForDraw(DataOscillograph *_data, int _range0, int _range1, double _rShiftAbs0, double _rShiftAbs1, int _tBase) 
{
    double* data0 = _data->dataAbs[0];
    double* data1 = _data->dataAbs[1];
    double max0 = mathDisplay.GetMaxVoltage(_range0);
    double min0 = -max0;
    double max1 = mathDisplay.GetMaxVoltage(_range1);
    double min1 = -max1;

    double k0 = 1.0 / (max0 - min0);
    double k1 = 1.0 / (max1 - min1);

    double* dataDraw0 = _data->channelDraw[0];
    double* dataDraw1 = _data->channelDraw[1];

    for(int i = 0; i < DB_SIGNED_LENGTH; i++) 
    {
        dataDraw0[i] = (data0[i] + _rShiftAbs0 - min0) * k0;
        dataDraw1[i] = (data1[i] + _rShiftAbs1 - min1) * k1;
    }

    if((_tBase < 6) && (_data->GetFraction() < 0.5))
    {
        for(int i = 0; i < DB_SIGNED_LENGTH; i++)
            if(!_data->readPoint[i])
            {
                dataDraw0[i] = -1.0;
                dataDraw1[i] = -1.0;
            }
    }

    if(_data->mathFunction) 
    {
        double max = -1000.0;
        double min = 1000.0;
        for(int i = 0; i < DB_SIGNED_LENGTH; i++) 
        {
            if(_data->channelMath[i] > max) 
                max = _data->channelMath[i];

            if(_data->channelMath[i] < min) 
                min = _data->channelMath[i];
        }
        // Находим наиболее подходящий масштаб по напряжению
        int rangeMathFunction = 0;
        for(; rangeMathFunction < RangeSize; rangeMathFunction++) 
        {
            if((max <= tables.GetAbsVoltageDraw(MAX_VALUE, rangeMathFunction)) &&
                (min >= tables.GetAbsVoltageDraw(MIN_VALUE, rangeMathFunction)))
                break;
        }
        Limitation(rangeMathFunction, 0, RangeSize - 1);
        _data->rangeMath = rangeMathFunction;

        double* dataDrawMath = _data->channelDraw[2];
        double minMath = -mathDisplay.GetMaxVoltage(rangeMathFunction);
        double k = 1.0 / (-minMath - minMath);

        for(int i = 0; i < DB_SIGNED_LENGTH; i++)
            dataDrawMath[i] = (_data->channelMath[i] - minMath) * k;
    }
}

void Math::SetPointsPalm(int _tBase, int &_pointsPalm0, int &_pointsPalm1)
{
    int array[] = { 17, 13, 11, 9 };

    if(_tBase <= TBase_100ns)
    {
        if(palm[0])
            _pointsPalm0 = (pointsPalm < array[_tBase]) ? array[_tBase] : pointsPalm;
        else
            _pointsPalm0 = array[_tBase];

        if(palm[1])
            _pointsPalm1 = (pointsPalm < array[_tBase]) ? array[_tBase] : pointsPalm;
        else
            _pointsPalm1 = array[_tBase];
    }
    else
    {
        _pointsPalm0 = _pointsPalm1 = pointsPalm;
    }
}

void Math::Palm(DataOscillograph *data, int _tBase) 
{
    double buffer0[DB_SIGNED_LENGTH] = {0.0};
    double buffer1[DB_SIGNED_LENGTH] = {0.0};

    int pointsPalm0 = 0, pointsPalm1 = 0;

    SetPointsPalm(_tBase, pointsPalm0, pointsPalm1);

    for(int pos = 0; pos < DB_SIGNED_LENGTH; pos++) 
    {
        int number = 0;
        if(palm[0] || _tBase <= TBase_100ns)
        {
            for(int j = pos - pointsPalm0 / 2; j <= pos + pointsPalm0 / 2; j++) 
            {
                if(j >= 0 && j < DB_SIGNED_LENGTH) 
                {
                    buffer0[pos] += data->dataAbs[0][j];
                    number++;
                }
            }
            buffer0[pos] /= number;
       }

        if(palm[1] || _tBase <= TBase_100ns)
        {
            number = 0;
            for(int j = pos - pointsPalm1 / 2; j <= pos + pointsPalm1 / 2; j++)
            {
                if(j >= 0 && j < DB_SIGNED_LENGTH)
                {
                    buffer1[pos] += data->dataAbs[1][j];
                    number++;
                }
            }
            buffer1[pos] /= number;
        }
    }

    if(palm[0] || _tBase <= TBase_100ns)
        for(int i = 0; i < DB_SIGNED_LENGTH; i++) 
            data->dataAbs[0][i] = buffer0[i];

    if(palm[1] || _tBase <= TBase_100ns)
        for(int i = 0; i < DB_SIGNED_LENGTH; i++) 
            data->dataAbs[1][i] = buffer1[i];
}

void Math::Average(DataOscillograph *_data) 
{
    dataStack->Push(*_data);
    *_data = dataStack->GetAverage();
}

void Math::Invert(DataOscillograph *data, double _rShiftAbs0, double _rShiftAbs1) 
{

    for(int pos = 0; pos < DB_SIGNED_LENGTH; pos++) 
    {
        if(invert[0])
            data->dataAbs[0][pos] = -(data->dataAbs[0][pos]) - 2 * _rShiftAbs0;    
        if(invert[1])
            data->dataAbs[1][pos] = -(data->dataAbs[1][pos]) - 2 * _rShiftAbs1;
    }
}

void Math::Addition(DataOscillograph *_data) 
{
    // Складываем значения обоих каналов - получаем искомые
    for(int i = 0; i < DB_SIGNED_LENGTH; i++) 
        _data->channelMath[i] = _data->dataAbs[0][i] + _data->dataAbs[1][i];
    // Теперь в DataOscillograph хранятся искомые значения. Процедура PreparingForDraw этой структуры
    // приведёт их к наилучшему масштабу для вывода
    _data->mathFunction = true;
}

void Math::Multiplication(DataOscillograph *_data) 
{
    for(int i = 0; i < DB_SIGNED_LENGTH; i++)
        _data->channelMath[i] = _data->dataAbs[0][i] * _data->dataAbs[1][i];

    _data->mathFunction = true;
}

void Math::Interpolation(DataOscillograph *data, int _tBase) 
{
    // Сколько точек нужно взять из массива в зависимости от развёртки
    int arrayNumPoints[6] = {5, 10, 25, 50, 100, 250};
    // Через сколько точек нужно брать отсчёты - нужно для InterpolationSin
    int arrayDeltas[6] = {100, 50, 20, 10, 5, 2};
    vector<double> values0;
    vector<double> values1;
    int numPoints = arrayNumPoints[_tBase];
    for(int i = 0; i < numPoints; i++) 
    {
        values0.push_back(data->dataAbs[0][i]);
        values1.push_back(data->dataAbs[1][i]);
    }
    int deltas = arrayDeltas[_tBase];
    InterpolationSin interpol0(deltas, &values0);
    InterpolationSin interpol1(deltas, &values1);
    for(int i = 0; i < DB_SIGNED_LENGTH - 1; i++) 
    {
        data->dataAbs[0][i] = interpol0.GetValue(i);
        data->dataAbs[1][i] = interpol1.GetValue(i);
    }
    data->dataAbs[0][DB_SIGNED_LENGTH-1] = data->dataAbs[0][DB_SIGNED_LENGTH-2];
    data->dataAbs[1][DB_SIGNED_LENGTH-1] = data->dataAbs[1][DB_SIGNED_LENGTH-2];
}

void Math::Spectr(DataOscillograph *_data, int _channel) 
{

    // Подготавливаем входные данные
    ap::real_1d_array data;

    data.setcontent(0, DB_SIGNED_LENGTH, _data->dataAbs[_channel]);

    // Подготавливаем массивы для результата
    ap::complex_1d_array outData;

    outData.setbounds(0, DB_SIGNED_LENGTH);

    // Расчитываем спектр
    fftr1d(data, DB_SIGNED_LENGTH, outData);

    // Заносим расчитанные данные в DataOscillograph
    for(int i = 0; i < DB_SIGNED_LENGTH; i++)
        _data->spectr[_channel][i] = abscomplex(outData(i));

    // Приводим значения спектральных составляющих к единице
    double max = 0.0;

    for(int i = 0; i < 250; i++) 
        if(_data->spectr[_channel][i] > max)
            max = _data->spectr[_channel][i];

    if(max == 0.0) 
        max = 1000.0;
    
    for(int i = 0; i < 250; i++)
        _data->spectrDraw[_channel][i] = _data->spectr[_channel][i] / max;
   
    _data->viewSpectr = _channel;
}
