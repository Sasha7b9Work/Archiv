//  [11/25/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QMainWindow>
#include <QObject>
#include <QComboBox>
#include <QThread>
#include <QTimer>
#include <QPushButton>
#pragma warning(default:4127)
#include <vector>
#include "../../common/Configurator.h"

/*
������ ������ ������.
��� �������� ���������� ���������� ���������� ������.
��� ���� ��������� ��� com-����� �� 1 �� 32.
����� ������ ��������� ���� ������������ �� ������� ����������� � ���� ������� ������� � ����������.
���� ������ ���������, �� ���� ���� ��������������� ��������, �������� ����������, � ���� ����������� ��������� ���������
� ���������� ������� ����.
� ����� � ����������� �������������� ���������� ����� ���������� �� ���������� ������.
���� ������ ������, �� ���������� ���������� ������ � �� ��������.
���� ������ �� ������, ����������� ����� �� ����������. ���� ����� ����, �� ��������� ��������� ������ �� �������.
���� ������ ���, �� 
*/

class Display;

class Voltmeter_Window:public QMainWindow {
    Q_OBJECT

#define EMPTY_MEASURE "-.-"     // ��� �������� ����� ���������� � ���������, ���� �� ������� ��������� ��������

public:
    Voltmeter_Window(QMainWindow *parent = 0);

private slots:
    void UpdatePorts();
    bool SetComPort(int);
    void Start(bool);
    void OnTimer();
    void SetU0(int);
    void SetU1(int);
    void SetSinchroChannel(int);
    void SetDC_Mode0(bool);
    void SetDC_Mode1(bool);
    void SetColorBackground();
    void SetColorSignal();
    void SetColorGrid();
    void SetColorFont();
    void Calibrate();
    void OnTimerCalibrateZero();

protected:
    virtual void closeEvent(QCloseEvent *);

private:
    // ���������, �������� ��������� �� �������
    struct FreqSet {
        FreqSet(int b, QString s) : tBase(b), string(s) {};
        int tBase;
        QString string;
    };

	std::vector<FreqSet> freqArray;

    // ��������� ��� �������� �� ����������
    struct VoltSet {
        VoltSet(int r, QString s) : range(r), string(s) {};
        int range;
        QString string;
    };

	std::vector<VoltSet> voltArray;

    QTimer *timer;
    QTimer *timerCalibrateZero;
    std::vector<Display*> displays;
    Configurator *configurator;
    QComboBox *cbPort;
    QPushButton *pbUpdate;
    QPushButton *pbStart;
    QComboBox *cbFreq0;
    QPushButton *pbAC0;
    QComboBox *cbFreq1;
    QPushButton *pbAC1;
    QComboBox *cbU0;
    QComboBox *cbU1;
    short ports[256];
    int numPorts;

    int currentChannelMeasure;  // ����� �������� ����� ������, ��� �������� ������������ ���������

    void ConnectingSlots();
    void ConfigureWidgets();
    void CreateMenu();
    void SetCommonSettings();
    void SetSettings();
    void LoadSettings();
    void LoadDefaultSettings();
    void LoadConfigSettings();
    void SaveSettings();
    void StartAltera(int channel);
};