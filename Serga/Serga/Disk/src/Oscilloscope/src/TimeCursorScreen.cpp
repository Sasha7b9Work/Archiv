#pragma warning(disable:4127)
#include <QPainter>
#pragma warning(default:4127)
#include "TimeCursorScreen.h"
#include "SettingsDisplay.h"
#include "../../common/TypesAltera.h"
#include "../../Device129Multi/src/Device129Multi.h"

using namespace NS_TypesAltera;
using namespace NS_TypesDisplay;

extern int numDevice;

TimeCursorScreen::TimeCursorScreen(QWidget *_parent, SettingsDisplay *_settings) : QFrame(_parent), setD(_settings) 
{
    resize(SCR_W_TSHIFT, SCR_H_TSHIFT);
}

void TimeCursorScreen::Reset() 
{
    update();
}

QImage TimeCursorScreen::GetImage() 
{
    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    render(&painter);
    return image;
}

void TimeCursorScreen::paintEvent(QPaintEvent *) 
{
    QImage tempImage(size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(this);
    painter.setBrush(QBrush(setD->colorBackground, Qt::SolidPattern));
    painter.setPen(QPen(setD->colorBackground, 1, Qt::SolidLine));
    painter.drawRect(0, 0, width() - 1, height() - 1);

    // Рисуем нулевую отметку
    int x = 0;
    if(D129_GetTPos(numDevice) == TPos_Left) 
        x = SCR_W_TR_LEV;
    else if(D129_GetTPos(numDevice) == TPos_Center) 
        x = width() / 2;
    else 
        x = width() - SCR_W_RSHIFT - 1;
    
    painter.setBrush(QBrush(setD->colorTShift0, Qt::SolidPattern));
    painter.setPen(QPen(setD->colorTShift0));
    painter.drawEllipse(QPoint(x, height() / 2), height() / 2, height() / 2);

    // Рисуем временной маркер
    x = TShiftZero - D129_GetTShiftRel(numDevice) + SCR_W / 2 + SCR_W_TR_LEV;
    if(D129_GetTPos(numDevice) == TPos_Left) 
        x -= SCR_W / 2;
    else if(D129_GetTPos(numDevice) == TPos_Right) 
        x += SCR_W / 2;
    
    QPoint cursor[3];
    int min = SCR_W_TR_LEV;
    int max = width() - SCR_W_RSHIFT - 1;
    int height = this->height();

    if(x >= min && x <= max) 
    {
        cursor[0] = QPoint(x - height+1, 0);
        cursor[1] = QPoint(x + height-1, 0);
        cursor[2] = QPoint(x, height - 1);
    }
    else if(x < min) 
    {
        cursor[0] = QPoint(0, height / 2);
        cursor[1] = QPoint(height, 0);
        cursor[2] = QPoint(height, height - 1);
    }
    else 
    {
        cursor[0] = QPoint(width() - height, height - 1);
        cursor[1] = QPoint(width() - height, 0);
        cursor[2] = QPoint(width() - 1, height / 2);
    }
    painter.setBrush(QBrush(setD->colorTShift, Qt::SolidPattern));
    painter.setPen(QPen(setD->colorTShift));
    painter.drawPolygon(cursor, 3);
}
