//  [4/23/2009 Sasha]
#pragma warning(disable:4127)
#include <QColorDialog>
#pragma warning(default:4127)
#include <cmath>
#include "../../common/defines.h"
#include "Display.h"
#include "SignalScreen.h"
#include "SettingsDisplay.h"

Display::Display(QFrame *_parent) : QFrame(_parent) 
{
    frame = _parent;

    signalScreen = NULL;
    signalScreen = new SignalScreen(this, &settings);
    signalScreen->move(2, 2);
    signalScreen->show();
}

Display::~Display() 
{
    SAFE_DELETE(signalScreen);
}


void Display::DrawData() 
{
    signalScreen->Draw();
}

void Display::Reset() 
{
    signalScreen->Reset();
}

void Display::Repaint() 
{
    signalScreen->Repaint();
}

void Display::paintEvent(QPaintEvent *) 
{
    signalScreen->update();
}

void Display::ConnectToDevice(bool _connect) 
{
    signalScreen->ConnectToDevice(_connect);
}

void Display::SetActiveSpectr(int _spectr) 
{
    settings.activeSpectr = _spectr;
    Reset();
}

void Display::SetColorBackground(const QColor _color) 
{
    if(_color.isValid()) 
    {
        settings.colorBackground = _color;
        signalScreen->Repaint();
    }
}

void Display::SetColorGrid(const QColor _color) 
{
    if(_color.isValid()) 
    {
        settings.colorGrid = _color;
        signalScreen->Repaint();
    }
}

void Display::SetColorCurs(const int _numCurs, const QColor _color) 
{
    if(_color.isValid())
    {
        settings.colorCurs[_numCurs] = _color;
        signalScreen->Repaint();
    }
}

void Display::EnableUCurs(bool _enable) 
{
    settings.enableCurs = _enable;
}

void Display::SetColorSignal(int _channel, QColor _color) 
{
    if((_channel < 0) || (_channel > 1)) 
        return;
   
    if(_color.isValid()) 
    {
        settings.colorSignal[_channel] = _color;
        signalScreen->Repaint();
    }
}

