//  [12/12/2009 Sasha]
#pragma once
#include "AbstractData.h"
#include "../../common/TypesAltera.h"

struct SettingsAltera;

struct DataOscillograph : public AbstractData 
{
    static const double FRACTION_FOR_ADDITION_DATA;

    DataOscillograph(int n = 0);
    void Clear();
    void AdditionData();
    void AdditionData(int channel);
    void AdditionInterpolationData(int _channel);
    double GetFraction() { return fraction; };

    double channelMath[NS_TypesAltera::DB_LENGTH];
    double channelDraw[3][NS_TypesAltera::DB_LENGTH];   // Здесь данные в готовом для вывода виде - без учёта смещения, приведённом к экрану
    double spectr[2][NS_TypesAltera::DB_LENGTH];
    double spectrDraw[2][NS_TypesAltera::DB_LENGTH];
    int viewSpectr;     // Показывает, спектр какого сигнала выводить. Если 2, то никакой
    bool readPoint[NS_TypesAltera::DB_LENGTH];          // В режиме рандомизатора указывает, что точка получена.
    bool drawPoints[NS_TypesAltera::DB_LENGTH];         // Для мухлежа в 10 и 20нс
    bool mathFunction;
    int rangeMath;
    double fraction;                                    // Здесь будет относительная часть точек, реально полученных от рандомизатора, для рисования сигнала
    DataOscillograph& operator/(int n);
    DataOscillograph& operator+=(DataOscillograph &rVal);
};