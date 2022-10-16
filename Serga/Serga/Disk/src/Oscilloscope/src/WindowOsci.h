#pragma once
#pragma warning(disable:4127)
#include "ui_WindowOsci.h"
#include <QTimer>
#pragma warning(default:4127)

class RunScreen;
class TrigScreen;
class Display;
class Slider;
class Tuner;
class Configurator;
class ThreadAutoSearch;

static const int singleStep = 4;    // ≈диничный шаг изменени€ смещени€ по напр€жению

class WindowOsci:public QMainWindow, public Ui::WindowOsci
{
    Q_OBJECT

    friend class Tuner;

public:
    WindowOsci(QMainWindow *parent = 0);
    ~WindowOsci();

public slots:
    void close();

protected:
    void closeEvent(QCloseEvent *);

private:
    RunScreen*  runScreen;
    TrigScreen* trigScreen;
    Display*    display;
    Slider*     vsTrigLev;
    Slider*     vsRShift0;
    Slider*     vsRShift1;
    Tuner*      tuner;
    Configurator* configurator;
    QTimer*     timer;
    QTimer      *timerCalibrateZero;
    short       ports[256];
    int         numPorts;
    ThreadAutoSearch *threadSearch;

    void CreateWidgets();
    void ConfigurationWidgets();
    void CreateMenu();
    void ConnectingSlots();
    void LoadSettings();
    void LoadConfigSettings();
    void LoadDefaultColors();
    void SaveSettings();
    void AutoMeasures();
    bool DataReady();

private slots:
    void UpdatePorts();
    void SetNumberPort(int);
    void SaveScreen();
    void OnTimer();
    void Start(bool);
    void SetModeCalibrator_4V_0Hz();
    void SetModeCalibrator_4V_5kHz();
    void Calibrate();
    void LoadDefaultSettings();
    void AutoSearchEnter();
    void AutoSearchExit(bool);
    void NewOutMeasure(int);
    void OnCalibrateZero();
    void OnChangeTab(int);
    void OnChangeMeasure(int);
};