//  [7/13/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QFrame>
#pragma warning(default:4127)

struct SettingsDisplay;

class TrigCursorScreen : public QFrame {
public:
    TrigCursorScreen(QFrame *parent, SettingsDisplay *settings);
    void Reset();
    QImage GetImage();

protected:
    void paintEvent(QPaintEvent *);

private:
    SettingsDisplay *setD;
};