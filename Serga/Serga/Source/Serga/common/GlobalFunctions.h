#pragma once  

class QString;
class AbstractAltera;

namespace NS_GlobalFunctions 
{

    // Возвращает сумму элементов массива
    template <class T>
    T SumArray(T* array, int size) 
    {
        T sum = T(0);
        for(int i = 0; i < size; i++) 
            sum += array[i];
        
        return sum;
    };

    // Проверяет, входит ли value в диапазон min...max и, если нет, приводит к границам
    template <class T>
    T Limitation(T &value, T min, T max) 
    {
        if(value > max) 
            value = max;
        else if(value < min) 
            value = min;
        
        return value;
    }

    template <class T>
    T Abs(T x) 
    {
        if(x < 0) 
            return -x;
        else 
            return x;
    }

    /*
    Возвращает true, если все элементы массива равны
    */
    template <class T>
    bool IsEqualsElements(T *array, int size)
    {
        T firstValue = array[0];
        int i = 0;
        for(i = 1; i < size; i++)
            if(array[i] != firstValue)
                break;
        return i == size;
    }

    template <class T>
    T MaxElem(T *array, int size);

    template <class T>
    T MinElem(T *array, int size);

    BYTE Average(BYTE *array, int size);

    QString VoltageToString(double voltage, int numDigits = 3);
    QString VoltageToStringFixedForPoint(double voltage, int numDigits = 3);
    QString TimeToString(double time, int numDigits = 3);
    QString TimeToStringFixedForPoint(double time, int numDigits = 3);
    QString DoubleToString(double n, int numDigits = 3);                // Количество значащих цифра ограничивает числом numDigits
    QString DoubleToStringFixedForPoint(double n, int numDigits = 3);   // После запятой будет numDigits цифр
    QString FrequencyToString(double frequency, int numDigits = 3);
    QString FrequencyToStringFixedForPoint(double frequency, int numDigits = 3);
}