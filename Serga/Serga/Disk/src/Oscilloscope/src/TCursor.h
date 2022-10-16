//  [7/29/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QObject>
#pragma warning(default:4127)

class QSpinBox;
class QLineEdit;
class QCheckBox;
class QComboBox;
struct SettingsDisplay;
class WindowOsci;

class TCursor : public QObject {
    Q_OBJECT

    static const int MinPos = 0;
    static const int MaxPos = 501;
    static const int DefaultValue1 = 150;
    static const int DefaultValue2 = 350;

public:
    TCursor(WindowOsci *w, QSpinBox *sbCur0, QSpinBox *sbCur1, QLineEdit *delta, QLineEdit *absPos0, QLineEdit *absPos1,
        QCheckBox *enable, QCheckBox *together, QComboBox *source, SettingsDisplay *settings);
    ~TCursor();

    void ChangedTBaseOrTPosOrTShift();
 
private slots:
    void OnCheckBoxEnable(int);
    void OnChangeSpinBoxCursor0(int);
    void OnChangeSpinBoxCursor1(int);
    void OnChangeSource(int);
    void OnChangeTogether(bool);

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
    WindowOsci *w;

    void WriteAbsPositions();
    void SelectLineEdit(QLineEdit *le = NULL);
};