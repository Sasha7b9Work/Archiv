#pragma once
#include "Measures.h"
#include "defines.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/DataStorage.h"


void Processing_SetSignal(uint8 *data0, uint8 *data1, DataSettings *ds, int firstPoint, int lastPoint);         // ���������� ������ ��� ���������.
void Processing_GetData(uint8 **data0, uint8 **data1, DataSettings **ds);        // �������� ������ ����� �������������� �������.
float Processing_GetCursU(Channel chan, float posCurT);                           // �������� ������� ������� ����������, �������������� �������� ������� ������� posCurT.
float Processing_GetCursT(Channel chan, float posCurU, int numCur);               // �������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU.
void Processing_InterpolationSinX_X(uint8 data[FPGA_MAX_POINTS], TBase tBase);   // ���������������� ��������� ��������� ������ ������������� �������� sinX/X.
char* Processing_GetStringMeasure(Measure measure, Channel chan, char buffer[20]);// ���������� ������ ��������������� ���������.
void Processing_CalculateMeasures(void);                                         // ��������� ��� ���������.
int Processing_GetMarkerHorizontal(Channel chan, int numMarker);                // ���������� �������� ��������������� �������. ���� ERROR_VALUE_INT - ������ �������� �� �����.
int Processing_GetMarkerVertical(Channel chan, int numMarker);                  // ���������� �������� ������������� �������. ���� ERROR_VALUE_INT - ������ �������� �� �����.
