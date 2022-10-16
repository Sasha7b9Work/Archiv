//  [3/1/2010 Sasha]
#pragma once
#include "defines.h"

class Display:public QFrame {
    Q_OBJECT
public:
    Display(QWidget *parent);
    void SetValue(QString string);
    void SetEnabled(bool enabled);
    void SetColorDisplay(QColor color);
    void SetColorSimbols(QColor color);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    QString string;
    QImage *screen;
    void Draw();
    bool enabled;
};