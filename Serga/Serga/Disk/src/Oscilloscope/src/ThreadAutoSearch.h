//  [1/22/2010 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QThread>
#include <QComboBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QPushButton>
#pragma warning(default:4127)

class WindowOsci;

class ThreadAutoSearch:public QThread {
    Q_OBJECT
#define timeWaiting 100000.0
public:
    ThreadAutoSearch(WindowOsci *_w);
protected:
    void run();
public:
    void FindSignal();       // ��������� ������� ������ ������� ������
signals:
    void AutoSearchExit(bool);
private:
    volatile bool find;            // ���� ��� �������� ����� 2, ����� � ������ �������� - ����� �������� ������ ����������� �� �����
    WindowOsci &w;
    void FindSignalInternal();
    bool VerifySinchro(int channel, int trigLev, double timeUs);     // ��������� ������� � �������� ������� ������������� � ������� timeUs �����������
                                                        // ���������� true, ���� ������������� ������
    bool VerifyTwoSinchro(int channel, int tBase, double timeUs);    // ���������, ���� �� ��� �������������
    bool OutFromScreen(int channel, int range, double timeUs);
    bool VerySmallSignal(int channel, double timeUs);                // ���������� true, ���� ������ ������� ���������
    int FindRange(int channel);                         // ����� Range ��� ���������������� ������
};