//  [3/1/2010 Sasha]
#pragma once
#include "defines.h"

class Holder:public QFrame {
    Q_OBJECT
public:
    Holder(QFrame *parent = 0);

    void SetRange(int max);
    void SetExactly(bool exactly);
    bool GetExactly() { return exactly; };
    int GetValue() { return degree; }

public slots:
    void SetValue(int value);

protected:
    void paintEvent(QPaintEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);

signals:
    void ChangeValue(int);

private:
    float max;
    float degree;
    QImage *screen;
    QPoint globalPosition;
    QCursor cursor;
    int prevPosX;
    int prevPosY;
    bool exactly;
    
    void Draw(QPaintDevice *paintDevice);
    void DrawBlackCircle(QPainter *painter);
};