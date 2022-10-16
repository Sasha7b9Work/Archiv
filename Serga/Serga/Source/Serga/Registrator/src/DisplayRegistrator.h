//  [4/23/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QFrame>
#pragma warning(default:4127)
#include <cmath>
#include "DataRegistrator.h"
#include "SettingsDisplayRegistrator.h"

class SignalScreenRegistrator;

class DisplayRegistrator : public QFrame{
    Q_OBJECT
public:
    DisplayRegistrator(QFrame *parent, int numChannel, std::vector<Data> &rDatas);
    ~DisplayRegistrator();

    // Последняя выводимая точка отсчитывается от конца вектора
	void DrawData(int lastPoint);

    // После изменения настроек альтеры нужно вызывать эту функцию для очистки буфера данных дисплея
    void Reset();
    void ConnectToDevice(bool connected);

    void SetColorBackground(const QColor color);
    void SetColorGrid(const QColor color);
    void SetColorSignal(const QColor color);
    void SetColorCurs(const QColor color);
    int GetHeight();
    void EnableUCurs(bool enable);
    void SetPosUCur(int numCur, int pos);
    void GetPosUCur(int numCur, int pos);

    SignalScreenRegistrator *signalScreen;

private slots:
    void SlotMoveSignal(int);

signals:
    void SignalMoveSignal(int);

private:
    QFrame *frame;
    SettingsDisplayRegistrator settings;
};