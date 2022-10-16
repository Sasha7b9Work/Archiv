#pragma once

struct DataAnalyzer;

class MathAnalyzer 
{
public:
    MathAnalyzer();

    DataAnalyzer* Processing(DataAnalyzer *_data, int _fBase0, int _fBase1);
    void SetWindow(int w) 
    { 
        window = w; 
    };

private:
    int window;

    void Spectr(DataAnalyzer *data, int channel);
    void WindowHamming(double *data);
    void WindowBlackman(double *data);
};