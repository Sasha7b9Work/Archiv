#pragma once
#pragma warning(disable:4127)
#include <QMainWindow>
#include <QObject>
#include "ui_WindowAnalyzer.h"
#pragma warning(default:4127)
#include "../../common/Configurator.h"
#include "Cursors.h"
#include "../common/TypesAltera.h"

class Display;

class WindowAnalyzer:public QMainWindow, public Ui::WindowAnalyzer 
{
    Q_OBJECT

public:
    WindowAnalyzer(QMainWindow *parent = 0);

private slots:
    void Start(bool);
    void OnTimer();
    void SetNumberPort(int);
    void Calibration();
    void SetFreq(int);
    void SetU(int);
    void SetActiveSpectr(int);
    void SetWindow(int);
    void EnableCursors(int);
    void UpdatePorts();
    void SetColorBackground();
    void SetColorGrid();
    void SetColorSignal();
    void MovedCursor(int cursor, int position);
    void close();

signals:
    void terminate();

protected:
    void closeEvent(QCloseEvent *);

private:
    Configurator *configurator;
    // Структура, хранящая настройки по частоте
    struct FreqSet 
    {
        FreqSet(NS_TypesAltera::FBase b, QString s) : fBase(b), string(s) {}
        NS_TypesAltera::FBase fBase;
        QString string;
    };
	std::vector<FreqSet> freqArray;

    // Структура для настроек по напряжению
    struct VoltSet 
    {
        VoltSet(int r, QString s) : range(r), string(s) {};
        int range;
        QString string;
    };
	std::vector<VoltSet> voltArray;

    QTimer  *timer;
    Display *display;
    Cursor  *cursors;
    short   ports[256];
    int     numPorts;

    int range[2];               // Здесь хранится индекс комбобокса диапазона напряжения для первого и второго каналов соответственно
    int freq[2];                // Здесь хранятся индексы комбобкса диапазона частот для первого и второго каналов соответсвенно

    void LoadSettings();
    void LoadDefaultColors();
    void LoadDefaultSettings();
    void LoadConfigSettings();
    void SaveSettings();
    void ConnectiongSlots();
    void ConfigureWidgets();
    void SetCommonSettings();
    void AutoMeasures();
};