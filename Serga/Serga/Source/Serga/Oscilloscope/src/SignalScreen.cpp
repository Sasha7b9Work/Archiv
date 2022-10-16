#include "SignalScreen.h"
#pragma warning(disable:4127)
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QMouseEvent>
#include <QTextCodec>
#pragma warning(default:4127)
#include <Windows.h>
#include <cmath>
#include <algorithm>
#include "../../common/defines.h"
#include "SettingsDisplay.h"
#include "../../common/GlobalFunctions.h"
#include "../../Device129Multi/src/Device129Multi.h"
#include "../common/TypesAltera.h"

using namespace NS_TypesAltera;
using namespace NS_GlobalFunctions;
using namespace NS_TypesDisplay;

extern int numDevice;

SignalScreen::SignalScreen(QWidget *_parent, SettingsDisplay *_settings)
    :QFrame(_parent),
    setD(_settings),
    connect(false),
    pressedLeft(false), 
    pressedRight(false), 
    pressedMid(false),
    cursorOnPress(Qt::ArrowCursor)
{
    enable[0] = enable[1] = true;
    screenBuffer = NULL;
    int size_x = SCR_W;
    int size_y = (setD->typeDisplay == TypeDisplay_Together) ? SCR_BG_H : SCR_BG_H + 1;
    screenBuffer = new QImage(size_x, size_y, QImage::Format_ARGB32_Premultiplied);
    QRect rectBuffer = screenBuffer->rect();
    setMouseTracking(true);
}

SignalScreen::~SignalScreen() 
{
    SAFE_DELETE(screenBuffer);
}

void SignalScreen::Draw() 
{
    if(D129_DataReadyForDraw(numDevice)) // Если данные прочитаны
    {
        DrawSignals(screenBuffer);
        update();
    }
}

void SignalScreen::DrawPoints(int _channel, uchar _buffer[501])
{
    Clear(screenBuffer);
    DrawGrid(screenBuffer);
    QPainter painter(screenBuffer);
    painter.setPen(QPen(setD->colorSignal[_channel], Qt::SolidLine));
    double k = 501.0 / (MAX_VALUE - MIN_VALUE);
    for(int i = 0; i < 500; i++)
    {
        double abs0 = 501 - (_buffer[i] - MIN_VALUE) * k;
        double abs1 = 501 - (_buffer[i + 1] - MIN_VALUE) * k;
        painter.drawLine(i, abs0, i + 1, abs1);
    }
    update();
}

QImage SignalScreen::GetImage() 
{
    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    render(&painter);
    return image;
}

void SignalScreen::Clear() 
{
    setD->countAccumulation = setD->numberAccumulation + 1;
    Clear(screenBuffer);
    DrawGrid(screenBuffer);
    update();
}

void SignalScreen::Reset() 
{
    int size_x = SCR_W;
    int size_y = (setD->GetTypeDisplay() == TypeDisplay_Separetly) ? SCR_BG_H + 1 : SCR_BG_H;
    resize(size_x, size_y);
    SAFE_DELETE(screenBuffer);
    screenBuffer = new QImage(size_x, size_y, QImage::Format_ARGB32_Premultiplied);
    setD->countAccumulation = setD->numberAccumulation + 1;
    Clear(screenBuffer);
    DrawGrid(screenBuffer);
    update();
}

void SignalScreen::Repaint() 
{
    setD->countAccumulation = setD->numberAccumulation + 1;
    Clear(screenBuffer);
    DrawGrid(screenBuffer);
    DrawSignals(screenBuffer);
    update();
}

void SignalScreen::ConnectToDevice(bool _connect) 
{
    connect = _connect;
    if(!connect) {
        Clear();
    }
}

void SignalScreen::SetEnableChannel(int _numChannel, bool _enable) 
{
    enable[_numChannel] = _enable;
}

void SignalScreen::ResetAccumulation() 
{
    Clear(screenBuffer);
    DrawGrid(screenBuffer);
}

void SignalScreen::paintEvent(QPaintEvent *) 
{
    QPainter painter(this);
    QRect rect = this->rect();
    painter.drawImage(rect, *screenBuffer);
    DrawCursors(&painter);
}

void SignalScreen::mousePressEvent(QMouseEvent *_e) 
{
    int posMouseY = _e->pos().y();
    int posMouseX = _e->pos().x();
    if(_e->button() & Qt::LeftButton) {
        prevMouseY1 = posMouseY;
        prevMouseXCur = posMouseX;
        VerifyMouseOnCursor(_e, true, cursorOnPress);
        pressedLeft = true;
    }
    if(_e->button() & Qt::RightButton) {
        prevMouseY2 = posMouseY;
        VerifyMouseOnCursor(_e, true, cursorOnPress);
        pressedRight = true;
    }
    if(_e->button() & Qt::MidButton) {
        prevMouseX = posMouseX;
        VerifyMouseOnCursor(_e, true, cursorOnPress);
        pressedMid = true;
    }
}

void SignalScreen::mouseReleaseEvent(QMouseEvent *_e) 
{
    if((_e->button() & Qt::LeftButton)) {
        pressedLeft = false;
    }
    if((_e->button() & Qt::RightButton)) {
        pressedRight = false;
    }
    if((_e->button() & Qt::MidButton)) {
        pressedMid = false;
    }
    if((pressedLeft == false) && (pressedRight == false) && (pressedMid == false)) {
        QCursor cursor;
        VerifyMouseOnCursor(_e, false, cursor);
    }
}

void SignalScreen::mouseMoveEvent(QMouseEvent *_e) 
{
    int posMouseY = _e->pos().y();
    int posMouseX = _e->pos().x();

    QCursor cursor;
    if((!pressedLeft) && (!pressedMid) && (!pressedRight)) {
        VerifyMouseOnCursor(_e, false, cursor);
        return;
    }

    else if(cursorOnPress.shape() == Qt::OpenHandCursor) {
        if(pressedLeft) {
            int deltaY = prevMouseY1 - posMouseY;
            if(deltaY != 0) {
                emit ChangeRShift0(deltaY);
                prevMouseY1 = posMouseY;
            }
        }
        if(pressedRight) {
            int deltaY = prevMouseY2 - posMouseY;
            if(deltaY != 0) {
                emit ChangeRShift1(deltaY);
                prevMouseY2 = posMouseY;
            }
        }
        if(pressedMid) {
            int deltaX = prevMouseX - posMouseX;
            if(deltaX != 0) {
                emit ChangeTShift(deltaX);
                prevMouseX = posMouseX;
            }
        }
    }
    else if(pressedLeft) {
        if(cursorOnPress.shape() == Qt::SplitVCursor) {
            int deltaY = prevMouseY1 - posMouseY;
            if(deltaY != 0) {
                emit ChangePosUCur(cursorChannel, numCursor, deltaY);
                prevMouseY1 = posMouseY;
            }
        }
        else {
            int deltaX = prevMouseXCur - posMouseX;
            if(deltaX != 0) {
                emit ChangePosTCur(cursorChannel, numCursor, deltaX);
                prevMouseXCur = posMouseX;
            }
        }
    }
}

void SignalScreen::Clear(QPaintDevice *_paintDevice) 
{
    QPainter *painter;
    painter = new QPainter(_paintDevice);
    painter->setBrush(QBrush(setD->colorBackground, Qt::SolidPattern));
    painter->drawRect(0, 0, width() - 1, height() - 1);
    delete painter;
}

void SignalScreen::DrawGrid(QPaintDevice *_paintDevice) 
{
    Clear(_paintDevice);
    QPainter painter(_paintDevice);
    if(setD->GetTypeDisplay() == TypeDisplay_Together) 
        DrawRectangle(&painter, 0, 0, width() - 1, height() - 1);
    else 
        DrawRectangle(&painter, 0, 0, width() - 1, height() / 2 - 1);
        DrawRectangle(&painter, 0, height() / 2, width() - 1, height() / 2 - 1);
    
    switch(setD->typeGrid) 
    {
        case TypeGrid_Base:
        case TypeGrid_Simple:
            if(setD->GetTypeDisplay() == TypeDisplay_Together) 
            {
                DrawHorizontalCrossLine(&painter, 0, 0, 10, 501);
                DrawHorizontalCrossLine(&painter, 0, 250, 10, 501);
                DrawHorizontalCrossLine(&painter, 0, 500, 10, 501);
                DrawVerticalCrossLine(&painter, 0, 0, 10, 501);
                DrawVerticalCrossLine(&painter, 250, 0, 10, 501);
                DrawVerticalCrossLine(&painter, 500, 0, 10, 501);
                if(setD->typeGrid == TypeGrid_Base) 
                {
                    for(int x = 0; x < 500; x +=  50) 
                        DrawVerticalPointLine(&painter, x, 0, 10, 501);
                   
                    for(int y = 0; y < 500; y += 50) 
                        DrawHorizontalPointLine(&painter, 0, y, 10, 501);
                }
            }
            else 
            {
                DrawHorizontalCrossLine(&painter, 0, 0, 10, 501);
                DrawHorizontalCrossLine(&painter, 0, 125, 10, 501);
                DrawHorizontalCrossLine(&painter, 0, 250, 10, 501);
                DrawVerticalCrossLine(&painter, 0, 0, 5, 251);
                DrawVerticalCrossLine(&painter, 250, 0, 5, 251);
                DrawVerticalCrossLine(&painter, 500, 0, 5, 251);
                if(setD->typeGrid == TypeGrid_Base) 
                {
                    for(int x = 0; x < 500; x += 50) 
                        DrawVerticalPointLine(&painter, x, 0, 5, 251);
                    
                    for(int y = 0; y < 250; y += 25) 
                        DrawHorizontalPointLine(&painter, 0, y, 10, 501);
                }
                DrawHorizontalCrossLine(&painter, 0, 251, 10, 501);
                DrawHorizontalCrossLine(&painter, 0, 376, 10, 501);
                DrawHorizontalCrossLine(&painter, 0, 501, 10, 501);
                DrawVerticalCrossLine(&painter, 0, 251, 5, 251);
                DrawVerticalCrossLine(&painter, 250, 251, 5, 251);
                DrawVerticalCrossLine(&painter, 501, 251, 5, 251);
                if(setD->typeGrid == TypeGrid_Base) 
                {
                    for(int x = 0; x < 500; x += 50) 
                        DrawVerticalPointLine(&painter, x, 251, 5, 251);
                    
                    for(int y = 251; y < 500; y += 25) 
                        DrawHorizontalPointLine(&painter, 0, y, 10, 501);
                }
            }
            break;
        case TypeGrid_None:
            break;
        default:
            //LOG("\nERRROR!!! Error in switch operator\n");
            break;
    }
    if(!connect) 
    {
        painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
        QFont font = painter.font();
        font.setPointSize(20);
        painter.setFont(font);
        painter.drawText(20, 50, QString(tr("УСТРОЙСТВО НЕ ГОТОВО")));
        painter.drawText(20, 80, QString(tr("1. Проверьте подключение модуля")));
        painter.drawText(20, 110, QString(tr("2. Нажмите кнопку Обновить")));
        painter.drawText(20, 140, QString(tr("3. Попробуйте выбрать другой порт")));
    }
}

void SignalScreen::DrawSignals(QPaintDevice * _paintDevice) 
{
    if(!D129_DataReadyForDraw(numDevice))
        return;

    if(setD->modeAccumulation != ModeAccumulation_NoLimitation) 
    {
        if(setD->modeAccumulation == ModeAccumulation_Disable || 
            (setD->modeAccumulation == ModeAccumulation_Limitation && setD->countAccumulation > (setD->numberAccumulation - 1))) 
        {
            Clear(_paintDevice);
            DrawGrid(_paintDevice);
            setD->countAccumulation = 0;
        }
    }

    setD->countAccumulation++;
    QPainter painter(_paintDevice);
    if(setD->antialiasing) 
        painter.setRenderHint(QPainter::Antialiasing, true);

    if(setD->GetTypeDisplay() == TypeDisplay_Together) 
    {
        if(enable[0]) 
        {
            painter.setPen(QPen(setD->colorSignal[0], Qt::SolidLine));
            DrawSignal(&painter, 0, 0, 501, D129_GetPointerToDataDraw(numDevice, 0));
        }
        if(enable[1]) 
        {
            painter.setPen(QPen(setD->colorSignal[1], Qt::SolidLine));
            DrawSignal(&painter, 0, 0, 501, D129_GetPointerToDataDraw(numDevice, 1));
        }
    }
    else 
    {
        if(enable[0]) 
        {
            painter.setPen(QPen(setD->colorSignal[0], Qt::SolidLine));
            DrawSignal(&painter, 0, 0, 251, D129_GetPointerToDataDraw(numDevice, 0));
        }
        if(enable[1]) 
        {
            painter.setPen(QPen(setD->colorSignal[1], Qt::SolidLine));
            DrawSignal(&painter, 0, 0, 251, D129_GetPointerToDataDraw(numDevice, 1));
        }
        if(D129_MathFunctionIsEnable(numDevice)) 
        {
            painter.setPen(QPen(setD->colorSignal[2], Qt::SolidLine));
            DrawSignal(&painter, 0, 251, 251, D129_GetPointerToDataMathFunction(numDevice));
            static QString strings[RangeSize] = {tr("5mV"), tr("10mV"), tr("20mV"), tr("50mV"), tr("100mV"), tr("200mV"),
                tr("500mV"), tr("1V"), tr("2V"), tr("5V")};
            painter.drawText(5, 265, strings[D129_GetRange(numDevice, 2)]);
        }
        if(D129_IsSpectrView(numDevice, 0)) 
        {
            painter.setPen(QPen(setD->colorSignal[0], Qt::SolidLine));
            DrawSpectr(&painter, 0, 251, 251, D129_GetPointerToSpectr(numDevice, 0));
        }
        else if (D129_IsSpectrView(numDevice, 1)) 
        {
            painter.setPen(QPen(setD->colorSignal[1], Qt::SolidLine));
            DrawSpectr(&painter, 0, 251, 251, D129_GetPointerToSpectr(numDevice, 1));
        }
    }
}

void SignalScreen::DrawCursors(QPainter *painter) 
{
    // Рисуем курсоры нулевого канала
    if(setD->enableCursU[0] && (0 == setD->GetActiveCurs())) {
        painter->setPen(QPen(setD->colorCurs[0], 1, Qt::SolidLine));
        int pos0 = setD->GetPosUCur(0, 0);
        int pos1 = setD->GetPosUCur(0, 1);
        switch(setD->GetTypeDisplay()) {
            case TypeDisplay_Together:
                DrawUCursor(painter, 0, 0, 501, pos0, pos1);
                break;
            case TypeDisplay_Separetly:
                DrawUCursor(painter, 0, 0, 251, pos0, pos1);
                break;
            default:
                //LOG("\nERROR!!! Error in switch operator\n");
                break;
        }
    }
    if(setD->enableCursT[0] && (0 == setD->GetActiveCurs())) {
        painter->setPen(QPen(setD->colorCurs[0], 1, Qt::SolidLine));
        int step = 5;
        int maxY = (setD->GetTypeDisplay() == TypeDisplay_Together) ? height() - 1 : height() / 2;
        for(int y = 0 - step / 2; y < maxY; y += 2 * step) {
            int x1 = setD->GetPosTCur(0, 0);
            int x2 = setD->GetPosTCur(0, 1);
            painter->drawLine(x1, y, x1, y + step - 1);
            painter->drawLine(x2, y, x2, y + step - 1);
        }
    }
    // Рисуем курсоры первого канала
    if(setD->enableCursU[1] && (1 == setD->GetActiveCurs())) {
        painter->setPen(QPen(setD->colorCurs[1], 1, Qt::SolidLine));
        int pos0 = setD->GetPosUCur(1, 0);
        int pos1 = setD->GetPosUCur(1, 1);
        switch(setD->GetTypeDisplay()) {
            case TypeDisplay_Together:
                DrawUCursor(painter, 0, 0, 501, pos0, pos1);
                break;
            case TypeDisplay_Separetly:
                DrawUCursor(painter, 0, 0, 251, pos0, pos1);
                break;
            default:
                //LOG("\nERRROR!!! Error in switch operator\n");
                break;
        }
    }
    if(setD->enableCursT[1] && (1 == setD->GetActiveCurs())) {
        painter->setPen(QPen(setD->colorCurs[1], 1, Qt::SolidLine));
        int step = 5;
        int maxY = (setD->GetTypeDisplay() == TypeDisplay_Together) ? height() - 1 : height() / 2;
        for(int y = 0 - step / 2; y < maxY; y += 2 * step) {
            int x1 = setD->GetPosTCur(1, 0);
            int x2 = setD->GetPosTCur(1, 1);
            painter->drawLine(x1, y, x1, y + step - 1);
            painter->drawLine(x2, y, x2, y + step - 1);
        }
    }
    /*
    // Рисуем математические курсоры
    if(setD->enableCursU[3] && (setD->GetActiveCurs() == 2)) {
        double voltage1 = setD->GetPosUCurAbs(3, 1, data.rangeMath);
        double voltage2 = setD->GetPosUCurAbs(3, 2, data.rangeMath);
        DrawUCursor(painter, 0, 252, 251, voltage1, voltage2);
    }
    if(setD->enableCursT[2] && (2 == setD->GetActiveCurs())) {
        painter->setPen(QPen(setD->colorCurs[3], 1, Qt::SolidLine));
        int step = 5;
        for(int y = 251; y < height() - 1; y += 2 * step) {
            int x1 = setD->GetPosTCur(3, 1);
            int x2 = setD->GetPosTCur(3, 2);
            painter->drawLine(x1, y, x1, y + step - 1);
            painter->drawLine(x2, y, x2, y + step - 1);
        }
    }
    */
}

void SignalScreen::DrawRectangle(QPainter *_painter, int _x1, int _y1, int _width, int _height) 
{
    _painter->setPen(QPen(setD->colorGrid, 1, Qt::SolidLine));
    _painter->drawRect(_x1, _y1, _width, _height);
}

void SignalScreen::DrawVerticalCrossLine(QPainter *_painter, const int _x, const int _y, int _dy, int _length) 
{
    for(int posY = _y; posY < (_length + _y); posY += _dy) 
        DrawCross(_painter, _x, posY, 5, 5);
}

void SignalScreen::DrawHorizontalCrossLine(QPainter *_painter, const int _x, const int _y, int _dx, int _length)
{
    for(int posX = _x; posX < (_length + _x); posX += _dx) 
        DrawCross(_painter, posX, _y, 5, 5);
}

void SignalScreen::DrawVerticalPointLine(QPainter *_painter, const int _x, const int _y, int _dy, int _length) 
{
    for(int posY = _y; posY < (_length + _y); posY += _dy) 
        _painter->drawPoint(_x, posY);
}

void SignalScreen::DrawHorizontalPointLine(QPainter *_painter, const int _x, const int _y, int _dx, int _length) 
{
    for(int posX = _x; posX < (_length + _x); posX += _dx) 
        _painter->drawPoint(posX, _y);
}

void SignalScreen::DrawCross(QPainter *_painter, int _x, int _y, int _dx, int _dy) 
{
    _painter->drawLine(_x - _dx / 2, _y, _x + _dx / 2, _y);
    _painter->drawLine(_x, _y - _dy / 2, _x, _y + _dy / 2);
}

void SignalScreen::DrawSignal(QPainter *_painter, const int _x, const int _y, const int _height, const double *_buffer) 
{
    DrawMode drawMode = setD->drawMode;

    if(D129_GetModeADC(numDevice) == ModeADC_Randomize) 
        if(D129_GetStartMode(numDevice) == StartMode_Single)
            setD->drawMode = DrawMode_Lines;
        else if(D129_GetFraction(numDevice) < 0.5)
            setD->drawMode = DrawMode_Points;

    switch(setD->drawMode) 
    {
        case DrawMode_Points:
            for(int x1 = 0; x1 < (DB_SIGNED_LENGTH - 1); x1++) 
            {
                int posX1 = _x + x1;
                int posY1 = _y + _height - _buffer[x1] * _height;
                _painter->drawPoint(posX1, posY1);

                /*
                switch(D129_GetModeADC()) 
                {
                    case ModeADC_Randomize:
                        if(D129_IsReadyPointRandomizer(x1) > 0)
                            _painter->drawPoint(posX1, posY1);
                        break;

                    case ModeADC_RealTime:
                        _painter->drawPoint(posX1, posY1);
                        break;
                }
                */
            }
            break;

        case DrawMode_Lines:
            for(int x1 = 0; x1 < (DB_SIGNED_LENGTH - 1); x1++) 
            {
                int x2 = x1 + 1;
                int posX1 = x1 + _x;
                int posX2 = x2 + _x;
                int posY1 = _y + _height - _buffer[x1] * _height;
                Limitation(posY1, _y - 1, _y + _height);
                int posY2 = _y + _height - _buffer[x2] * _height;
                Limitation(posY2, _y - 1, _y + _height);
                _painter->drawLine(posX1, posY1, posX2, posY2);
            }
            break;

        default:
      //      LOG("Error in switch operator");
            break;
    }

    if(D129_GetModeADC(numDevice) == ModeADC_Randomize)
        setD->drawMode = drawMode;
}

void SignalScreen::DrawSpectr(QPainter *_painter, const int _x, const int _y, const int _height, const double *_buffer) 
{
    int posY_0 = _y + _height;

    for(int x1 = 0; x1 < 250; x1++) 
    {
        double val = _buffer[x1];
        int posX = x1 * 2 + _x;
        int posY = posY_0 - val * _height;
        _painter->drawLine(posX, posY_0, posX, posY);
    }

    char buffer[15];
    D129_GetStepFreq(numDevice, buffer, 15);
    _painter->drawText(460, 265, QString(buffer));
}

void SignalScreen::DrawUCursor(QPainter *_painter, const int _x, const int _y, const int _height, int _pos1, int _pos2) 
{
    int step = 5;
    if(setD->typeDisplay == TypeDisplay_Separetly) 
    {
        _pos1 /= 2;
        _pos2 /= 2;
    }
    int posY_0 = _y + _height /2;
    int posY1 = posY_0 - _pos1;
    int posY2 = posY_0 - _pos2;
    for(int x1 = -step/2; x1 < NS_TypesAltera::DB_SIGNED_LENGTH; x1 += step * 2) {
        int posX = x1 + _x;
        _painter->drawLine(posX, posY1, posX + step - 1, posY1);
        _painter->drawLine(posX, posY2, posX + step - 1, posY2);
    }
}

void SignalScreen::MergeImage(QImage *_image) 
{
    QPainter painter(this);
    painter.setBrush((QBrush(setD->colorBackground, Qt::SolidPattern)));
    painter.begin(this);
    for(int x = 0; x < width() - 1; x++) 
    {
        for(int y = 0; y < height() - 1; y++) 
        {
            QRgb qRgb = _image->pixel(x, y);
            if(qRgb != setD->colorBackground.rgb()) 
            {
                painter.setPen(QPen((QColor(qRed(qRgb), qGreen(qRgb), qBlue(qRgb))), Qt::SolidLine));
                painter.drawPoint(x, y);
            }
        }
    }
}

bool SignalScreen::VerifyMouseOnCursor(QMouseEvent *_e, bool _pressed, QCursor &_cursor) 
{
    int posMouseX = _e->pos().x();
    int posMouseY = _e->pos().y();
    QCursor cursorEmpty = _pressed ? Qt::OpenHandCursor : Qt::ArrowCursor;
    _cursor = cursorEmpty;
    if(setD->enableCursU[0] && (0 == setD->GetActiveCurs())) 
    {
        SetCursorOnScreenU(0, 0, posMouseY, _cursor);
        SetCursorOnScreenU(0, 1, posMouseY, _cursor);
    }
    if(setD->enableCursU[1] && (1 == setD->GetActiveCurs())) 
    {
        SetCursorOnScreenU(1, 0, posMouseY, _cursor);
        SetCursorOnScreenU(1, 1, posMouseY, _cursor);
    }
    if(setD->enableCursT[0] && (0 == setD->GetActiveCurs())) 
    {
        SetCursorOnScreenT(0, 0, posMouseX, _cursor);
        SetCursorOnScreenT(0, 1, posMouseX, _cursor);
    }
    if(setD->enableCursT[1] && (1 == setD->GetActiveCurs())) 
    {
        SetCursorOnScreenT(1, 0, posMouseX, _cursor);
        SetCursorOnScreenT(1, 1, posMouseX, _cursor);
    }
    setCursor(_cursor);
    return (_cursor.shape() != cursorEmpty.shape());
}

void SignalScreen::SetCursorOnScreenU(int _channel, int _numCursor, int _posMouse, QCursor &_cursor) 
{
    if((Abs(setD->GetPosUCurInScreen(_channel, _numCursor) - _posMouse) < deltaCursor)) 
    {
        _cursor = Qt::SplitVCursor;
        cursorChannel = _channel;
        numCursor = _numCursor;
    }
}

void SignalScreen::SetCursorOnScreenT(int _channel, int _numCursor, int _posMouse, QCursor &_cursor) 
{
    if((Abs(setD->GetPosTCurInScreen(_channel, _numCursor) - _posMouse) < deltaCursor)) 
    {
        _cursor = Qt::SplitHCursor;
        cursorChannel = _channel;
        numCursor = _numCursor;
    }
}