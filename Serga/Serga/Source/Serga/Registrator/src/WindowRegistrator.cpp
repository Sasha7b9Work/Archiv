//  [11/27/2009 Sasha]
#include "WindowRegistrator.h"
#pragma warning(disable:4127)
#include <QTimer>
#include <QTextCodec>
#include <QFileDialog>
#include <QDate>
#include <QColorDialog>
#pragma warning(default:4127)
#include <fstream>
#include <iostream>
#include <iomanip>
#include "DisplayRegistrator.h"
#include "MathDisplayRegistrator.h"
#include "../../common/GeometryMath.h"
#include "../../common/GlobalFunctions.h"
#include "../Device129Multi/src/Device129Multi.h"
#include "../common/defines.h"
#include "../common/TypesAltera.h"

using namespace NS_GeometryMath;
using namespace NS_GlobalFunctions;
using namespace NS_Configurator;
using namespace NS_TypesAltera;

using namespace std;

MathDisplayRegistrator mathDisplay;

static const char * const CONFIGURE_FILE = "registrator.cfg";

int numDevice = -1;

WindowRegistrator::WindowRegistrator(QMainWindow *_parent):
    QMainWindow(_parent) 
{
    setupUi(this);
    numDevice = D129_CreateDevice(ModeRegistrator);
	strDir = tr("c:");
    display0 = new DisplayRegistrator(frame0, 0, datas);
    display1 = new DisplayRegistrator(frame1, 1, datas);
    ConfigureWidgets();
    ConnectingSlots();
    timer = new QTimer();
    timer->setInterval(1);
    configurator = new Configurator(CONFIGURE_FILE);
    LoadSettings();
    UpdatePorts();
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}

void WindowRegistrator::Start(bool _start) 
{
    if(_start) 
    {
        D129_CalibrateZero(numDevice);
        datas.clear();
        QString nameFile = QDate::currentDate().toString() + tr(" ") + QTime::currentTime().toString() + tr(".rif");
        nameFile.replace(QString(":"), QString("_"));
        nameFile = strDir + tr("//") + nameFile;
        file = new QFile(nameFile);
        if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            //LOG("Can\'t create file %s for write", nameFile.toAscii());
        }
         

        QTextStream out(file);
        // Записываем текущие параметры 
        out << cbU0->currentIndex() << endl;
        out << cbU1->currentIndex() << endl;
        out << cbTime->currentIndex() << endl;
        //D129_SetAutoCalibrate(numDevice, true);
        D129_SetRange(numDevice, 0, cbU0->currentIndex());
        D129_SetRange(numDevice, 1, cbU1->currentIndex());
        //D129_SetAutoCalibrate(numDevice, false);
        // Запускаем прибор
        D129_CalibrateZero(numDevice);
        D129_Start(numDevice, true);

        QTime prevTime = QTime::currentTime();
        while(QTime::currentTime().second() == prevTime.second()) 
            // Выжидаем, когда будет 0 мс     
        
        // И теперь запускаем таймер
        timer->start();
        t.Reset();
    }
    else 
    {
        file->close();
        delete file;
        D129_Stop(numDevice);
        timer->stop();
    }
    pbStart->setText(_start ? tr("Стоп") : tr("Старт"));
    cbPort->setEnabled(!_start);
    pbUpdatePorts->setEnabled(!_start);
    cbTime->setEnabled(!_start);
    cbU0->setEnabled(!_start);
    cbU1->setEnabled(!_start);
	cbLengthRecord->setEnabled(!_start);
    pbOpen->setEnabled(!_start);
    pbDir->setEnabled(!_start);
    aCalibrate->setEnabled(!_start);
}

void WindowRegistrator::OnTimer() 
{
    timer->stop();

    if(!D129_IsConnect(numDevice)) 
    {
        display0->ConnectToDevice(false);
        pbStart->setChecked(false);
        pbStart->setEnabled(false);
        return;
    }

    Time currentTime = t.GetTime();
	QTime currentQTime = QTime::currentTime();
	currentQTime.setHMS(currentQTime.hour(), currentQTime.minute(), currentQTime.second(), 
		(currentQTime.msec() / times[cbTime->currentIndex()].timeMs) * times[cbTime->currentIndex()].timeMs);
	long long passedTime = currentTime.ToMSec();
	unsigned int exprNumPoint = passedTime / times[cbTime->currentIndex()].timeMs;
	if(exprNumPoint < (int)datas.size()) 
    {
		timer->start();
		return;
	}
    D129_Stop(numDevice);
    D129_Start(numDevice, true);
	bool pointIsOk = (exprNumPoint == datas.size());
    if(D129_IsRunning(numDevice)) 
    {
        if(D129_DataReadyForReadRegistrator(numDevice)) 
        {
            int size = datas.size();
            double d1 = D129_GetValue(numDevice, 0);
            double d2 = D129_GetValue(numDevice, 1);
			if(!pointIsOk) 
            {
				unsigned int numPoints = exprNumPoint - size;
				QTime time;
				if(size != 0) {
					Line line1(size - 1, datas[size - 1].data[0], size + numPoints, d1);
					Line line2(size - 1, datas[size - 1].data[1], size + numPoints, d2);
					for(size_t i = size; i < (size + numPoints); i++) 
                    {
						Line line12(i, 0.0, i, 1.0);
						Point p1 = line1.GetIntersection(line12);
						Point p2 = line2.GetIntersection(line12);
						time = datas[i-1].time.addMSecs(times[cbTime->currentIndex()].timeMs);
						Data d(time, p1.y, p2.y, false);
						datas.push_back(d);
						QTextStream out(file);
						out << d;
					}
				}
				else 
                {
					time = currentQTime;
					time.setHMS(time.hour(), time.minute(), time.second(), 0);
					for(size_t i = 0; i < numPoints; i++) 
                    {
						Data d(time, d1, d2, false);
						datas.push_back(d);
						QTextStream out(file);
						out << d;
						time.addMSecs(times[cbTime->currentIndex()].timeMs);
					}
				}
			}
            Data d(currentQTime, d1, d2);
            datas.push_back(d);
            QTextStream out(file);
            out << d;
            SetNewRanges();
            hsbCh0->setValue(size - 1);
			hsbCh1->setValue(size - 1);
        }
        else
            cout << "Data not ready!!!" << endl;
    }

    int numPoints = lengths[cbLengthRecord->currentIndex()].numPoints;
    if((numPoints != 0) && ((int)datas.size() == numPoints)) 
    {
        pbStart->setChecked(false);
        return;
    }
    timer->start();
}

bool WindowRegistrator::SetNumberPort(int _indexPort) 
{
    if((_indexPort < 0) || (_indexPort > (cbPort->count() - 1)))
        return false;

    D129_Stop(numDevice);
    bool created = false;
    int counter = 0;
    do 
    {
        created = D129_ConnectDeviceToPort(numDevice, ports[_indexPort]);
        counter++;
    } while((counter < 5) && (!created));
    pbStart->setEnabled(created);
    D129_SetSettings(numDevice);
    display0->ConnectToDevice(created);
    display0->Reset();
    display1->Reset();

    return created;
}

void WindowRegistrator::SetRange0(int _range) 
{
	D129_SetRange(numDevice, 0, _range);
    UpdateLineEdits(sbUCur0Ch0);
    UpdateLineEdits(sbUCur1Ch0);
    display0->Reset();
}

void WindowRegistrator::SetRange1(int _range) 
{
    D129_SetRange(numDevice, 1, _range);
    UpdateLineEdits(sbUCur0Ch1);
    UpdateLineEdits(sbUCur1Ch1);
	display1->Reset();
}

//  [12/7/2009 Sasha]
void WindowRegistrator::SB0valueChanged(int val) {
    display0->DrawData(val);
}

//  [12/7/2009 Sasha]
void WindowRegistrator::SB1valueChanged(int val) {
    display1->DrawData(val);
}

//  [12/7/2009 Sasha]
void WindowRegistrator::Open(bool _isDown) {
    if(_isDown) {
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this, "", strDir, tr("*.rif"));
    pbOpen->setChecked(false);
    datas.clear();
    file = new QFile(fileName);
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        delete file;
        pbOpen->setChecked(false);
        return;
    }
    QTextStream out(file);
    int param;
    // Восстанаваливаем масштаб по напряжению первого канала
    out >> param;
    cbU0->setCurrentIndex(param);
    // Восстанавливаем масштаб по наряжению второго канала
    out >> param;
    cbU1->setCurrentIndex(param);
    // Восстанавливаем частоту опроса
    out >> param;
    cbTime->setCurrentIndex(param);
    Data d;
    while(!out.atEnd()) {
        out >> d;
        datas.push_back(d);
    }
    datas.pop_back();
    SetNewRanges();
    hsbCh0->setValue(hsbCh0->maximum());
    hsbCh0->setValue(hsbCh0->minimum());
    hsbCh1->setValue(hsbCh1->maximum());
    hsbCh1->setValue(hsbCh1->minimum());
}

//  [12/7/2009 Sasha]
void WindowRegistrator::SetDir(bool _isDown) {
    if(_isDown) {
        return;
    }
    QFileDialog fileDialog(this);
    QStringList strList;
    fileDialog.setFileMode(QFileDialog::Directory);
    if(fileDialog.exec()) {
        strList = fileDialog.selectedFiles();
        strDir = strList.at(0);
		pbStart->setEnabled(true);
    }
}

//  [12/7/2009 Sasha]
void WindowRegistrator::MoveSignal0(int delta) {
    hsbCh0->setValue(hsbCh0->value() + delta);
}

void WindowRegistrator::MoveSignal1(int delta) {
    hsbCh1->setValue(hsbCh1->value() + delta);
}

//  [12/18/2009 Admin]
void WindowRegistrator::EnableUCursCh0(bool _enable) {
	int value = display0->GetHeight() / 2 * (2.0 / 5.0);
	sbUCur0Ch0->setValue(value);
	sbUCur1Ch0->setValue(-value);
    display0->SetPosUCur(0, value);
    display0->SetPosUCur(1, -value);
	leUCur0Ch0->setEnabled(_enable);
	sbUCur0Ch0->setEnabled(_enable);
	leUCur1Ch0->setEnabled(_enable);
	sbUCur1Ch0->setEnabled(_enable);
	leDeltaUCurCh0->setEnabled(_enable);
	display0->EnableUCurs(_enable);
}

//  [12/18/2009 Admin]
void WindowRegistrator::EnableUCursCh1(bool _enable) {
	int value = display1->GetHeight() / 2 * (2.0 / 5.0);
	sbUCur0Ch1->setValue(value);
	sbUCur1Ch1->setValue(-value);
	leUCur0Ch1->setEnabled(_enable);
	sbUCur0Ch1->setEnabled(_enable);
	leUCur1Ch1->setEnabled(_enable);
	sbUCur1Ch1->setEnabled(_enable);
	leDeltaUCurCh1->setEnabled(_enable);
	display1->EnableUCurs(_enable);
}

void WindowRegistrator::ChangePosUCur0Ch0(int _pos) 
{
	double step = mathDisplay.GetMaxVoltage(voltages[cbU0->currentIndex()].range) / sbUCur0Ch0->maximum();
	double absVolts = step * _pos;
	leUCur0Ch0->setText(VoltageToString(absVolts));
	display0->SetPosUCur(0, _pos);
	WriteDeltasU();
}

void WindowRegistrator::ChangePosUCur1Ch0(int _pos) 
{
    double step = mathDisplay.GetMaxVoltage(voltages[cbU0->currentIndex()].range) / sbUCur1Ch0->maximum();
    double absVolts = step * _pos;
    leUCur1Ch0->setText(VoltageToString(absVolts));
    display0->SetPosUCur(1, _pos);
    WriteDeltasU();
}

void WindowRegistrator::ChangePosUCur0Ch1(int _pos)  
{
    double step = mathDisplay.GetMaxVoltage(voltages[cbU1->currentIndex()].range) / sbUCur0Ch1->maximum();
    double absVolts = step * _pos;
    leUCur0Ch1->setText(VoltageToString(absVolts));
    display1->SetPosUCur(0, _pos);
    WriteDeltasU();
}

void WindowRegistrator::ChangePosUCur1Ch1(int _pos) 
{
	double step = mathDisplay.GetMaxVoltage(voltages[cbU1->currentIndex()].range) / sbUCur1Ch1->maximum();
	double absVolts = step * _pos;
	leUCur1Ch1->setText(VoltageToString(absVolts));
	display1->SetPosUCur(1, _pos);
	WriteDeltasU();
}

void WindowRegistrator::close() 
{
    emit terminate();
    if(timer != NULL) 
    {
        timer->stop();
        SAFE_DELETE(timer);
        SaveSettings();
    }
    SAFE_DELETE(display0);
    SAFE_DELETE(display1);
    QMainWindow::close();
}

void WindowRegistrator::SetColorBackground() 
{
    QColor colorBackground = QColorDialog::getColor();
    if(colorBackground.isValid()) 
    {
        display0->SetColorBackground(colorBackground);
        display1->SetColorBackground(colorBackground);
    }
}

void WindowRegistrator::SetColorGrid() 
{
    QColor colorGrid = QColorDialog::getColor();
    if(colorGrid.isValid()) 
    {
        display0->SetColorGrid(colorGrid);
        display1->SetColorGrid(colorGrid);
    }
}

void WindowRegistrator::SetColorSignal() 
{
    QColor colorSignal = QColorDialog::getColor();
    if(colorSignal.isValid()) 
    {
        display0->SetColorSignal(colorSignal);
        display1->SetColorSignal(colorSignal);
    }
}

void WindowRegistrator::ChangePosUCurFromMouse0(int _numCur, int _deltaY) 
{
    QSpinBox *spinbox = (_numCur == 0) ? sbUCur0Ch0 : sbUCur1Ch0;
    spinbox->setValue(spinbox->value() + _deltaY);
}

void WindowRegistrator::ChangePosUCurFromMouse1(int _numCur, int _deltaY) 
{
    QSpinBox *spinbox = (_numCur == 0) ? sbUCur0Ch1 : sbUCur1Ch1;
    spinbox->setValue(spinbox->value() + _deltaY);
}

void WindowRegistrator::Calibration() 
{
    D129_Stop(numDevice);

    uchar buffer[512];

    int rez = QMessageBox::information(NULL, tr("Важное сообщение"), tr("Подключите выход калибратора к первому каналу и нажмите кнопку Ок \
                                                                        для проведения калибровки или Cancel для отмены"), QMessageBox::Ok | QMessageBox::Cancel);
    if(rez == QMessageBox::Ok)
    {
        if(0 == D129_Calibrate(numDevice, 0, buffer))
            QMessageBox::information(NULL, tr("Важное сообщение"), tr("Первый канал не откалиброван"), QMessageBox::Ok);
    }

    rez = QMessageBox::information(NULL, tr("Важное сообщение"), tr("Подключите выход калибратора ко второму каналу и нажмите кнопку Ок \
                                                                    для проведения калибровки или Cancel для отмены"), QMessageBox::Ok | QMessageBox::Cancel);
    if(rez == QMessageBox::Ok)
    {
        if(0 == D129_Calibrate(numDevice, 1, buffer))
            QMessageBox::information(NULL, tr("Важное сообщение"), tr("Второй канал не откалиброван"), QMessageBox::Ok);
    }

    QMessageBox::information(NULL, tr("Важное сообщение"), tr("Калибровка завершена"), QMessageBox::Ok);
}

void WindowRegistrator::closeEvent(QCloseEvent *) 
{
    close();
}

void WindowRegistrator::UpdatePorts() 
{
    disconnect(cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetNumberPort(int)));
    cbPort->clear();
    cbPort->setEnabled(false);
    update();

    numPorts = D129_GetPorts(ports, 256);
    if(numPorts < 1)
        return;

    for(int i = 0; i < numPorts; i++)
        cbPort->addItem(QString(tr("COM")) + QString("").setNum(ports[i], 10));
    
    cbPort->setCurrentIndex(numPorts - 1);
    SetNumberPort(numPorts - 1);
    connect(cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetNumberPort(int)));
    cbPort->setEnabled(true);
}

void WindowRegistrator::LoadSettings() 
{
    ifstream file(CALIBRATE_NAME_FILE);
    if(!file.fail()) {
        double value = 0.0;
        if(file >> value)
            D129_SetKoeffCalibrate(numDevice, 0, value);
        else 
            //ERROR_LOG("Error input koeff calibrate from file");
            ;
        
        if(file >> value)
            D129_SetKoeffCalibrate(numDevice, 1, value);
        else
            //ERROR_LOG("Error input koeff calibrate from file");
            ;
        
        if(file >>value) 
            D129_SetAddr_Min(numDevice, value);
        else
            //ERROR_LOG("Error input addr min for randomize from file");
            ;
        
        if(file >> value)
            D129_SetAddr_Max(numDevice, value);
        else
            //ERROR_LOG("Error input addr max for randomize from file");
            ;
    }
    else 
        //LOG("Calibrate file not found");
        ;

    SetCommonSettings();
    
    if(configurator->IsExist())
        LoadConfigSettings();
    else
        LoadDefaultSettings();

    LoadDefaultColors();
}

void WindowRegistrator::LoadDefaultSettings() 
{
    cbTime->setCurrentIndex(0);
    cbU0->setCurrentIndex(NS_TypesAltera::Range_1v);
    cbU1->setCurrentIndex(NS_TypesAltera::Range_1v);
}

//  [2/27/2010 Sasha]
void WindowRegistrator::LoadConfigSettings() {
    cbTime->setCurrentIndex(configurator->LoadParameter(P_CBKoeffRazv));
    cbU0->setCurrentIndex(configurator->LoadParameter(P_Range0));
    cbU1->setCurrentIndex(configurator->LoadParameter(P_Range1));
    int lengthRecord = configurator->LoadParameter(P_CBLengthRecord);
    cbLengthRecord->setCurrentIndex((lengthRecord + 1) % 3);
    cbLengthRecord->setCurrentIndex(lengthRecord);
}

//  [2/27/2010 Sasha]
void WindowRegistrator::LoadDefaultColors() {

}

void WindowRegistrator::SaveSettings() 
{
    configurator->CreateConfig();
    configurator->SaveParameter(P_CBKoeffRazv, cbTime->currentIndex());
    configurator->SaveParameter(P_Range0, cbU0->currentIndex());
    configurator->SaveParameter(P_Range1, cbU1->currentIndex());
    configurator->SaveParameter(P_CBLengthRecord, cbLengthRecord->currentIndex());
    configurator->CloseConfig();

    ofstream file(CALIBRATE_NAME_FILE, ios::out);
    file << setprecision(15) << D129_GetKoeffCalibrate(numDevice, 0) << endl;
    file << D129_GetKoeffCalibrate(numDevice, 1) << endl;
    file << D129_GetAddr_Min(numDevice) << endl;
    file << D129_GetAddr_Max(numDevice) << endl;
    file.close();
}

//  [11/30/2009 Sasha]
void WindowRegistrator::ConnectingSlots() {
    connect(pbStart, SIGNAL(toggled(bool)), this, SLOT(Start(bool)));
    connect(pbUpdatePorts, SIGNAL(clicked()), this, SLOT(UpdatePorts()));
    connect(cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetNumberPort(int)));
    connect(cbU0, SIGNAL(currentIndexChanged(int)), this, SLOT(SetRange0(int)));
    connect(cbU1, SIGNAL(currentIndexChanged(int)), this, SLOT(SetRange1(int)));
    connect(hsbCh0, SIGNAL(valueChanged(int)), this, SLOT(SB0valueChanged(int)));
    connect(hsbCh1, SIGNAL(valueChanged(int)), this, SLOT(SB1valueChanged(int)));
    connect(pbOpen, SIGNAL(clicked(bool)), this, SLOT(Open(bool)));
    connect(pbDir, SIGNAL(clicked(bool)), this, SLOT(SetDir(bool)));
    connect(display0, SIGNAL(SignalMoveSignal(int)), this, SLOT(MoveSignal0(int)));
    connect(display1, SIGNAL(SignalMoveSignal(int)), this, SLOT(MoveSignal1(int)));

	// Курсоры
	connect(chbUCursCh0, SIGNAL(toggled(bool)), this, SLOT(EnableUCursCh0(bool)));
	connect(chbUCursCh1, SIGNAL(toggled(bool)), this, SLOT(EnableUCursCh1(bool)));
	connect(sbUCur0Ch0, SIGNAL(valueChanged(int)), this, SLOT(ChangePosUCur0Ch0(int)));
	connect(sbUCur1Ch0, SIGNAL(valueChanged(int)), this, SLOT(ChangePosUCur1Ch0(int)));
	connect(sbUCur0Ch1, SIGNAL(valueChanged(int)), this, SLOT(ChangePosUCur0Ch1(int)));
	connect(sbUCur1Ch1, SIGNAL(valueChanged(int)), this, SLOT(ChangePosUCur1Ch1(int)));

    connect(aExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(aColorBackground, SIGNAL(triggered()), this, SLOT(SetColorBackground()));
    connect(aColorGrid, SIGNAL(triggered()), this, SLOT(SetColorGrid()));
    connect(aColorSignal, SIGNAL(triggered()), this, SLOT(SetColorSignal()));
    connect(aCalibrate, SIGNAL(triggered()), this, SLOT(Calibration()));

    connect(reinterpret_cast<QObject*>(display0->signalScreen), SIGNAL(ChangePosUCur(int, int)), this, SLOT(ChangePosUCurFromMouse0(int, int)));
    connect(reinterpret_cast<QObject*>(display1->signalScreen), SIGNAL(ChangePosUCur(int, int)), this, SLOT(ChangePosUCurFromMouse1(int, int)));
}

//  [11/30/2009 Sasha]
void WindowRegistrator::ConfigureWidgets() {
    QTextCodec *rusCodec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(rusCodec);

    times.push_back(ComboTime(tr("1с/дел"), 20));
    times.push_back(ComboTime(tr("2.5с/дел"), 50));
    times.push_back(ComboTime(tr("5с/дел"), 100));
    times.push_back(ComboTime(tr("10с/дел"), 200));
    times.push_back(ComboTime(tr("25с/дел"), 500));
    times.push_back(ComboTime(tr("50с/дел"), 1000));
    times.push_back(ComboTime(tr("100с/дел"), 2000));
    times.push_back(ComboTime(tr("250с/дел"), 5000));
    times.push_back(ComboTime(tr("500с/дел"), 10000));
    times.push_back(ComboTime(tr("1500с/дел"), 30000));
    times.push_back(ComboTime(tr("3000с/дел"), 60000));

    // Заполняем комбобокс времён
    for(size_t i = 0; i < times.size(); i++) {
        cbTime->addItem(times[i].string);
    }

    voltages.push_back(ComboVoltage(tr("5мВ/дел"), 0));
    voltages.push_back(ComboVoltage(tr("10мВ/дел"), 1));
    voltages.push_back(ComboVoltage(tr("20мВ/дел"), 2));
    voltages.push_back(ComboVoltage(tr("50мВ/дел"), 3));
    voltages.push_back(ComboVoltage(tr("100мВ/дел"), 4));
    voltages.push_back(ComboVoltage(tr("200мВ/дел"), 5));
    voltages.push_back(ComboVoltage(tr("500мВ/дел"), 6));
    voltages.push_back(ComboVoltage(tr("1В/дел"), 7));
    voltages.push_back(ComboVoltage(tr("2В/дел"), 8));
    voltages.push_back(ComboVoltage(tr("5В/дел"), 9));

    cbU0->clear();
    cbU1->clear();
    for(size_t i = 0; i < voltages.size(); i++) {
        cbU0->addItem(voltages[i].string);
        cbU1->addItem(voltages[i].string);
    }

    lengths.push_back(ComboLengthRecord(tr("неограничено"), 0));
    cbLengthRecord->addItem(tr("неограничено"));
    for(size_t i = 1; i <= 10; i++) {
        QString string = QString(tr("%1дел")).arg(i * 10, 0, 10);
        int num = i * 500;
        lengths.push_back(ComboLengthRecord(string, num));
        cbLengthRecord->addItem(string);
    }
    hsbCh0->setRange(-1, -1);
    hsbCh1->setRange(-1, -1);

    int height = display0->GetHeight();
    sbUCur0Ch0->setRange(-height/2, height/2);
    sbUCur1Ch0->setRange(-height/2, height/2);
    sbUCur0Ch1->setRange(-height/2, height/2);
    sbUCur1Ch1->setRange(-height/2, height/2);

    display1->ConnectToDevice(true);
}

void WindowRegistrator::SetCommonSettings() 
{
    D129_SetRange(numDevice, 0, NS_TypesAltera::Range_1v);
    D129_SetRange(numDevice, 1, NS_TypesAltera::Range_1v);
    D129_SetModeInput(numDevice, 0, ModeDC);
    D129_SetModeInput(numDevice, 1, ModeDC);
    D129_SetTBase(numDevice, NS_TypesAltera::TBase_1us);
    D129_SetStartMode(numDevice, NS_TypesAltera::StartMode_Auto);
    D129_SetTPos(numDevice, NS_TypesAltera::TPos_Center);
	D129_SetFiltr(numDevice, false);
	D129_SetCalibrator(numDevice, true);
    D129_SetRShiftRel(numDevice, 0, NS_TypesAltera::RShiftZero);
    D129_SetRShiftRel(numDevice, 1, NS_TypesAltera::RShiftZero);
    D129_SetTShiftRel(numDevice, NS_TypesAltera::TShiftZero);
    D129_SetTrigLevRel(numDevice, 0, NS_TypesAltera::TrigLevZero);
    D129_SetTrigLevRel(numDevice, 1, NS_TypesAltera::TrigLevZero);
    D129_SetSinchroChannel(numDevice, -1);
}

//  [12/7/2009 Sasha]
void WindowRegistrator::SetNewRanges() 
{
    int max = datas.size() - 1;
    if(max < 0) 
        max = 0;
    
    int min = 500;
    if(min > max) 
        min = max;
    
    hsbCh0->setRange(min, max);
    hsbCh1->setRange(min, max);
}

void WindowRegistrator::WriteDeltasU() 
{
    double absU0Ch0 = mathDisplay.GetMaxVoltage(voltages[cbU0->currentIndex()].range) / sbUCur0Ch0->maximum() * sbUCur0Ch0->value();
    double absU1Ch0 = mathDisplay.GetMaxVoltage(voltages[cbU0->currentIndex()].range) / sbUCur1Ch0->maximum() * sbUCur1Ch0->value();
    double delta0 = fabs(absU0Ch0 - absU1Ch0);
    double absU0Ch1 = mathDisplay.GetMaxVoltage(voltages[cbU1->currentIndex()].range) / sbUCur0Ch1->maximum() * sbUCur0Ch1->value();
    double absU1Ch1 = mathDisplay.GetMaxVoltage(voltages[cbU1->currentIndex()].range) / sbUCur1Ch1->maximum() * sbUCur1Ch1->value();
    double delta1 = fabs(absU0Ch1 - absU1Ch1);
    leDeltaUCurCh0->setText(VoltageToString(delta0));
    leDeltaUCurCh1->setText(VoltageToString(delta1));
}

void WindowRegistrator::UpdateLineEdits(QSpinBox *_spinbox) 
{
    int value = _spinbox->value();
    _spinbox->setValue(_spinbox->maximum());
    _spinbox->setValue(_spinbox->minimum());
    _spinbox->setValue(value);
}
