//  [11/30/2009 Sasha]
#pragma warning(disable:4127)
#include <QSpinBox>
#include <QLineEdit>
#pragma warning(default:4127)
#include <cmath>
#include "Cursors.h"
#include "SettingsDisplay.h"
#include "../../common/GlobalFunctions.h"
#include "MathDisplayAnalyzer.h"
#include "../common/TablesOfConverting.h"
#include "../Device129Multi/src/Device129Multi.h"
#include "../common/TypesAltera.h"

using NS_GlobalFunctions::FrequencyToString;
using NS_GlobalFunctions::VoltageToString;
using namespace NS_TypesAltera;

extern TablesOfConverting tables;

extern int numDevice;

// [11/30/2009 Sasha]
Cursor::Cursor(QLineEdit *_freq0, QSpinBox *_pos0, QLineEdit *_freq1, QSpinBox *_pos1, QLineEdit *_deltaFreq, QLineEdit *_u0, QLineEdit *_u1, QLineEdit *_deltaU, SettingsDisplay *_setD, QComboBox *_cbFrequency, QComboBox *_cbRange) :
    QObject(),
    leFreq0(_freq0),
    sbPos0(_pos0),
    leFreq1(_freq1),
    sbPos1(_pos1),
    leDeltaFreq(_deltaFreq),
    leU0(_u0),
    leU1(_u1),
    leDeltaU(_deltaU),
    setD(_setD),
    cbFrequency(_cbFrequency),
    cbRange(_cbRange)
{
    ConfigurationWidgets();
    LoadDefaultSettings();
    ConnectingSlots();
}

void Cursor::SetData() 
{
    // Здесь в лайны выводится информация о частотах, на которых находятся курсоры, и величине их спектральных составляющих
    WriteLineEditsVoltage();
}

//  [11/30/2009 Sasha]
void Cursor::ChangeChannel() {
    sbPos0->setValue(setD->GetPosCur(setD->activeSpectr, 0));
    sbPos1->setValue(setD->GetPosCur(setD->activeSpectr, 1));
    UnselectAllLineEdits();
}

//  [11/30/2009 Sasha]
void Cursor::OnChangePos0(int _pos) {
    setD->SetPosCur(setD->activeSpectr, 0, _pos);
    WriteLineEditsFreq();
    WriteLineEditsVoltage();
    leFreq0->selectAll();
    sbPos0->setFocus();
    emit Repaint();
}

//  [11/30/2009 Sasha]
void Cursor::OnChangePos1(int _pos) {
    setD->SetPosCur(setD->activeSpectr, 1, _pos);
    WriteLineEditsFreq();
    WriteLineEditsVoltage();
    leFreq1->selectAll();
    sbPos1->setFocus();
    emit Repaint();
}

void Cursor::OnChangeCBFrequency(int) 
{
    WriteLineEditsFreq();
    WriteLineEditsVoltage();
    UnselectAllLineEdits();
}

void Cursor::OnChangeCBRange(int) 
{
    WriteLineEditsVoltage();
    UnselectAllLineEdits();
}

//  [11/30/2009 Sasha]
void Cursor::ConfigurationWidgets() {
    sbPos0->setSingleStep(1);
    sbPos0->setRange(MinPos, MaxPos);
    sbPos1->setSingleStep(1);
    sbPos1->setRange(MinPos, MaxPos);
}

//  [11/30/2009 Sasha]
void Cursor::LoadDefaultSettings() {
    setD->SetPosCur(0, 0, 75);
    setD->SetPosCur(0, 1, 175);
    setD->SetPosCur(1, 0, 75);
    setD->SetPosCur(1, 1, 175);
}

//  [11/30/2009 Sasha]
void Cursor::ConnectingSlots() {
    connect(sbPos0, SIGNAL(valueChanged(int)), this, SLOT(OnChangePos0(int)));
    connect(sbPos1, SIGNAL(valueChanged(int)), this, SLOT(OnChangePos1(int)));
    connect(cbFrequency, SIGNAL(currentIndexChanged(int)), this, SLOT(OnChangeCBFrequency(int)));
    connect(cbRange, SIGNAL(currentIndexChanged(int)), this, SLOT(OnChangeCBRange(int)));
}

void Cursor::WriteLineEditsFreq() 
{
    double stepF = tables.GetStepFrequency(tables.GetTBaseFromFBase(static_cast<FBase>(cbFrequency->currentIndex())));
    double freq0 = sbPos0->value() * stepF;
    double freq1 = sbPos1->value() * stepF;
    leFreq0->setText(FrequencyToString(freq0, 2));
    leFreq1->setText(FrequencyToString(freq1, 2));
    leDeltaFreq->setText(FrequencyToString(fabs(freq1 - freq0), 2));
}

void Cursor::WriteLineEditsVoltage() 
{
    static MathDisplayAnalyzer mathDisplay;
    if(D129_DataReadyForDraw(numDevice)) 
    {
        double *spectr = D129_GetPointerToDataDraw(numDevice, setD->activeSpectr);
        double volt0 = spectr[sbPos0->value()];
        if(volt0 < 1e-6) 
            volt0 = 0.0;
        
        double volt1 = spectr[sbPos1->value()];
        if(volt1 < 1e-6) 
            volt1 = 0.0;
        
        double deltaVolt = fabs(volt0 - volt1);
        double max = mathDisplay.GetMaxVoltage(cbRange->currentIndex());
        leU0->setText((volt0 <= max) ? VoltageToString(volt0) : EMPTY_STRING);
        leU1->setText((volt1 <= max) ? VoltageToString(volt1) : EMPTY_STRING);
        leDeltaU->setText(((volt0 <= max) && (volt1 <= max)) ? VoltageToString(deltaVolt) : EMPTY_STRING);
    }
    else 
    {
        leU0->setText(EMPTY_STRING);
        leU1->setText(EMPTY_STRING);
        leDeltaU->setText(EMPTY_STRING);
    }
}

//  [2/24/2010 Sasha]
void Cursor::UnselectAllLineEdits() {
    leFreq0->deselect();
    leFreq1->deselect();
}