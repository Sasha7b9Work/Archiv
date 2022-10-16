//  [9/17/2009 Sasha]
/*
    ����� ������� �������� ��� �������� ������������� ������� ������� � ����������
*/
#pragma  once
#include <vector>
#include "defines.h"
#include "TypesAltera.h"

class TablesOfConverting 
{

private:

    struct TimeStruct 
    {
		NS_TypesAltera::TBase tBase;
		double absStep;                 // ����� ����� ����� ��������� �������. ������������ �����, �������������� ��������� tShift �� 1
        double absMinTShift;
        double absMaxTShift;
    };

    struct VoltageStruct 
    {
		NS_TypesAltera::Range range;
        double absMinVoltageMeas;       // ����������� ���������� ��������. ������������� ������ ������� ������
        double absMaxVoltageMeas;       // ������������ ���������� ��������. ������������ ������� ������� ������
        double absStepVoltageMeas;      // ���, �������� ��������� - ���������� ����� ����� ��������� ������� �� ��� ����������
        double absMinRShift;            // ����������� �������� �������� � ���������� ����
        double absMaxRShift;            // ������������ ���������� �������� ��������
        double absStepRShift;           // ��������� ����������� �������� �� ���������� ��� ��������� ��������������(���������� � �������) �� 1.
    };

    TimeStruct tableTShift[30];
    VoltageStruct tableRShift[30];

    std::vector<double> tablesStepFreq;
    std::vector<int> rangeFromIndexFrequency;

public:
    TablesOfConverting();

    double GetStepFrequency(int tBase);
    double GetStepTime(int _tBase);                     // ���������� ����� ����� ��������� � �������� ��� ������� tBase
    int GetTBaseFromFBase(int _fBase);       // �������� �������� �������� �� ���������� �� ������� ���������� ��������� ������. ������������ � ����������� �������
    int GetFBaseFromTBase(int _tBase);

    // �������� ���������� �������� ���������� �� ��������������, ����������� �� ������������. ���������� �������� �� 
    // ��������� �������� � ����� ���� ������������ ������ ��� ������ ������� �� �����
    double GetAbsVoltageDraw(ulong relValue, int range);

    // �������� ���������� �������� ���������� �� ��������������, ����������� �� ������������. ���������� �������� ���������
    // �������� � ����� ���� ������������ ��� �������������� ��������
    double GetAbsVoltage(ulong relValue, int range, int rShift);

    // �������� ������������� �������� ���������� � ������ ��������� �� �����������
    int GetRelVoltage(double absValue, int range);

    // ���������� ���������� �������� �������� �� ���������� � �������
    double GetRShiftAbs(int rShift, int range);

    // ���������� ������������� �������� �������� �� ���������� (������� ����� �������� � �������), ��������������� ��-
    // ������� ����������� � �������� �� ����������
    int GetRShiftRel(double rShiftAbs, int range);

    // ���������� ���������� �������� �������� �� ������� � ��������
    double GetTShiftAbs(int tShift, int tBase);

    // ���������� ������������� �������� �� ������� (������� ����� �������� � �������), ��������������� ��������� ����������� � �������� �� �������
    int GetTShiftRel(double tShiftAbs, int tBase);
};
