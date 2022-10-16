//  [9/28/2009 Sasha]
#include "Tuner.h"
#include <cmath>
#include "WindowOsci.h"
#include "SettingsDisplay.h"
#include "Display.h"
#include "UCursor.h"
#include "TCursor.h"
#include "Slider.h"
#include "../../common/GlobalFunctions.h"
#include "RShiftCursorScreen.h"
#include "TrigCursorScreen.h"
#include "SignalScreen.h"
#include "../../common/TypesAltera.h"
#include "../../common/MathDisplayOscilloscope.h"
#include "../../Device129Multi/src/Device129Multi.h"
#include "../../Device129Multi/src/TypesMath.h"
#include "../../common/defines.h"

using namespace NS_TypesAltera;
using namespace NS_GlobalFunctions;
using namespace NS_TypesDisplay;
using namespace NS_TypesMath;

extern MathDisplayOscilloscope mathDisplay;

extern int numDevice;

Tuner::Tuner(WindowOsci *w) : 
    QObject()
{
    this->w = w;
    sbUCur1 = w->sbUCur1;
    sbUCur2 = w->sbUCur2;
    leDeltaUAbs = w->leDeltaUAbs;
    leUCur1Abs = w->leUCur1Abs;
    leUCur2Abs = w->leUCur2Abs;
    chbUCurs = w->chbUCurs;
    chbUCursTogether = w->chbUCursTogether;
    sbTCur1 = w->sbTCur1;
    sbTCur2 = w->sbTCur2;
    leDeltaTAbs = w->leDeltaTAbs;
    leTCur1Abs = w->leTCur1Abs;
    leTCur2Abs = w->leTCur2Abs;
    chbTCurs = w->chbTCurs;
    chbTCursTogether = w->chbTCursTogether;
    cbSourceCurs = w->cbSourceCurs;
    display = w->display;
    leRShift0Abs = w->leRShift0Abs;
    leRShift1Abs = w->leRShift1Abs;
    cbRangeChannel0 = w->cbRangeChannel0;
    cbRangeChannel1 = w->cbRangeChannel1;
    vsRShift0 = w->vsRShift0;
    vsRShift1 = w->vsRShift1;
    rbTrigCh0 = w->rbTrigCh0;
    vsTrigLev = w->vsTrigLev;
    leTrigLevAbs = w->leTrigLevAbs;
    rbTrigCh1 = w->rbTrigCh1;
    hsTShift = w->hsTShift;
    leTShiftAbs = w->leTShiftAbs;
    sbTShift = w->sbTShift;
    pbStart = w->pbStart;
    cbSpectr = w->cbSpectr;
    chbEnableMath = w->chbEnableMath;
    cbMathFunc = w->cbMathFunc;
    chbEnableSpectr = w->chbEnableSpectr;
    sbAccumulation = w->sbAccumulation;
    chbDisableAccumulationLimit = w->chbDisableAccumulationLimit;
    pbResetAccumulation = w->pbResetAccumulation;

    uCurs = new UCursor(w, sbUCur1, sbUCur2, leDeltaUAbs, leUCur1Abs, leUCur2Abs, chbUCurs, chbUCursTogether, cbSourceCurs, w->lePercentage, w->pbPercentage, w->display->GetPointerSettings());
    tCurs = new TCursor(w, sbTCur1, sbTCur2, leDeltaTAbs, leTCur1Abs, leTCur2Abs, chbTCurs, chbTCursTogether, cbSourceCurs, w->display->GetPointerSettings());

    // Çäåñü óïðàâëåíèå ìûøüþ
    connect(display->signalScreen, SIGNAL(ChangeRShift0(int)), this, SLOT(ChangeRShift0Display(int)));
    connect(display->signalScreen, SIGNAL(ChangeRShift1(int)), this, SLOT(ChangeRShift1Display(int)));
    connect(display->signalScreen, SIGNAL(ChangeTShift(int)), this, SLOT(ChangeTShiftDisplay(int)));
    connect(display->signalScreen, SIGNAL(ChangePosUCur(int, int, int)), this, SLOT(ChangePosUCur(int, int, int)));
    connect(display->signalScreen, SIGNAL(ChangePosTCur(int, int, int)), this, SLOT(ChangePosTCur(int, int, int)));

    connect(uCurs, SIGNAL(Repaint()), display, SLOT(Repaint()));
    connect(tCurs, SIGNAL(Repaint()), display, SLOT(Repaint()));
}

Tuner::~Tuner() 
{
    SAFE_DELETE(uCurs);
}

void Tuner::SetRShift0(int _rShift) 
{
    display->signalScreen->Clear();
    D129_SetRShiftRel(numDevice, 0, _rShift);
    leRShift0Abs->setText(VoltageToString(D129_GetRShiftAbs(numDevice, 0)));
    SelectLineEdit(leRShift0Abs);
    w->sbRShift0->setFocus();
    uCurs->ChangeRangeOrRShift();
    display->rShiftScreen->update();
    display->trigLevScreen->update();
}

void Tuner::SetRange0(int _range) 
{
    D129_SetRange(numDevice, 0, _range);
    vsRShift0->SetValue(D129_GetRShiftRel(numDevice, 0));
    leRShift0Abs->setText(VoltageToString(D129_GetRShiftAbs(numDevice, 0)));
    if(rbTrigCh0->isChecked()) 
    {
        vsTrigLev->SetValue(D129_GetTrigLevRel(numDevice, 0));
        leTrigLevAbs->setText(VoltageToString(D129_GetTrigLevAbs(numDevice, 0)));
    }
    uCurs->ChangeRangeOrRShift();
    display->rShiftScreen->update();
    display->trigLevScreen->update();
    display->signalScreen->Clear();
    cbRangeChannel0->setFocus();
    Reset();
}

void Tuner::ResetRShift0() 
{
    vsRShift0->setValue(RShiftZero);
}

void Tuner::SetRange1(int _range) 
{
    D129_SetRange(numDevice, 1, _range);
    vsRShift1->SetValue(D129_GetRShiftRel(numDevice, 1));
    leRShift1Abs->setText(VoltageToString(D129_GetRShiftAbs(numDevice, 1)));
    if(rbTrigCh1->isChecked()) 
    {
        vsTrigLev->SetValue(D129_GetTrigLevRel(numDevice, 1));
        leTrigLevAbs->setText(VoltageToString(D129_GetTrigLevAbs(numDevice, 1)));
    }
    uCurs->ChangeRangeOrRShift();
    display->rShiftScreen->update();
    display->trigLevScreen->update();
    display->signalScreen->Clear();
    Reset();
    cbRangeChannel1->setFocus();
}

void Tuner::SetRShift1(int _rShift) 
{
    D129_SetRShiftRel(numDevice, 1, _rShift);
    leRShift1Abs->setText(VoltageToString(D129_GetRShiftAbs(numDevice, 1)));
    SelectLineEdit(leRShift1Abs);
    w->sbRShift1->setFocus();
    uCurs->ChangeRangeOrRShift();
    display->rShiftScreen->update();
    display->trigLevScreen->update();
    display->signalScreen->Clear();
}

void Tuner::ResetRShift1() 
{
    vsRShift1->setValue(RShiftZero);
}

void Tuner::SetTrigLev(int _trigLev) 
{
    display->signalScreen->Clear();
    D129_SetTrigLevRel(numDevice, D129_GetSinchroChannel(numDevice), _trigLev);
    leTrigLevAbs->setText(VoltageToString(D129_GetTrigLevAbs(numDevice, D129_GetSinchroChannel(numDevice))));
    SelectLineEdit(leTrigLevAbs);
    w->sbTrigLev->setFocus();
    display->trigLevScreen->update();
}

void Tuner::ResetTrigLev() 
{
    vsTrigLev->setValue(TrigLevZero);
}

void Tuner::SetTBase(int _tBase) 
{
    D129_SetTBase(numDevice, _tBase);
    hsTShift->setValue(D129_GetTShiftRel(numDevice));
    leTShiftAbs->setText(TimeToString(D129_GetTShiftAbs(numDevice)));
    tCurs->ChangedTBaseOrTPosOrTShift();
    w->chbEnableAverage->setEnabled(!(_tBase < (int)TBase_1us));
    w->sbNumAverage->setEnabled(!(_tBase < (int)TBase_1us));
    Reset();

    if(!w->timer)
        return;

    if(_tBase < TBase_1us)
        w->timer->setInterval(1);
    else
        w->timer->setInterval(25);

    w->cbTBase->setFocus();
}

void Tuner::SetTShift(int _tShift) 
{
    D129_SetTShiftRel(numDevice, _tShift);
    leTShiftAbs->setText(TimeToString(D129_GetTShiftAbs(numDevice)));
    SelectLineEdit(leTShiftAbs);
    w->sbTShift->setFocus();
    tCurs->ChangedTBaseOrTPosOrTShift();
    Reset();
}

void Tuner::ResetTShift() 
{
    hsTShift->setValue(TShiftZero);
}

void Tuner::SetTShift0(int _tPos) 
{
    static int tShiftMin[3] = {0, 250, 500};
    D129_SetTPos(numDevice, (TPos)_tPos);
    sbTShift->setRange(tShiftMin[_tPos], TShiftMax);
    hsTShift->setRange(tShiftMin[_tPos], TShiftMax);
    tCurs->ChangedTBaseOrTPosOrTShift();
    Reset();
    w->cbTPos->setFocus();
}

void Tuner::SetSinchroChannel(bool _one) 
{
    static int trigLev1 = TrigLevZero;
    static int trigLev2 = TrigLevZero;
    D129_SetSinchroChannel(numDevice, (_one) ? 0 : 1);
    if(_one) 
    {
        trigLev2 = vsTrigLev->value();
        vsTrigLev->setValue(trigLev1);
    }
    else 
    {
        trigLev1 = vsTrigLev->value();
        vsTrigLev->setValue(trigLev2);
    }
    Reset();
}

void Tuner::SetSinchroType(bool _b) 
{
    D129_SetSinchroType(numDevice, (_b) ? Sinchro_Back : Sinchro_Front);
    Reset();
}

void Tuner::SetStartMode(int _startMode) 
{
    D129_SetStartMode(numDevice, (StartMode)_startMode);
    if(_startMode == StartMode_Single) 
        pbStart->setChecked(false);
    pbStart->setChecked((_startMode != StartMode_Single));
    SetTBase(w->cbTBase->currentIndex());
    Reset();
}

void Tuner::SetAC0() 
{
    D129_SetModeInput(numDevice, 0, ModeAC);
    display->signalScreen->Clear();
}

void Tuner::SetAC1() 
{
    D129_SetModeInput(numDevice, 1, ModeAC);
    display->signalScreen->Clear();
}

void Tuner::SetDC0() 
{
    D129_SetModeInput(numDevice, 0, ModeDC);
    display->signalScreen->Clear();
}

void Tuner::SetDC1() 
{
    D129_SetModeInput(numDevice, 1, ModeDC);
    display->signalScreen->Clear();
}

void Tuner::SetGround0() 
{
    D129_SetModeInput(numDevice, 0, ModeGRND);
    display->signalScreen->Clear();
}

void Tuner::SetGround1() 
{
    D129_SetModeInput(numDevice, 1, ModeGRND);
    display->signalScreen->Clear();
}

void Tuner::SetFiltr(bool _filtr) 
{
    D129_SetFiltr(numDevice, _filtr);
    if(_filtr) 
        w->pbFiltrSinchro->setText(tr("Í×"));
    else 
        w->pbFiltrSinchro->setText(tr("ÏÑ"));
}

void Tuner::EnableSpectr(int _state) 
{
    switch(_state) 
    {
        case Qt::Checked:
            display->SetTypeDisplay(TypeDisplay_Separetly);
            D129_EnableSpectr(numDevice, cbSpectr->currentIndex(), true);
            chbEnableMath->setEnabled(false);
            vsTrigLev->setSingleStep(8);
            vsTrigLev->SetValue(vsTrigLev->value());
            vsRShift0->setSingleStep(8);
            vsRShift0->SetValue(vsRShift0->value());
            vsRShift1->setSingleStep(8);
            vsRShift1->SetValue(vsRShift1->value());
            break;

        case Qt::Unchecked:
            display->SetTypeDisplay(TypeDisplay_Together);
            D129_EnableSpectr(numDevice, cbSpectr->currentIndex(), false);
            chbEnableMath->setEnabled(true);
            vsTrigLev->setSingleStep(4);
            vsRShift0->setSingleStep(4);
            vsRShift1->setSingleStep(4);
            break;

        default:
            break;
    }
}

void Tuner::ChangeChannelSpectr(int _channel) 
{
    D129_EnableSpectr(numDevice, _channel, true);
}

void Tuner::EnableCh0(bool en) 
{
    if(en)
        D129_SetInvert(numDevice, 0, false); 
    display->Reset();
}

void Tuner::EnableCh1(bool en) 
{
    if(en)
        D129_SetInvert(numDevice, 1, false); 
    display->Reset();
}

void Tuner::DisableCh0(bool _dis) 
{
    display->SetEnableChannel(0, !_dis);
    display->Reset();
}

void Tuner::DisableCh1(bool _dis) 
{
    display->SetEnableChannel(1, !_dis);
    display->Reset();
}

void Tuner::InvertCh0(bool inv) 
{
    if(inv)
        D129_SetInvert(numDevice, 0, true); 
    display->Reset();
}

void Tuner::InvertCh1(bool inv) 
{
    if(inv)
        D129_SetInvert(numDevice, 1, true); 
    display->Reset();
}

void Tuner::EnableMath(int state) 
{
    switch(state) 
    {
        case Qt::Checked:
            display->SetTypeDisplay(TypeDisplay_Separetly);
            D129_SetMathFunction(numDevice, (cbMathFunc->currentIndex() == 0) ? Function_Addition : Function_Multiplication, true);
            chbEnableSpectr->setEnabled(false);
            vsTrigLev->setSingleStep(8);
            vsTrigLev->SetValue(vsTrigLev->value());
            vsRShift0->setSingleStep(8);
            vsRShift0->SetValue(vsRShift0->value());
            vsRShift1->setSingleStep(8);
            vsRShift1->SetValue(vsRShift1->value());
            break;

        case Qt::Unchecked:
            display->SetTypeDisplay(TypeDisplay_Together);
            D129_SetMathFunction(numDevice, (cbMathFunc->currentIndex() == 0) ? Function_Addition : Function_Multiplication, false);
            chbEnableSpectr->setEnabled(true);
            vsTrigLev->setSingleStep(4);
            vsRShift0->setSingleStep(4);
            vsRShift1->setSingleStep(4);
            break;

        default:
            break;
    }
}

void Tuner::ChangeMathFunction(int state) 
{
    if(!chbEnableMath->isChecked()) 
        return;
    D129_SetMathFunction(numDevice, (state == 0) ? Function_Addition : Function_Multiplication, true);
}

void Tuner::EnableAccumulation(int state) 
{
    bool en = (state == Qt::Checked) ? true : false;
    sbAccumulation->setEnabled(en);
    chbDisableAccumulationLimit->setEnabled(en);
    pbResetAccumulation->setEnabled(en);

    if(state == Qt::Checked) {
        if(chbDisableAccumulationLimit->isChecked())
            display->SetModeAccumulation(ModeAccumulation_NoLimitation);
        else 
            display->SetModeAccumulation(ModeAccumulation_Limitation);
    }
    else 
        display->SetModeAccumulation(ModeAccumulation_Disable);
    w->leAccumulation->setText(QString(tr("%1")).arg(w->sbAccumulation->value(), 0, 10));
}

void Tuner::SetCountAccumulation(int accum) 
{
    int counAccumulation = pow(2.0, accum);
    w->leAccumulation->setText(QString(tr("%1")).arg(counAccumulation, 0, 10));
    display->SetCountAccumulation(counAccumulation);
}

void Tuner::DisableAccumulationLimit(int state) 
{
    if(state == Qt::Checked) 
        display->SetModeAccumulation(ModeAccumulation_NoLimitation);
    else
        display->SetModeAccumulation(ModeAccumulation_Limitation);
}

void Tuner::SetModePalm0(bool palm) 
{
    D129_SetModePalm(numDevice, 0, palm); 
    w->pbFiltrPalm0->setText(palm ? tr("Í×") : tr("ÏÑ"));
    Reset();
}

void Tuner::SetModePalm1(bool palm) 
{
    D129_SetModePalm(numDevice, 1, palm);
    w->pbFiltrPalm1->setText(palm ? tr("Í×") : tr("ÏÑ"));
    Reset();
}

void Tuner::SetPointsPalm(int points) 
{
    D129_SetPointsPalm(numDevice, points);
    Reset();
}

void Tuner::SetModeAverage(int _average) 
{
    D129_SetModeAverage(numDevice, Qt::Checked == _average);
    D129_SetNumberAverage(numDevice, w->sbNumAverage->value());
    Reset();
}

void Tuner::SetNumberAverage(int _number) 
{
    D129_SetNumberAverage(numDevice, _number);
    Reset();
}

void Tuner::ChangeTShiftDisplay(int delta) 
{
    hsTShift->setValue(hsTShift->value() + delta);
}

void Tuner::ChangeRShift0Display(int delta) 
{
    vsRShift0->SetValue(vsRShift0->value() + delta * 4);
}

void Tuner::ChangeRShift1Display(int delta) 
{
    vsRShift1->SetValue(vsRShift1->value() + delta * 4);
}

void Tuner::ChangePosUCur(int /*_channel*/, int _numCur, int _deltaY) 
{
    if(display->settings.GetTypeDisplay() == TypeDisplay_Separetly)
        _deltaY *= 2;
   
    switch(_numCur) 
    {
        case 0:
            sbUCur1->setValue(sbUCur1->value() + _deltaY);
            break;
        case 1:
            sbUCur2->setValue(sbUCur2->value() + _deltaY);
            break;
        default:
            //ERROR_LOG("Error in input parameter: channel = %d", _channel);
            break;
    }
}

void Tuner::ChangePosTCur(int /*_channel*/, int _numCur, int _deltaX) 
{
    switch(_numCur) 
    {
        case 0:
            sbTCur1->setValue(sbTCur1->value() - _deltaX);
            break;
        case 1:
            sbTCur2->setValue(sbTCur2->value() - _deltaX);
            break;
        default:
            //ERROR_LOG("Error in input parameter: channel = %d", _channel);
            break;
    }
}

void Tuner::Reset() 
{
    display->Reset();
}

void Tuner::SelectLineEdit(QLineEdit *le) 
{
    leTrigLevAbs->setSelection(0, 0);
    leRShift0Abs->setSelection(0, 0);
    leRShift1Abs->setSelection(0, 0);
    leTShiftAbs->setSelection(0, 0);
    leTCur1Abs->setSelection(0, 0);
    leTCur2Abs->setSelection(0, 0);
    leUCur1Abs->setSelection(0, 0);
    leUCur2Abs->setSelection(0, 0);
    if(le != NULL) {
        le->selectAll();
    }
}
