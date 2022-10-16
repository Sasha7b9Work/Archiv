//  [7/21/2009 Sasha]
/*
����� ������ ������� ��� ��������� ������� ����� �������
1. ������������ ���������� ������ - Palm.
2. ���������� �� ���������� �������� - Average
3. �������������� - Invert
4. ���������� ������� - Disable
5. �������� �������� - Addition
6. ������������ �������� - Multiplication
*/

#pragma once
#include "../../common/CircularStack.h"
#include "../../common/TablesOfConverting.h"
#include "TypesMath.h"
#include "../../common/TypesAltera.h"

struct DataOscillograph;

class Math
{
public:
    Math();
    ~Math();

    void Reset();
    DataOscillograph* Processing(DataOscillograph *_data, StartMode _startMode, int _tBase, int _range0, int _range1, double _rShiftAbs0, double _rShiftAbs1);

    // ��� ������ ������������� �������
    void SetModePalm(int channel, bool palm);
    void SetPointsPalm(int);

    // ��� ����������
    void SetModeAverage(bool);
    void SetNumberAverage(int);

    // ��� �������� - ���������� ������
    void SetInvert(int channel, bool invert);

    void SetMathFunction(NS_TypesMath::Function func, bool enable);

    void EnableSpectr(int channel, bool spectr);

private:
    CircularStack <DataOscillograph> *dataStack;

    bool palm[2];           // ������� ����, ��� ����� ���������� ������������ ���������� ����������
    int pointsPalm;         // ���������� �����, �� ������� ������������ ������������ ���������� ����������

    bool enableAverage;     // ������� ����, ��� ����� ����������� ���������� �� numberAverage �����������
    int numberAverage;      // ���������� ����������� ���������� �������

    bool invert[2];         // ������� ����, ��� ����� ������������� ������ ������� ������

    bool addition;          // ������� ����, ��� ������ ����� �������

    bool multiplication;    // ������� ����, ��� ������ ����� �����������

    bool spectr[2];

    void PreparingForDraw(DataOscillograph *data, int _range0, int _range1, double _rShiftAbs0, double _rShiftAbs1, int _tBase);    // ����������� ������ � ��������� - �������� ��������� �������� � ������
    
    // ����������� ������������� ����� - ��� ������ ��������� ������� ����� ����� (< 1���) � ��� ������ �������������
    void Interpolation(DataOscillograph *data, int _tBase);

    // ���������� ���������� �� palmPoints ������ ������
    void Palm(DataOscillograph *data, int _tBase);

    // ���������� �� �����������
    void Average(DataOscillograph *data);

    // ��������������
    void Invert(DataOscillograph *data, double _rShiftAbs0, double _rShiftAbs1);

    // �������� �������
    void Addition(DataOscillograph *data);

    // ������������ �������
    void Multiplication(DataOscillograph *data);

    // ���������� �������
    void Spectr(DataOscillograph *data, int channel);

    void SetPointsPalm(int _tBase, int &_pointsPalm0, int &_pointsPalm1);
};