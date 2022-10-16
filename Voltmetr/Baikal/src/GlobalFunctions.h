//  [7/7/2009 Sasha]
#pragma once  

class QString;
class AbstractAltera;


namespace NS_GlobalFunctions {

    // Возвращает сумму элементов массива
    template <class T>
    T SumArray(T* array, int size) {
        T sum = T(0);
        for(int i = 0; i < size; i++) {
            sum += array[i];
        }
        return sum;
    };

    // Проверяет, входит ли value в диапазон min...max и, если нет, приводит к границам
    template <class T>
    T Limitation(T &value, T min, T max) {
        if(value > max) {
            value = max;
        }
        else if(value < min) {
            value = min;
        }
        return value;
    }

    //  [10/30/2009 Sasha]
    template <class T>
    void Max(T &x1, T &x2) {
        if(x2 > x1) {
            x1 = x2;
        }
    }

    //  [11/2/2009 Sasha]
    template <class T>
    void Min(T &x1, T &x2) {
        if(x2 < x1) {
            x1 = x2;
        }
    }

    //  [2/12/2010 Sasha]
    template <class T>
    T Abs(T x) {
        if(x < 0) {
            return -x;
        }
        else {
            return x;
        }
    }
}