//  [4/23/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QFrame>
#pragma warning(default:4127)
#include "../../common/AbstractSettingsDisplay.h"

class QPaintDevice;
class QImage;

class Display : public QFrame{
    Q_OBJECT

    static const int RECT_COORD_X = 0;
    static const int RECT_COORD_Y = 0;
    static const int RECT_WIDTH = 200;
    static const int RECT_HEIGHT = 100;
    static const int X_Urms = 5;
    static const int Y_Urms = RECT_HEIGHT + 20;
    static const int X_Upp = 5;
    static const int Y_Upp = Y_Urms + 25;
    static const int WIDTH = RECT_WIDTH;
    static const int HEIGHT = Y_Upp + 10;

public:
    Display(QWidget *parent, int channel);
    ~Display();

    void DrawData();
    void Reset();
    void ConnectToDevice(bool connected);
    AbstractSettingsDisplay* GetPointerSettings() { return &settings; };

public slots:
    void SetColorBackground(QColor);
    void SetColorGrid(QColor);
    void SetColorFont(QColor);
    void SetColorSignal(QColor);

protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    AbstractSettingsDisplay settings;
    bool readyDevice;
    QImage *screen;
    int channel;
    void Repaint();
    void DrawGrid(QPaintDevice *paintDevice);
    void DrawSignal(QPaintDevice *paintDevice);
};