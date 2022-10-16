#pragma warning(disable:4127)
#include <QFont>
#include <QString>
#include <QPainter>
#include <QPaintDevice>
#include <QTextCodec>
#pragma warning(default:4127)
#include <cmath>
#include "../../common/defines.h"
#include "Display.h"
#include "../../common/GlobalFunctions.h"
#include "../Device129Multi/src/Device129Multi.h"

using NS_GlobalFunctions::VoltageToString;

extern int numDevice;

Display::Display(QWidget *_parent, int _channel):
    QFrame(_parent), 
    channel(_channel), 
    readyDevice(false) 
{
    QTextCodec *rusCodec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(rusCodec);
    setFrameShadow(QFrame::Sunken);
    setFrameShape(QFrame::StyledPanel);
    resize(WIDTH, HEIGHT);
    setMaximumHeight(height());
    setMinimumHeight(height());
    setMaximumWidth(width());
    setMinimumWidth(width());
    _parent->setFixedSize(width() + 4, height() + 4);
    screen = new QImage(WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied);
    move(2, 2);
    Reset();
}

Display::~Display() 
{
    delete screen;
}

void Display::DrawData() 
{
    Repaint();
}

void Display::Reset() 
{
    DrawGrid(screen);
    update();
}

void Display::ConnectToDevice(bool _connected) 
{
    readyDevice = _connected;
    Reset();
}

void Display::SetColorBackground(QColor _color) 
{
    if(_color.isValid()) 
    {
        settings.colorBackground = _color;
        Repaint();
    }
}

void Display::SetColorSignal(QColor _color) 
{
    if(_color.isValid()) 
    {
        settings.colorSignal[channel] = _color;
        Repaint();
    }
}

void Display::SetColorFont(QColor _color) 
{
    if(_color.isValid()) 
    {
        settings.colorFont = _color;
        Repaint();
    }
}

void Display::SetColorGrid(QColor _color) 
{
    if(_color.isValid()) 
    {
        settings.colorGrid = _color;
        Repaint();
    }
}

void Display::paintEvent(QPaintEvent * /*e*/) 
{
    QPainter painter(this);
    QRect rect = this->rect();
    QSize size = this->size();
    painter.drawImage(rect, *screen);
}

void Display::Repaint() 
{
    Reset();
    DrawSignal(screen);
    update();
}

void Display::DrawGrid(QPaintDevice *_paintDevice) 
{
    QPainter painter(_paintDevice);
    painter.setBrush(QBrush(settings.colorBackground, Qt::SolidPattern));
    painter.fillRect(0, 0, WIDTH, HEIGHT, Qt::SolidPattern);
    //painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(settings.colorGrid, 1, Qt::SolidLine));
    painter.drawRect(0, 0, WIDTH - 1, HEIGHT - 1);
    painter.drawRect(RECT_COORD_X, RECT_COORD_Y, RECT_WIDTH - 1, RECT_HEIGHT - 1);
    painter.drawLine(RECT_COORD_X, RECT_HEIGHT/2-1, RECT_COORD_X + RECT_WIDTH, RECT_HEIGHT/2-1);
    if(!readyDevice) 
    {
        painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
        QFont font = painter.font();
        font.setPointSize(15);
        painter.setFont(font);
        painter.drawText(10, RECT_HEIGHT + 27, QString(tr("Õ≈ √Œ“Œ¬")));
    }
}

void Display::DrawSignal(QPaintDevice *_paintDevice) 
{
    static double input[501];
    QPainter painter(_paintDevice);
    D129_GetDataDraw(numDevice, channel, input, 501);

    painter.setPen(QPen(settings.colorSignal[channel], Qt::SolidLine));

    double step = (double)400 / WIDTH;

    for(int i = 0; i < 400; i++) 
    {
        int x1 = i * step;
        int x2 = x1 + step;
        int y1 = RECT_HEIGHT - input[x1] * RECT_HEIGHT;
        int y2 = RECT_HEIGHT - input[x2] * RECT_HEIGHT;
        painter.drawLine(i, y1, i+1, y2);
    }

    painter.setPen(QPen(settings.colorFont, 1, Qt::SolidLine));
    QFont font = painter.font();
    font.setPointSize(15);
    painter.setFont(font);
    char buffer[15];
    double Urms = D129_CalculateRMS(numDevice, channel, buffer, 15);
    if(Urms == INVALID_VALUE_DOUBLE)
        painter.drawText(X_Urms, Y_Urms, QString(tr("Urms = ")) + QString(tr(EMPTY_STRING)));
    else
    {
        QString strUrms = QString(tr("Urms = ")) + VoltageToString(Urms, 4);
        painter.drawText(X_Urms, Y_Urms, strUrms);
    }

    double Upp = D129_CalculatePP(numDevice, channel, buffer, 15);
    if(Upp == INVALID_VALUE_DOUBLE)
        painter.drawText(X_Upp, Y_Upp, QString(tr("Upp  = ")) + QString(tr(EMPTY_STRING)));
    else 
    {
        QString strUpp = QString(tr("Upp  = ")) + VoltageToString(Upp, 4);
        painter.drawText(X_Upp, Y_Upp, strUpp);
    }
}
