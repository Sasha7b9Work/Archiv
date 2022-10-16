#include "SignalScreenRegistrator.h"
#pragma warning(disable:4127)
#include <QPainter>
#include <QMouseEvent>
#pragma warning(default:4127)
#include <cmath>
#include "../../common/defines.h"
#include "MathDisplayRegistrator.h"
#include "../../common/TablesOfConverting.h"
#include "../../common/GlobalFunctions.h"
#include "SettingsDisplayRegistrator.h"
#include "../Device129Multi/src/Device129Multi.h"

using namespace NS_GlobalFunctions;
using namespace NS_TypesAltera;
using std::vector;

extern MathDisplayRegistrator mathDisplay;
extern TablesOfConverting tables;

extern int numDevice;

SignalScreenRegistrator::SignalScreenRegistrator(QWidget *_parent, SettingsDisplayRegistrator *_setD, int _channel, std::vector<Data> &_rDatas)
    :QFrame(_parent),
    channel(_channel),
    setD(_setD),
    cursor(Qt::ArrowCursor),
    rDatas(_rDatas),
    connected(false)
{
    screenBuffer = NULL;
    int size_x = SCR_W;
    int size_y = SCR_H;
    screenBuffer = new QImage(size_x, size_y, QImage::Format_ARGB32_Premultiplied);
    QRect rectBuffer = screenBuffer->rect();
    FillTables();
    Reset();
    setMouseTracking(true);
}

SignalScreenRegistrator::~SignalScreenRegistrator() 
{
    SAFE_DELETE(screenBuffer);
}

int SignalScreenRegistrator::GetHeight() 
{
    return RECT_HEIGHT;
}

void SignalScreenRegistrator::Draw(int _lastPoint) 
{
    lastPoint = _lastPoint;
    firstPoint = _lastPoint - (RECT_WIDTH - 1);
    if(firstPoint < 0) 
        firstPoint = 0;
    
    numPoints = lastPoint - firstPoint + 1;
    Repaint();
}

void SignalScreenRegistrator::Reset() 
{
    int size_x = SCR_W;
    int size_y = SCR_H;
    resize(size_x, size_y);
    SAFE_DELETE(screenBuffer);
    screenBuffer = new QImage(size_x, size_y, QImage::Format_ARGB32_Premultiplied);
    Repaint();
}

void SignalScreenRegistrator::ConnectToDevice(bool _connected) 
{
    if(!(connected = _connected)) 
        Repaint();
}

void SignalScreenRegistrator::Repaint() 
{
    Clear(screenBuffer);
    DrawGrid(screenBuffer);
    DrawSignals(screenBuffer);
    DrawCursors(screenBuffer);
    update();
}

void SignalScreenRegistrator::paintEvent(QPaintEvent *) 
{
    QPainter painter(this);

    QRect rect = this->rect();
    painter.drawImage(rect, *screenBuffer);
}

void SignalScreenRegistrator::mouseMoveEvent(QMouseEvent *e) 
{
    if(e->buttons()) 
    {
        if(Qt::OpenHandCursor == cursor.shape()) 
        {
            int deltaX = prevMousePos - e->pos().x();
            if(deltaX != 0) 
            {
                emit MoveSignal(deltaX);
                prevMousePos = e->pos().x();
            }
        }
        else {
            int deltaY = prevMouseY - e->pos().y();
            if(deltaY != 0) {
                emit ChangePosUCur(activeCur, deltaY);
                prevMouseY = e->pos().y();
            }
        }
        return;
    }
    int posX = e->pos().x();
    int posY = e->pos().y();
    setCursor(VerifyMouseOnCursor(posX, posY) ? Qt::SplitVCursor : Qt::ArrowCursor);
    int numElem = RECT_COORD_X + RECT_WIDTH - posX;
    if((numElem < numPoints) && (numElem > 0 ) && (posY > RECT_COORD_Y) &&  (posY < (RECT_COORD_Y + RECT_HEIGHT))) {
        Clear(screenBuffer);
        DrawGrid(screenBuffer);
        DrawSignals(screenBuffer);
        DrawCursors(screenBuffer);
        QPainter painter(screenBuffer);
        painter.setPen(QPen(Qt::yellow, Qt::SolidLine));
        painter.drawText(posX, posY, VoltageToString(rDatas[numElem].data[channel]) + " - " + rDatas[numElem].time.toString());
        update();
    }
    else {
        Repaint();
    }
}

//  [12/7/2009 Sasha]
void SignalScreenRegistrator::mousePressEvent(QMouseEvent *_event) {
    if(_event->button() & Qt::LeftButton) {
        prevMousePos = _event->pos().x();
        prevMouseY = _event->pos().y();
        cursor = (VerifyMouseOnCursor(prevMousePos, prevMouseY) ? Qt::SplitVCursor : Qt::OpenHandCursor);
        setCursor(cursor);
    }
}

// [12/7/2009 Sasha]
void SignalScreenRegistrator::mouseReleaseEvent(QMouseEvent *) {
    cursor = (VerifyMouseOnCursor(prevMousePos, prevMouseY) ? Qt::SplitVCursor : Qt::ArrowCursor);
    setCursor(cursor);
}

//  [7/13/2009 Administrator]
void SignalScreenRegistrator::Clear(QPaintDevice *paintDevice) {
    QPainter *painter;
    painter = new QPainter(paintDevice);
    
    painter->setBrush(QBrush(setD->colorBackground, Qt::SolidPattern));
    painter->drawRect(0, 0, width() - 1, height() - 1);
    delete painter;
}

void SignalScreenRegistrator::DrawGrid(QPaintDevice * _paintDevice) 
{
    QPainter painter(_paintDevice);
    painter.setPen(QPen(setD->colorGrid, Qt::SolidLine));

    // Заполняем шкалу напряжений
    int coordY = RECT_COORD_Y + 10;
    int stepY = RECT_HEIGHT / 10 - 1;
    for(int i = 10; i >= 0; i--) 
    {
        QString string = VoltageToStringFixedForPoint(scaleVoltage[D129_GetRange(numDevice, channel)].volts[i], 1);
        painter.drawText(3, coordY, string);
        coordY += stepY;
    }

    // Рисуем прямоугольник
    DrawRectangle(&painter, RECT_COORD_X, RECT_COORD_Y, RECT_WIDTH, RECT_HEIGHT);

    // Рисуем горизонтальные линии
    coordY = RECT_COORD_Y;
    stepY = RECT_HEIGHT / 10;
    for(int i = 0; i < 10; i++) 
    {
        DrawHorizontalPointLine(&painter, RECT_COORD_X, coordY, 5, RECT_WIDTH);
        coordY += stepY;
    }

    // Рисуем вертикальные линии
    int coordX = RECT_COORD_X;
    int stepX = RECT_WIDTH / 10;
    for(int i = 0; i < 10; i++) 
    {
        DrawVerticalPointLine(&painter, coordX, RECT_COORD_Y, 5, RECT_HEIGHT);
        coordX += stepX;
    }

    if(!connected) 
    {
        painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
        QFont font = painter.font();
        font.setPointSize(20);
        painter.setFont(font);
        int coordX = 80;
        painter.drawText(coordX, 50, QString(tr("УСТРОЙСТВО НЕ ГОТОВО")));
        painter.drawText(coordX, 80, QString(tr("1. Проверьте подключение модуля")));
        painter.drawText(coordX, 110, QString(tr("2. Нажмите кнопку Обновить")));
        painter.drawText(coordX, 140, QString(tr("3. Попробуйте выбрать другой порт")));
    }
}

void SignalScreenRegistrator::DrawCursors(QPaintDevice *_paintDevice) 
{
    QPainter painter(_paintDevice);

    // Рисуем курсоры
    if(setD->enableUCurs) 
    {
        painter.setPen(QPen(setD->colorSignal[0], Qt::SolidLine));
        int posY0 = RECT_COORD_Y + RECT_HEIGHT / 2 - setD->posUCur[0];
        int posY1 = RECT_COORD_Y + RECT_HEIGHT / 2 - setD->posUCur[1];
        DrawPointForPointHorizontalLine(&painter, RECT_COORD_X, posY0, RECT_WIDTH, 6);
        DrawPointForPointHorizontalLine(&painter, RECT_COORD_X, posY1, RECT_WIDTH, 6);
    }
}

void SignalScreenRegistrator::DrawRectangle(QPainter *painter, int x1, int y1, int width, int height) 
{
    painter->drawRect(x1, y1, width, height);
}

void SignalScreenRegistrator::DrawVerticalPointLine(QPainter *painter, const int x, const int y, int dy, int length) 
{
    for(int posY = y; posY < (length + y); posY += dy) 
        painter->drawPoint(x, posY);
}

void SignalScreenRegistrator::DrawHorizontalPointLine(QPainter *painter, const int x, const int y, int dx, int length) 
{
    for(int posX = x; posX < (length + x); posX += dx) 
        painter->drawPoint(posX, y);
}

void SignalScreenRegistrator::DrawPointForPointHorizontalLine(QPainter *_painter, int _x0, int _y0, int _width, int _delta) 
{
    int maxX = _x0 + _width;
    for(int x = _x0; x < maxX; x += _delta * 2) 
        _painter->drawLine(x, _y0, x + _delta, _y0);
}

void SignalScreenRegistrator::DrawCross(QPainter *painter, int x, int y, int dx, int dy) 
{
    painter->drawLine(x - dx / 2, y, x + dx / 2, y);
    painter->drawLine(x, y - dy / 2, x, y + dy / 2);
}

void SignalScreenRegistrator::FillTables() 
{
    for(int range = 0; range < RangeSize; range++) 
    {
        double step = mathDisplay.GetMaxVoltage(range) * 2.0 / 10.0;
        double value = -mathDisplay.GetMaxVoltage(range);
        for(int i = 0; i < 11; i++) 
        {
            scaleVoltage[range].volts[i] = value;
            value += step;
        }
    }
}

bool SignalScreenRegistrator::VerifyMouseOnCursor(int _x, int _y) 
{
    int rectCoordX1 = RECT_COORD_X + RECT_WIDTH;
    int rectCoordY1 = RECT_COORD_Y + RECT_HEIGHT;
    if((!setD->enableUCurs) || (_x < RECT_COORD_X) || (_x > rectCoordX1) || (_y < RECT_COORD_Y) || (_y > rectCoordY1)) 
            return false;

    int posCur0 = RECT_COORD_Y + RECT_HEIGHT / 2 - setD->posUCur[0];
    int posCur1 = RECT_COORD_Y + RECT_HEIGHT / 2 - setD->posUCur[1];
    int delta0 = Abs(posCur0 - _y);
    int delta1 = Abs(posCur1 - _y);
    static const int delta = 2;
    if(delta0 < delta) 
        activeCur = 0;
    else if(delta1 < delta) 
        activeCur = 1;
    else 
        activeCur = 2;
    
    return ((delta0 < delta) || (delta1 < delta));
}

void SignalScreenRegistrator::DrawSignals(QPaintDevice *_paintDevice) 
{
    QPainter painter(_paintDevice);

    if(rDatas.empty()) 
        return;

    int y0 = RECT_COORD_Y + RECT_HEIGHT / 2;
    int x = RECT_COORD_X + RECT_WIDTH;
    double k1 = (RECT_HEIGHT / 2) / mathDisplay.GetMaxVoltage(D129_GetRange(numDevice, channel));
    painter.setPen(QPen(setD->colorSignal[0], Qt::SolidLine));
    for(int i = lastPoint; i > firstPoint; i--) 
    {
        int y1 = y0 - rDatas[i].data[channel] * k1;
        int y2 = y0 - rDatas[i - 1].data[channel] * k1;
        int min = RECT_COORD_Y;
        int max = RECT_COORD_Y + RECT_HEIGHT;
        y1 = max(y1, min);
        y1 = min(y1, max);
        y2 = max(y2, min);
        y2 = min(y2, max);
        painter.drawLine(x, y1, x - 1, y2);
        x--;
    }

    // Теперь пишем время
    painter.setPen(QPen(setD->colorGrid, Qt::SolidLine));
    //int posX = 0;
    for(int point = lastPoint, posX = RECT_COORD_X + RECT_WIDTH - 25; point >= firstPoint; point -= 100, posX -= 100) 
        painter.drawText(posX, RECT_COORD_Y + RECT_HEIGHT + 15, rDatas[point].time.toString());
}