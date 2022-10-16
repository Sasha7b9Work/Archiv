//  [7/13/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QFrame>
#pragma warning(default:4127)

struct SettingsDisplay;

class SignalScreen : public QFrame {
    Q_OBJECT
// Разность между положением указателя мыши и курсором, чтобы менять внещний вид мыши
#define deltaCursor 3
public:
    // Если в конструктор передаётся TypeScreen::TypeDisplay_Together, то в этом объекте будет производиться
    // отрисовка обоих сигналов - большой экран. Если TypeDisplay_Separately, то экран предназначен для 
    // отрисовки одного сигнала - channel
    SignalScreen(QWidget *parent, SettingsDisplay *settings);
    ~SignalScreen();

    void Draw();
    void DrawPoints(int _channel, uchar _buffer[501]);
    QImage GetImage();

    // Очищает буфер данных
    void Clear();
    void Reset();
    void Repaint();
    void ConnectToDevice(bool connect);
    void SetEnableChannel(int numChannel, bool enable);

    void EnableAntialiasing(bool enable);

    void SetCountAccumulation(int count);
    void ResetAccumulation();

protected:
    void paintEvent(QPaintEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);

signals:
    void ChangePosUCur(int channel, int numCur, int deltaY);
    void ChangePosTCur(int channel, int numCur, int deltaX);
    void ChangeRShift0(int);
    void ChangeRShift1(int);
    void ChangeTShift(int);

private:
    SettingsDisplay *setD;
    QImage *screenBuffer;       // Буффер для изображения
    bool enable[2];
    bool connect;

    int prevMouseX;
    int prevMouseY1;
    int prevMouseY2;
    int prevMouseXCur;
    bool pressedLeft;
    bool pressedRight;
    bool pressedMid;
    QCursor cursorOnPress;
    int cursorChannel;
    int numCursor;

    void Clear(QPaintDevice *paintDevice);
    void DrawGrid(QPaintDevice *paintDevice);
    void DrawSignals(QPaintDevice *paintDevice);
    void DrawCursors(QPainter *painter);

    // Нарисовать рамку
    void DrawRectangle(QPainter *painter, int x1, int y1, int width, int height);

    // Нарисовать линию крестиков
    void DrawVerticalCrossLine(QPainter *painter, const int x, const int y, int dy, int length);
    void DrawHorizontalCrossLine(QPainter *painter, const int x, const int y, int dx, int length);

    // Нарисовать линию точек
    void DrawVerticalPointLine(QPainter *painter, const int x, const int y, int dy, int length);
    void DrawHorizontalPointLine(QPainter *painter, const int x, const int y, int dx, int length);

    // Нарисовать крест с центром в x, y, длина (dx, dy)
    void DrawCross(QPainter *painter, int x, int y, int dx, int dy);

    // Нарисовать график. В буфере должны быть данные, приведённые к границам данного масштаба по напряжению
    // График выводится в окне, координаты верхнего правого угла которого x, y, высота height
    void DrawSignal(QPainter *painter, const int x, const int y, const int height, const double* buffer);

    // Нарисовать спектр
    // Спектр строится в предположении, что максимальная составляющая равна 1.0
    void DrawSpectr(QPainter *painter, const int x, const int y, const int height, const double *buffer);

    // Нарисовать курсор напряжения.
    void DrawUCursor(QPainter *painter, const int x, const int y, const int height, int pos1, int pos2);

    // Нарисовать курсор времени
    void DrawTCursor(QPainter *painter, const int x, const int y, double time1, double time2, int numChannel);

    // Произвести наложение изображения на экран. colorBackground не копируется.
    void MergeImage(QImage *image);

    // Проверить, находится ли курсор мыши над экранным курсором, установить соответсвующее изображение курсора
    bool VerifyMouseOnCursor(QMouseEvent *e, bool pressed, QCursor &cursor);

    void SetCursorOnScreenU(int channel, int numCursor, int posMouse, QCursor &cursor);
    void SetCursorOnScreenT(int channel, int numCursor, int posMouse, QCursor &cursor);
};