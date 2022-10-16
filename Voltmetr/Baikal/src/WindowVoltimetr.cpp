//  [3/1/2010 Sasha]
#include "defines.h"
#include "WindowVoltimetr.h"
#include "GlobalFunctions.h"
#include "MathDevice.h"
#include "GlobalVars.h"
#include "TableProg.h"
#include "ProgrammingThread.h"

using namespace std;
using namespace NS_GlobalFunctions;

extern MathDevice mathDevice;
extern vector<SetProg> settingsProg;
extern SettingsWindow setW;
extern TableProg *table;
extern QPushButton *g_pbUpdate;
extern QComboBox *g_cbPort;
extern ProgrammingThread progrThread;

bool debugMode = false;         // Если переменная установлена в false, обычный режим работы, если true - режим отладки

//  [3/1/2010 Sasha]
WindowVoltimetr::WindowVoltimetr(QMainWindow *_parent) : 
    QMainWindow(_parent),
    recordData(false)
    {
    setupUi(this);

    progrExt[0] = tr("*.prg0");
    progrExt[1] = tr("*.prg1");

    debugMode = false;
    setW.mw = MW_Manual;

    QTextCodec *rusCodec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(rusCodec);

    LoadSettings();
    ConfigureWidgets();
	CreateContextMenu();
    EnableManualMode();
    UpdatePorts();
	progrThread.start();
	//EnableAutoMode();
}

//  [3/1/2010 Sasha]
void WindowVoltimetr::ConfigureWidgets() {
    displayUSet = new Display(frameUSet);
    displayISet = new Display(frameISet);
    displayUMeas = new Display(frameUMeas);
    displayIMeas = new Display(frameIMeas);
    displays[0] = displayUSet;
    displays[1] = displayISet;
    displays[2] = displayUMeas;
    displays[3] = displayIMeas;
    holderU = new Holder(frameHolderU);
    sbHolderU->setSingleStep(1);
    holderI = new Holder(frameHolderI);
    sbHolderI->setSingleStep(1);

	table = new TableProg(frame, frame->width(), frame->height());

    ConnectingSlots();

    holderI->SetValue(0);
    holderU->SetValue(0);

    pbStart->setEnabled(false);

    pbSaveProgram->setVisible(false);
    pbLoadPorgram->setVisible(false);
}

//  [3/24/2010 Sasha]
QString& WindowVoltimetr::VoltageToString(QString &_voltageString) {
    QString stringTemp = _voltageString;
    return _voltageString = QObject::tr("U = ") + stringTemp + QObject::tr("V");
}

//  [3/24/2010 Sasha]
QString& WindowVoltimetr::AmperageToString(QString &_amperageString) {
    QString stringTemp = _amperageString;
    return _amperageString = QObject::tr("I = ") + stringTemp + QObject::tr("A");
}

//  [3/2/2010 Sasha]
void WindowVoltimetr::ChangeHolderU(int _value) {
    static int prevValue = INT_MAX;
    static int prevModel = 2;
    if(_value == prevValue && prevModel == mathDevice.GetModel()) {
        return;
    }
    prevValue = _value;
    prevModel = mathDevice.GetModel();
    QString stringVoltage = mathDevice.VoltageToString(_value, 0);
    displayUSet->SetValue(VoltageToString(stringVoltage));
    sbHolderU->setValue(_value);
    sbHolderU->setFocus();
}

//  [3/2/2010 Sasha]
void WindowVoltimetr::ChangeHolderI(int _value) {
    static int prevValue = INT_MAX;
    static int prevModel = 2;
    if(_value == prevValue && prevModel == mathDevice.GetModel()) {
        return;
    }
    prevValue = _value;
    prevModel = mathDevice.GetModel();
    QString stringAmperage = mathDevice.AmperageToString(_value, 0);
    displayISet->SetValue(AmperageToString(stringAmperage));
    sbHolderI->setValue(_value);
    sbHolderI->setFocus();
}

//  [3/2/2010 Sasha]
void WindowVoltimetr::UpdatePorts() {
    disconnect(g_cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetPort(int)));

    // Заполняем комбобокс портов
    g_cbPort->clear();
    vector<QString> stringPorts = device.GetPorts();
    for(size_t i = 0; i < stringPorts.size(); i++) {
        g_cbPort->addItem(stringPorts[i]);
    }

    // Теперь пробуем подконнектиться к каждому порту, начиная с последнего, и если получается, на этом заканчиваем, а если нет, устанавливаем активным первый индекс комбобокса портов
    for(int iPort = (int)stringPorts.size() - 1; iPort >= 0; iPort--) {
        g_cbPort->setCurrentIndex(iPort);
        if(SetPort(iPort)) {
            break;
        }
    }

    connect(g_cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetPort(int)));
}

//  [3/2/2010 Sasha]
bool WindowVoltimetr::SetPort(int _indexPort) {
    int model = 2;
    bool created = device.CreateDevice(_indexPort, model, true);
    if(debugMode) {
        created = true;
    }
    mathDevice.SetModel(model); 
	pbEnable->setEnabled(created);
    aAutoMode->setEnabled(created && (MW_Manual == setW.mw));    // Установить активной данную action, если создание устройства произошло успешно и сейчас установлен ручной режим управления
    aManualMode->setEnabled(created && (MW_Auto == setW.mw));    // -//- и установлен автоматический режим управления
    QString str = tr("Источник питания ");
    if(0 == model) {
        str += tr("Б5-89");
    }
    else if(1 == model) {
        str += tr("Б5-89/1");
    }
    holderU->SetRange(mathDevice.GetRangeU(0));
    holderI->SetRange(mathDevice.GetRangeI(0));
    sbHolderU->setRange(0, mathDevice.GetRangeU(0));
    sbHolderI->setRange(0, mathDevice.GetRangeI(0));

    bool condition = (0 == model) || (1 == model);
    table->setEnabled(condition);
    setWindowTitle(str);
    return created;
}

//  [3/2/2010 Sasha]
void WindowVoltimetr::StartDevice() {

    int u = holderU->GetValue();
    device.WriteU(u);
    int i = holderI->GetValue();
    device.WriteI(i);
    device.Start();
}

//  [3/2/2010 Sasha]
void WindowVoltimetr::StopDevice() {
	if(MW_Manual == setW.mw) {
		device.Stop();
		holderI->SetValue(0);
		holderU->SetValue(0);
	}
}

//  [3/2/2010 Sasha]
void WindowVoltimetr::OnTimer() {
    int measU = 0;
    int measI = 0;
    // Если удалось прочитать напряжение и ток
    if(device.ReadU(measU) && device.ReadI(measI)) {
        // Выводим их на дисплей
        displayIMeas->SetEnabled(true);
        displayUMeas->SetEnabled(true);
        QString voltage = mathDevice.VoltageToString(measU, 1);
        displayUMeas->SetValue(VoltageToString(voltage));
        QString amperage = mathDevice.AmperageToString(measI, 1);
        displayIMeas->SetValue(AmperageToString(amperage));
        // и записываем в файл
        if(recordData && settingsDialog.chbSaveData->isChecked()) {
            QTextStream out(fileData);
            double voltSet = mathDevice.VoltageToAbs(holderU->GetValue(), 0);
            double ampSet = mathDevice.AmperageToAbs(holderI->GetValue(), 0);
            out << StructData(voltSet, ampSet, mathDevice.VoltageToAbs(measU, 1), mathDevice.AmperageToAbs(measI, 1));
        }
		// Если находимся в автоматическом режиме и программа запущена - нужно записать в таблицу текущие значения параметров и времени
		if((MW_Auto == setW.mw) && pbStart->isChecked() && (!pbStop->isChecked())) {
			table->WriteToCell(currentRow, 4, mathDevice.VoltageToString(measU, 1));
			table->WriteToCell(currentRow, 5, mathDevice.AmperageToString(measI, 1));
			table->selectRow(currentRow);
			float time = progrThread.GetTime().ToMSec() / 1000.0f;
			table->WriteToCell(currentRow, 3, QString().setNum(time));
			table->selectRow(currentRow);
		}
    }
    else {
        EnableDevice(false);
        return;
    }
}

//  [3/4/2010 Sasha]
void WindowVoltimetr::SetExactly() {
    bool currentExactly = holderI->GetExactly();
    pbExactly->setText(currentExactly ? tr("Грубо") : tr("Точно"));
    holderI->SetExactly(!currentExactly);
    holderU->SetExactly(!currentExactly);
}

//  [3/5/2010 Sasha]
void WindowVoltimetr::EnableDevice(bool _checked) {
    if(!_checked) {
        recordData = false;
		if(settingsDialog.chbSaveData->isChecked()) {
			CreateDataFile(false);
		}
    }
	disconnect(pbEnable, SIGNAL(toggled(bool)), this, SLOT(EnableDevice(bool)));
	if(!_checked) {
		timer.stop();
        SetPort(g_cbPort->currentIndex());
	}
    bool isOk = false;      // Признак того, что команда выполнена успешно
    int counter = 0;
    while((!isOk) && (counter < 5)) {
        isOk = device.Enable(_checked);
		counter++;
    }
    bool condition = debugMode ? true : (counter < 5) && (isOk);
	int uSet = 0;
	condition &= device.ReadSetU(uSet);
	int iSet = 0;
	condition &= device.ReadSetI(iSet);
    if(_checked) {
        ChangeHolderI(iSet);
        ChangeHolderU(uSet);
        displayUSet->SetEnabled(condition);
        displayISet->SetEnabled(condition);

        if(condition) {
			OnTimer();
		    timer.start(100);
			if(settingsDialog.chbSaveData->isChecked()) {
				CreateDataFile(true);
			}
            recordData = true;
			menuMode->setEnabled(false);
        }
    }
    else {
        displayUSet->SetEnabled(false);
        displayISet->SetEnabled(false);
		displayUMeas->SetEnabled(false);
		displayIMeas->SetEnabled(false);
		menuMode->setEnabled(true);
    }
    pbEnable->setChecked(condition && _checked);
    pbEnable->setText(condition && _checked ? QObject::tr("Откл") : QObject::tr("Вкл"));
    pbStart->setEnabled(condition && _checked);
    pbStop->setEnabled(condition && _checked);
    g_pbUpdate->setEnabled(condition && (!_checked));
    g_cbPort->setEnabled(condition && (!_checked));
	if((MW_Auto == setW.mw) && (!_checked)) {
		pbStart->setChecked(false);
		pbStop->setChecked(false);
	}

	connect(pbEnable, SIGNAL(toggled(bool)), this, SLOT(EnableDevice(bool)));
}

//  [3/15/2010 Sasha]
void WindowVoltimetr::EnableManualMode() {

    aAutoMode->setEnabled(true);
    aManualMode->setEnabled(false);
    setMinimumHeight(197);
    setMaximumHeight(197);
    setW.mw = MW_Manual;

    pbStart->setCheckable(false);
    pbStart->setText(QObject::tr("Старт")); 
    
    pbStop->setCheckable(false);
    pbStop->setText(QObject::tr("Стоп"));

    pbSaveProgram->setVisible(false);
    pbLoadPorgram->setVisible(false);
    connect(holderU, SIGNAL(ChangeValue(int)), this, SLOT(ChangeHolderU(int)));
    connect(holderI, SIGNAL(ChangeValue(int)), this, SLOT(ChangeHolderI(int)));
    connect(sbHolderU, SIGNAL(valueChanged(int)), holderU, SLOT(SetValue(int)));
    connect(sbHolderI, SIGNAL(valueChanged(int)), holderI, SLOT(SetValue(int)));
}

//  [3/15/2010 Sasha]
void WindowVoltimetr::EnableAutoMode() {
    aAutoMode->setEnabled(false);
    aManualMode->setEnabled(true);
    setMaximumHeight(500);
    setMinimumHeight(500);
    setW.mw = MW_Auto;
    table->FillTable();
    pbStart->setCheckable(true);
    pbStart->setChecked(false);
    pbStart->setText(QObject::tr("Старт"));

    pbStop->setCheckable(true);
    pbStop->setChecked(false);
    pbStop->setText(QObject::tr("Пауза"));
    pbStop->setEnabled(false);

    pbSaveProgram->setVisible(true);
    pbLoadPorgram->setVisible(true);
    //disconnect(holderU, SIGNAL(ChangeValue(int)), this, SLOT(ChangeHolderU(int)));
    //disconnect(holderI, SIGNAL(ChangeValue(int)), this, SLOT(ChangeHolderI(int)));
    //disconnect(sbHolderU, SIGNAL(valueChanged(int)), holderU, SLOT(SetValue(int)));
    //disconnect(sbHolderI, SIGNAL(valueChanged(int)), holderI, SLOT(SetValue(int)));
}

//  [3/26/2010 Sasha]
void WindowVoltimetr::StartProgramDevice(bool _startProgram) {
	table->setEnabled(!_startProgram);
	if(_startProgram) {
		settingsDialog.chbSaveData->setEnabled(false);
		table->FillTable();
		pbStart->setText(QObject::tr("Стоп"));
		pbStop->setEnabled(true);				// В автоматическом режиме это кнопка паузы
		if(settingsProg.size() != 0) {
			int i = settingsProg[0].amperage;
			int u = settingsProg[0].voltage;
			sbHolderU->setValue(u);
			sbHolderI->setValue(i);
			device.WriteU(u);
			device.WriteI(i);
			//timer.start(100);
			device.Start();
			progrThread.StartProgram();
		}
	}
	else {
		settingsDialog.chbSaveData->setEnabled(true);
		pbStart->setText(QObject::tr("Старт"));
		pbStop->setEnabled(false);
		//timer.stop();
		//device.Stop();
		progrThread.StopProgram();
	}
}

//  [3/26/2010 Sasha]
void WindowVoltimetr::PauseProgramDevice(bool _pauseProgram) {
	progrThread.PauseProgram(_pauseProgram);
}

//  [3/15/2010 Sasha]
void WindowVoltimetr::closeEvent(QCloseEvent *) {
    SaveSettings();
    close();
}

//  [3/15/2010 Sasha]
void WindowVoltimetr::close() {
    if(pbEnable->isEnabled()) {
        pbEnable->setChecked(false);
    }
    settingsDialog.close();
    QMainWindow::close();
}

//  [4/6/2010 Sasha]]
void WindowVoltimetr::ShowDialogSettings() {
    settingsDialog.show();
}

//  [4/7/2010 Sasha]
void WindowVoltimetr::SetColorDisplay() {
    QColor color = QColorDialog::getColor();
    if(color.isValid()) {
        for(int i = 0; i < 4; i++) {
            displays[i]->SetColorDisplay(color);
        }
    }
}

//  [4/7/2010 Sasha]
void WindowVoltimetr::SetColorSimbols() {
    QColor color = QColorDialog::getColor();
    if(color.isValid()) {
        for(int i = 0; i < 4; i++) {
            displays[i]->SetColorSimbols(color);
        }
    }
}

//  [4/7/2010 Sasha]
void WindowVoltimetr::ChangeDirDatas() {
    QFileDialog fileDialog(this);
    QStringList strList;
    fileDialog.setFileMode(QFileDialog::Directory);
    if(fileDialog.exec()) {
        strList = fileDialog.selectedFiles();
        setW.dirDatas = strList.at(0);
        settingsDialog.Update();
    }
}

//  [4/7/2010 Sasha]
void WindowVoltimetr::ChangeDirPrograms() {
    QFileDialog fileDialog(this);
    QStringList strList;
    fileDialog.setFileMode(QFileDialog::Directory);
    if(fileDialog.exec()) {
        strList = fileDialog.selectedFiles();
        setW.dirPrograms = strList.at(0);
        settingsDialog.Update();
    }
}

//  [4/19/2010 Sasha]
void WindowVoltimetr::SaveProgram() {
    QString nameFile = QFileDialog::getSaveFileName(this, "", setW.dirPrograms, progrExt[mathDevice.GetModel()]);
    QFile file(nameFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ERROR_LOG("Error write program");
        return;
    }
    QTextStream out(&file);
    size_t i = 0;
    while(settingsProg[i].IsValid()) {
        out << settingsProg[i].time << "\t" << settingsProg[i].voltage << "\t" << settingsProg[i].amperage << endl;
        i++;
    }
    out.flush();
}

//  [4/19/2010 Sasha]
void WindowVoltimetr::LoadProgram() {
    QString fileName = QFileDialog::getOpenFileName(this, "", setW.dirPrograms, progrExt[mathDevice.GetModel()]);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ERROR_LOG("Error load program");
        return;
    }
    QTextStream inStream(&file);
    settingsProg.clear();
    while (!inStream.atEnd()) {
        SetProg newData;
        if(!inStream.atEnd()) {
            inStream >> newData.time;
            if(!inStream.atEnd()) {
                inStream >> newData.voltage;
                if(!inStream.atEnd()) {
                    inStream >> newData.amperage;
                    settingsProg.push_back(newData);
                }
            }
        }
    }
	settingsProg.push_back(SetProg());
    table->FillTable();
    table->setCurrentCell(settingsProg.size(), 0);
}

//  [4/27/2010 Sasha]
void WindowVoltimetr::LoadSettings() {
    configurator.LoadDirData();
    configurator.LoadDirProgram();
    configurator.LoadColorBackground();
    configurator.LoadColorSymbols();
}

//  [4/27/2010 Sasha]
void WindowVoltimetr::SaveSettings() {
    configurator.SaveDirData();
    configurator.SaveDirProgram();
    configurator.SaveColorBackground();
    configurator.SaveColorSymbols();
}

//  [4/28/2010 Sasha]
void WindowVoltimetr::ConnectingSlots() {
    connect(holderU, SIGNAL(ChangeValue(int)), this, SLOT(ChangeHolderU(int)));
    connect(holderI, SIGNAL(ChangeValue(int)), this, SLOT(ChangeHolderI(int)));
    connect(g_pbUpdate, SIGNAL(clicked()), this, SLOT(UpdatePorts()));
    connect(g_cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetPort(int)));
    connect(sbHolderU, SIGNAL(valueChanged(int)), holderU, SLOT(SetValue(int)));
    connect(sbHolderI, SIGNAL(valueChanged(int)), holderI, SLOT(SetValue(int)));
    connect(pbStart, SIGNAL(clicked()), this, SLOT(StartDevice()));
    connect(pbStart, SIGNAL(toggled(bool)), this, SLOT(StartProgramDevice(bool)));
    connect(pbStop, SIGNAL(clicked()), this, SLOT(StopDevice()));
    connect(pbStop, SIGNAL(toggled(bool)), this, SLOT(PauseProgramDevice(bool)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    connect(pbExactly, SIGNAL(clicked()), this, SLOT(SetExactly()));
    connect(pbEnable, SIGNAL(toggled(bool)), this, SLOT(EnableDevice(bool)));
    connect(aExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(aManualMode, SIGNAL(triggered()), this, SLOT(EnableManualMode()));
    connect(aAutoMode, SIGNAL(triggered()), this, SLOT(EnableAutoMode()));
    connect(aSettings, SIGNAL(triggered()), this, SLOT(ShowDialogSettings()));

    connect(settingsDialog.pbColorDisplay, SIGNAL(clicked()), this, SLOT(SetColorDisplay()));
    connect(settingsDialog.pbColorSimbols, SIGNAL(clicked()), this, SLOT(SetColorSimbols()));
    connect(settingsDialog.pbDirPrograms, SIGNAL(clicked()), this, SLOT(ChangeDirPrograms()));
    connect(settingsDialog.pbDirData, SIGNAL(clicked()), this, SLOT(ChangeDirDatas()));
    connect(pbSaveProgram, SIGNAL(clicked()), this, SLOT(SaveProgram()));
    connect(pbLoadPorgram, SIGNAL(clicked()), this, SLOT(LoadProgram()));

    // Коннектим слот автоматического управления
    connect(&progrThread, SIGNAL(ControlSignal(int, int, int, int)), this, SLOT(OnControlProgram(int, int, int, int)));

	connect(aDeleteRow, SIGNAL(triggered()), this, SLOT(DeleteRow()));
	connect(aInsertRow, SIGNAL(triggered()), this, SLOT(InsertRow()));
}

//  [4/28/2010 Sasha]
void WindowVoltimetr::CreateDataFile(bool _create) {
    if(_create) {
        QString nameFile = QDate::currentDate().toString("yyyy_MM_dd") + tr(" ") + QTime::currentTime().toString() + tr(".dat");
        nameFile.replace(QString(":"), QString("_"));
        nameFile = setW.dirDatas + tr("//") + nameFile;
        fileData = new QFile(nameFile);
        if(!fileData->open(QIODevice::WriteOnly | QIODevice::Text)) {
            ERROR_LOG("Can\'t create file %s for write", nameFile.toAscii());
        }
        QTextStream out(fileData);
        out << tr("t, s\tUin, V\tIin, A\tUout, V\tIout, A\n");
    }
    else {
        fileData->close();
        delete fileData;
    }
}

//  [4/30/2010 Sasha]
void WindowVoltimetr::OnControlProgram(int _type, int _voltage, int _amperage, int _numRow) {
	if(TCS_Stop == _type) {
		pbStart->setChecked(false);
	}
	else {
        table->selectRow(_numRow);
		sbHolderI->setValue(_amperage);
		sbHolderU->setValue(_voltage);
		currentRow = _numRow;
	}
	if(_numRow != 0) {
		table->WriteToCell(_numRow - 1, 3, QString().setNum(settingsProg[_numRow].time));
		table->selectRow(currentRow);
	}
    StartDevice();
}

//  [5/31/2010 Sasha]
void WindowVoltimetr::CreateContextMenu() {
	table->addAction(aDeleteRow);
	table->addAction(aInsertRow);
	table->setContextMenuPolicy(Qt::ActionsContextMenu);
}

//  [5/31/2010 Sasha]
void WindowVoltimetr::DeleteRow() {
	aDeleteRow->setEnabled(table->DeleteCurrentRow());
}

//  [5/31/2010 Sasha]
void WindowVoltimetr::InsertRow() {
	table->InsertRowAfterCurrent();
}