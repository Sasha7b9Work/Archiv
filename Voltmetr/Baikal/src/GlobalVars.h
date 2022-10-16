//  [3/24/2010 Sasha]
#pragma once

enum ModeWork {
    MW_Manual,
    MW_Auto
};

// Структура для хранения одного шага программы
struct SetProg {
    // Значения -1 означают, что эта структура не заполнена
    SetProg(int _time = -1, int _voltage = -1, int _amperage = -1) : time(_time), voltage(_voltage), amperage(_amperage) {}
    bool IsValid() { return (time >= 0) && (voltage >= 0) && (amperage >= 0); };
    int time;           // Время в секундах наступления события
    int voltage;        // Вольты в условных единицах
    int amperage;       // Амперы в условных единицах
};

// Структура для хранения данных
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
	static int prevSec;		// Здесь количество секунд при предыдущем измерении. Нужно для того, чтобы выводить один раз в секунду
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
    QColor colorDisplayDisabled;    // Цвет выключенного дисплея
    QColor colorDisplay;
    QColor colorSymbols;
    QString dirPrograms;    // В этой строке хранится имя каталога, в который будут сохраняться программы
    QString dirDatas;       // В этой строке хранится имя каталога, в котором будут хранитсья данные
    ModeWork mw;            // В каком режиме находится программа - в ручном или автоматическом
};