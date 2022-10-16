//  [7/29/2009 Sasha]
#include "TCursor.h"
#pragma warning(disable:4127)
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#pragma warning(default:4127)
#include <cmath>
#include "../../common/GlobalFunctions.h"
#include "SettingsDisplay.h"
#include "../../common/defines.h"
#include "WindowOsci.h"

using NS_GlobalFunctions::TimeToString;

//  [7/31/2009 Sasha]
TCursor::TCursor(WindowOsci *_w, QSpinBox *_sbCur0, QSpinBox *_sbCur1, QLineEdit *_delta, QLineEdit *_absPos0, QLineEdit *_absPos1, 
                 QCheckBox *_enable, QCheckBox *_together, QComboBox *_source, SettingsDisplay *_setD) :
    QObject(),
    w(_w),
    sbCursor0(_sbCur0),
    sbCursor1(_sbCur1),
    leDelta(_delta),
    leAbsPos0(_absPos0),
    leAbsPos1(_absPos1),
    chbTogether(_together),
    chbEnable(_enable),
    setD(_setD)
{
    together[0] = together[1] = false;
    sbCursor0->setSingleStep(1);
    sbCursor1->setSingleStep(1);
    sbCursor0->setRange(MinPos, MaxPos);
    sbCursor1->setRange(MinPos, MaxPos);
    setD->SetPosTCur(0, 0, DefaultValue1);
    setD->SetPosTCur(0, 1, DefaultValue2);
    setD->SetPosTCur(1, 0, DefaultValue1);
    setD->SetPosTCur(1, 1, DefaultValue2);
    sbCursor0->setValue(setD->GetPosTCur(setD->GetActiveCurs(), 0));
    sbCursor1->setValue(setD->GetPosTCur(setD->GetActiveCurs(), 1));
    connect(chbEnable, SIGNAL(stateChanged(int)), this, SLOT(OnCheckBoxEnable(int)));
    connect(sbCursor0, SIGNAL(valueChanged(int)), this, SLOT(OnChangeSpinBoxCursor0(int)));
    connect(sbCursor1, SIGNAL(valueChanged(int)), this, SLOT(OnChangeSpinBoxCursor1(int)));
    connect(chbTogether, SIGNAL(toggled(bool)), this, SLOT(OnChangeTogether(bool)));
    connect(_source, SIGNAL(currentIndexChanged(int)), this, SLOT(OnChangeSource(int)));
}

//  [10/26/2009 Sasha]
TCursor::~TCursor() {

}

//  [10/26/2009 Sasha]
void TCursor::OnCheckBoxEnable(int _enable) {
    setD->enableCursT[setD->GetActiveCurs()] = (Qt::Checked == _enable);
    bool state = (_enable == Qt::Checked);
    leDelta->setEnabled(state);
    leAbsPos0->setEnabled(state);
    leAbsPos1->setEnabled(state);
    sbCursor0->setEnabled(state);
    sbCursor1->setEnabled(state);
    emit Repaint();
}

//  [10/26/2009 Sasha]
void TCursor::OnChangeSpinBoxCursor0(int _pos) {
    int channel = setD->GetActiveCurs();
    int prevPos0 = setD->GetPosTCur(channel, 0);
    int pos0 = _pos;
    if(chbTogether->isChecked()) {
        int pos1 = setD->GetPosTCur(channel, 1);
        int delta = pos0 - prevPos0;
        pos1 += delta;
        chbTogether->setCheckState(Qt::Unchecked);
        sbCursor1->setValue(pos1);
        chbTogether->setCheckState(Qt::Checked);
        setD->SetPosTCur(channel, 1, pos1);
    }
    setD->SetPosTCur(channel, 0, pos0);
    WriteAbsPositions();
    SelectLineEdit(leAbsPos0);
    emit Repaint();
}

//  [10/26/2009 Sasha]
void TCursor::OnChangeSpinBoxCursor1(int _pos) {
    int channel = setD->GetActiveCurs();
    int prevPos1 = setD->GetPosTCur(channel, 1);
    int pos1 = _pos;
    if(chbTogether->isChecked()) {
        int pos0 = setD->GetPosTCur(channel, 0);
        int delta = pos1 - prevPos1;
        pos0 += delta;
        chbTogether->setCheckState(Qt::Unchecked);
        sbCursor0->setValue(pos0);
        chbTogether->setCheckState(Qt::Checked);
        setD->SetPosTCur(channel, 0, pos0);
    }
    setD->SetPosTCur(channel, 1, pos1);
    WriteAbsPositions();
    SelectLineEdit(leAbsPos1);
    emit Repaint();
}

//  [11/28/2009 Sasha]
void TCursor::OnChangeSource(int _source) {
    int activeCurs = setD->GetActiveCurs();
    setD->SetActiveCurs(_source);
    chbTogether->setChecked(false);
    int pos0 = setD->GetPosTCur(activeCurs, 0);
    int pos1 = setD->GetPosTCur(activeCurs, 1);
    sbCursor0->setValue(pos0);
    sbCursor1->setValue(pos1);
    chbEnable->setChecked(setD->enableCursT[activeCurs]);
    chbTogether->setChecked(together[activeCurs]);
    SelectLineEdit();
}

//  [11/28/2009 Sasha]
void TCursor::OnChangeTogether(bool _together) {
    together[setD->GetActiveCurs()] = _together;
}

//  [10/26/2009 Sasha]
void TCursor::ChangedTBaseOrTPosOrTShift() {
    WriteAbsPositions();
}

//  [10/26/2009 Sasha]
void TCursor::WriteAbsPositions() {
    int activeCurs = setD->GetActiveCurs();
    double pos0 = setD->GetPosTCurAbs(activeCurs, 0);
    double pos1 = setD->GetPosTCurAbs(activeCurs, 1);
    leDelta->setText(TimeToString(fabs(pos1 - pos0)));
    leAbsPos0->setText(TimeToString(pos0));
    leAbsPos1->setText(TimeToString(pos1));
}

//  [2/26/2010 Sasha]
void TCursor::SelectLineEdit(QLineEdit *_le /* = NULL */) {
    leAbsPos0->setSelection(0, 0);
    leAbsPos1->setSelection(0, 0);
    w->leUCur1Abs->setSelection(0, 0);
    w->leUCur2Abs->setSelection(0, 0);
    if(_le != NULL) {
        _le->selectAll();
    }
}