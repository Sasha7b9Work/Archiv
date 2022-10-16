// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Containers/Values_.h"


namespace MathFPGA
{
    void  CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1,
        float *density1, int *y0, int *y1);

    Float GetMaxFromArrayWithErrorCode(puchar data, int numPoints);

    Float GetMinFromArrayWithErrorCode(puchar data, int numPoints);

    void  CalculateMathFunction(float *data0andResult, const float *data1, int numPoints);

    float MaxVoltageOnScreen(Range::E);

    void PointsRelToVoltage(const uint8 *points, int numPoints, Range::E, int16 rShift, float *voltage);

    void PointsVoltageToRel(const float *voltage, int numPoints, Range::E, int16 rShift, uint8 *points);

    // Сдвинуть буфер на shift элементов вправо или влево. При этом потерянные элементы заменяются ближайшими к ним
    template<class T>
    void ShiftBuffer(T *const first, T *const last, int shift);

    extern const float voltsInPixel[Range::Count];                  // Столько вольт умещается в одном пикселе экрана
};
