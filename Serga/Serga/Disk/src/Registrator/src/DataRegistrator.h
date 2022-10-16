//  [12/6/2009 Sasha]
#pragma once
#include <QTextStream>
#include <QTime>

// ¬ этой структуре хран€тс€ данные одной точки
struct Data 
{
    friend QTextStream &operator << (QTextStream &_output, const Data &_data)
    {
        _output << _data.time.hour() << " ";
        _output << _data.time.minute() << " ";
        _output << _data.time.second() << " ";
        _output << _data.time.msec() << " ";
        _output << _data.data[0] << " ";
        _output << _data.data[1] << " ";
        _output << endl;
        return _output;
    };

    friend QTextStream &operator >> (QTextStream &_input, Data &_data)
    {
        int hour = 0;
        int minute = 0;
        int sec = 0;
        int msec = 0;
        double data1 = 0.0;
        double data2 = 0.0;
        _input >> hour;
        _input >> minute;
        _input >> sec;
        _input >> msec;
        _input >> data1;
        _input >> data2;
        _data.time = QTime(hour, minute, sec, msec);
        _data.data[0] = data1;
        _data.data[1] = data2;
        return _input;
    };

    Data(QTime t, double d1, double d2, bool _isTrue = true) : time(t), isTrue(_isTrue) { data[0] = d1; data[1] = d2; };
    Data() {};
    QTime time;
    double data[2];
    bool isTrue;                // ѕризнак того, "насто€ща€" это точка или расчитанна€ - нужно дл€ того, чтобы знать, каким цветом выводить
};

