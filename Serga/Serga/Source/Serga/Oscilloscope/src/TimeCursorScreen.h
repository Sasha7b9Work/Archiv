//  [7/13/2009 Sasha]
#pragma once 
#pragma warning(disable:4127)
#include <QFrame>
#include <QColor>
#pragma warning(default:4127)

struct SettingsDisplay;

class TimeCursorScreen : public QFrame {
public:
    TimeCursorScreen(QWidget *parent, SettingsDisplay *settings);
    void Reset();
    QImage GetImage();

protected:
    void paintEvent(QPaintEvent *);

private:
    SettingsDisplay *setD;
};