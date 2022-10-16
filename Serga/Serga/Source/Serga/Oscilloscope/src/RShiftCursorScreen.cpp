//  [4/15/2009 Sasha]
#include "../../Device129Multi/src/Device129Multi.h"
#pragma warning(disable:4127)
#include <QPainter>
#pragma warning(default:4127)
#include "RShiftCursorScreen.h"
#include "SettingsDisplay.h"
#include "../common/TypesAltera.h"

using namespace NS_TypesAltera;
using namespace NS_TypesDisplay;

extern int numDevice;

//  [7/13/2009 Sasha]
RShiftCursorScreen::RShiftCursorScreen(QWidget *_parent, SettingsDisplay *_settings):
    QFrame(_parent), 
    setD(_settings)
{

}

//  [9/28/2009 Sasha]
void RShiftCursorScreen::Reset() {
    int size_x = SCR_W_RSHIFT;
    int size_y = (setD->GetTypeDisplay() == TypeDisplay_Separetly) ? SCR_BG_H + 1 : SCR_BG_H;
    resize(size_x, size_y);
    update();
}

//  [2/4/2010 Sasha]
QImage RShiftCursorScreen::GetImage() {
    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    render(&painter);
    return image;
}

//  [7/14/2009 Sasha]
void RShiftCursorScreen::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setBrush(QBrush(setD->colorBackground, Qt::SolidPattern));
    painter.setPen(QPen(setD->colorBackground));
    painter.drawRect(0, 0, width() - 1, height() - 1);

    // Рисуем курсоры
    if(D129_GetSinchroChannel(numDevice) == 1) {
        DrawCursor(&painter, 0);
        DrawCursor(&painter, 1);
    }
    else {
        DrawCursor(&painter, 1);
        DrawCursor(&painter, 0);
    }
}

//  [7/14/2009 Sasha]
void RShiftCursorScreen::DrawCursor(QPainter * _painter, const int _channel) {
    int rShift = D129_GetRShiftRel(numDevice, _channel);

    int min = 0;
    int max = (setD->GetTypeDisplay() == TypeDisplay_Together) ? this->height() - 1 : (this->height() - 1) / 2;
    int width = this->width() - 1;

    int coord = (max + min) / 2 - (float)(max - min) / (RShiftMaxScreen - RShiftMinScreen) *
        (rShift - RShiftZero);

    QPoint cursor[3];
    if(coord >= min && coord <= max) {
        cursor[0] = QPoint(width, coord - width);
        cursor[1] = QPoint(0, coord);
        cursor[2] = QPoint(width, coord + width);
    }
    else if(coord < min) {
        cursor[0] = QPoint(0, min + width);
        cursor[1] = QPoint(width / 2, min);
        cursor[2] = QPoint(width, min + width);
    }
    else {
        cursor[0] = QPoint(0, max - width);
        cursor[1] = QPoint(width, max - width);
        cursor[2] = QPoint(width / 2, max);
    }

    QColor color = setD->colorSignal[_channel];
    _painter->setBrush(QBrush(color, Qt::SolidPattern));
    _painter->setPen(QPen(color));
    _painter->drawPolygon(cursor, 3);
}
