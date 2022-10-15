#pragma once
#include "Utils/Measures.h"
#include "FPGA/FPGATypes.h"
#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Processing
{
public:
    /// ���������� ������ ��� ���������.
    static void SetSignal(uint8 *data0, uint8 *data1, DataSettings *ds, int firstPoint, int lastPoint);
    /// �������� ������ ����� �������������� �������.
    static void GetData(uint8 **data0, uint8 **data1, DataSettings **ds);
    /// �������� ������� ������� ����������, �������������� �������� ������� ������� posCurT.
    static float GetCursU(Channel chan, float posCurT);
    /// �������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU.
    static float GetCursT(Channel chan, float posCurU, int numCur);
    /// ���������������� ��������� ��������� ������ ������������� �������� sinX/X.
    static void InterpolationSinX_X(uint8 data[FPGA_MAX_POINTS], TBase tBase);
    /// ���������� ������ ��������������� ���������.
    static char* GetStringMeasure(Meas measure, Channel chan, char buffer[20]);
    /// ��������� ��� ���������.
    static void CalculateMeasures();
    /// ���������� �������� ��������������� �������. ���� ERROR_VALUE_INT - ������ �������� �� �����.
    static int GetMarkerHorizontal(Channel chan, int numMarker);
    /// ���������� �������� ������������� �������. ���� ERROR_VALUE_INT - ������ �������� �� �����.
    static int GetMarkerVertical(Channel chan, int numMarker);
};
