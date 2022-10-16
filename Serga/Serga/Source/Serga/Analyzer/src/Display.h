//  [4/23/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QFrame>
#pragma warning(default:4127)
#include "SettingsDisplay.h"

class SignalScreen;

class Display : public QFrame
{
    Q_OBJECT
    friend class WindowAnalyzer;

public:
    Display(QFrame *parent);
    ~Display();

    void DrawData();

    // После изменения настроек альтеры нужно вызывать эту функцию для очистки буфера данных дисплея
    void Reset();
    void Repaint();
    void ConnectToDevice(bool connect);
    void SetActiveSpectr(int spectr);
    void SetColorBackground(const QColor color);
    void SetColorGrid(const QColor color);
    void SetColorSignal(const int channel, const QColor color);
    void SetColorCurs(const int numCurs, const QColor color);
    void EnableUCurs(bool enable);
    SettingsDisplay * GetPointerSettings() { return &settings; };
    SignalScreen *signalScreen; 

protected:
    void paintEvent(QPaintEvent *);

private:
    QFrame *frame;
    SettingsDisplay settings;
};