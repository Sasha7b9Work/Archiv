//  [7/21/2009 Sasha]
/*
Здесь разные функции для обработки сигнала перед выводом
1. Сгалживающий скользящий фильтр - Palm.
2. Усреднение по нескольким сигналам - Average
3. Инвертирование - Invert
4. Отключение сигнала - Disable
5. Сложение сигналов - Addition
6. Перемножение сигналов - Multiplication
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

    // Для режима сглаживающего фильтра
    void SetModePalm(int channel, bool palm);
    void SetPointsPalm(int);

    // Для усреднения
    void SetModeAverage(bool);
    void SetNumberAverage(int);

    // Для негатива - отключения канала
    void SetInvert(int channel, bool invert);

    void SetMathFunction(NS_TypesMath::Function func, bool enable);

    void EnableSpectr(int channel, bool spectr);

private:
    CircularStack <DataOscillograph> *dataStack;

    bool palm[2];           // Признак того, что нужно проиводить сглаживающую скользящую фильтрацию
    int pointsPalm;         // Количество точек, по которым производится сглаживающая скользящая фильтрация

    bool enableAverage;     // Признак того, что нужно производить усреднение по numberAverage реализациям
    int numberAverage;      // Количество усредняемых реализаций сигнала

    bool invert[2];         // Признак того, что нужно инвертировать сигнал первого канала

    bool addition;          // Признак того, что каналы нужно сложить

    bool multiplication;    // Признак того, что каналы нужно перемножить

    bool spectr[2];

    void PreparingForDraw(DataOscillograph *data, int _range0, int _range1, double _rShiftAbs0, double _rShiftAbs1, int _tBase);    // Подготовить данные к рисованию - привести выводимые значения к экрану
    
    // Расчитывает промежуточные точки - для режима реального времени малых времён (< 1мкс) и для режима рандомизатора
    void Interpolation(DataOscillograph *data, int _tBase);

    // Возвращает сглаженный по palmPoints точкам сигнал
    void Palm(DataOscillograph *data, int _tBase);

    // Усреднение по реализациям
    void Average(DataOscillograph *data);

    // Инвертирование
    void Invert(DataOscillograph *data, double _rShiftAbs0, double _rShiftAbs1);

    // Сложение каналов
    void Addition(DataOscillograph *data);

    // Перемножение каналов
    void Multiplication(DataOscillograph *data);

    // Вычисление спектра
    void Spectr(DataOscillograph *data, int channel);

    void SetPointsPalm(int _tBase, int &_pointsPalm0, int &_pointsPalm1);
};