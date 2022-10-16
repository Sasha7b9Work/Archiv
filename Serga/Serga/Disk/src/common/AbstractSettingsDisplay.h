//  [12/13/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QColor>
#pragma warning(default:4127)

struct AbstractSettingsDisplay:public QObject {

    AbstractSettingsDisplay()
    {
        colorBackground = Qt::black;
        colorSignal[0] = Qt::white;
        colorSignal[1] = Qt::white;
        colorGrid = Qt::yellow;
        colorFont = Qt::yellow;
        colorCurs[0] = Qt::white;
        colorCurs[1] = Qt::white;
    };

    QColor colorBackground;
    QColor colorSignal[2];
    QColor colorGrid;
    QColor colorFont;
    QColor colorCurs[2];
};