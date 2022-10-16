//  [3/24/2010 Sasha]
#pragma once

enum ModeWork {
    MW_Manual,
    MW_Auto
};

// ��������� ��� �������� ������ ���� ���������
struct SetProg {
    // �������� -1 ��������, ��� ��� ��������� �� ���������
    SetProg(int _time = -1, int _voltage = -1, int _amperage = -1) : time(_time), voltage(_voltage), amperage(_amperage) {}
    bool IsValid() { return (time >= 0) && (voltage >= 0) && (amperage >= 0); };
    int time;           // ����� � �������� ����������� �������
    int voltage;        // ������ � �������� ��������
    int amperage;       // ������ � �������� ��������
};

// ��������� ��� �������� ������
struct StructData {
    StructData(double _voltSet, double _ampSet, double _voltMeas, double _ampMeas) {
        QTime time = QTime::currentTime();
        hours = time.hour();
        min = time.minute();
        sec = time.second();
        voltageSet = _voltSet;
        amperageSet = _ampSet;
        voltageMeas = _voltMeas;
        amperageMeas = _ampMeas;
    }
	static int prevSec;		// ����� ���������� ������ ��� ���������� ���������. ����� ��� ����, ����� �������� ���� ��� � �������
    int hours;
    int min;
    int sec;
    double voltageSet;
    double amperageSet;
    double voltageMeas;
    double amperageMeas;
    friend QTextStream &operator<<(QTextStream &_output, const StructData &_data);
};

struct SettingsWindow {
    SettingsWindow() {
        dirPrograms = QObject::tr("\\");
        dirDatas = QObject::tr("\\");
        colorDisplayDisabled = Qt::black;
        colorDisplay = Qt::green;
        colorSymbols = Qt::black;
    }
    QColor colorDisplayDisabled;    // ���� ������������ �������
    QColor colorDisplay;
    QColor colorSymbols;
    QString dirPrograms;    // � ���� ������ �������� ��� ��������, � ������� ����� ����������� ���������
    QString dirDatas;       // � ���� ������ �������� ��� ��������, � ������� ����� ��������� ������
    ModeWork mw;            // � ����� ������ ��������� ��������� - � ������ ��� ��������������
};