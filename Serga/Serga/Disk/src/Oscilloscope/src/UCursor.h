//  [7/24/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QObject>
#pragma warning(default:4127)

class QSpinBox;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QPushButton;
struct SettingsDisplay;
class WindowOsci;
    
class UCursor : public QObject {
    Q_OBJECT

    static const int MinPos = -250;
    static const int MaxPos = 250;
    static const int DefaultValue = 100;

public:
    UCursor(WindowOsci *w, QSpinBox *sbCur0, QSpinBox *sbCur1, QLineEdit *delta, QLineEdit *absPos0, 
        QLineEdit *absPos1, QCheckBox *enable, QCheckBox *together, QComboBox *source,
        QLineEdit *lePercent, QPushButton *pbPercent, SettingsDisplay *settings);
    ~UCursor();

    void ChangeRangeOrRShift();                 // После измененения масштаба по напряжению нужно вызвать эту функцию для пересчёта значений в лайнэдитах

private slots:
    void OnCheckBoxEnable(int);
    void OnChangeSpinBoxCursor0(int);
    void OnChangeSpinBoxCursor1(int);
    void OnChangeSource(int);
    void OnChangeTogether(bool);
    void OnPressPBPercentage();

signals:
    void Repaint();

private:
    SettingsDisplay *setD;
    bool together[3];
    QSpinBox *sbCursor0;
    QSpinBox *sbCursor1;
    QLineEdit *leDelta;
    QLineEdit *leAbsPos0;
    QLineEdit *leAbsPos1;
    QCheckBox *chbTogether;
    QCheckBox *chbEnable;
    QLineEdit *lePercentage;
    QPushButton *pbPercentage;
    WindowOsci *w;

    double voltage100Percentage[2][2];          // Напряжение, соответствующее 100 процентам

    void WriteAbsPositions();
    void WritePercentage();
    void SelectLineEdit(QLineEdit *le = NULL);
};