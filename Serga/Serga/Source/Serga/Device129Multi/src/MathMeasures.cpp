/*
 *  [12/12/2011 by Sasha7b9] 
 */
#include "MathMeasures.h"
#include "../../common/TablesOfConverting.h"
#include "../../common/GlobalFunctions.h"
#include "../../common/GeometryMath.h"
#include "DataOscillograph.h"
#include "AlteraOscillograph.h"

#include <limits>
#include <algorithm>

#pragma warning(disable:4127)
#include <QComboBox>
#include <QLineEdit>
#include <QTextCodec>
#pragma warning(default:4127)

#define NULL 0
// #define tr QObject::tr

using namespace NS_GlobalFunctions;
using namespace NS_TypesAltera;
using namespace NS_GeometryMath;
using namespace std;

extern TablesOfConverting tables;

MathMeasures::MathMeasures(AlteraOscillograph *_altera) : 
    altera(_altera)
{
    QTextCodec *rusCodec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(rusCodec);

    measures.push_back(Measure(TM_Frequency,    QObject::tr("Частота"),                          &MathMeasures::CalculateFrequncy,   FrequencyToString));
    measures.push_back(Measure(TM_Period,       QObject::tr("Период"),                           &MathMeasures::CalculatePeriod,     TimeToString));
    measures.push_back(Measure(TM_TimeImpuls,   QObject::tr("Длительность импульса"),            &MathMeasures::CalculateTimeImpuls, TimeToString));
    measures.push_back(Measure(TM_TimePause,    QObject::tr("Длительность паузы"),               &MathMeasures::CalculateTimePause,  TimeToString));
    measures.push_back(Measure(TM_Skvazhnost,   QObject::tr("Скважность"),                       &MathMeasures::CalculateSkvaznost,  DoubleToString));
    measures.push_back(Measure(TM_VoltageMin,   QObject::tr("Минимальное значение напряжения"),  &MathMeasures::CalculateVoltageMin, VoltageToString));
    measures.push_back(Measure(TM_VoltageMax,   QObject::tr("Максимальное значение напряжения"), &MathMeasures::CalculateVoltageMax, VoltageToString));
    measures.push_back(Measure(TM_VoltagePic,   QObject::tr("Размах напряжения"),                &MathMeasures::CalculateVoltagePic, VoltageToString));
    measures.push_back(Measure(TM_RMS,          QObject::tr("Действующее напряжение"),           &MathMeasures::CalculateRMS,        VoltageToString));
}

MathMeasures::~MathMeasures()
{
}

void MathMeasures::FillComboMeasures(QComboBox *_combo)
{
    for(size_t i = 0; i < measures.size(); i++)
    {
        _combo->addItem(measures[i].str);
    }
}

double MathMeasures::GetMeasure0(int _func, char *_buffer, int _size)
{
    static int prevFunc = - 1;
    static CircularStack<double>results(5);
    if(prevFunc != _func)
    {
        results.Reset(5);
        prevFunc = _func;
    }

    QString resultString;
    double result = (this->*measures[_func].func)(0);

    if(INVALID_VALUE_DOUBLE == result)
    {
        resultString = QObject::tr(EMPTY_STRING);
        results.Reset(5);
        strcpy_s(_buffer, _size - 1, resultString.toAscii());
        return result;
    }

    if(results.IsEmty())        // Если первое измерение
        results.Push(result);
    else
    {
        //double delta = fabs(results.GetAverage()) * 0.1;                // Если не первое измерение
        //if(fabs(fabs(results.GetAverage()) - fabs(result)) < delta)     // Если очередное измерение отличается не более чем на 10% от предыдущих
            results.Push(result);
    }
    result = results.GetAverage();
    resultString = measures[_func].funcString(result, 2);
    strcpy_s(_buffer, _size - 1, resultString.toAscii());
    return result;
}

double MathMeasures::GetMeasure1(int _func, char *_buffer, int _size)
{
    static int prevFunc = - 1;
    static CircularStack<double>results(5);
    if(prevFunc != _func)
    {
        results.Reset(5);
        prevFunc = _func;
    }

    QString resultString;
    double result = (this->*measures[_func].func)(1);

    if(INVALID_VALUE_DOUBLE == result)
    {
        resultString = QObject::tr(EMPTY_STRING);
        results.Reset(5);
        strcpy_s(_buffer, _size - 1, resultString.toAscii());
        return result;
    }

    if(results.IsEmty())        // Если первое измерение
        results.Push(result);
    else
    {
        //double delta = fabs(results.GetAverage()) * 0.1;                // Если не первое измерение
        //if(fabs(fabs(results.GetAverage()) - fabs(result)) < delta)     // Если очередное измерение отличается не более чем на 10% от предыдущих
            results.Push(result);
    }
    result = results.GetAverage();
    resultString = measures[_func].funcString(result, 2);
    strcpy_s(_buffer, _size - 1, resultString.toAscii());
    return result;
}

double MathMeasures::GetMeasure(int _channel, int _func, char *_buffer, int _size)
{
    if(0 == _channel)
        return GetMeasure0(_func, _buffer, _size);
    else
        return GetMeasure1(_func, _buffer, _size);
}

double MathMeasures::CalculateFrequncy(int _channel)
{
    double period = CalculatePeriod(_channel);
    if(INVALID_VALUE_DOUBLE == period)
        return INVALID_VALUE_DOUBLE;

    return 1.0 / period;
}

double MathMeasures::CalculatePeriod(int _channel)
{
    double aveVoltage = (CalculateVoltageMax(_channel) + CalculateVoltageMin(_channel)) / 2.0;
    
    // Найдём первое пересечение нашего сигнала среднего напряжения "вверх"

    int n0 = -1;                                                        // Номер точки, после которой будем искать следующее пересечение
    double tFirst = CalculateCrossingUp(_channel, 0, aveVoltage, &n0);
    if(INVALID_VALUE_DOUBLE == tFirst)
        return INVALID_VALUE_DOUBLE;

    // Находим второе пересечение "вверх"

    int n1 = -1;
    double tSecond = CalculateCrossingUp(_channel, n0, aveVoltage, &n1);
    if(INVALID_VALUE_DOUBLE == tSecond)
        return INVALID_VALUE_DOUBLE;

    return (tSecond - tFirst);
}

double MathMeasures::CalculateTimeImpuls(int _channel)
{
    double aveVoltage = (CalculateVoltageMax(_channel) + CalculateVoltageMin(_channel)) / 2.0;

    int n0 = -1;
    double tFirst = CalculateCrossingUp(_channel, 0, aveVoltage, &n0);
    if(INVALID_VALUE_DOUBLE == tFirst)
        return INVALID_VALUE_DOUBLE;

    int n1 = -1;
    double tSecond = CalculateCrossingDown(_channel, n0, aveVoltage, &n1);
    if(INVALID_VALUE_DOUBLE == tSecond)
        return INVALID_VALUE_DOUBLE;

    return (tSecond - tFirst);
}

double MathMeasures::CalculateTimePause(int _channel)
{
    double aveVoltage = (CalculateVoltageMax(_channel) + CalculateVoltageMin(_channel)) / 2.0;

    int n0 = -1;
    double tFirst = CalculateCrossingDown(_channel, 0, aveVoltage, &n0);
    if(INVALID_VALUE_DOUBLE == tFirst)
        return INVALID_VALUE_DOUBLE;

    int n1 = -1;
    double tSecond = CalculateCrossingUp(_channel, n0, aveVoltage, &n1);
    if(INVALID_VALUE_DOUBLE == tSecond)
        return INVALID_VALUE_DOUBLE;

    return (tSecond - tFirst);
}

double MathMeasures::CalculateSkvaznost(int _channel)
{
    double period = CalculatePeriod(_channel);
    if(INVALID_VALUE_DOUBLE == period)
        return INVALID_VALUE_DOUBLE;

    double timeImpuls = CalculateTimeImpuls(_channel);
    if(INVALID_VALUE_DOUBLE == timeImpuls)
        return INVALID_VALUE_DOUBLE;

    return (CalculatePeriod(_channel) / CalculateTimeImpuls(_channel));
}

double MathMeasures::CalculateVoltageMin(int _channel)
{
    double *values = altera->data.dataAbs[_channel];

    return *min_element(values, values + DB_SIGNED_LENGTH);
}

double MathMeasures::CalculateVoltageMax(int _channel)
{
    double *values = altera->data.dataAbs[_channel];

    return *max_element(values, values + DB_SIGNED_LENGTH);
}

double MathMeasures::CalculateVoltagePic(int _channel)
{
    return (CalculateVoltageMax(_channel) - CalculateVoltageMin(_channel));
}

double MathMeasures::CalculateRMS(int _channel)
{
    int period = CalculatePeriodForRMS(_channel);
    double *values = altera->data.dataAbs[_channel];

    double RMS = 0.0;
    
    for(int n = 0; n < period; n++)
    {
        RMS += values[n] * values[n];
    }

    RMS = sqrt(RMS / period);
    return RMS;
}

int MathMeasures::CalculatePeriodForRMS(int _channel)
{
    const int MAX_PERIOD = 400;
    const int MIN_PERIOD = 200;

    double *values = altera->data.dataAbs[_channel];

    static double integrals[DB_SIGNED_LENGTH] = {0.0};  // Здесь будут храниться суммы отсчётов относительно нулевого.

    integrals[0] = values[0];
    for(int i = 1; i < DB_SIGNED_LENGTH; i++)
        integrals[i] = integrals[i-1] + values[i];

    // Теперь изменяем предполагаемый период от минимума до максимума и заполняем массив максимальный отклонений значений интеграла для данного периода на разных местах сигнала.
    static double maxDeviationSum[DB_SIGNED_LENGTH] = {0.0};

    for(int period = MIN_PERIOD; period <= MAX_PERIOD; period++)
    {
        double minSum = 1e300;
        double maxSum = 1e-300;

        for(int start = 0; start < (DB_SIGNED_LENGTH - MAX_PERIOD); start++)
        {
            double sum = integrals[start + period] - integrals[start];
            if(sum < minSum)
                minSum = sum;
            if(sum > maxSum)
                maxSum = sum;
        }
        maxDeviationSum[period] = fabs(maxSum - minSum) * period * period * period * period * period * period * period * period;
    }

    double minDeviation = 1e300;
    double period = -1;

    for(int i = MIN_PERIOD; i <= MAX_PERIOD; i++)
    {
        if(maxDeviationSum[i] < minDeviation)
        {
            minDeviation = maxDeviationSum[i];
            period = i;
        }
    }

    return static_cast<int>(period);
}

double MathMeasures::CalculateCrossingUp(int _channel, int _firstPoint, double _aveValue, int *_point)
{
    double *values = altera->data.dataAbs[_channel];

    double retValue = INVALID_VALUE_DOUBLE;

    for(int n = _firstPoint + 1; n < DB_SIGNED_LENGTH; n++)
    {
        if((values[n-1] < _aveValue) && (values[n] >= _aveValue))
        {
            double secsInPoint = tables.GetStepTime(altera->settings.GetTBase());
            double t1 = secsInPoint * (n-1);
            double t2 = secsInPoint * n;
            double u1 = values[n-1];
            double u2 = values[n];
            Line line0(t1, _aveValue, t2, _aveValue);
            Line line1(t1, u1, t2, u2);
            Point pointCrossing = line0.GetIntersection(line1);
            retValue = pointCrossing.x;
            *_point = n;
            break;
        }
    }

    return retValue;
}

double MathMeasures::CalculateCrossingDown(int _channel, int _firstPoint, double _aveValue, int *_point)
{
    double *values = altera->data.dataAbs[_channel];

    double retValue = INVALID_VALUE_DOUBLE;

    for(int n = _firstPoint + 1; n < DB_SIGNED_LENGTH; n++)
    {
        if((values[n-1] > _aveValue) && (values[n] <= _aveValue))
        {
            double secsInPoint = tables.GetStepTime(altera->settings.GetTBase());
            double t1 = secsInPoint * (n - 1);
            double t2 = secsInPoint * n;
            double u1 = values[n - 1];
            double u2 = values[n];
            Line line0(t1, _aveValue, t2, _aveValue);
            Line line1(t1, u1, t2, u2);
            Point pointCrossing = line0.GetIntersection(line1);
            retValue = pointCrossing.x;
            *_point = n;
            break;
        }
    }

    return retValue;
}