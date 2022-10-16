// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/Buffer_.h"
#include "common/Utils/Containers/Values_.h"
#include "FPGA/FPGA.h"


struct MarkersVertical
{
    MarkersVertical();

    void SetInvalid();

    void SetRawValue(int num, int);

    void Draw();

    Int marker[2];
};


struct MarkersHorizontal
{
    MarkersHorizontal();

    void SetInvalid();

    void SetValue(int num, const Float &);

    void SetRawValue(int num, int);

    void Draw();

    Int marker[2];
};


namespace Processing
{
    // ���������� ������ �������� � ��������� ������
    void Process(ReadingFrame &dr);

    void GetData(uint8 **dataA, uint8 **dataB, DataSettings **);

    // �������� ������� ������� ����������, �������������� �������� ������� ������� posCurT.
    float GetCursU(const Chan &ch, float posCurT);

    // �������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU.
    float GetCursT(const Chan &ch, float posCurU, int numCur);

    // ���������������� ��������� ��������� ������ ������������� �������� sinX/X.
    void InterpolationSinX_X(Buffer<uint16> &buffer, TBase::E tBase);

    void CalculateMathFunction(float *absAandResult, float *absB, int num_points);

    // ���������� ������ ��������������� ���������.
    Text GetStringMeasure(TypeMeasure::E measure, const Chan &ch);

    extern MarkersVertical markersVert[Chan::Count];
    extern MarkersHorizontal markersHor[Chan::Count];
}
