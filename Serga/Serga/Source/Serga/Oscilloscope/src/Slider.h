#pragma once
#pragma warning(disable:4127)
#include <QSlider>
#pragma warning(default:4127)

class Slider : public QSlider
{
    Q_OBJECT

public:
    Slider(Qt::Orientation _orientation) : QSlider(_orientation) {};

public slots:
    void SetValue(int _val)
    {
        int newValue = (_val / singleStep()) * singleStep();
        setValue(newValue);
    };
};