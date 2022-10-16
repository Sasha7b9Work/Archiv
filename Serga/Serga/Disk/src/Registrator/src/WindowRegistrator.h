//  [11/27/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include "ui_WindowRegistrator.h"
#include <QMainWindow>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QTextStream>
#pragma warning(default:4127)
#include <iostream>
#include "DataRegistrator.h"
#include "../../common/Timer.h"
#include "../../common/Configurator.h"

class DisplayRegistrator;
class QTimer;

class WindowRegistrator:public QMainWindow, public Ui::WindowRegistrator 
{
    Q_OBJECT

public:
    WindowRegistrator(QMainWindow *parent = 0);

private slots:
    void UpdatePorts();
    void Start(bool);
    void OnTimer();
    bool SetNumberPort(int _indexPort);
    void SetRange0(int);
    void SetRange1(int);
    void SB0valueChanged(int);
    void SB1valueChanged(int);
    void Open(bool);
    void SetDir(bool);
    void MoveSignal0(int);
    void MoveSignal1(int);
	void EnableUCursCh0(bool);
	void EnableUCursCh1(bool);
	void ChangePosUCur0Ch0(int);
	void ChangePosUCur1Ch0(int);
	void ChangePosUCur0Ch1(int);
	void ChangePosUCur1Ch1(int);
    void close();
    void SetColorBackground();
    void SetColorGrid();
    void SetColorSignal();
    void ChangePosUCurFromMouse0(int, int);
    void ChangePosUCurFromMouse1(int, int);
    void Calibration();

signals:
    void terminate();

protected:
    void closeEvent(QCloseEvent *);

private:
    Timer t;            // По этому таймеру будем отсчитывать интервалы времени
                        // Его нужно сбрасывать после запуска альтеры
    DisplayRegistrator *display0;
    DisplayRegistrator *display1;
    QTimer *timer;
    QFile *file;
	std::vector<Data> datas;
    QString strDir;             // Текущий каталог
    Configurator *configurator;
    short   ports[256];
    int     numPorts;

    // В этой структуре хранятся значения для заполнения комбобокса времён
    struct ComboTime 
    {
        ComboTime(QString _string, int _timeMs) : string(_string), timeMs(_timeMs) {}
        QString string;
        int timeMs;               // Это время, через которое должны считываться точки, в миллисекундах
    };
	std::vector<ComboTime> times;

	// Здесь значения для заполнения комбобокса напряжений
	struct ComboVoltage 
    {
		ComboVoltage(QString _string, int _range) : string(_string), range(_range) {};
		QString string;
		int range;
	};
	std::vector<ComboVoltage> voltages;

	// В этой структуре хранятся значения для заполнения комбобокса длин записи
    struct ComboLengthRecord 
    {
        ComboLengthRecord(QString s, int n) : string(s), numPoints(n) {}
        QString string;
        int numPoints;
    };
	std::vector<ComboLengthRecord> lengths;
    
    void LoadSettings();
    void LoadDefaultSettings();
    void LoadConfigSettings();
    void LoadDefaultColors();
    void SaveSettings();
    void ConnectingSlots();
    void ConfigureWidgets();
    void SetCommonSettings();
    void SetNewRanges();
	void WriteDeltasU();		// Записывает значения дельт напряжения в соответсвующие лайнэдиты
    void UpdateLineEdits(QSpinBox *spinbox);     // После изменения комбобоксов напряжения нужно вызывать эту функцию для обновления информации в лайнэдитах
};