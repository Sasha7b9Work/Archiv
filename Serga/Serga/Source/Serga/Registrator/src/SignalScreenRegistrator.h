//  [7/13/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QFrame>
#pragma warning(default:4127)
#include <vector>
#include "DataRegistrator.h"
#include "../../common/TypesAltera.h"

struct SettingsDisplayRegistrator;

class SignalScreenRegistrator : public QFrame 
{
    Q_OBJECT

	static const int SCR_W = 597;
	static const int SCR_H = 300;
	static const int RECT_WIDTH = 501;
	static const int RECT_HEIGHT = 250;
	static const int RECT_COORD_X = 55;
	static const int RECT_COORD_Y = 10;

public:
    // ���� � ����������� ��������� TypeScreen::TypeDisplay_Together, �� � ���� ������� ����� �������������
    // ��������� ����� �������� - ������� �����. ���� TypeDisplay_Separately, �� ����� ������������ ��� 
    // ��������� ������ ������� - channel
    SignalScreenRegistrator(QWidget *parent, SettingsDisplayRegistrator *setD, int channel, std::vector<Data> &rDatas);
    ~SignalScreenRegistrator();

    int GetHeight();

    // lastPoint - ��������� ��������� �����. ���� 0 - ��� ��������� ����� �������, ��� � data.size() - 1
	void Draw(int lastPoint);

    // ������� ����� ������
    void Reset();
    void ConnectToDevice(bool connected);
    void Repaint();

protected:
    void paintEvent(QPaintEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

public slots:

signals:
    void MoveSignal(int);
    void ChangePosUCur(int numCur, int deltaY);

private:
    QImage *screenBuffer;       // ������ ��� �����������
    int channel;
    std::vector<Data> &rDatas;
    SettingsDisplayRegistrator *setD;
    bool connected;

    int prevMousePos;           // ���������� X �����, ���������� ��� ������� ������ ������ ����
    int prevMouseY;             // ����������� Y �����. ������������ ��� ��������� ��������� ������� ����������
    int activeCur;
    QCursor cursor;
    int lastPoint;              // ��������� ��������� �� ����� �����
    int firstPoint;             // ������ ��������� �� ����� �����
    int numPoints;              // ���������� ��������� �� ����� �����. ����� ���� �� RECT_WIDTH

    void Clear(QPaintDevice *paintDevice);
    void DrawGrid(QPaintDevice *paintDevice);
	void DrawSignals(QPaintDevice *paintDevice);
    void DrawCursors(QPaintDevice *paintDevice);

    // ���������� �����
    void DrawRectangle(QPainter *painter, int x1, int y1, int width, int height);

    void DrawVerticalPointLine(QPainter *painter, const int x, const int y, int dy, int length);
    void DrawHorizontalPointLine(QPainter *painter, const int x, const int y, int dx, int length);
    void DrawPointForPointHorizontalLine(QPainter *painter, int x0, int y0, int width, int delta);

    // ���������� ����� � ������� � x, y, ����� (dx, dy)
    void DrawCross(QPainter *painter, int x, int y, int dx, int dy);

    struct ScaleVoltage {
        double volts[11];
    } scaleVoltage[NS_TypesAltera::RangeSize];

    void FillTables();
    bool VerifyMouseOnCursor(int x, int y);
};