//  [4/23/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QFrame>
#include <QMouseEvent>
#pragma warning(disable:4127)
#include "TypesDisplay.h"
#include "SettingsDisplay.h"

class RShiftCursorScreen;
class TrigCursorScreen;
class TimeCursorScreen;
class SignalScreen;

class Display : public QFrame{
    Q_OBJECT
    friend class Tuner;
public:
    Display(QFrame *parent);
    ~Display();

    SettingsDisplay* GetPointerSettings() { return &settings; };

    void DrawData();
    void DrawPoints(int _channel, uchar _buffer[501]);
    QImage GetImage();

    // После изменения настроек альтеры нужно вызывать эту функцию для очистки буфера данных дисплея
    void Reset();               
    void ConnectToDevice(bool connect);                 // Если нет связи с устройсвом - вызывается со значением false
    void SetEnableChannel(int numChannel, bool enable);
    void    SetColorBackground(const QColor color);
    QColor  GetColorBackground()    { return settings.colorBackground; };
    void    SetColorGrid(const QColor color);
    QColor  GetColorGrid()              { return settings.colorGrid; };
    void    SetColorSignal(const int channel, const QColor color);
    QColor  GetColorSignal(int channel) { return settings.colorSignal[channel]; };
    void    SetColorCurs(const int numCurs, const QColor color);
    QColor  GetColorCurs(int numCurs)   { return settings.colorCurs[numCurs]; };
    void    SetColorTPos(const QColor color);
    QColor  GetColorTPos()              { return settings.colorTShift0; };
    void    SetColorTShift(const QColor color);
    QColor  GetColorTShift()            { return settings.colorTShift; };
    void SetTypeDisplay(NS_TypesDisplay::TypeDisplay type);

    void SetModeAccumulation(NS_TypesDisplay::ModeAccumulation mode);
    void SetCountAccumulation(int count);

public slots:
    void Repaint();
    void SetColorBackground();
    void SetColorGrid();
    void SetColorTPos();
    void SetColorTShift();
    void SetColorSignal0();
    void SetColorSignal1();
    void SetColorMath();
    void SetColorCurs0();
    void SetColorCurs1();
    void SetColorCursMath();
    void SetDrawMode(int);
    void SetTypeGridBase();
    void SetTypeGridSimple();
    void SetTypeGridNone();

    void EnableAntialiasing(bool);
    void ResetAccumulation();

private:
    SettingsDisplay settings;
    QFrame *frame;
    RShiftCursorScreen *rShiftScreen;
    TrigCursorScreen *trigLevScreen;
    TimeCursorScreen *timeScreen;
    SignalScreen *signalScreen;             // Для вывода сигналов, математической функции и спектрограммы
};