//  [7/27/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QColor>
#pragma warning(default:4127)
#include "TypesDisplay.h"

struct SettingsDisplay {

    SettingsDisplay();
    ~SettingsDisplay();

    NS_TypesDisplay::TypeDisplay typeDisplay;

    NS_TypesDisplay::TypeDisplay GetTypeDisplay() {
        return typeDisplay;
    }

    inline void SetActiveCurs(int _activeCurs) {
        activeCurs = _activeCurs;
    }

    inline int GetActiveCurs() {
        return activeCurs;
    }

    NS_TypesDisplay::TypeGrid typeGrid;

    NS_TypesDisplay::DrawMode drawMode;

    NS_TypesDisplay::ModeAccumulation modeAccumulation;

    bool antialiasing;
    bool trigImpulse;           // Хранит признак того, нужно ли рисовать признак синхроимпульса

    int numberAccumulation;     // Количесво накоплений
    int countAccumulation;      // Порядковый номер накопления

    // Курсоры
    //int spectr;                 // Если 0, спектры не выводятся, если 1 - выводится спектр первого канала, если 2 - выводится спектр второго канала
    bool enableCursU[3];        // Если курсоры находятся в активном состоянии - галочка выбора включена - true, иначе - false
    bool enableCursT[3];        // --//--

    QColor colorBackground;
    QColor colorGrid;
    QColor colorSignal[3];     
    QColor colorCurs[3];       
    QColor colorCursSpectr[2];
    QColor colorTShift;
    QColor colorTShift0;

    void SetPosUCur(int channel, int numCur, int value);
    double GetPosUCurAbs(int channel, int numCur);
    int GetPosUCur(int channel, int numCur);

    int GetPosUCurInScreen(int channel, int numCur);             // Получить абсолютную позицию курсора на экране - верху соответствует 0, низу - height() - 1

    void SetPosTCur(int channel, int numCur, int value);
    double GetPosTCurAbs(int channel, int numCur);
    int GetPosTCur(int channel, int numCur);

    int GetPosTCurInScreen(int channel, int numCur);

private:
    int posCursU[4][3];         // Позиции курсоров напряжения. В центре имеют 0, могут принимать очень большие значения
    int posCursT[4][3];         // Позиции курсоров времени. В центре имеют 0, могут принимать очень большие значения
    int activeCurs;
};