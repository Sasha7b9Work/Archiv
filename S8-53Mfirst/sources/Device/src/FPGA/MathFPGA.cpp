// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Math_.h"
#include "common/Utils/Containers/Values_.h"
#include "Display/Screen/Grid.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include <cmath>
#include <cstring>


#define MAX_VOLTAGE_ON_SCREEN(range) (tableScalesRange[(range)] * 5.0f)

#define RSHIFT_2_ABS(rShift, range) (-((float)RShift::ZERO - (float)(rShift)) * MathFPGA::voltsInPixel[(uint)(range)])


template void MathFPGA::ShiftBuffer(uint8 *const first, uint8 *const last, int shift);


namespace MathFPGA
{
    static const float tableScalesRange[Range::Count] = {2e-3F, 5e-3F, 10e-3F, 20e-3F, 50e-3F, 100e-3F, 200e-3F,
                                                                500e-3F, 1.0F, 2.0F, 5.0F, 10.0F, 20.0F};

    static const int voltsInPixelInt[] =   // Коэффициент 20000
    {
        2,      // 2
        5,      // 5
        10,     // 10
        20,     // 20
        50,     // 50
        100,    // 100
        200,    // 200
        500,    // 500
        1000,   // 1
        2000,   // 2
        5000,   // 5
        1000,   // 10
        20000   // 20
    };

    const float voltsInPixel[Range::Count] =
    {
        2e-3F   / 20,   // 2mV
        5e-3F   / 20,   // 5mV
        10e-3F  / 20,   // 10mV
        20e-3F  / 20,   // 20mV
        50e-3F  / 20,   // 50mV
        100e-3F / 20,   // 100mV
        200e-3F / 20,   // 200mV
        500e-3F / 20,   // 500mV
        1.0F    / 20,   // 1V
        2.0F    / 20,   // 2V
        5.0F    / 20,   // 5V
        10.0F   / 20,   // 10V
        20.0F   / 20    // 20V
    };

    static const float voltsInPoint[Range::Count] =
    {
        2e-3F * 10.0F   / (ValueFPGA::MAX - ValueFPGA::MIN),    // 2mV
        5e-3F * 10.0F   / (ValueFPGA::MAX - ValueFPGA::MIN),    // 5mV
        10e-3F * 10.0F  / (ValueFPGA::MAX - ValueFPGA::MIN),    // 10mV
        20e-3F * 10.0F  / (ValueFPGA::MAX - ValueFPGA::MIN),    // 20mV
        50e-3F * 10.0F  / (ValueFPGA::MAX - ValueFPGA::MIN),    // 50mV
        100e-3F * 10.0F / (ValueFPGA::MAX - ValueFPGA::MIN),    // 100mV
        200e-3F * 10.0F / (ValueFPGA::MAX - ValueFPGA::MIN),    // 200mV
        500e-3F * 10.0F / (ValueFPGA::MAX - ValueFPGA::MIN),    // 500mV
        1.0F * 10.0F    / (ValueFPGA::MAX - ValueFPGA::MIN),    // 1V
        2.0F * 10.0F    / (ValueFPGA::MAX - ValueFPGA::MIN),    // 2V
        5.0F * 10.0F    / (ValueFPGA::MAX - ValueFPGA::MIN),    // 5V
        10.0F * 10.0F   / (ValueFPGA::MAX - ValueFPGA::MIN),    // 10V
        20.0F * 10.0F   / (ValueFPGA::MAX - ValueFPGA::MIN)     // 20V
    };

    static void MultiplyToWindow(float *data, int numPoints);

    static void Normalize(float *data, int);
}


float MathFPGA::MaxVoltageOnScreen(Range::E range)
{
    return tableScalesRange[range] * 5.0f;
}


int RShift::ToRel(float rShiftAbs, Range::E range)
{
    int retValue = (int)(RShift::ZERO + rShiftAbs / abs_step[range]);
    if (retValue < RShift::MIN)
    {
        retValue = RShift::MIN;
    }
    else if (retValue > RShift::MAX)
    {
        retValue = RShift::MAX;
    }
    return retValue;
};


Voltage RShift::ToAbs(int16 rshift, Range::E range)
{
    return (-((float)ZERO - (float)(rshift)) * abs_step[(uint)(range)]);
}


Voltage RShift::ToAbs(int16 rShiftRel, Range::E range, Divider::E multiplier)
{
    return RShift::ToAbs(rShiftRel, range) * Divider::ToAbs(multiplier);
};


pchar RShift::ToString(int16 rShiftRel, Range::E range, Divider::E multiplier)
{
    static char buffer[20];

    float rShiftVal = RSHIFT_2_ABS(rShiftRel, range) * Divider::ToAbs(multiplier);

    std::strcpy(buffer, Voltage(rShiftVal).ToText(true).c_str());

    return buffer;
};


void ValueFPGA::ToVoltage(puchar points, int numPoints, Range::E range, int16 rShift, float* voltage)
{
    int voltInPixel = MathFPGA::voltsInPixelInt[range];
    float maxVoltsOnScreen = MathFPGA::MaxVoltageOnScreen(range);
    float rShiftAbs = RShift::ToAbs(rShift, range);
    int diff = (int)((ValueFPGA::MIN * voltInPixel) + (maxVoltsOnScreen + rShiftAbs) * 20e3F);
    float koeff = 1.0F / 20e3F;
    for (int i = 0; i < numPoints; i++)
    {
        voltage[i] = (float)(points[i] * voltInPixel - diff) * koeff;
    }
}


Voltage ValueFPGA::ToVoltage(uint8 value, Range::E range, int16 rshift)
{
    float x = ((float)(value)-(float)ValueFPGA::MIN);

    float abs = x * MathFPGA::voltsInPoint[range];

    float max = MathFPGA::MaxVoltageOnScreen(range);

    float shift = RShift::ToAbs((int16)rshift, range);

    return abs - max - shift;
}


void ValueFPGA::FromVoltage(const float* voltage, int numPoints, Range::E range, int16 rShift, uint8* points)
{
    float maxVoltOnScreen = MathFPGA::MaxVoltageOnScreen(range);
    float rShiftAbs = RShift::ToAbs(rShift, range);
    float voltInPixel = 1.0F / MathFPGA::voltsInPixel[range];

    float add = maxVoltOnScreen + rShiftAbs;

    float delta = add * voltInPixel + ValueFPGA::MIN;

    for (int i = 0; i < numPoints; i++)
    {
        int value = (int)(voltage[i] * voltInPixel + delta);
        if (value < 0)
        {
            points[i] = 0;
            continue;
        }
        else if (value > 255)
        {
            points[i] = 255;
            continue;
        }
        points[i] = (uint8)value;
    }
}


uint8 ValueFPGA::FromVoltage(const Voltage &voltage, Range::E range, int16 rshift)
{
    int relValue = (int)((voltage + MathFPGA::MaxVoltageOnScreen(range) + RShift::ToAbs(rshift, range)) /
        MathFPGA::voltsInPixel[range] + ValueFPGA::MIN);

    Math::Limitation(&relValue, 0, 255);

    return (uint8)relValue;
}


uint8 ValueFPGA::ConvertToDisplay(uint8 value)
{
    int min_y = Grid::Top();
    int max_y = Grid::Channel::Bottom() - 1;
    float scale_y = (float)(max_y - min_y) / (ValueFPGA::MAX - ValueFPGA::MIN);

    int result = (int)(max_y - (value - ValueFPGA::MIN) * scale_y);

    if (result < min_y)
    {
        return (uint8)min_y;
    }

    if (result > max_y)
    {
        return (uint8)max_y;
    }

    return (uint8)result;;
}


/*
    Быстрое преобразование Фурье. Вычисляет модуль спектра для дейсвтительного сигнала.
    Количество отсчётов должно быть 2**N
*/

static void MathFPGA::MultiplyToWindow(float* data, int numPoints)
{
    if (FFT_WINDOW_IS_HAMMING)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.53836F - 0.46164F * std::cos(2 * 3.1415926F * i / (numPoints - 1));
        }
    }
    else if (FFT_WINDOW_IS_BLACKMAN)
    {
        float alpha = 0.16F;
        float a0 = (1.0F - alpha) / 2.0F;
        float a1 = 0.5F;
        float a2 = alpha / 2.0F;
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= a0 - a1 * std::cos(2 * 3.1415926F * i / (numPoints - 1)) + a2 * std::cos(4 * 3.1415926F * i /
                (numPoints - 1));
        }
    }
    else if (FFT_WINDOW_IS_HANN)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.5F * (1.0F - std::cos(2.0F * 3.1415926F * i / (numPoints - 1.0F)));
        }
    }
}


static void MathFPGA::Normalize(float* data, int)
{
    float max = 0.0;
    for (int i = 0; i < 256; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
        }
    }

    for (int i = 0; i < 256; i++)
    {
        data[i] /= max;
    }
}


void MathFPGA::CalculateFFT(float* dataR, int numPoints, float* result, float* freq0, float* density0, float* freq1,
    float* density1, int* y0, int* y1)
{
    float scale = 1.0F / TShift::abs_step[TBASE] / 1024.0F;

    float K = 1024.0F / numPoints;

    *freq0 = scale * FFT_POS_CUR_0 * K;
    *freq1 = scale * FFT_POS_CUR_1 * K;

    if (PEAK_DET_IS_ENABLED)
    {
        *freq0 *= 2;
        *freq1 *= 2;
    }

    for (int i = 0; i < numPoints; i++)
    {
        result[i] = 0.0F;
    }

    MultiplyToWindow(dataR, numPoints);

    int logN = 8;

    if (numPoints == 512)
    {
        logN = 9;
    }
    else if (numPoints == 1024)
    {
        logN = 10;
    }

    static const float Rcoef[14] =
    {
        -1.0000000000000000F, 0.0000000000000000F, 0.7071067811865475F,
        0.9238795325112867F, 0.9807852804032304F, 0.9951847266721969F,
        0.9987954562051724F, 0.9996988186962042F, 0.9999247018391445F,
        0.9999811752826011F, 0.9999952938095761F, 0.9999988234517018F,
        0.9999997058628822F, 0.9999999264657178F
    };

    static const float Icoef[14] =
    {
        0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
        -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
        -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
        -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
        -0.0007669903187427F, -0.0003834951875714F
    };

    int nn = numPoints >> 1;
    int ie = numPoints;

    for (int n = 1; n <= logN; n++)
    {
        float rw = Rcoef[logN - n];
        float iw = Icoef[logN - n];
        int in = ie >> 1;
        float ru = 1.0F;
        float iu = 0.0F;
        for (int j = 0; j < in; j++)
        {
            for (int i = j; i < numPoints; i += ie)
            {
                int io = i + in;
                float dRi = dataR[i];
                float dRio = dataR[io];
                float ri = result[i];
                float rio = result[io];
                dataR[i] = dRi + dRio;
                result[i] = ri + rio;
                float rtq = dRi - dRio;
                float itq = ri - rio;
                dataR[io] = rtq * ru - itq * iu;
                result[io] = itq * ru + rtq * iu;
            }
            float sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }
        ie >>= 1;
    }

    for (int j = 1, i = 1; i < numPoints; i++)
    {
        if (i < j)
        {
            int io = i - 1;
            int in = j - 1;
            float rtp = dataR[in];
            float itp = result[in];
            dataR[in] = dataR[io];
            result[in] = result[io];
            dataR[io] = rtp;
            result[io] = itp;
        }

        int k = nn;

        while (k < j)
        {
            j = j - k;
            k >>= 1;
        }

        j = j + k;
    }

    for (int i = 0; i < 256; i++)
    {
        result[i] = std::sqrtf(dataR[i] * dataR[i] + result[i] * result[i]);
    }

    result[0] = 0.0F;       // WARN нулевая составляющая мешает постоянно. надо её убрать

    Normalize(result, 256);

    if (FFT_SCALE_IS_LOG)
    {
        float minDB = MaxDbFFT::GetReal(FFT_MAX_DB);

        for (int i = 0; i < 256; i++)
        {
            result[i] = 20 * std::log10f(result[i]);

            if (i == FFT_POS_CUR_0)
            {
                *density0 = result[i];
            }
            else if (i == FFT_POS_CUR_1)
            {
                *density1 = result[i];
            }
            if (result[i] < minDB)
            {
                result[i] = minDB;
            }
            result[i] = 1.0F - result[i] / minDB;
        }
    }
    else
    {
        *density0 = result[FFT_POS_CUR_0];
        *density1 = result[FFT_POS_CUR_1];
    }

    *y0 = (int)(Grid::Math::Bottom() - result[FFT_POS_CUR_0] * Grid::Math::Height());
    *y1 = (int)(Grid::Math::Bottom() - result[FFT_POS_CUR_1] * Grid::Math::Height());
}


Float MathFPGA::GetMaxFromArrayWithErrorCode(puchar data, int numPoints)
{
    Float max = Math::GetMaxFromArray(data, numPoints);

    if (max >= ValueFPGA::MAX)
    {
        max.SetInvalid();
    }

    return max;
}


Float MathFPGA::GetMinFromArrayWithErrorCode(puchar data, int numPoints)
{
    Float min = Math::GetMinFromArray(data, numPoints);

    if ((uint8)min < ValueFPGA::MIN || min >= ValueFPGA::MAX)
    {
        min.SetInvalid();
    }

    return min;
}


void MathFPGA::CalculateMathFunction(float* data0andResult, const float* data1, int numPoints)
{
    if (MATH_FUNC_IS_SUM)
    {
        int delta = data1 - data0andResult;
        float* end = &data0andResult[numPoints];
        while (data0andResult < end)
        {
            *data0andResult += *(data0andResult + delta);
            data0andResult++;
        }
    }
    else if (MATH_FUNC_IS_MUL)
    {
        int delta = data1 - data0andResult;
        float* end = &data0andResult[numPoints];
        while (data0andResult < end)
        {
            *data0andResult *= *(data0andResult + delta);
            data0andResult++;
        }
    }
}


template <class T>
void MathFPGA::ShiftBuffer(T * const first, T * const last, int shift)
{
    if (shift < 0)
    {
        T *pointer = first - shift;

        while (pointer < last)
        {
            *(pointer + shift) = *pointer;
            pointer++;
        }

        T value = *(last + shift - 1);

        for (int i = 0; i < -shift; i++)
        {
            *(last - i - 1) = value;
        }
    }
    else if (shift > 0)
    {
        T *pointer = last - shift - 1;

        while (pointer >= first)
        {
            *(pointer + shift) = *pointer;
            pointer--;
        }

        T value = *(first + shift);

        for (int i = 0; i < shift; i++)
        {
            *(first + i) = value;
        }
    }
}


void MathFPGA::PointsRelToVoltage(const uint8 *points, int numPoints, Range::E range, int16 rShift, float *voltage)
{
    int voltInPixel = voltsInPixelInt[range];
    float maxVoltsOnScreen = MAX_VOLTAGE_ON_SCREEN(range);
    float rShiftAbs = RSHIFT_2_ABS(rShift, range);
    int diff = (int)((ValueFPGA::MIN * voltInPixel) + (maxVoltsOnScreen + rShiftAbs) * 20e3f);
    float koeff = 1.0f / 20e3f;

    for (int i = 0; i < numPoints; i++)
    {
        voltage[i] = (points[i] * voltInPixel - diff) * koeff;
    }
}


void MathFPGA::PointsVoltageToRel(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points)
{
    float maxVoltOnScreen = MAX_VOLTAGE_ON_SCREEN(range);
    float rShiftAbs = RSHIFT_2_ABS(rShift, range);
    float voltInPixel = 1.0f / voltsInPixel[range];

    float add = maxVoltOnScreen + rShiftAbs;

    float delta = add * voltInPixel + ValueFPGA::MIN;

    for (int i = 0; i < numPoints; i++)
    {
        int value = (int)(voltage[i] * voltInPixel + delta);

        if (value < 0)
        {
            points[i] = 0;
            continue;
        }
        else if (value > 255)
        {
            points[i] = 255;
            continue;
        }

        points[i] = (uint8)value;
    }
}
