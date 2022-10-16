#pragma warning(disable:4127)
#include <QPainter>
#include <QMoveEvent>
#pragma warning(default:4127)
#include <cmath>
#include <Windows.h>
#include "../../common/defines.h"
#include "SignalScreen.h"
#include "SettingsDisplay.h"
#include "Strings.h"
#include "MathDisplayAnalyzer.h"
#include "../../common/GlobalFunctions.h"
#include "SettingsDisplay.h"
#include "../Device129Multi/src/Device129Multi.h"
#include "../common/TablesOfConverting.h"

using namespace::NS_GlobalFunctions;

extern TablesOfConverting tables;

extern int numDevice;

SignalScreen::SignalScreen(QWidget *_parent, SettingsDisplay *_setD)
    :QFrame(_parent),
    mathDisplay(NULL),
    enable1(true),
    enable2(true),
    setD(_setD),
    connect(false),
    activeCur(2),
    cursor(Qt::ArrowCursor)
{
    mathDisplay = new MathDisplayAnalyzer;
    screenBuffer = NULL;
    int size_x = SCR_W;
    int size_y = SCR_H;
    screenBuffer = new QImage(size_x, size_y, QImage::Format_ARGB32_Premultiplied);
    QRect rectBuffer = screenBuffer->rect();
    FillScales();
    Reset();
    setMouseTracking(true);
}

SignalScreen::~SignalScreen() 
{
    SAFE_DELETE(screenBuffer);
    SAFE_DELETE(mathDisplay);
}

void SignalScreen::Draw() 
{
    if(D129_DataReadyForDraw(numDevice)) 
    {
        DrawSignals(screenBuffer);
        update();
    }
}

void SignalScreen::Reset() 
{
    int size_x = SCR_W;
    int size_y = SCR_H;
    resize(size_x, size_y);
    SAFE_DELETE(screenBuffer);
    screenBuffer = new QImage(size_x, size_y, QImage::Format_ARGB32_Premultiplied);
    Clear(screenBuffer);
    DrawGrid(screenBuffer);
    update();
}

void SignalScreen::Repaint() 
{
    Clear(screenBuffer);
    DrawGrid(screenBuffer);
    DrawSignals(screenBuffer);
    update();
}

//  [1/29/2010 Sasha]
void SignalScreen::ConnectToDevice(int _connect) {
    connect = _connect;
    if(!connect) {
        Reset();
    }
}

//  [9/29/2009 Sasha]
void SignalScreen::SetEnableChannel(int numChannel, bool enable) {
    if(numChannel == 1) {
        enable1 = enable;
    }
    else {
        enable2 = enable;
    }
}

//  [7/13/2009 Administrator]
void SignalScreen::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    QRect rect = this->rect();
    painter.drawImage(rect, *screenBuffer);
    DrawCursors(&painter);
}

//  [2/24/2010 Sasha]
void SignalScreen::mouseMoveEvent(QMouseEvent *_event) {
    if(_event->buttons()) {
        if(Qt::SplitHCursor == cursor.shape()) {
            if(activeCur != 2) {
                emit MovedCursor(activeCur, (_event->pos().x() - RECT_COORD_X) / 2);
            }
        }
    }
    else {
        setCursor(VerifyMouseOnCursor(_event->pos().x(), _event->pos().y()) ? Qt::SplitHCursor : Qt::ArrowCursor);
    }
}

//  [2/24/2010 Sasha]
void SignalScreen::mousePressEvent(QMouseEvent *_event) {
    if(_event->button() & Qt::LeftButton) {
        prevMouseX = _event->pos().x();
        if(VerifyMouseOnCursor(prevMouseX, _event->pos().y())) {
            cursor = Qt::SplitHCursor;
            setCursor(cursor);
        }
        else {
            cursor = Qt::ArrowCursor;
            setCursor(cursor);
        }
    }
}

//  [2/24/2010 Sasha]]
void SignalScreen::mouseReleaseEvent(QMouseEvent *_event) {
    cursor = VerifyMouseOnCursor(_event->pos().x(), _event->pos().y()) ? Qt::SplitHCursor : Qt::ArrowCursor;
    setCursor(cursor);
}

//  [7/13/2009 Administrator]
void SignalScreen::Clear(QPaintDevice *paintDevice) {
    QPainter *painter;
    painter = new QPainter(paintDevice);
    
    painter->setBrush(QBrush(setD->colorBackground, Qt::SolidPattern));
    painter->drawRect(0, 0, width() - 1, height() - 1);
    delete painter;
}

void SignalScreen::DrawGrid(QPaintDevice *_paintDevice) 
{
    Clear(_paintDevice);
    QPainter painter(_paintDevice);
    painter.setPen(QPen(setD->colorGrid, Qt::SolidLine));
    int range = D129_GetRange(numDevice, setD->activeSpectr);
    if(range < 0)
        cout << "range = " << range << endl;

    // Заполняем шкалу напряжений
    int coordY = RECT_COORD_Y + 10;
    int stepY = RECT_HEIGHT / 10 - 1;
    for(int i = 10; i >= 0; i--) 
    {
        QString string = VoltageToStringFixedForPoint(scaleVoltage[range].volts[i], 1);
        painter.drawText(3, coordY, string);
        coordY += stepY;
    }

    // Заполняем шкалу частот
    int coordX = RECT_COORD_X - 20;
    int stepX = RECT_WIDTH / 10;
    coordY = RECT_COORD_Y + RECT_HEIGHT + 20;
    for(int i = 0; i < 11; i += 2) 
    {
        QString string = FrequencyToStringFixedForPoint(scaleFreq[D129_GetTBase(numDevice)].freq[i], 2);
        painter.drawText(coordX, coordY, string);
        coordX += stepX * 2;
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
    coordX = RECT_COORD_X;
    stepX = RECT_WIDTH / 10;
    for(int i = 0; i < 10; i++) 
    {
        DrawVerticalPointLine(&painter, coordX, RECT_COORD_Y, 5, RECT_HEIGHT);
        coordX += stepX;
    }

    if(!connect) 
    {
        painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
        QFont font = painter.font();
        font.setPointSize(20);
        painter.setFont(font);
        painter.drawText(75, 50, QString(tr("УСТРОЙСТВО НЕ ГОТОВО")));
        painter.drawText(75, 80, QString(tr("1. Проверьте подключение модуля")));
        painter.drawText(75, 110, QString(tr("2. Нажмите кнопку Обновить")));
        painter.drawText(75, 140, QString(tr("3. Попробуйте выбрать другой порт")));
    }
}

void SignalScreen::DrawSignals(QPaintDevice *paintDevice) 
{
    Clear(paintDevice);
    DrawGrid(paintDevice);
    QPainter painter(paintDevice);
    painter.setPen(QPen(setD->colorSignal[0], Qt::SolidLine));
    if(D129_DataReadyForDraw(numDevice))
        DrawSpectr(&painter, 0, 501, 501, D129_GetPointerToDataDraw(numDevice, setD->activeSpectr), D129_GetRange(numDevice, setD->activeSpectr));
}

void SignalScreen::DrawCursors(QPainter *_painter) 
{
    if(setD->enableCurs) 
    {
        _painter->setPen(QPen(setD->colorGrid, Qt::SolidLine));
        int x1 = RECT_COORD_X + setD->GetPosCur(setD->activeSpectr, 0) * 2;
        int x2 = RECT_COORD_X + setD->GetPosCur(setD->activeSpectr, 1) * 2;
        DrawPointForPointVerticalLine(_painter, x1);
        DrawPointForPointVerticalLine(_painter, x2);
    }
}

void SignalScreen::FillScales() 
{
    MathDisplayAnalyzer mathDisplay;

    for(int range = 0; range < NS_TypesAltera::RangeSize; range++) 
    {
        double step = mathDisplay.GetMaxVoltage(range) / 10.0;
        double value = 0.0;
        for(int i = 0; i < 11; i++) 
        {
            scaleVoltage[range].volts[i] = value;
            value += step;
        }
    }

    for(int tBase = 0; tBase < NS_TypesAltera::TBaseSize; tBase++) 
    {
        double step = mathDisplay.GetPosTCurAbs(1000, tBase);   // Длительность одного экрана
        step = 1.0 / step;                               // Один шаг спектра
        step = step * 50.0;                              // Десятая часть максимальной частоты спектра
        double value = 0.0;
        for(int i = 0; i < 11; i++) 
        {
            scaleFreq[tBase].freq[i] = value;
            value += step;
        }
    }
}

void SignalScreen::DrawRectangle(QPainter *painter, int x1, int y1, int width, int height) 
{
    painter->drawRect(x1, y1, width, height);
}

void SignalScreen::DrawVerticalCrossLine(QPainter *painter, const int x, const int y, int dy, int length) 
{
    for(int posY = y; posY < (length + y); posY += dy)
        DrawCross(painter, x, posY, 5, 5);
}

void SignalScreen::DrawHorizontalCrossLine(QPainter *painter, const int x, const int y, int dx, int length)
{
    for(int posX = x; posX < (length + x); posX += dx) 
        DrawCross(painter, posX, y, 5, 5);
}

void SignalScreen::DrawVerticalPointLine(QPainter *painter, const int x, const int y, int dy, int length) 
{
    for(int posY = y; posY < (length + y); posY += dy)
        painter->drawPoint(x, posY);
}

void SignalScreen::DrawHorizontalPointLine(QPainter *painter, const int x, const int y, int dx, int length) 
{
    for(int posX = x; posX < (length + x); posX += dx)
        painter->drawPoint(posX, y);
}

void SignalScreen::DrawPointForPointVerticalLine(QPainter *_painter, int _x0) 
{
    static int lengthLine = 11;
    static int edgeCell = 25;
    _painter->drawLine(_x0, RECT_COORD_Y, _x0, RECT_COORD_Y + lengthLine / 2 + 1);
    int y = RECT_COORD_Y + edgeCell - lengthLine / 2;
    for(int i = 0; i < 9; i++) 
    {
        _painter->drawLine(_x0, y, _x0, y + lengthLine);
        y += edgeCell;
    }
    _painter->drawLine(_x0, y, _x0, y + lengthLine / 2);
}

void SignalScreen::DrawCross(QPainter *painter, int x, int y, int dx, int dy) 
{
    painter->drawLine(x - dx / 2, y, x + dx / 2, y);
    painter->drawLine(x, y - dy / 2, x, y + dy / 2);
}

void SignalScreen::DrawSpectr(QPainter *_painter, const int /*_x*/, const int /*_y*/, const int _height, const double *_buffer, int _range) 
{
    int posY_0 = RECT_HEIGHT + RECT_COORD_Y;
    double doubleY = (double)RECT_COORD_Y;
    int posX = RECT_COORD_X;
    double k = _height / mathDisplay->GetMaxVoltage(_range) / 2.0;
    for(int i = 0; i < 250; i++) 
    {
        double deltaY = _buffer[i] * k;
        double posY_1 = posY_0 - deltaY;
        _painter->drawLine(posX, posY_0, posX, (posY_1 > doubleY) ? posY_1 : doubleY);
        posX += 2;
    }
}

bool SignalScreen::VerifyMouseOnCursor(int _x, int _y) 
{
    int rectCoordX1 = RECT_COORD_X + RECT_WIDTH;
    int rectCoordY1 = RECT_COORD_Y + RECT_HEIGHT;

    if((!setD->enableCurs) || (_x > rectCoordX1) || (_y < RECT_COORD_Y) || (_y > rectCoordY1) || (_x < RECT_COORD_X))
        return false;

    int channel = setD->activeSpectr;
    int posCur0 = RECT_COORD_X + setD->GetPosCur(channel, 0) * 2;
    int posCur1 = RECT_COORD_X + setD->GetPosCur(channel, 1) * 2;
    int delta0 = Abs(posCur0 - _x);
    int delta1 = Abs(posCur1 - _x);
    //LOG("Position cursor x = %d, y = %d, posCur0 = %d, posCur1 = %d, delta0 = %d, delta1 = %d", _x, _y, posCur0, posCur1, delta0, delta1);
    static const int delta = 3;
    if(delta0 < delta)
        activeCur = 0;
    else if(delta1 < delta)
        activeCur = 1;
    else
        activeCur = 2;

    return ((delta0 < delta) || (delta1 < delta));
}
