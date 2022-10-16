//  [4/23/2009 Sasha]
#include "Display.h"
#pragma warning(disable:4127)
#include <QColorDialog>
#pragma warning(default:4127)
#include <QPainter>
#include <cmath>
#include "../../common/defines.h"
#include "RShiftCursorScreen.h"
#include "TrigCursorScreen.h"
#include "TimeCursorScreen.h"
#include "SignalScreen.h"
#include "SettingsDisplay.h"

using namespace NS_TypesDisplay;

//  [4/23/2009 Sasha]
Display::Display(QFrame *_parent) : QFrame(_parent) 
{

    frame = _parent;
    rShiftScreen = NULL;
    rShiftScreen = new RShiftCursorScreen(this, &settings);
    rShiftScreen->move(SCR_W_TR_LEV + SCR_W + 2, SCR_H_TSHIFT + 2);
    rShiftScreen->show();

    trigLevScreen = NULL;
    trigLevScreen = new TrigCursorScreen(this, &settings);
    trigLevScreen->move(2, SCR_H_TSHIFT + 2);
    trigLevScreen->show();

    timeScreen = NULL;
    timeScreen = new TimeCursorScreen(this, &settings);
    timeScreen->move(2, 2);
    timeScreen->show();

    signalScreen = NULL;
    signalScreen = new SignalScreen(this, &settings);
    signalScreen->move(2 + SCR_W_TR_LEV, 2 + SCR_H_TSHIFT);
    signalScreen->show();

    SetTypeDisplay(TypeDisplay_Together);
}

Display::~Display() {
    SAFE_DELETE(rShiftScreen);
    SAFE_DELETE(trigLevScreen);
    SAFE_DELETE(timeScreen);
    SAFE_DELETE(signalScreen);
}

void Display::DrawData() 
{
    signalScreen->Draw();
}

void Display::DrawPoints(int _channel, uchar _buffer[501])
{
    signalScreen->DrawPoints(_channel, _buffer);
}

QImage Display::GetImage() 
{
    QImage imageTrigScreen = trigLevScreen->GetImage();
    QImage imageTimeScreen = timeScreen->GetImage();
    QImage imageRShiftScreen = rShiftScreen->GetImage();
    QImage imageScreen = signalScreen->GetImage();
    int sizeX = imageTimeScreen.width();
    int sizeY = imageTimeScreen.height() + imageScreen.height();
    QImage image(sizeX, sizeY, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    painter.drawImage(0, 0, imageTimeScreen);
    painter.drawImage(0, imageTimeScreen.height(), imageTrigScreen);
    painter.drawImage(imageTrigScreen.width(), imageTimeScreen.height(), imageScreen);
    painter.drawImage(imageTrigScreen.width() + imageScreen.width(), imageTimeScreen.height(), imageRShiftScreen);
    return image;
}

//  [7/21/2009 Sasha]
void Display::Reset() {
    signalScreen->Reset();
    trigLevScreen->Reset();
    rShiftScreen->Reset();
    timeScreen->Reset();
}

//  [1/28/2010 Sasha]
void Display::ConnectToDevice(bool _connect) {
    signalScreen->ConnectToDevice(_connect);
}

//  [9/29/2009 Sasha]
void Display::SetEnableChannel(int numChannel, bool enable) {
    signalScreen->SetEnableChannel(numChannel, enable);
    signalScreen->Repaint();
}

//  [7/13/2009 Administrator]
void Display::SetColorBackground(const QColor color) {
    settings.colorBackground = color;
    signalScreen->Repaint();
}

//  [7/13/2009 Administrator]
void Display::SetColorGrid(const QColor color) {
    settings.colorGrid = color;
    signalScreen->Repaint();
}

//  [7/13/2009 Administrator]
void Display::SetColorSignal(const int channel, const QColor color) {
    settings.colorSignal[channel] = color;
    signalScreen->Repaint();
}

//  [9/30/2009 Sasha]
void Display::SetColorCurs(const int numCurs, const QColor color) {
    settings.colorCurs[numCurs] = color;
    signalScreen->Repaint();
}

//  [7/16/2009 Sasha]
void Display::SetColorTPos(const QColor color) {
    settings.colorTShift0 = color;
    signalScreen->Repaint();
}

//  [7/16/2009 Sasha]
void Display::SetColorTShift(const QColor color) {
    settings.colorTShift = color;
    signalScreen->Repaint();
}

//  [7/13/2009 Administrator]
void Display::SetTypeDisplay(const TypeDisplay type) {

    if(settings.typeDisplay == type) {
        return;
    }
    settings.typeDisplay = type;

    int size_x = SCR_W + 4 + SCR_W_TR_LEV + SCR_W_RSHIFT;
    int size_y = ((type == TypeDisplay_Together) ? SCR_BG_H : SCR_BG_H + 1) + 4 + SCR_H_TSHIFT;
    frame->resize(size_x, size_y);
    update();
    Reset();
}

//  [7/13/2009 Sasha]
void Display::SetColorBackground() {
    QColor colorBackground = QColorDialog::getColor();
	if(colorBackground.isValid()) {
		SetColorBackground(colorBackground);
	}
}

//  [7/13/2009 Sasha]
void Display::SetColorGrid() {
    QColor colorGrid = QColorDialog::getColor();
	if(colorGrid.isValid()) {
		SetColorGrid(colorGrid);
	}
}

//  [7/16/2009 Sasha]
void Display::SetColorTPos() {
    QColor colorTPos = QColorDialog::getColor();
	if(colorTPos.isValid()) {
		SetColorTPos((QColor)colorTPos);
	}
}

//  [7/16/2009 Sasha]
void Display::SetColorTShift() {
    QColor colorTShift = QColorDialog::getColor();
	if(colorTShift.isValid()) {
		SetColorTShift((QColor)colorTShift);
	}
}

//  [7/13/2009 Sasha]
void Display::SetColorSignal0() {
    QColor colorSignal0 = QColorDialog::getColor();
	if(colorSignal0.isValid()) {
        SetColorSignal(0, colorSignal0);
	}
}

//  [7/13/2009 Sasha]
void Display::SetColorSignal1() {
    QColor colorSignal1 = QColorDialog::getColor();
	if(colorSignal1.isValid()) {
		SetColorSignal(1, colorSignal1);
	}
}

//  [9/30/2009 Sasha]
void Display::SetColorMath() {
    QColor colorMath = QColorDialog::getColor();
    if(colorMath.isValid()) {
        SetColorSignal(2, colorMath);
    }
}

//  [9/30/2009 Sasha]
void Display::SetColorCurs0() {
    QColor color = QColorDialog::getColor();
    if(color.isValid()) {
        SetColorCurs(0, color);
    }
}

//  [9/30/2009 Sasha]
void Display::SetColorCurs1() {
    QColor color = QColorDialog::getColor();
    if(color.isValid()) {
        SetColorCurs(1, color);
    }
}

//  [9/30/2009 Sasha]
void Display::SetColorCursMath() {
    QColor color = QColorDialog::getColor();
    if(color.isValid()) {
        SetColorCurs(2, color);
    }
}

//  [1/20/2010 Sasha]
void Display::SetTypeGridBase() {
    settings.typeGrid = TypeGrid_Base;
    signalScreen->Repaint();
}

//  [1/20/2010 Sasha]
void Display::SetTypeGridSimple() {
    settings.typeGrid = TypeGrid_Simple;
    signalScreen->Repaint();
}

//  [1/20/2010 Sasha]
void Display::SetTypeGridNone() {
    settings.typeGrid = TypeGrid_None;
    signalScreen->Repaint();
}

//  [7/29/2009 Sasha]
void Display::SetModeAccumulation(ModeAccumulation mode) {
    settings.modeAccumulation = mode;
}

//  [7/29/2009 Sasha]
void Display::SetCountAccumulation(int count) {
    settings.numberAccumulation = count;
}

//  [7/31/2009 Sasha]
void Display::Repaint() {
    signalScreen->update();
}

//  [7/30/2009 Sasha]
void Display::ResetAccumulation() {
    signalScreen->ResetAccumulation();
    
}

//  [7/21/2009 Sasha]
void Display::SetDrawMode(int drawMode) {
    settings.drawMode = (DrawMode)drawMode;
    Repaint();
}

//  [7/29/2009 Sasha]
void Display::EnableAntialiasing(bool enable) {
    settings.antialiasing = enable;
}