#pragma once
#pragma warning(disable:4127)
#include <QFrame>
#include <QPainter>
#pragma warning(default:4127)

class TrigScreen : public QFrame
{
    static const int WIDTH = 30;
    static const int HEIGHT = 25;

public:
    TrigScreen(QWidget *_parent) : QFrame(_parent), isTrig(false)
    {
        update();
        resize(WIDTH, HEIGHT);
    };
    void SetTrig(bool _trig)
    {
        isTrig = _trig;
        update();
    };

protected:
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter.drawRect(0, 0, width() - 1, height() - 1);
        QString string = isTrig ? tr("Trig") : tr("");
        QColor color = Qt::blue;
        painter.setPen(QPen(color, Qt::SolidLine));
        painter.drawText(6, 17, string);
    };

private:
    bool isTrig;
};