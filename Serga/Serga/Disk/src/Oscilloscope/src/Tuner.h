//  [9/28/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QObject>
#pragma warning(default:4127)

class QSpinBox;
class QLineEdit;
class QCheckBox;
class QTabWidget;
class QComboBox;
class QRadioButton;
class QSlider;
class QPushButton;

class WindowOsci;
class UCursor;
class TCursor;
class SCursor;
class Display;
class Slider;

class Tuner:public QObject {
    Q_OBJECT

public:
    Tuner(WindowOsci *w);
    ~Tuner();

public slots:
    void SetRange0(int);
    void SetRShift0(int);
    void ResetRShift0();
    void SetRange1(int);
    void SetRShift1(int);
    void ResetRShift1();
    void SetTrigLev(int);
    void ResetTrigLev();
    void SetTBase(int);
    void SetTShift(int);
    void ResetTShift();
    void SetTShift0(int);
    void SetSinchroChannel(bool);
    void SetSinchroType(bool);
    void SetStartMode(int);
    void SetAC0();
    void SetAC1();
    void SetDC0();
    void SetDC1();
    void SetGround0();
    void SetGround1();
    void SetFiltr(bool);

    void EnableSpectr(int);         // Включает режим расчёта и отображения спектра
    void ChangeChannelSpectr(int);  // Изменение канала вывода спектра

    void EnableCh0(bool);
    void EnableCh1(bool);
    void DisableCh0(bool);
    void DisableCh1(bool);
    void InvertCh0(bool);
    void InvertCh1(bool);

    void EnableMath(int);
    void ChangeMathFunction(int);

    void EnableAccumulation(int);       // Слот для включения режима накопления дисплея
    void SetCountAccumulation(int);     // Установка времени и количества точек накопления
    void DisableAccumulationLimit(int);

    void SetModePalm0(bool);            // Установка скользящего фильтра
    void SetModePalm1(bool);
    void SetPointsPalm(int);

    void SetModeAverage(int);
    void SetNumberAverage(int);

    void ChangeTShiftDisplay(int);
    void ChangeRShift0Display(int);
    void ChangeRShift1Display(int);
    void ChangePosUCur(int, int, int);
    void ChangePosTCur(int, int, int);

private:
    WindowOsci *w;

    Display *display;

    QSpinBox *sbUCur1;
    QSpinBox *sbUCur2;
    QLineEdit *leDeltaUAbs;
    QLineEdit *leUCur1Abs;
    QLineEdit *leUCur2Abs;
    QCheckBox *chbUCurs;
    QCheckBox *chbUCursTogether;
    QSpinBox *sbTCur1;
    QSpinBox *sbTCur2;
    QLineEdit *leDeltaTAbs;
    QLineEdit *leTCur1Abs;
    QLineEdit *leTCur2Abs;
    QCheckBox *chbTCurs;
    QCheckBox *chbTCursTogether;
    QComboBox *cbSourceCurs;
    QLineEdit *leRShift0Abs;
    QLineEdit *leRShift1Abs;
    QComboBox *cbRangeChannel0;
    QComboBox *cbRangeChannel1;
    Slider *vsRShift0;
    Slider *vsRShift1;
    QRadioButton *rbTrigCh0;
    Slider *vsTrigLev;
    QLineEdit *leTrigLevAbs;
    QRadioButton *rbTrigCh1;
    QSlider *hsTShift;
    QLineEdit *leTShiftAbs;
    QSpinBox *sbTShift;
    QPushButton *pbStart;
    QComboBox *cbSpectr;
    QCheckBox *chbEnableMath;
    QComboBox *cbMathFunc;
    QCheckBox *chbEnableSpectr;
    QSpinBox *sbAccumulation;
    QCheckBox *chbDisableAccumulationLimit;
    QPushButton *pbResetAccumulation;

    UCursor *uCurs;
    TCursor *tCurs;

    void Reset();
    void SelectLineEdit(QLineEdit *le = NULL);
};