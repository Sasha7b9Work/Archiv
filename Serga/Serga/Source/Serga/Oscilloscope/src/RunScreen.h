#pragma once;
#pragma warning(disable:4127)
#include <QFrame>
#include <QPainter>
#pragma warning(default:4127)

class RunScreen : public QFrame
{
    static const int WIDTH = 30;
    static const int HEIGHT = 25;

public:
    RunScreen(QWidget *_parent) : QFrame(_parent), isRun(false)
    {
        update();
        resize(WIDTH, HEIGHT);
    };

    void SetState(bool _isRun)
    {
        isRun = _isRun;
        update();
    };

protected:
    void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter.drawRect(0, 0, width() - 1, height() - 1);
        QString string = isRun ? tr("Run") : tr("Stop");
        QColor color = isRun ? Qt::blue : Qt::red;
        painter.setPen(QPen(color, Qt::SolidLine));
        painter.drawText(isRun ? 6 : 4, 17, string);
    };

private:
    bool isRun;
};