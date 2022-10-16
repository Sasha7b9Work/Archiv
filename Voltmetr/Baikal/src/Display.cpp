//  [3/1/2010 Sasha]
#include "defines.h"
#include "Display.h"
#include "GlobalVars.h"

extern SettingsWindow setW;

//  [3/1/2010 Sasha]
Display::Display(QWidget *_parent) : QFrame(_parent), enabled(false) {
    move(2, 2);
    resize(_parent->width() - 4, _parent->height() - 4);
    screen = new QImage(width() / 2, height() / 2, QImage::Format_ARGB32_Premultiplied);
    string = tr("U=8.127V");
}

//  [3/1/2010 Sasha]
void Display::SetValue(QString _string) {
    string = _string;
    update();
}

//  [3/5/2010 Sasha]
void Display::SetEnabled(bool _enabled) {
    enabled = _enabled;
    update();
}

//  [4/7/2010 Sasha]
void Display::SetColorDisplay(QColor _color) {
    setW.colorDisplay = _color;
    update();
}

//  [4/7/2010 Sasha]
void Display::SetColorSimbols(QColor _color) {
    setW.colorSymbols = _color;
    update();
}

//  [3/1/2010 Sasha]
void Display::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    Draw();
    painter.drawImage(rect(), *screen);
}

//  [3/1/2010 Sasha]
void Display::Draw() {
    QPainter painter(screen);
    const int fontSize = 10 ;
    painter.setBrush(QBrush(enabled ? setW.colorDisplay : setW.colorDisplayDisabled, Qt::SolidPattern));
    painter.setPen(QPen(enabled ? setW.colorDisplay : setW.colorDisplayDisabled, 1, Qt::SolidLine));
    painter.drawRect(0, 0, width(), height());
    if(!enabled) {
        return;
    }
    painter.setPen(QPen(setW.colorSymbols, 1, Qt::SolidLine));
    QFont font = painter.font();
    font.setPointSize(fontSize);
    painter.setFont(font);
    int posY = screen->height() / 2 + fontSize / 2;
    painter.drawText(6, posY, string);
}