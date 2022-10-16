//  [9/17/2009 Sasha]
/*
    Здесь таблицы значений для перевода относительных величин альтеры в абсолютные
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
		double absStep;                 // Время между двумя соседними точками. Одновременно время, соответсвующее изменению tShift на 1
        double absMinTShift;
        double absMaxTShift;
    };

    struct VoltageStruct 
    {
		NS_TypesAltera::Range range;
        double absMinVoltageMeas;       // Минимальное измеряемое значение. Соответствует нижней границе экрана
        double absMaxVoltageMeas;       // Максимальное измеряемое значение. Соответсвует верхней границе экрана
        double absStepVoltageMeas;      // Шаг, точность измерения - расстояние между двумя соседними точками по оси напряжения
        double absMinRShift;            // Минимальное значение смещения в абсолютном виде
        double absMaxRShift;            // Максимальное абсолютное значение смещения
        double absStepRShift;           // Изменение абсолютного смещения по напряжению при изменении относительного(засылаемго в регистр) на 1.
    };

    TimeStruct tableTShift[30];
    VoltageStruct tableRShift[30];

    std::vector<double> tablesStepFreq;
    std::vector<int> rangeFromIndexFrequency;

public:
    TablesOfConverting();

    double GetStepFrequency(int tBase);
    double GetStepTime(int _tBase);                     // Возвращает время между отсчётами в секундах для данного tBase
    int GetTBaseFromFBase(int _fBase);       // Получить значение масштаба по напряжению из индекса комбобокса диапазона частот. Используется в анализаторе спектра
    int GetFBaseFromTBase(int _tBase);

    // Получить абсолютное значение напряжения из относительного, полученного от осциллографа. Полученное значение не 
    // учитывает смещение и может быть использовано только для вывода сигнала на экран
    double GetAbsVoltageDraw(ulong relValue, int range);

    // Получить абсолютное значение напряжения из относительного, полученного от осциллографа. Полученное значение учитывает
    // смещение и может быть использовано для математических расчётов
    double GetAbsVoltage(ulong relValue, int range, int rShift);

    // Получить относительное значение напряжения в точках измерения из абсолютного
    int GetRelVoltage(double absValue, int range);

    // Возвращает абсолютное значение смещения по напряжению в вольтах
    double GetRShiftAbs(int rShift, int range);

    // Возвращает относительное значение смещения по напряжению (которое нужно засылать в Альтеру), соответствующее за-
    // данному абсолютному и масштабу по напряжению
    int GetRShiftRel(double rShiftAbs, int range);

    // Возвращает абсолютное значение смещения по времени в секундах
    double GetTShiftAbs(int tShift, int tBase);

    // Возвращает относительное смещение по времени (которое нужно засылать в Альтеру), соответствующее заданному абсолютному и масштабу по времени
    int GetTShiftRel(double tShiftAbs, int tBase);
};
