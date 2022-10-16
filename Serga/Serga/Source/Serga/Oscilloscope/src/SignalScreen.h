//  [7/13/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QFrame>
#pragma warning(default:4127)

struct SettingsDisplay;

class SignalScreen : public QFrame {
    Q_OBJECT
// �������� ����� ���������� ��������� ���� � ��������, ����� ������ ������� ��� ����
#define deltaCursor 3
public:
    // ���� � ����������� ��������� TypeScreen::TypeDisplay_Together, �� � ���� ������� ����� �������������
    // ��������� ����� �������� - ������� �����. ���� TypeDisplay_Separately, �� ����� ������������ ��� 
    // ��������� ������ ������� - channel
    SignalScreen(QWidget *parent, SettingsDisplay *settings);
    ~SignalScreen();

    void Draw();
    void DrawPoints(int _channel, uchar _buffer[501]);
    QImage GetImage();

    // ������� ����� ������
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
    QImage *screenBuffer;       // ������ ��� �����������
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

    // ���������� �����
    void DrawRectangle(QPainter *painter, int x1, int y1, int width, int height);

    // ���������� ����� ���������
    void DrawVerticalCrossLine(QPainter *painter, const int x, const int y, int dy, int length);
    void DrawHorizontalCrossLine(QPainter *painter, const int x, const int y, int dx, int length);

    // ���������� ����� �����
    void DrawVerticalPointLine(QPainter *painter, const int x, const int y, int dy, int length);
    void DrawHorizontalPointLine(QPainter *painter, const int x, const int y, int dx, int length);

    // ���������� ����� � ������� � x, y, ����� (dx, dy)
    void DrawCross(QPainter *painter, int x, int y, int dx, int dy);

    // ���������� ������. � ������ ������ ���� ������, ���������� � �������� ������� �������� �� ����������
    // ������ ��������� � ����, ���������� �������� ������� ���� �������� x, y, ������ height
    void DrawSignal(QPainter *painter, const int x, const int y, const int height, const double* buffer);

    // ���������� ������
    // ������ �������� � �������������, ��� ������������ ������������ ����� 1.0
    void DrawSpectr(QPainter *painter, const int x, const int y, const int height, const double *buffer);

    // ���������� ������ ����������.
    void DrawUCursor(QPainter *painter, const int x, const int y, const int height, int pos1, int pos2);

    // ���������� ������ �������
    void DrawTCursor(QPainter *painter, const int x, const int y, double time1, double time2, int numChannel);

    // ���������� ��������� ����������� �� �����. colorBackground �� ����������.
    void MergeImage(QImage *image);

    // ���������, ��������� �� ������ ���� ��� �������� ��������, ���������� �������������� ����������� �������
    bool VerifyMouseOnCursor(QMouseEvent *e, bool pressed, QCursor &cursor);

    void SetCursorOnScreenU(int channel, int numCursor, int posMouse, QCursor &cursor);
    void SetCursorOnScreenT(int channel, int numCursor, int posMouse, QCursor &cursor);
};