//  [7/13/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QFrame>
#include <QCursor>
#pragma warning(default:4127)
#include "../../common/TypesAltera.h"

class MathDisplayAnalyzer;
struct SettingsDisplay;

#define SCR_W 597
#define SCR_H 300
#define RECT_WIDTH 500
#define RECT_HEIGHT 251
#define RECT_COORD_X 60
#define RECT_COORD_Y 10

class SignalScreen : public QFrame {
    Q_OBJECT
public:
    SignalScreen(QWidget *parent, SettingsDisplay *setD);
    ~SignalScreen();

    void Draw();

    // Очищает буфер данных
    void Reset();
    void ConnectToDevice(int connect);
    void SetEnableChannel(int numChannel, bool enable);

public slots:
    void Repaint();         // Вызывается после изменения цвета

signals:
    void MovedCursor(int cursor, int delta);

protected:
    void paintEvent(QPaintEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    QImage *screenBuffer;       // Буффер для изображения
    bool enable1;
    bool enable2;
    MathDisplayAnalyzer *mathDisplay;
    SettingsDisplay *setD;
    bool connect;
    int activeCur;
    QCursor cursor;
    int prevMouseX;

    void Clear(QPaintDevice *paintDevice);
    void DrawGrid(QPaintDevice *paintDevice);
    void DrawSignals(QPaintDevice *paintDevice);
    void DrawCursors(QPainter *painter);

    void FillScales();    // Заполняем массив значений напряжения

    // Нарисовать рамку
    void DrawRectangle(QPainter *painter, int x1, int y1, int width, int height);

    // Нарисовать линию крестиков
    void DrawVerticalCrossLine(QPainter *painter, const int x, const int y, int dy, int length);
    void DrawHorizontalCrossLine(QPainter *painter, const int x, const int y, int dx, int length);
    void DrawVerticalPointLine(QPainter *painter, const int x, const int y, int dy, int length);
    void DrawHorizontalPointLine(QPainter *painter, const int x, const int y, int dx, int length);
    void DrawPointForPointVerticalLine(QPainter *painter, int x0);

    // Нарисовать крест с центром в x, y, длина (dx, dy)
    void DrawCross(QPainter *painter, int x, int y, int dx, int dy);

     // Нарисовать спектр
    // Спектр строится в предположении, что максимальная составляющая равна 1.0
    void DrawSpectr(QPainter *painter, const int x, const int y, const int height, const double *buffer, int range);

    bool VerifyMouseOnCursor(int x, int y);

    struct ScaleVoltage {
        double volts[11];
    } scaleVoltage[NS_TypesAltera::RangeSize];

    struct ScaleFreqs {
        double freq[11];
    } scaleFreq[NS_TypesAltera::TBaseSize];
};