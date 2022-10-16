//  [11/30/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QObject>
#include <QComboBox>
#pragma warning(default:4127)

class QCheckBox;
class QSpinBox;
class QLineEdit;
struct SettingsDisplay;

class Cursor:public QObject {
    Q_OBJECT

    static const int MinPos = 0;
    static const int MaxPos = 249;

public:
    Cursor(QLineEdit *leFreq0, QSpinBox *sbPos0, QLineEdit *leFreq1, QSpinBox *sbPos1, QLineEdit *deltaFreq,
           QLineEdit *leU0, QLineEdit *leU1, QLineEdit *leDeltaU, SettingsDisplay *settings,
           QComboBox *cbFrequency, QComboBox *cbRange);
    void SetData();
    void ChangeChannel();
    void UnselectAllLineEdits();

private slots:
    void OnChangePos0(int);
    void OnChangePos1(int);
    void OnChangeCBFrequency(int);
    void OnChangeCBRange(int);

signals:
    void Repaint();

private:
    SettingsDisplay *setD;
    QLineEdit *leFreq0;
    QSpinBox *sbPos0;
    QLineEdit *leFreq1;
    QSpinBox *sbPos1;
    QLineEdit *leDeltaFreq;
    QLineEdit *leU0;
    QLineEdit *leU1;
    QLineEdit *leDeltaU;
    QComboBox *cbFrequency;
    QComboBox *cbRange;

    void ConfigurationWidgets();
    void LoadDefaultSettings();
    void ConnectingSlots();
    void WriteLineEditsFreq();
    void WriteLineEditsVoltage();
};