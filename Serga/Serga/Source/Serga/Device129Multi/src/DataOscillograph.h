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
    double channelDraw[3][NS_TypesAltera::DB_LENGTH];   // ����� ������ � ������� ��� ������ ���� - ��� ����� ��������, ���������� � ������
    double spectr[2][NS_TypesAltera::DB_LENGTH];
    double spectrDraw[2][NS_TypesAltera::DB_LENGTH];
    int viewSpectr;     // ����������, ������ ������ ������� ��������. ���� 2, �� �������
    bool readPoint[NS_TypesAltera::DB_LENGTH];          // � ������ ������������� ���������, ��� ����� ��������.
    bool drawPoints[NS_TypesAltera::DB_LENGTH];         // ��� ������� � 10 � 20��
    bool mathFunction;
    int rangeMath;
    double fraction;                                    // ����� ����� ������������� ����� �����, ������� ���������� �� �������������, ��� ��������� �������
    DataOscillograph& operator/(int n);
    DataOscillograph& operator+=(DataOscillograph &rVal);
};