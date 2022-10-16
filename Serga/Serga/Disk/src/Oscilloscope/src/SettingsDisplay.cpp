#include "../../Device129Multi/src/Device129Multi.h"
#include "SettingsDisplay.h"
#include "../../common/defines.h"
#include "../../common/TypesAltera.h"
#include "../../common/MathDisplayOscilloscope.h"
#include "../common/TablesOfConverting.h"
#include "../common/MathDisplayOscilloscope.h"

using namespace NS_TypesAltera;
using namespace NS_TypesDisplay;

extern TablesOfConverting tables;
extern MathDisplayOscilloscope mathDisplay;

extern int numDevice;

//  [9/25/2009 Sasha]
SettingsDisplay::SettingsDisplay():
    typeDisplay(TypeDisplay_Separetly),
    typeGrid(TypeGrid_Base),
    drawMode(DrawMode_Points),
    modeAccumulation(ModeAccumulation_Disable),
    antialiasing(false),
    numberAccumulation(1),
    countAccumulation(1),
    activeCurs(0),
    colorBackground(Qt::black),
    colorGrid(Qt::white),
    colorTShift(Qt::white),
    colorTShift0(Qt::yellow),
     trigImpulse(false)
{
    for(int i = 0; i < 3; i++) {
        enableCursU[i] = false;
        enableCursT[i] = false;
        colorSignal[i] = Qt::white;
        colorCurs[i] = Qt::white;
    }
    colorCursSpectr[1] = Qt::white;
    colorCursSpectr[2] = Qt::white;
}

SettingsDisplay::~SettingsDisplay() 
{
};

void SettingsDisplay::SetPosUCur(int channel, int numCur, int value) 
{
    posCursU[channel][numCur] = value;
}

//  [10/26/2009 Sasha]
double SettingsDisplay::GetPosUCurAbs(int _channel, int _numCur) 
{
    return mathDisplay.GetPosUCurAbs(posCursU[_channel][_numCur], D129_GetRange(numDevice, _channel));
}

//  [10/26/2009 Sasha]
int SettingsDisplay::GetPosUCur(int channel, int numCur) {
    return posCursU[channel][numCur];
}

//  [2/12/2010 Sasha]
int SettingsDisplay::GetPosUCurInScreen(int _channel, int _numCur) {
    int pos = posCursU[_channel][_numCur];
    if(TypeDisplay_Together == typeDisplay) {
        return 250 - pos;
    }
    else {
        return 125 - pos / 2;
    }
}

//  [10/26/2009 Sasha]
void SettingsDisplay::SetPosTCur(int channel, int numCur, int value) {
    posCursT[channel][numCur] = value;
}

double SettingsDisplay::GetPosTCurAbs(int _channel, int _numCur) 
{
    int pos = posCursT[_channel][_numCur];
    if(TPos_Center == D129_GetTPos(numDevice))
        pos -= 250;
    else if((int)TPos_Right == D129_GetTPos(numDevice)) 
        pos -= 500;
    
    double shiftFull = 0.0;
    double shiftCur = tables.GetTShiftAbs(500 + pos, D129_GetTBase(numDevice));
    shiftFull = shiftCur + D129_GetTShiftAbs(numDevice);
    return shiftFull;
}

int SettingsDisplay::GetPosTCur(int channel, int numCur) 
{
    int pos = posCursT[channel][numCur];
    return pos;
}

int SettingsDisplay::GetPosTCurInScreen(int _channel, int _numCur) 
{
    int pos = posCursT[_channel][_numCur];
    return pos;
}