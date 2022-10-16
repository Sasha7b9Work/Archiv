//  [4/15/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QObject>
#include <QFrame>
#pragma warning(default:4127)

struct SettingsDisplay;

class RShiftCursorScreen : public QFrame {

public:
    RShiftCursorScreen(QWidget *parent, SettingsDisplay *settings);

    void Reset();
    QImage GetImage();

protected:
    void paintEvent(QPaintEvent *);

private:
    SettingsDisplay *setD;
    void DrawCursor(QPainter *painter, const int channel);
};