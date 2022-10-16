#include "GlobalFunctions.h"
#pragma warning(disable:4127)
#include <QObject>
#include <QMessageBox>
#pragma warning(default:4127)
#include <cmath>
#include "../../Device129Multi/src/AbstractAltera.h"

#define tr QObject::tr

namespace NS_GlobalFunctions 
{

    QString VoltageToString(double voltage, int numDigits) 
    {
        QString string = "";
        if     (fabs(voltage) < 9.999e-4) { string += DoubleToString(voltage * 1e6, numDigits) + tr(" uV"); }
        else if(fabs(voltage) < 9.999e-1) { string += DoubleToString(voltage * 1e3, numDigits) + tr(" mV"); }
        else                              { string += DoubleToString(voltage, numDigits) + tr(" V"); }
        return string;
    }

    QString VoltageToStringFixedForPoint(double _voltage, int _numDigits) 
    {
        QString string = "";
        if     (fabs(_voltage) < 9.999e-4) { string += DoubleToStringFixedForPoint(_voltage * 1e6, _numDigits) + tr(" uV"); }
        else if(fabs(_voltage) < 9.999e-1) { string += DoubleToStringFixedForPoint(_voltage * 1e3, _numDigits) + tr(" mV"); }
        else                               { string += DoubleToStringFixedForPoint(_voltage, _numDigits) + tr(" V"); }
        return string;
    }

    QString TimeToString(double time, int numDigits) 
    {
        QString string = "";
        if     (fabs(time) < 9.999e-10) { string += DoubleToString(time * 1e12, numDigits) + tr(" ps"); }
        else if(fabs(time) < 9.999e-7)  { string += DoubleToString(time * 1e9, numDigits) + tr(" ns");  }
        else if(fabs(time) < 9.999e-4)  { string += DoubleToString(time * 1e6, numDigits) + tr(" us");  }
        else if(fabs(time) < 9.999e-1)  { string += DoubleToString(time * 1e3, numDigits) + tr(" ms");  }
        else                            { string += DoubleToString(time, numDigits) + tr(" s");         };
        return string;
    }

    QString TimeToStringFixedForPoint(double _time, int _numDigits) 
    {
        QString string = "";
        if     (fabs(_time) < 9.999e-10) { string += DoubleToStringFixedForPoint(_time * 1e12, _numDigits) + tr(" ps"); }
        else if(fabs(_time) < 9.999e-7)  { string += DoubleToStringFixedForPoint(_time * 1e9, _numDigits) + tr(" ns");  }
        else if(fabs(_time) < 9.999e-4)  { string += DoubleToStringFixedForPoint(_time * 1e6, _numDigits) + tr(" us");  }
        else if(fabs(_time) < 9.999e-1)  { string += DoubleToStringFixedForPoint(_time * 1e3, _numDigits) + tr(" ms");  }
        else                            { string += DoubleToStringFixedForPoint(_time, _numDigits) + tr(" s");         };
        return string;
    }

    QString DoubleToString(double n, int numDigits) 
    {
        QString retString = QString("%1").arg(n, 0, 'g', 4);
        return retString;
    }

    QString DoubleToStringFixedForPoint(double _n, int _numDigits) 
    {
        static char buffer[100];
        if(_numDigits == 1) 
            sprintf_s(buffer, 100, "%.1f", _n);
        else if(_numDigits == 2) 
            sprintf_s(buffer, 100, "%.2f", _n);
        else 
            sprintf_s(buffer, 100, "%.3f", _n);
   
        return QString(buffer);
    }

    //  [8/19/2009 Sasha]
    QString FrequencyToString(double _frequency, int _numDigits) {
        QString string = "";
        if     (fabs(_frequency) <= 1e3) { string += DoubleToString(_frequency, _numDigits) + tr(" Hz");        }
        else if(fabs(_frequency) <= 1e6) { string += DoubleToString(_frequency / 1e3, _numDigits) + tr(" kHz"); }
        else if(fabs(_frequency) <= 1e9) { string += DoubleToString(_frequency / 1e6, _numDigits) + tr(" MHz"); }
        else                             { string += DoubleToString(_frequency / 1e9, _numDigits) + tr(" GHz"); }
        return string;
    }

    QString FrequencyToStringFixedForPoint(double _frequency, int _numDigits) {
        QString string = "";
        if     (fabs(_frequency) <= 1e3) { string += DoubleToStringFixedForPoint(_frequency, _numDigits) + tr(" Hz");        }
        else if(fabs(_frequency) <= 1e6) { string += DoubleToStringFixedForPoint(_frequency / 1e3, _numDigits) + tr(" kHz"); }
        else if(fabs(_frequency) <= 1e9) { string += DoubleToStringFixedForPoint(_frequency / 1e6, _numDigits) + tr(" MHz"); }
        else                             { string += DoubleToStringFixedForPoint(_frequency / 1e9, _numDigits) + tr(" GHz"); }
        return string;
    }

    template <class T>
    T MaxElem(T *array, int size)
    {
        T retValue = array[0];
        for(int i = 1; i < size; i++)
            if(array[i] > retValue)
                retValue = array[i];
        return retValue;
    }

    template <class T>
    T MinElem(T *array, int size)
    {
        T retValue = array[0];
        for(int i = 1; i < size; i++)
            if(array[i] < retValue)
                retValue = array[i];
        return retValue;
    }

    BYTE Average(BYTE *_array, int _size)
    {
        int sum = 0;
        for(int i = 0; i < _size; i++)
            sum += (int)_array[i];

        return (BYTE)(sum / _size);
    }
}

#undef tr