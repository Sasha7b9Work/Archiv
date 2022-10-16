#include "UCursor.h"
#pragma warning(disable:4127)
#include <QSpinBox>
#include <QCheckBox>
#include <QColorDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#pragma warning(default:4127)
#include <cmath>
#include "SettingsDisplay.h"
#include "../../common/GlobalFunctions.h"
#include "../../common/defines.h"
#include "WindowOsci.h"
#include "../../Device129Multi/src/Device129Multi.h"

using namespace NS_GlobalFunctions;

extern int numDevice;

//  [7/31/2009 Sasha]
UCursor::UCursor(WindowOsci *_w, QSpinBox *_sbCur0, QSpinBox *_sbCur1, 
                   QLineEdit *_delta, QLineEdit *_absPos0, QLineEdit *_absPos1, QCheckBox *_enable, 
                   QCheckBox *_together, QComboBox *_source, QLineEdit *_lePercent, QPushButton *_pbPercent,
                   SettingsDisplay *_settings) : 
    QObject(),
    w(_w),
    sbCursor0(_sbCur0),
    sbCursor1(_sbCur1),
    leDelta(_delta),
    leAbsPos0(_absPos0),
    leAbsPos1(_absPos1),
    chbTogether(_together),
    chbEnable(_enable),
    lePercentage(_lePercent),
    pbPercentage(_pbPercent),
    setD(_settings)
{
    this->together[0] = false;
    this->together[1] = false;
    sbCursor0->setSingleStep(1);
    sbCursor1->setSingleStep(1);
    sbCursor0->setRange(MinPos, MaxPos);
    sbCursor1->setRange(MinPos, MaxPos);
    setD->SetPosUCur(0, 0, DefaultValue);
    setD->SetPosUCur(0, 1, -DefaultValue);
    setD->SetPosUCur(1, 0, DefaultValue);
    setD->SetPosUCur(1, 1, -DefaultValue);
    sbCursor0->setValue(setD->GetPosUCur(setD->GetActiveCurs(), 0));
    sbCursor1->setValue(setD->GetPosUCur(setD->GetActiveCurs(), 1));
    connect(chbEnable, SIGNAL(stateChanged(int)), this, SLOT(OnCheckBoxEnable(int)));
    connect(sbCursor0, SIGNAL(valueChanged(int)), this, SLOT(OnChangeSpinBoxCursor0(int)));
    connect(sbCursor1, SIGNAL(valueChanged(int)), this, SLOT(OnChangeSpinBoxCursor1(int)));
    connect(_source, SIGNAL(currentIndexChanged(int)), this, SLOT(OnChangeSource(int)));
    connect(chbTogether, SIGNAL(toggled(bool)), this, SLOT(OnChangeTogether(bool)));
    connect(pbPercentage, SIGNAL(pressed()), this, SLOT(OnPressPBPercentage()));
}

//  [10/24/2009 Sasha]
UCursor::~UCursor() {

}

//  [12/3/2009 Sasha]
void UCursor::ChangeRangeOrRShift() {
    WriteAbsPositions();
    pbPercentage->click();
}

//  [10/24/2009 Sasha]
void UCursor::OnCheckBoxEnable(int _enable) {
    setD->enableCursU[setD->GetActiveCurs()] = (_enable == Qt::Checked);
    bool state = (_enable == Qt::Checked);
    leDelta->setEnabled(state);
    leAbsPos0->setEnabled(state);
    leAbsPos1->setEnabled(state);
    sbCursor0->setEnabled(state);
    sbCursor1->setEnabled(state);
    lePercentage->setEnabled(state);
    pbPercentage->setEnabled(state);
    pbPercentage->click();
    emit Repaint();
}

//  [10/24/2009 Sasha]
void UCursor::OnChangeSpinBoxCursor0(int _pos) {
    int activeCurs = setD->GetActiveCurs();
    int prevPos0 = setD->GetPosUCur(activeCurs, 0);
    int pos0 = _pos;

    if(chbTogether->isChecked()) {
        int pos1 = setD->GetPosUCur(activeCurs, 1);
        int delta = pos0 - prevPos0;
        pos1 += delta;
        chbTogether->setCheckState(Qt::Unchecked);
        sbCursor1->setValue(pos1);
        chbTogether->setCheckState(Qt::Checked);
        setD->SetPosUCur(activeCurs, 1, pos1);
    }

    setD->SetPosUCur(activeCurs, 0, pos0);

    WriteAbsPositions();
    WritePercentage();

    SelectLineEdit(leAbsPos0);

    emit Repaint();
}

//  [10/24/2009 Sasha]
void UCursor::OnChangeSpinBoxCursor1(int _pos) {
    int activeCurs = setD->GetActiveCurs();
    int prevPos1 = setD->GetPosUCur(activeCurs, 1);
    int pos1 = _pos;
    int pos0 = setD->GetPosUCur(activeCurs, 0);

    if(chbTogether->isChecked()) {
        int delta = pos1 - prevPos1;
        pos0 += delta;
        chbTogether->setCheckState(Qt::Unchecked);
        sbCursor0->setValue(pos0);
        chbTogether->setCheckState(Qt::Checked);
    }

    setD->SetPosUCur(activeCurs, 0, pos0);
    setD->SetPosUCur(activeCurs, 1, pos1);

    WriteAbsPositions();
    WritePercentage();

    SelectLineEdit(leAbsPos1);

    emit Repaint();
}

//  [11/28/2009 Sasha]
void UCursor::OnChangeSource(int _activeCurs) {
    setD->SetActiveCurs(_activeCurs);
    chbTogether->setChecked(false);
    sbCursor0->setValue(setD->GetPosUCur(_activeCurs, 0));
    sbCursor1->setValue(setD->GetPosUCur(_activeCurs, 1));
    chbEnable->setChecked(setD->enableCursU[_activeCurs]);
    chbTogether->setChecked(together[_activeCurs]);
    pbPercentage->click();
    SelectLineEdit();
}

//  [11/28/2009 Sasha]
void UCursor::OnChangeTogether(bool _together) {
    together[setD->GetActiveCurs()] = _together;
}

void UCursor::OnPressPBPercentage() 
{
    int channel = setD->GetActiveCurs();
    voltage100Percentage[channel][0] = setD->GetPosUCurAbs(channel, 0) - D129_GetRShiftAbs(numDevice, channel);
    voltage100Percentage[channel][1] = setD->GetPosUCurAbs(channel, 1) - D129_GetRShiftAbs(numDevice, channel);
    WritePercentage();
}

void UCursor::WriteAbsPositions() 
{
    int activeCurs = setD->GetActiveCurs();
    double pos0 = setD->GetPosUCurAbs(activeCurs, 0) - D129_GetRShiftAbs(numDevice, activeCurs);
    double pos1 = setD->GetPosUCurAbs(activeCurs, 1) - D129_GetRShiftAbs(numDevice, activeCurs);
    leDelta->setText(VoltageToString(fabs(pos1 - pos0)));
    leAbsPos0->setText(VoltageToString(pos0));
    leAbsPos1->setText(VoltageToString(pos1));
}

void UCursor::WritePercentage() 
{
    int channel = setD->GetActiveCurs();
    double delta100Percent = fabs(voltage100Percentage[channel][0] - voltage100Percentage[channel][1]);
    double pos0 = setD->GetPosUCurAbs(channel, 0) - D129_GetRShiftAbs(numDevice, channel);
    double pos1 = setD->GetPosUCurAbs(channel, 1) - D129_GetRShiftAbs(numDevice, channel);
    double deltaPercent = fabs(pos0 - pos1);
    double percent = deltaPercent / delta100Percent * 100;
    lePercentage->setText(DoubleToString(percent));
}

//  [2/26/2010 Sasha]
void UCursor::SelectLineEdit(QLineEdit *_le /* = NULL */) {
    leAbsPos0->setSelection(0, 0);
    leAbsPos1->setSelection(0, 0);
    w->leTCur1Abs->setSelection(0, 0);
    w->leTCur2Abs->setSelection(0, 0);
    if(_le != NULL) {
        _le->selectAll();
    }
}