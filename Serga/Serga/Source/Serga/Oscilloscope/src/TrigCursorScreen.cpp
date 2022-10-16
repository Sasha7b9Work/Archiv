#pragma warning(disable:4127)
#include <QPainter>
#pragma warning(default:4127)
#include "TrigCursorScreen.h"
#include "SettingsDisplay.h"
#include "../common/TypesAltera.h"
#include "../../Device129Multi/src/Device129Multi.h"

using namespace NS_TypesAltera;
using namespace NS_TypesDisplay;

extern int numDevice;

TrigCursorScreen::TrigCursorScreen(QFrame *_parent, SettingsDisplay *_settings):QFrame(_parent), setD(_settings)
{
}

void TrigCursorScreen::Reset() 
{
    int size_x = SCR_W_TR_LEV;
    int size_y = (setD->GetTypeDisplay() == TypeDisplay_Separetly) ? SCR_BG_H + 1 : SCR_BG_H; 
    resize(size_x, size_y);
    update();
}

QImage TrigCursorScreen::GetImage() 
{
    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    render(&painter);
    return image;
}

void TrigCursorScreen::paintEvent(QPaintEvent *) 
{
    QPainter painter(this);
    painter.setBrush(QBrush(setD->colorBackground, Qt::SolidPattern));
    painter.setPen(QPen(setD->colorBackground));
    painter.drawRect(0, 0, width() - 1, height() - 1);

    // Рисуем курсор
    int trigLev = D129_GetTrigLevRel(numDevice, D129_GetSinchroChannel(numDevice)) + D129_GetRShiftRel(numDevice, D129_GetSinchroChannel(numDevice)) - RShiftZero;

    int min = 0;
    int max = (setD->GetTypeDisplay() == TypeDisplay_Together) ? this->height() - 1 : (this->height() - 1) / 2;
    int width = this->width() - 1;

    int coord = max / 2 - (double)(trigLev - TrigLevZero) / (RShiftMaxScreen - RShiftZero) * max / 2;

    QPoint cursor[3];
    if(coord >= min && coord <= max) 
    {
        cursor[0] = QPoint(0, coord - width);
        cursor[1] = QPoint(width, coord);
        cursor[2] = QPoint(0, coord + width);
    }
    else if(coord < min) 
    {
        cursor[0] = QPoint(0, min + width);
        cursor[1] = QPoint(width / 2, min);
        cursor[2] = QPoint(width, min + width);
    }
    else 
    {
        cursor[0] = QPoint(0, max - width);
        cursor[1] = QPoint(width, max - width);
        cursor[2] = QPoint(width / 2, max);
    }
    QColor color = setD->colorSignal[D129_GetSinchroChannel(numDevice)];
    painter.setBrush(QBrush(color, Qt::SolidPattern));
    painter.setPen(QPen(color));
    painter.drawPolygon(cursor, 3);
}
