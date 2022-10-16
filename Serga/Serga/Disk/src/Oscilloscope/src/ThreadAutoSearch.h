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
    void FindSignal();       // Запустить процесс поиска данного канала
signals:
    void AutoSearchExit(bool);
private:
    volatile bool find;            // Если это значение равно 2, поток в режиме ожидания - поиск никакого канала производить не нужно
    WindowOsci &w;
    void FindSignalInternal();
    bool VerifySinchro(int channel, int trigLev, double timeUs);     // Запускает альтеру с заданным уровнем синхронизации и ожидает timeUs микросекунд
                                                        // Возвращает true, если синхронизация прошла
    bool VerifyTwoSinchro(int channel, int tBase, double timeUs);    // Проверить, есть ли две синхронизации
    bool OutFromScreen(int channel, int range, double timeUs);
    bool VerySmallSignal(int channel, double timeUs);                // Возвращает true, если сигнал слишком маленький
    int FindRange(int channel);                         // Найти Range для соответствующего канала
};