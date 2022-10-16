//  [3/24/2010 Sasha]
#include "MathDevice.h"
#include "GlobalVars.h"
#include "TableProg.h"
#include "ProgrammingThread.h"

MathDevice mathDevice;
std::vector<SetProg> settingsProg;
QPushButton *g_pbUpdate = NULL;
QComboBox *g_cbPort = NULL;
SettingsWindow setW;
TableProg *table = NULL;
ProgrammingThread progrThread;      // Этот поток используется для автоматического управления по программе

int StructData::prevSec = 70;

//  [4/29/2010 Sasha]
QTextStream& operator<<(QTextStream &_output, const StructData &_data) {
	if(fabs((float)(_data.sec - StructData::prevSec)) != 0.0f) {
		QChar fillChar = QLatin1Char('0');
		QString str = QString("%1:%2:%3")
			.arg((uint)_data.hours, 2, 10, fillChar)
			.arg(_data.min, 2, 10, fillChar)
			.arg(_data.sec, 2, 10, fillChar);
		_output << str << "\t";
		_output << mathDevice.VoltageToString(_data.voltageSet) << "\t";
		_output << mathDevice.AmperageToString(_data.amperageSet) << "\t";
		_output << mathDevice.VoltageToString(_data.voltageMeas) << "\t";
		_output << mathDevice.AmperageToString(_data.amperageMeas) << "\n";
		StructData::prevSec = _data.sec;
	}
    return _output;
}