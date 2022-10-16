// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Containers/Values_.h"
#include "Data/InputStorage/InputStorage.h"
#include "Data/ProcessingData.h"
#include "common/Utils/Math_.h"
#include "FPGA/MathFPGA.h"
#include "common/Display/Painter/Primitives_.h"
#include <cstring>
#include <cmath>


#define EXIT_IF_ERROR_FLOAT(x)      if(!x.IsValid()) { return x; }

#define EXIT_IF_ERRORS_FLOAT(x, y)  if(!x.IsValid() || !y.IsValid())  { return x; }

#define EXIT_IF_ERROR_INT(x)        if(!x.IsValid()) { return InvalidFloat(); }


namespace Processing
{
    // Точно вычисляет период или целое число периодов в точках сигнала.
    static Int CalculatePeriodAccurately(const Chan &ch);

    // Найти точку пересечения сигнала с горизонтальной линией, проведённой на уровне yLine. numItersection - порядковый номер пересечения, 
    // начинается с 1. downToTop - если true, ищем пересечение сигнала со средней линией при прохождении из "-" в "+".
    static Float FindIntersectionWithHorLine(const Chan &ch, int numIntersection, bool downToUp, uint8 yLine);

    static void CountedToCurrentSettings(const ReadingFrame &dr);

    // Расчитать все измерения.
    static void CalculateMeasures();

    // Привести данные к текущим настройкам
    static void LeadToCurrentSettings(ReadingFrame &dr);
    static void LeadToCurrentSettings(ReadingFrame &dr, const Chan &ch);

    typedef Float(*pFuncCalculate)(const Chan &);

    struct MeasureCalculate
    {
        char *name;
        pFuncCalculate FuncCalculate;
        pFuncCFBC      FucnConvertate;
        // Если true, нужно показывать знак.
        bool           showSign;
    };

    // Функции расчёта значений
    struct Calculate
    {
        static Float VoltageMax(const Chan &ch);
        static Float VoltageMin(const Chan &ch);
        static Float VoltagePic(const Chan &ch);
        static Float VoltageMaxSteady(const Chan &ch);
        static Float VoltageMinSteady(const Chan &ch);
        static Float VoltageAmpl(const Chan &ch);
        static Float VoltageAverage(const Chan &ch);
        static Float VoltageRMS(const Chan &ch);
        static Float VoltageVybrosPlus(const Chan &ch);
        static Float VoltageVybrosMinus(const Chan &ch);
        static Float Period(const Chan &ch);
        static Float DelayPlus(const Chan &ch);
        static Float DelayMinus(const Chan &ch);
        static Float Frequency(const Chan &ch);
        static Float TimeNarastaniya(const Chan &ch);
        static Float TimeSpada(const Chan &ch);
        static Float DurationPlus(const Chan &ch);
        static Float DurationMinus(const Chan &ch);
        static Float SkvaznostPlus(const Chan &ch);
        static Float SkvaznostMinus(const Chan &ch);
        static Float PhazaPlus(const Chan &ch);
        static Float PhazaMinus(const Chan &ch);
    };

    // Расчёт относительных значений
    struct CalculateRel
    {
        // Возвращает минимальное значение относительного сигнала    
        static Float Min(const Chan &ch);

        // Возвращает максимальное значение относительного сигнала
        static Float Max(const Chan &ch);

        static Float Peak(const Chan &ch);

        // Возвращает минимальное установившееся значение относительного сигнала
        static float MinSteady(const Chan &ch);

        // Возвращает максимальное установившееся значение относительного сигнала
        static float MaxSteady(const Chan &ch);

        // Возвращает среденее значение относительного сигнала
        static Float Average(const Chan &ch);
    };

    static char *FloatFract2StringC(float value, bool always_sign, char *buffer)
    {
        return std::strcpy(buffer, Float(value).ToText(always_sign, 4).c_str());
    }

    static char *Voltage2StringC(float value, bool always_sign, char *buffer)
    {
        return std::strcpy(buffer, Voltage(value).ToText(always_sign).c_str());
    }

    static char *Time2StringC(float value, bool always_sign, char *buffer)
    {
        return std::strcpy(buffer, Time(value).ToText(always_sign).c_str());
    }

    static char *Freq2StringC(float value, bool, char *buffer)
    {
        return std::strcpy(buffer, Frequency(value).ToText().c_str());
    }

    static char *Phase2StringC(float value, bool, char *buffer)
    {
        return std::strcpy(buffer, Phase(value).ToText().c_str());
    }

    static const Processing::MeasureCalculate measures[TypeMeasure::Count] =
    {
        {0, 0},
        {"CalculateVoltageMax",         Calculate::VoltageMax,           Voltage2StringC, true},
        {"CalculateVoltageMin",         Calculate::VoltageMin,           Voltage2StringC, true},
        {"CalculateVoltagePic",         Calculate::VoltagePic,           Voltage2StringC, false},
        {"CalculateVoltageMaxSteady",   Calculate::VoltageMaxSteady,     Voltage2StringC, true},
        {"CalculateVoltageMinSteady",   Calculate::VoltageMinSteady,     Voltage2StringC, true},
        {"CalculateVoltageAmpl",        Calculate::VoltageAmpl,          Voltage2StringC, false},
        {"CalculateVoltageAverage",     Calculate::VoltageAverage,       Voltage2StringC, true},
        {"CalculateVoltageRMS",         Calculate::VoltageRMS,           Voltage2StringC, false},
        {"CalculateVoltageVybrosPlus",  Calculate::VoltageVybrosPlus,    Voltage2StringC, false},
        {"CalculateVoltageVybrosMinus", Calculate::VoltageVybrosMinus,   Voltage2StringC, false},
        {"CalculatePeriod",             Calculate::Period,               Time2StringC, false},
        {"CalculateFreq",               Calculate::Frequency,            Freq2StringC, false},
        {"CalculateTimeNarastaniya",    Calculate::TimeNarastaniya,      Time2StringC, false},
        {"CalculateTimeSpada",          Calculate::TimeSpada,            Time2StringC, false},
        {"CalculateDurationPlus",       Calculate::DurationPlus,         Time2StringC, false},
        {"CalculateDurationPlus",       Calculate::DurationMinus,        Time2StringC, false},
        {"CalculateSkvaznostPlus",      Calculate::SkvaznostPlus,        FloatFract2StringC, false},
        {"CalculateSkvaznostMinus",     Calculate::SkvaznostMinus,       FloatFract2StringC, false},
        {"CalculateDelayPlus",          Calculate::DelayPlus,            Time2StringC, false},
        {"CalculateDelayMinus",         Calculate::DelayMinus,           Time2StringC, false},
        {"CalculatePhazaPlus",          Calculate::PhazaPlus,            Phase2StringC, false},
        {"CalculatePhazaMinus",         Calculate::PhazaMinus,           Phase2StringC, false}
    };

    struct MeasureValue
    {
        float value[2];
    };

    static uint8 *input[Chan::Count] = {nullptr, nullptr};      // Указатель на данные, по которым нужно производить расчёт
    static int input_points = 0;                                // Количество точек для расчёта
    static DataSettings *pds = nullptr;                         // Настройки обрабатываемых данных


    static MeasureValue values[TypeMeasure::Count] = {{0.0F, 0.0F}};

    MarkersVertical   markersVert[Chan::Count];
    MarkersHorizontal markersHor[Chan::Count];

    static bool maxIsCalculating[2] = {false, false};
    static bool minIsCalculating[2] = {false, false};
    static bool maxSteadyIsCalculating[2] = {false, false};
    static bool minSteadyIsCalculating[2] = {false, false};
    static bool aveIsCalculating[2] = {false, false};
    static bool periodIsCaclulating[2] = {false, false};
    static bool periodAccurateIsCalculating[2];
    static bool picIsCalculating[2] = {false, false};
}


void Processing::Process(ReadingFrame &dr)
{
    LeadToCurrentSettings(dr);       // Приводим к текущим настройкам

    CalculateMeasures();            // И рассчитываем результаты автоматических измерений
}


void Processing::GetData(uint8 **dataA, uint8 **dataB, DataSettings **ds)
{
    *dataA = input[ChA];
    *dataB = input[ChB];
    *ds = pds;
}


void Processing::CalculateMeasures()
{
    if(!SHOW_MEASURES || !pds)
    {
        return;
    }

    maxIsCalculating[0] = false;
    maxIsCalculating[1] = false;
    maxSteadyIsCalculating[0] = false;
    maxSteadyIsCalculating[1] = false;
    minIsCalculating[0] = false;
    minIsCalculating[1] = false;
    minSteadyIsCalculating[0] = false;
    minSteadyIsCalculating[1] = false;
    aveIsCalculating[0] = false;
    aveIsCalculating[1] = false;
    periodIsCaclulating[0] = false;
    periodIsCaclulating[1] = false;
    periodAccurateIsCalculating[0] = false;
    periodAccurateIsCalculating[1] = false;
    picIsCalculating[0] = false;
    picIsCalculating[1] = false;

    for(int str = 0; str < TableAutoMeasures::NumRows(); str++)
    {
        for(int elem = 0; elem < TableAutoMeasures::NumCols(); elem++)
        {
            TypeMeasure::E meas = TableAutoMeasures::Type(str, elem);
            pFuncCalculate func = measures[meas].FuncCalculate;
            if(func)
            {
                if(MEASURE_IS_MARKED(meas) || MEASURE_IS_MARKED(TypeMeasure::None))
                {
                    markersVert[ChA].SetInvalid();
                    markersVert[ChB].SetInvalid();
                    markersHor[ChA].SetInvalid();
                    markersHor[ChB].SetInvalid();
                }

                if(MEASURES_SOURCE_IS_A || MEASURES_SOURCE_IS_A_B)
                {
                    values[meas].value[ChA] = func(ChA.value);
                }

                if(MEASURES_SOURCE_IS_B || MEASURES_SOURCE_IS_A_B)
                {
                    values[meas].value[ChB] = func(ChB.value);
                }
            }
        }
    }
}


void Processing::LeadToCurrentSettings(ReadingFrame &dr)
{
    UNUSED(dr);

#ifndef GUI

    LeadToCurrentSettings(dr, ChA);
    LeadToCurrentSettings(dr, ChB);

    pds = &dr.Settings();

//    CountedToCurrentSettings();

#endif
}


void Processing::LeadToCurrentSettings(ReadingFrame &dr, const Chan &ch)
{
    if (!ENABLED(ch))
    {
        input[ch] = nullptr;

        return;
    }

    BitSet64 p = SettingsDisplay::BytesOnDisplay();

    input_points = p.second - p.first;

    input[ch] = dr.Data(ch) + p.first;

    int numSmoothing = (int)Smoothing::NumPoints(SMOOTHING);

    Buffer<uint8> out(input_points);

    Math::CalculateFiltrArray(input[ch], out.Data(), input_points, numSmoothing);

    std::memcpy(dr.Data(ch) + p.first, out.Data(), (uint)input_points);
}


void Processing::CountedToCurrentSettings(const ReadingFrame & /*dr*/)
{
//    DataSettings &ds = dr.Settings();
//
//    out[ChA].Realloc(ds.BytesInChannel());
//    out[ChB].Realloc(ds.BytesInChannel());
//
//    int input_points = (int)in[ChA].Size();
//
//    int dataTShift = pds->tshift;
//
//    int dTShift = TSHIFT - dataTShift;
//
//    for (int i = 0; i < input_points; i++)
//    {
//        int index = i - dTShift;
//        if (index >= 0 && index < input_points)
//        {
//            out[ChA][index] = in[0][i];
//            out[ChB][index] = in[1][i];
//        }
//    }
//
//    if (pds->IsEnabled(ChA) &&
//        (pds->range[0] != RANGE_A || pds->r_shift_a != (uint)RSHIFT_A))
//    {
//        Range::E range = RANGE_A;
//        int16 rShift = RSHIFT_A;
//
//        for (int i = 0; i < input_points; i++)
//        {
//            float absValue = Value::ToVoltage(out[ChA][i], pds->range[0], (int16)pds->r_shift_a);
//            int relValue = (int)((absValue + MAX_VOLTAGE_ON_SCREEN(range) + RShift::ToAbs(rShift, range)) /
//                MathFPGA::voltsInPixel[range] + Value::MIN);
//
//            if (relValue < Value::MIN) { out[ChA][i] = Value::MIN; }
//            else if (relValue > Value::MAX) { out[ChA][i] = Value::MAX; }
//            else { out[ChA][i] = (uint8)relValue; }
//        }
//    }
//    if (pds->IsEnabled(ChB) &&
//        (pds->range[1] != RANGE_B || pds->r_shift_b != (uint)RSHIFT_B))
//    {
//        Range::E range = RANGE_B;
//        int16 rShift = RSHIFT_B;
//
//        for (int i = 0; i < input_points; i++)
//        {
//            float absValue = Value::ToVoltage(out[ChB][i], pds->range[1], (int16)pds->r_shift_b);
//            int relValue = (int)((absValue + MAX_VOLTAGE_ON_SCREEN(range) + RShift::ToAbs(rShift, range)) /
//                MathFPGA::voltsInPixel[range] + Value::MIN);
//
//            if (relValue < Value::MIN) { out[ChB][i] = Value::MIN; }
//            else if (relValue > Value::MAX) { out[ChB][i] = Value::MAX; }
//            else { out[ChB][i] = (uint8)relValue; }
//        }
//    }
}


Float Processing::Calculate::VoltageMax(const Chan &ch)
{
    const Float max = CalculateRel::Max(ch);
   
    EXIT_IF_ERROR_FLOAT(max);

    if(MEASURE_IS_MARKED(TypeMeasure::VoltageMax))
    {
        markersHor[ch].SetValue(0, max);
    }

    Float result = ValueFPGA::ToVoltage
        ((uint8)max, pds->range[ch], (int16)(ch.IsA() ? pds->r_shift_a : pds->r_shift_b)) * Divider::ToAbs(ch);

    return result;
}


Float Processing::Calculate::VoltageMin(const Chan &ch)
{
    Float min = CalculateRel::Min(ch);

    EXIT_IF_ERROR_FLOAT(min);

    if(MEASURE_IS_MARKED(TypeMeasure::VoltageMin))
    {
        markersHor[ch].SetValue(0, min);
    }

    Float result = ValueFPGA::ToVoltage((uint8)min, pds->range[ch],
        (int16)(ch.IsA() ? pds->r_shift_a : pds->r_shift_b)) * Divider::ToAbs(ch);

    return result;
}

Float Processing::Calculate::VoltagePic(const Chan &ch)
{
    Float max = VoltageMax(ch);
    Float min = VoltageMin(ch);

    EXIT_IF_ERRORS_FLOAT(min, max);

    if(MEASURE_IS_MARKED(TypeMeasure::VoltagePic))
    {
        markersHor[ch].SetValue(0, CalculateRel::Max(ch));
        markersHor[ch].SetValue(1, CalculateRel::Min(ch));
    }

    return max - min;
}

Float Processing::Calculate::VoltageMinSteady(const Chan &ch)
{
    Float min = CalculateRel::MinSteady(ch);

    EXIT_IF_ERROR_FLOAT(min);

    if(MEASURE_IS_MARKED(TypeMeasure::VoltageMinSteady))
    {
        markersHor[ch].SetValue(0, min.Round());
    }

    return (ValueFPGA::ToVoltage((uint8)min, pds->range[ch], ch.IsA() ?
        (int16)pds->r_shift_a :
        (int16)pds->r_shift_b) * Divider::ToAbs(ch));
}

Float Processing::Calculate::VoltageMaxSteady(const Chan &ch)
{
    Float max = CalculateRel::MaxSteady(ch);

    EXIT_IF_ERROR_FLOAT(max);

    if(MEASURE_IS_MARKED(TypeMeasure::VoltageMaxSteady))
    {
        markersHor[ch].SetValue(0, max);
    }

    Range::E range = pds->range[ch];
    uint rShift = ch.IsA() ? pds->r_shift_a : pds->r_shift_b;

    return (ValueFPGA::ToVoltage((uint8)max, range, (int16)rShift) * Divider::ToAbs(ch));
}

Float Processing::Calculate::VoltageVybrosPlus(const Chan &ch)
{
    Float max = CalculateRel::Max(ch);
    Float maxSteady = CalculateRel::MaxSteady(ch);

    EXIT_IF_ERRORS_FLOAT(max, maxSteady);

    if (MEASURE_IS_MARKED(TypeMeasure::VoltageVybrosPlus))
    {
        markersHor[ch].SetValue(0, max);
        markersHor[ch].SetValue(1, maxSteady);
    }

    int16 rShift = ch.IsA() ? (int16)pds->r_shift_a : (int16)pds->r_shift_b;

    return std::fabsf(ValueFPGA::ToVoltage((uint8)maxSteady, pds->range[ch], (int16)rShift) -
        ValueFPGA::ToVoltage((uint8)max, pds->range[ch], (int16)rShift)) * Divider::ToAbs(ch);
}

Float Processing::Calculate::VoltageVybrosMinus(const Chan &ch)
{
    Float min = CalculateRel::Min(ch);
    Float minSteady = CalculateRel::MinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(min, minSteady);

    if (MEASURE_IS_MARKED(TypeMeasure::VoltageVybrosMinus))
    {
        markersHor[ch].SetValue(0, min);
        markersHor[ch].SetValue(1, minSteady);
    }

    int16 rShift = ch.IsA() ? (int16)pds->r_shift_a : (int16)pds->r_shift_b;

    return std::fabsf(ValueFPGA::ToVoltage((uint8)minSteady, pds->range[ch], (int16)rShift) -
        ValueFPGA::ToVoltage((uint8)min, pds->range[ch], (int16)rShift)) * Divider::ToAbs(ch);
}

Float Processing::Calculate::VoltageAmpl(const Chan &ch)
{
    Float max = VoltageMaxSteady(ch);
    Float min = VoltageMinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(min, max);

    if(MEASURE_IS_MARKED(TypeMeasure::VoltageAmpl))
    {
        markersHor[ch].SetValue(0, CalculateRel::MaxSteady(ch));
        markersHor[ch].SetValue(1, CalculateRel::MinSteady(ch));
    }
    return max - min;
}

Float Processing::Calculate::VoltageAverage(const Chan &ch)
{
    Int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period);

    int sum = 0;

    uint8 *data = input[ch];

    for(int i = 0; i < period; i++)
    {
        sum += *data++;
    }

    uint8 aveRel = (uint8)((float)sum / period);

    if(MEASURE_IS_MARKED(TypeMeasure::VoltageAverage))
    {
        markersHor[ch].SetValue(0, aveRel);
    }

    return (ValueFPGA::ToVoltage((uint8)aveRel, pds->range[ch], ch.IsA() ?
        (int16)pds->r_shift_a :
        (int16)pds->r_shift_b) *
        Divider::ToAbs(ch));
}

Float Processing::Calculate::VoltageRMS(const Chan &ch)
{
    Int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period);

    float rms = 0.0F;

    int16 rShift = ch.IsA() ? (int16)pds->r_shift_a : (int16)pds->r_shift_b;

    for(uint i = 0; i < (uint)period; i++)
    {
        float volts = ValueFPGA::ToVoltage(input[ch][i], pds->range[ch], rShift);
        rms +=  volts * volts;
    }

    if(MEASURE_IS_MARKED(TypeMeasure::VoltageRMS))
    {
        markersHor[ch].SetRawValue(0, ValueFPGA::FromVoltage(std::sqrtf(rms / period), pds->range[ch], rShift));
    }

    return std::sqrtf(rms / period) * Divider::ToAbs(ch);
}

Float Processing::Calculate::Period(const Chan &ch)
{
    static Float period[2] = {0.0F, 0.0F};

    if(!periodIsCaclulating[ch])
    {
        Float aveValue = CalculateRel::Average(ch);

        if(!aveValue.IsValid())
        {
            period[ch].SetInvalid();
        }
        else
        {
            Float downToTop = FindIntersectionWithHorLine(ch, 1, true, (uint8)aveValue);    // Пересечение "снизу вверх"
            Float topToDown = FindIntersectionWithHorLine(ch, 1, false, (uint8)aveValue);   // Пересечение "сверху вниз"

            EXIT_IF_ERRORS_FLOAT(downToTop, topToDown);

            Float first = downToTop < topToDown ? downToTop : topToDown;

            Float second = FindIntersectionWithHorLine(ch, 2, downToTop < topToDown, (uint8)(aveValue));

            EXIT_IF_ERRORS_FLOAT(first, second);

            float per = TShift::ToAbs((second - first), pds->GetTBase());

            period[ch] = per;
            periodIsCaclulating[ch] = true;
        }
    }

    return period[ch];
}

#define EXIT_FROM_PERIOD_ACCURACY               \
    period[ch].SetInvalid();                    \
    periodAccurateIsCalculating[ch] = true;     \
    return period[ch];

Int Processing::CalculatePeriodAccurately(const Chan &ch)
{
    static Int period[2];

    Buffer<int> sums(input_points);

    if(!periodAccurateIsCalculating[ch])
    {
        period[ch] = 0;
        Float pic = CalculateRel::Peak(ch);

        if(!pic.IsValid())
        {
            EXIT_FROM_PERIOD_ACCURACY
        }
        int delta = (int)(pic * 5);
        sums[0] = input[ch][0];

        uint i = 1;
        int *sum = &sums[i];
        uint8 *data = input[ch];
        uint8 *end = data + input_points;

        while (data < end)
        {
            uint8 point = *data++;
            if(point < ValueFPGA::MIN || point >= ValueFPGA::MAX)
            {
                EXIT_FROM_PERIOD_ACCURACY
            }
            *sum = *(sum - 1) + point;
            sum++;
        }

        int addShift = -1;
        int maxPeriod = (int)(input_points * 0.95F);

        for(int nextPeriod = 10; nextPeriod < maxPeriod; nextPeriod++)
        {
            int s = sums[addShift + nextPeriod];

            int maxDelta = 0;
            int maxStart = input_points - nextPeriod;

            int *pSums = &sums[1];
            for(int start = 1; start < maxStart; start++)
            {
                int nextSum = *(pSums + nextPeriod) - (*pSums);
                pSums++;

                int nextDelta = nextSum - s;
                if (nextSum < s)
                {
                    nextDelta = -nextDelta;
                }

                if(nextDelta > delta)
                {
                    maxDelta = delta + 1;
                    break;
                }
                else if(nextDelta > maxDelta)
                {
                    maxDelta = nextDelta;
                }
            }

            if(maxDelta < delta)
            {
                delta = maxDelta;
                period[ch] = nextPeriod;
            }
        }

        if(period[ch] == 0)
        {
            period[ch].SetInvalid();
        }
        periodAccurateIsCalculating[ch] = true;
    }

    return period[ch];
}

Float Processing::Calculate::Frequency(const Chan &ch)
{
    Float period = Period(ch);

    if (period.IsValid())
    {
        period = 1.0f / period;
    }

    return period;
}

Float Processing::FindIntersectionWithHorLine(const Chan &ch, int numIntersection, bool downToUp, uint8 yLine)
{
    int num = 0;
    int x = 0;
    int compValue = input_points - 1;

    uint8 *data = input[ch];

    if(downToUp)
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] < yLine && data[x + 1] >= yLine)
            {
                num++;
            }
            x++;
        }
    }
    else
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] > yLine && data[x + 1] <= yLine)
            {
                num++;
            }
            x++;
        }
    }
    x--;

    if (num < numIntersection)
    {
        return InvalidFloat();
    }
    return Math::GetIntersectionWithHorizontalLine((int)x, (int)data[x], (int)x + 1, (int)data[x + 1], (int)yLine);
}


Float Processing::Calculate::DurationPlus(const Chan &ch)
{
    Float aveValue = CalculateRel::Average(ch);

    EXIT_IF_ERROR_FLOAT(aveValue);

    Float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(aveValue));
    Float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, (uint8)(aveValue));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());
}


Float Processing::Calculate::DurationMinus(const Chan &ch)
{
    Float aveValue = CalculateRel::Average(ch);

    EXIT_IF_ERROR_FLOAT(aveValue);

    Float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(aveValue));
    Float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, (uint8)(aveValue));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());
}

Float Processing::Calculate::TimeNarastaniya(const Chan &ch)   // WARN Здесь, возможно, нужно увеличить точность - брать
                           // не целые значени расстояний между отсчётами по времени, а рассчитывать пересечения линий
{
    Float maxSteady = CalculateRel::MaxSteady(ch);
    Float minSteady = CalculateRel::MinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady);

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    Float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(min01));
    Float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(max09));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);
    
    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, (uint8)(max09));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    float retValue = TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());

    if (MEASURE_IS_MARKED(TypeMeasure::TimeNarastaniya))
    {
        markersHor[ch].SetRawValue(0, (int)max09);
        markersHor[ch].SetRawValue(1, (int)min01);
        markersVert[ch].SetRawValue(0, (int)(firstIntersection - SHIFT_IN_MEMORY_IN_POINTS));
        markersVert[ch].SetRawValue(1, (int)(secondIntersection - SHIFT_IN_MEMORY_IN_POINTS));
    }

    return retValue;
}

Float Processing::Calculate::TimeSpada(const Chan &ch)                          // WARN Аналогично времени нарастания
{
    Float maxSteady = CalculateRel::MaxSteady(ch);
    Float minSteady = CalculateRel::MinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady);

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    Float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(max09));
    Float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(min01));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, (uint8)(min01));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    float result = TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());

    if (MEASURE_IS_MARKED(TypeMeasure::TimeSpada))
    {
        markersHor[ch].SetRawValue(0 ,(int)max09);
        markersHor[ch].SetRawValue(1, (int)min01);
        markersVert[ch].SetRawValue(0, (int)(firstIntersection - SHIFT_IN_MEMORY_IN_POINTS));
        markersVert[ch].SetRawValue(1, (int)(secondIntersection - SHIFT_IN_MEMORY_IN_POINTS));
    }

    return result;
}


Float Processing::Calculate::SkvaznostPlus(const Chan &ch)
{
    Float period = Period(ch);
    Float duration = DurationPlus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration);

    return period / duration;
}


Float Processing::Calculate::SkvaznostMinus(const Chan &ch)
{
    Float period = Period(ch);
    Float duration = DurationMinus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration);

    return period / duration;
}


float Processing::CalculateRel::MinSteady(const Chan &ch)
{
    static Float min[2] = {255.0F, 255.0F};

    if(!minSteadyIsCalculating[ch])
    {
        Float aveValue = Average(ch);

        if(!aveValue.IsValid())
        {
            min[ch].SetInvalid();
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = input[ch];
            puchar const end = data;

            while(data <= end)
            {
                uint8 d = *data++;
                if(d < aveValue)
                {
                    sum += d;
                    numSums++;
                }
            }

            min[ch] = (float)sum / numSums;
            int numMin = numSums;

            Float pic = Peak(ch);

            if (!pic.IsValid())
            {
                min[ch].SetInvalid();
            }
            else
            {
                int numDeleted = 0;
                float value = pic / 9.0F;
                data = input[ch];
                float _min = min[ch];

                while (data <= end)
                {
                    uint8 d = *data++;
                    if (d < aveValue)
                    {
                        if (d < _min)
                        {
                            if (_min - d > value)
                            {
                                sum -= d;
                                --numSums;
                                ++numDeleted;
                            }
                        }
                        else if (d - _min > value)
                        {
                            sum -= d;
                            --numSums;
                            ++numDeleted;
                        }
                    }
                }

                min[ch] = (numDeleted > numMin / 2.0F) ? Min(ch) : Float((float)sum / numSums);
            }
        }

        minSteadyIsCalculating[ch] = true;
    }

    return min[ch];
}


float Processing::CalculateRel::MaxSteady(const Chan &ch)
{
    static Float max[2] = {255.0F, 255.0F};

    if(!maxSteadyIsCalculating[ch])
    {
        Float aveValue = Average(ch);
        
        if(!aveValue.IsValid())
        {
            max[ch].SetInvalid();
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = input[ch];
            puchar const end = data + input_points;
            while (data <= end)
            {
                uint8 d = *data++;
                if(d > aveValue)
                {
                    sum += d;
                    numSums++;
                }
            }
            max[ch] = (float)sum / numSums;

            Float pic = Peak(ch);

            if (!pic.IsValid())
            {
                max[ch].SetInvalid();
            }
            else
            {
                int numDeleted = 0;
                int numMax = numSums;
                float value = pic / 9.0F;

                data = input[ch];
                uint8 _max = (uint8)(max[ch]);
                while (data <= end)
                {
                    uint8 d = *data++;
                    if (d > aveValue)
                    {
                        if (d > _max)
                        {
                            if (d - _max > value)
                            {
                                sum -= d;
                                numSums--;
                                numDeleted++;
                            }
                        }
                        else if (_max - d > value)
                        {
                            sum -= d;
                            numSums--;
                            numDeleted++;
                        }
                    }
                }

                max[ch] = (numDeleted > numMax / 2) ? Max(ch) : Float((float)sum / numSums);
            }
        }
        maxSteadyIsCalculating[ch] = true;
    }

    return max[ch];
}


Float Processing::CalculateRel::Max(const Chan &ch)
{
    static Float max[2] = {0.0F, 0.0F};

    if(!maxIsCalculating[ch])
    {
        max[ch] = MathFPGA::GetMaxFromArrayWithErrorCode(input[ch], input_points);

        maxIsCalculating[ch] = true;
    }

    return max[ch];
}


Float Processing::CalculateRel::Min(const Chan &ch)
{
    static Float min[2] = {255.0F, 255.0F};

    if (!minIsCalculating[ch])
    {
        min[ch] = MathFPGA::GetMinFromArrayWithErrorCode(input[ch], input_points);

        minIsCalculating[ch] = true;
    }

    return min[ch];
}


Float Processing::CalculateRel::Average(const Chan &ch)
{
    static Float ave[2] = {0.0F, 0.0F};

    if(!aveIsCalculating[ch])
    {
        Float min = Min(ch);
        Float max = Max(ch);

        if (!min.IsValid() || !max.IsValid())
        {
            ave[ch].SetInvalid();
        }
        else
        {
            ave[ch] = (min + max) / 2.0F;
        }

        aveIsCalculating[ch] = true;
    }

    return ave[ch];
}


Float Processing::CalculateRel::Peak(const Chan &ch)
{
    static Float pic[2] = {0.0F, 0.0F};

    if(!picIsCalculating[ch])
    {
        Float min = CalculateRel::Min(ch);
        Float max = CalculateRel::Max(ch);

        if (!min.IsValid() || !max.IsValid())
        {
            pic[ch].SetInvalid();
        }
        else
        {
            pic[ch] = max - min;
        }

        picIsCalculating[ch] = true;
    }
    return pic[ch];
}


Float Processing::Calculate::DelayPlus(const Chan &ch)
{
    Float period0 = Period(ChA);
    Float period1 = Period(ChB);

    EXIT_IF_ERRORS_FLOAT(period0, period1);

    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return InvalidFloat();
    }

    Float average0 = CalculateRel::Average(ChA);
    Float average1 = CalculateRel::Average(ChB);

    EXIT_IF_ERRORS_FLOAT(average0, average1);

    float averageFirst = ch.IsA() ? average0 : average1;
    float averageSecond = ch.IsA() ? average1 : average0;

    Chan firstChannel = ch;
    Chan secondChannel = firstChannel.Opposed();

    Float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, true, (uint8)averageFirst);
    Float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, true, (uint8)averageSecond);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, true, (uint8)(averageSecond));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());
}


Float Processing::Calculate::DelayMinus(const Chan &ch)
{
    Float period0 = Period(ChA.value);
    Float period1 = Period(ChB.value);

    EXIT_IF_ERRORS_FLOAT(period0, period1);

    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return InvalidFloat();
    }

    Float average0 = CalculateRel::Average(ChA.value);
    Float average1 = CalculateRel::Average(ChB.value);

    EXIT_IF_ERRORS_FLOAT(average0, average1);

    float averageFirst = ch.IsA() ? average0 : average1;
    float averageSecond = ch.IsA() ? average1 : average0;
    Chan firstChannel = ch;
    Chan secondChannel = firstChannel.Opposed();

    Float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, false, (uint8)averageFirst);
    Float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, false, (uint8)averageSecond);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, false, (uint8)averageSecond);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());
}


Float Processing::Calculate::PhazaPlus(const Chan &ch)
{
    Float delay = DelayPlus(ch);
    Float period = Period(ch);

    if(!delay.IsValid() || !period.IsValid())
    {
        return InvalidFloat();
    }

    return delay / period * 360.0F;
}


Float Processing::Calculate::PhazaMinus(const Chan &ch)
{
    Float delay = DelayMinus(ch);
    Float period = Period(ch);

    if(!delay.IsValid() || !period.IsValid())
    {
        return InvalidFloat();
    }

    return delay / period * 360.0F; 
}


float Processing::GetCursU(const Chan &ch, float posCurT)
{   
    BitSet64 p = SettingsDisplay::PointsOnDisplay();

    return Math::Limitation((float)(200.0F - input[ch][p.first + (int)posCurT] + ValueFPGA::MIN), 0.0F, 200.0F);
}


float Processing::GetCursT(const Chan &ch, float posCurU, int numCur)
{
    BitSet64 p = SettingsDisplay::PointsOnDisplay();

    int prevData = 200 - input[ch][p.first] + ValueFPGA::MIN;

    int numIntersections = 0;

    for(int i = p.first + 1; i < p.second; i++)
    {
        int curData = 200 - input[ch][i] + ValueFPGA::MIN;

        if(curData <= posCurU && prevData > posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - p.second);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - p.first);
                }
            }
        }

        if(curData >= posCurU && prevData < posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - p.first);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - p.first);
                }
            }
        }
        prevData = curData;
    }
    return 0;
}

void Processing::InterpolationSinX_X(Buffer<uint16> &buffer, TBase::E tBase)
{
/*
     Последовательности x в sin(x)
2    1. 50нс : pi/2, -pi/2 ...
8    2. 20нс : pi/5, pi/5 * 2, pi/5 * 3, pi/5 * 4, -pi/5 * 4, -pi/5 * 3, -pi/5 * 2, -pi/5 ...
18   3. 10нс : pi/10, pi/10 * 2 ... pi/10 * 9, -pi/10 * 9 .... -pi/10 * 2, -pi/10 ...
38   4. 5нс  : pi/20, pi/20 * 2 ... pi/20 * 19, -pi/20 * 19 ... -pi/20 * 2, -pi/20 ...
98   5. 2нс  : pi/50, pi/50 * 2 ... pi/50 * 49, -pi/50 * 49 ... -pi/50 * 2, -pi/50 ...
*/

#define MUL_SIN 1e7F
#define MUL     1e6F
#define KOEFF   (MUL / MUL_SIN)

    int deltas[5] = {50, 20, 10, 5, 2};
    int delta = deltas[tBase];

    Buffer<uint8> signedData(buffer.Size() / 2);

    int numSignedPoints = 0;
    
    for (int pos = 0; pos < buffer.Size(); pos++)
    {
        if (buffer[pos] > 0)
        {
            signedData[numSignedPoints] = (uint8)buffer[pos];
            numSignedPoints++;
        }
    }

    // Найдём смещение первой значащей точки

    int shift = 0;

    for (int pos = 0; pos < buffer.Size(); pos++)
    {
        if (buffer[pos] > 0)
        {
            shift = (int)pos;
            break;
        }
    }

    float deltaX = Math::Pi;
    float stepX0 = Math::Pi / (float)delta;
    float x0 = Math::Pi - stepX0;
    int num = 0;
    
    for(int i = 0; i < buffer.Size(); i++)
    {
        x0 += stepX0;
        if((i % delta) == 0)
        {
            buffer[i] = signedData[i / delta];
        }
        else
        {
            int part = num % ((delta - 1) * 2);
            num++;
            float sinX = (part < delta - 1) ?
                std::sin(Math::Pi / delta * (part + 1)) :
                std::sin(Math::Pi / delta * (part - (delta - 1) * 2));

            if (tBase > TBase::_5ns)    // Здесь используем более быструю, но более неправильную арифметику целвых чисел
            {
                int sinXint = (int)(sinX * MUL_SIN);
                int value = 0;
                int x = (int)((x0 - deltaX) * MUL);
                int deltaXint = (int)(deltaX * MUL);

                for (int n = 0; n < numSignedPoints; n++)
                {
                    value += signedData[n] * sinXint / (x - n * deltaXint);
                    sinXint = -sinXint;
                }

                buffer[i] = (uint8)(value * KOEFF);
            }
            else                     // На этих развёртках арифметика с плавающей запятой даёт приемлемое быстродействие
            {
                float value = 0.0F;
                float x = x0;

                for (int n = 0; n < numSignedPoints; n++)
                {
                    x -= deltaX;
                    value += signedData[n] * sinX / x;
                    sinX = -sinX;
                }

                buffer[i] = (uint8)value;
            }
        }
    }

    int pos = (int)(buffer.Size() - 1);

    while (pos > shift)
    {
        buffer[pos] = buffer[pos - shift];
        pos--;
    }
}

Text Processing::GetStringMeasure(TypeMeasure::E measure, const Chan &ch)
{
    if (!ch.IsEnabled())
    {
        return Text("");
    }

    Text result("%s", ch.IsA() ? "1: " : "2: ");

    if(pds == nullptr)
    {
        result.Append("-.-");
    }
    else if((ch.IsA() && !pds->enabled[ChA]) || (ch.IsB() && !pds->enabled[ChB]))
    {
    }
    else if(measures[measure].FuncCalculate)
    {
        char bufferForFunc[20];
        pFuncCFBC func = measures[measure].FucnConvertate;
        float value = values[measure].value[ch];
        char *text = func(value, measures[measure].showSign, bufferForFunc);
        result.Append(text);
    }

    return result;
}


MarkersHorizontal::MarkersHorizontal()
{
    marker[0].SetInvalid();
    marker[1].SetInvalid();
}


MarkersVertical::MarkersVertical()
{
    marker[0].SetInvalid();
    marker[1].SetInvalid();
}


void MarkersHorizontal::Draw()
{
    if (SHOW_MEASURES)
    {
        for (int i = 0; i < 2; i++)
        {
            if (marker[i].IsValid())
            {
                DashedHLine(4, 6).Draw(marker[i].value, Grid::Left(), Grid::Right(), 0);
            }
        }
    }
}


void MarkersVertical::Draw()
{
    if (SHOW_MEASURES)
    {
        for (int i = 0; i < 2; i++)
        {
            if (marker[i].IsValid())
            {
                DashedVLine(4, 6).Draw(marker[i].value, Grid::Top(), Grid::Bottom(), 0);
            }
        }
    }
}


void MarkersVertical::SetInvalid()
{
    marker[0].SetInvalid();
    marker[1].SetInvalid();
}


void MarkersVertical::SetRawValue(int num, int value)
{
    marker[num].SetValue(value);
}


void MarkersHorizontal::SetInvalid()
{
    marker[0].SetInvalid();
    marker[1].SetInvalid();
}


void MarkersHorizontal::SetValue(int num, const Float &value)
{
    marker[num].SetValue((int)ValueFPGA::ConvertToDisplay((uint8)value));
}


void MarkersHorizontal::SetRawValue(int num, int value)
{
    marker[num].SetValue(value);
}


void Processing::CalculateMathFunction(float *data0andResult, float *data1, int num_points)
{
    if (MATH_FUNC_IS_SUM)
    {
        int delta = data1 - data0andResult;
        float *end = &data0andResult[num_points];

        while (data0andResult < end)
        {
            *data0andResult += *(data0andResult + delta);
            data0andResult++;
        }
    }
    else if (MATH_FUNC_IS_MUL)
    {
        int delta = data1 - data0andResult;
        float *end = &data0andResult[num_points];

        while (data0andResult < end)
        {
            *data0andResult *= *(data0andResult + delta);
            data0andResult++;
        }
    }
}
