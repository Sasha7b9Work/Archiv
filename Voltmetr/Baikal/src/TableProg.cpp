//  [3/15/2010 Sasha]
#include "defines.h"
#include "TableProg.h"
#include "GlobalFunctions.h"
#include "GlobalVars.h"

using namespace std;
using namespace NS_GlobalFunctions;

extern MathDevice mathDevice;
extern vector<SetProg> settingsProg;

//  [3/15/2010 Sasha]
TableProg::TableProg(QWidget *_parent, int _width, int _height) : QTableWidget(_parent) {
    resize(_width, _height);
    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(ChangeValue(int, int)));
    clear();
    setRowCount(1);
    settingsProg.clear();
    settingsProg.push_back(SetProg());
    setColumnCount(6);
	setColumnWidth(0, 60);
	setColumnWidth(1, 125);
	setColumnWidth(2, 70);
	setColumnWidth(3, 110);
	setColumnWidth(4, 125);
	setColumnWidth(5, 70);
    setRowHeight(0, HEIGHT_ROW);
}

//  [3/17/2010 Sasha]
void TableProg::keyPressEvent(QKeyEvent *_event) {
    if(_event->key() == Qt::Key_Return) {
        if((currentColumn() == 3)) {   // Если в ячейке уже введено значение
            if(currentRow() <= ((int)settingsProg.size() - 1)) {
                bool validCurrentRow = settingsProg[currentRow()].IsValid();
                bool validNextRow = settingsProg[currentRow() + 1].IsValid();
                int deltaTime = settingsProg[currentRow() + 1].time - settingsProg[currentRow()].time;
                if(validCurrentRow && validNextRow && (deltaTime > 1)) {
                    settingsProg.insert(settingsProg.begin() + currentRow() + 1, SetProg());
                    FillTable(currentRow() + 1, 0);
                }
            }
        }
    }
    QTableWidget::keyPressEvent(_event);
}

//  [3/23/2010 Sasha]
void TableProg::ChangeValue(int _row, int _column) {
    if(settingsProg.empty()) {
        settingsProg.push_back(SetProg());
    }
    disconnect(this, SIGNAL(cellChanged(int, int)), this, SLOT(ChangeValue(int, int)));
    // Вводим время
    if(0 == _column) {
        bool isOk = false;
        int value = currentItem()->text().toInt(&isOk, 10);
        if(isOk) {
            QString str;
            if(value > MAX_SECONDS) {
                value = MAX_SECONDS;
            }
            if(currentRow() == 0) {
                value = 0;
            }
            else if(value <= settingsProg[currentRow() - 1].time) {
                value = settingsProg[currentRow() - 1].time + 1;
            }
            else if((_row < ((int)settingsProg.size() - 2)) && (value >= settingsProg[currentRow() + 1].time)) {
                value = settingsProg[currentRow() + 1].time - 1;
            }
            str.setNum(value);
            currentItem()->setText(str);
            setCurrentCell(currentRow(), 1);
            settingsProg[currentRow()].time = value;
        }
        else {
            removeCellWidget(_row, _column);
            settingsProg[currentRow()].time = -1;
        }
    }
    // Вводим напряжение
    else if(1 == _column) {
        bool isOk = false;
        float value = currentItem()->text().toFloat(&isOk);
        if(isOk) {
            QString str;
            float max = mathDevice.GetMaxVoltage(0);
            Limitation(value, 0.0f, max);
            int relValue = mathDevice.VoltageToRel(value, 0);
            currentItem()->setText(mathDevice.VoltageToString(relValue, 0));
            settingsProg[currentRow()].voltage = relValue;
            setCurrentCell(currentRow(), 2);
        }
        else {
            removeCellWidget(_row, _column);
            settingsProg[currentRow()].voltage = -1;
        }
    }
    // Вводим ток
    else if(2 == _column) {
        bool isOk = false;
        float value = currentItem()->text().toFloat(&isOk);
        if(isOk) {
            QString str;
            float max = mathDevice.GetMaxAmperage(0);
            Limitation(value, 0.0f, max);
            int relValue = mathDevice.AmperageToRel(value, 0);
            currentItem()->setText(mathDevice.AmperageToString(relValue, 0));
            settingsProg[currentRow()].amperage = relValue;
        }
        else {
            removeCellWidget(_row, _column);
            settingsProg[currentRow()].amperage = -1;
        }
        if(settingsProg[currentRow()].IsValid() && (currentRow() == (settingsProg.size() - 1))) {
            settingsProg.push_back(SetProg());
            FillTable(currentRow() + 1, 0);
        }
    }
    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(ChangeValue(int, int)));
}

//  [4/8/2010 Sasha]
void TableProg::FillTable(int _row, int _column) {
    disconnect(this, SIGNAL(cellChanged(int, int)), this, SLOT(ChangeValue(int, int)));
    clear();
    QStringList list;
    list.push_back(tr("Время, с"));
    list.push_back(tr("Напряжение уст., В"));
    list.push_back(tr("Ток уст., А"));
    list.push_back(tr("Текущее время, с"));
    list.push_back(tr("Напряжение изм., В"));
    list.push_back(tr("Ток изм, А"));
    setHorizontalHeaderLabels(list);
    setRowCount(0);
    for(size_t i = 0; i < settingsProg.size(); i++) {
        insertRow(rowCount());
        setRowHeight(rowCount() - 1, HEIGHT_ROW);
        if(-1 != settingsProg[i].time) {
            setItem(i, 0, new QTableWidgetItem(QString().setNum(settingsProg[i].time)));
        }
        if(-1 != settingsProg[i].voltage) {
            setItem(i, 1, new QTableWidgetItem(mathDevice.VoltageToString(settingsProg[i].voltage, 0)));
        }
        if(-1 != settingsProg[i].amperage) {
            setItem(i, 2, new QTableWidgetItem(mathDevice.AmperageToString(settingsProg[i].amperage, 0)));        
        }
    }
    setCurrentCell(_row, _column);
    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(ChangeValue(int, int)));
}

//  [4/23/2010 Sasha]
void TableProg::WriteParameters() {
    LOG("num rows = %d, num columns = %d", rowCount(), columnCount());
}

//  [5/27/2010 Sasha]
void TableProg::WriteToCell(int _row, int _column, QString _str) {
	setCurrentCell(_row, _column);
	QTableWidgetItem *widgetItem = currentItem();
	if(widgetItem) {
		currentItem()->setText(_str);
	}
	else {
		setItem(currentRow(), currentColumn(), new QTableWidgetItem(_str));
	}
}

//  [5/31/2010 Sasha]
bool TableProg::DeleteCurrentRow() {
	int curRow = currentRow();
	int curColumn = currentColumn();
	if(settingsProg.size() > 1) {
		settingsProg.erase(settingsProg.begin() + currentRow());
		FillTable();
		setCurrentCell(curRow - 1, curColumn);
	}
	return (settingsProg.size() > 1);
}

//  [5/31/2010 Sasha]
bool TableProg::InsertRowAfterCurrent() {
	return false;
}