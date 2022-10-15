#pragma once
#include "Utils/Measures.h"
#include "defines.h"
#include "Data/DataStorage.h"
#include "FPGA/FPGAtypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Utils
 *  @{
 *  @defgroup ProcessingSignal
 *  @{
 */

#define MARKER_VOLTAGE(ch, num)  (markerVoltage[ch][num] - MIN_VALUE)
#define MARKER_TIME(ch, num)    (markerTime[ch][num])

extern int markerVoltage[NumChannels][2];
extern int markerTime[NumChannels][2];

class Processing
{
public:
    /// \brief ���������� ������ ��� ���������. ������ ������� �� DS, inA, inB.
    /// �������� ������, ��������������� ������� ����������� set, �������������� ����� � ������������ � outA, outB.
    void SetData(bool needSmoothing);
    /// �������� ������� ������� ����������, �������������� �������� ������� ������� posCurT
    float GetCursU(Channel ch, float posCurT);
    /// �������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU
    float GetCursT(Channel ch, float posCurU, int numCur);
    /// ���������������� ��������� ��������� ������ ������������� �������� sinX/X
    void InterpolationSinX_X(uint8 *data, int numPoints, TBase tBase);
    /// ���������� ������ ��������������� ���������
    char* GetStringMeasure(Meas measure, Channel ch, char *buffer, int lenBuf);
    /// ��������� ��� ���������
    void CalculateMeasures();
private:
    /// ���������� ������� � ������������� � ������� ����������
    void CountedToCurrentSettings();
    /// ���������� ������� � �������������� � ������� Range
    void CountedRange(Channel ch);
    /// ���������� ������� � ������ � �������������� � ������� TBase
    void CountedTBase();
    /// ���������� ���������� ����� � ������� � ������������� ����������
    void CountedEnumPoints();
};


extern Processing processing;


/** @}  @}
 */
