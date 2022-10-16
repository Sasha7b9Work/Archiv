
#pragma once
/*
    Интерполяция sin(x) / x
*/
#include <vector>
#include <cmath>

class InterpolationSin {

public:
    /*
    delta - через сколько отсчётов производится измерения.
    500нс - 2, 200нс - 5, 100нс - 10, 50нс - 20, 20нс - 50, 10нс - 100
    values - указатель на вектор, содержажий измеренные значения
    */
	InterpolationSin(int _delta, std::vector<double> *_values)
    {
        values = *_values;
        delta = _delta;
    };

    // Возвращает значение для i-го вычесленного отсчёта. i принимает значения от 1 до 500
    double GetValue(int i)
    {
        if((i % delta) == 0)
            return values[i / delta];

        double retValue = 0.0;
        double X[501] = {0};
        double sinX_X[501] = {0};
        for(int n = 0; n < (int)values.size(); n++) 
        {
            double s = values[n];
            double x = 2 * asin(1.0) * 2.0 * 0.5 * ((1.0 / delta) * i - n);
            X[n] = x;
            double fi = SinXdivX(x);
            sinX_X[n] = fi;
            if(x != 0)
                retValue = retValue + s * fi;

        }
        return retValue;
    };

private:
	std::vector<double> values;
    int delta;

    double SinXdivX(double x) // Возвращает sin(x)/x
    {
        return sin(x) / x;
    };  
};