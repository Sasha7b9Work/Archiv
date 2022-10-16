//  [3/1/2010 Sasha]
#include "defines.h"
#include "Holder.h"
#include "GlobalVars.h"

extern SettingsWindow setW;

//  [3/2/2010 Sasha]
Holder::Holder(QFrame *_parent) :  QFrame(_parent), exactly(false) {
    setMouseTracking(true);
    degree = 0;
    int w = _parent->width();
    int h = _parent->height();
    resize(w, h);
    screen = new QImage(w, h, QImage::Format_ARGB32_Premultiplied);
    Draw(screen);
}

//  [3/5/2010 Sasha]
void Holder::SetRange(int _max) {
    max = _max;
    if(degree > max) {
        degree = max;
    }
    update();
}

//  [3/4/2010 Sasha]
void Holder::SetExactly(bool _exactly) {
    exactly = _exactly;
}

//  [3/2/2010 Sasha]
void Holder::SetValue(int _value) {
    degree = _value;
    emit ChangeValue(degree);
    Draw(screen);
    update();
}

//  [3/2/2010 Sasha]
void Holder::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QRect rect = this->rect();
    painter.drawImage(rect, *screen);
}

//  [3/2/2010 Sasha]
void Holder::mousePressEvent(QMouseEvent *_event) {
    if(MW_Auto == setW.mw) {
        return;
    }
    cursor = Qt::BlankCursor;
    setCursor(cursor);
    globalPosition = _event->globalPos();
    prevPosX = _event->pos().x();
    prevPosY = _event->pos().y();
}

//  [3/2/2010 Sasha]
void Holder::mouseReleaseEvent(QMouseEvent *) {
    if(MW_Auto == setW.mw) {
        return;
    }
    cursor = Qt::OpenHandCursor;
    setCursor(cursor);
}

//  [3/2/2010 Sasha]
void Holder::mouseMoveEvent(QMouseEvent *_event) {
    if(MW_Auto == setW.mw) {
        setCursor(Qt::ArrowCursor);
        return;
    }
    int posY = _event->pos().y();
    if(cursor.shape() == Qt::BlankCursor) {
        QCursor::setPos(globalPosition);
        int deltaY = - posY + prevPosY;
        float k = exactly ? 1/5.0f : 10.0f;
        float delta = deltaY * k;
        LOG("delta = %f", delta);
        degree += delta;
        float min = 0.0f;
        degree = qBound(min, degree, max);
        emit ChangeValue(degree);
        Draw(screen);
        update();
    }
    if(!_event->buttons()) {
        cursor = Qt::OpenHandCursor;
        setCursor(cursor);
    }
}

//  [3/2/2010 Sasha]
void Holder::Draw(QPaintDevice *_paintDevice) {
    QPainter painter(_paintDevice);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(0, 0, width(), height(), palette().window());
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter.drawEllipse(1, 1, width() - 2, height() - 2);

    int startAngle = 75 * 16;
    int spanAngle = - 60 * 16;
    painter.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
    painter.setBrush(QBrush(Qt::gray));
    painter.drawPie(2, 2, width() - 4, height() - 4, startAngle, spanAngle);

    startAngle += 180 * 16;
    painter.drawPie(2, 2, width() - 4, height() - 4, startAngle, spanAngle);

    DrawBlackCircle(&painter);
}

//  [3/2/2010 Sasha]
void Holder::DrawBlackCircle(QPainter *_painter) {
    _painter->setRenderHint(QPainter::Antialiasing, false);
    _painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    _painter->setBrush(QBrush(Qt::black));

    int radius = 12;        // Радиус, по которому происходит поворот окружности

    float speed = 0.005f;
    int cX = width() / 2 - cos(degree * speed) * radius;
    int cY = height() / 2 - sin(degree * speed) * radius;

    const int SIZE_CIRCLE = 4;

    _painter->drawEllipse(cX - SIZE_CIRCLE / 2, cY - SIZE_CIRCLE / 2, SIZE_CIRCLE, SIZE_CIRCLE);
}
