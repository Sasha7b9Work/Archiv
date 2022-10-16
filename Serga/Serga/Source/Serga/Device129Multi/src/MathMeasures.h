/*
 *  [12/12/2011 by Sasha7b9] 
 */
#ifndef _MATHMEASURES_H_
#define _MATHMEASURES_H_

#pragma warning(disable:4127)
#include <QString>
#pragma warning(default:4127)
#include <vector>

class   QComboBox;
class   AlteraOscillograph;

class MathMeasures
{
public:
    enum TypeMeasure
    {
        TM_Frequency    = 0,
        TM_Period       = 1,
        TM_TimeImpuls   = 2,
        TM_TimePause    = 3,
        TM_Skvazhnost   = 4,
        TM_VoltageMin   = 5,
        TM_VoltageMax   = 6,
        TM_VoltagePic   = 7,
        TM_RMS          = 8,
        TM_Size
    };

private:
    struct Measure
    {
        Measure(TypeMeasure _t, QString _s, double (MathMeasures::*_f)(int), QString(*_fs)(double, int)) : type(_t), str(_s), func(_f), funcString(_fs) {}
        TypeMeasure type;
        QString str;
                                            // numChannel
        double (MathMeasures::*func)(int);         // Здесь указатель на соответствующую функцию расчёта
        QString(*funcString)(double, int);
    };

public:
    MathMeasures(AlteraOscillograph *_altera);
    ~MathMeasures();
    void FillComboMeasures(QComboBox *_combo);
    double GetMeasure(int _channel, int _func, char *_buffer, int _size);
    double GetMeasure0(int _func, char *_buffer, int _size);
    double GetMeasure1(int _func, char *_buffer, int _size);

private:
    AlteraOscillograph*         altera;
    std::vector<Measure>        measures;

    double  CalculateFrequncy(int _channel);
    double  CalculatePeriod(int _channel);
    double  CalculateTimeImpuls(int _channel);
    double  CalculateTimePause(int _channel);
    double  CalculateSkvaznost(int _channel);
    double  CalculateVoltageMin(int _channel);
    double  CalculateVoltageMax(int _channel);
    double  CalculateVoltagePic(int _channel);
    double  CalculateRMS(int _channel);
    int     CalculatePeriodForRMS(int _channel);    // Период, вычисляемый этой функцией, может отличаться от реального в целое число раз. Возвращаемое значение - число точек
    double  CalculateCrossingUp(int _channel, int _firstPoint, double _aveValue, int *_point);   // Находит первое пересечение "вверх" сигнала данного канала линии среднего значения
    double  CalculateCrossingDown(int _channel, int _firstPoint, double _aveValue, int *_point);
};

#endif  // #ifndef _MATHMEASURES_H_