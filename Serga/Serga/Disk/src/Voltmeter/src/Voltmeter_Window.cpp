#include "Voltmeter_Window.h"
#pragma warning(disable:4127)
#include <QTimer>
#include <QFrame>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextCodec>
#include <QMessageBox>
#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <QColorDialog>
#pragma warning(default:4127)
#include <cmath>
#include <iostream>
#include <iomanip>
#include "Display.h"
#include "../../common/TablesOfConverting.h"
#include "../../common/GlobalFunctions.h"
#include "../../common/CircularStack.h"
#include "../../common/Configurator.h"
#include "../Device129Multi/src/Device129Multi.h"

using namespace NS_TypesAltera;
using namespace NS_Configurator;

using std::vector;
using namespace std;

static const char * const CONFIGURE_NAME = "voltmeter.cfg";

int numDevice = -1;

Voltmeter_Window::Voltmeter_Window(QMainWindow *_parent):
    QMainWindow(_parent),
    currentChannelMeasure(0) 
{
    setWindowIcon(QIcon(":/newPrefix/Volt16.ico"));
    numDevice = D129_CreateDevice(ModeVoltmeter);
    //D129_SetAutoCalibrate(numDevice, true);
    timer = new QTimer();
    timer->setInterval(1);
    ConfigureWidgets();
    ConnectingSlots();
    configurator = new Configurator(CONFIGURE_NAME);
    LoadSettings();
    UpdatePorts();
    if(pbStart->isEnabled())
        pbStart->setChecked(true);

    timerCalibrateZero = new QTimer();
    timerCalibrateZero->setInterval(1000 * 120);
    timerCalibrateZero->start();

    connect(timerCalibrateZero, SIGNAL(timeout()), this, SLOT(OnTimerCalibrateZero()));
    OnTimerCalibrateZero();
}

void Voltmeter_Window::Start(bool _start) 
{
    if(!pbStart->isEnabled()) 
        return;
    
    if(_start) 
    {
        SetCommonSettings();
        SetSettings();
        StartAltera(currentChannelMeasure);
        timer->start();
    }
    else 
    {
        D129_Stop(numDevice);
        timer->stop();
    }
    pbUpdate->setEnabled(!_start);
    cbPort->setEnabled(!_start);
    pbStart->setText(_start ? tr("Стоп") : tr("Старт"));
}

void Voltmeter_Window::OnTimer() 
{
    timer->stop();
    if(!D129_IsConnect(numDevice)) 
    {
        displays[0]->ConnectToDevice(false);
        displays[1]->ConnectToDevice(false);
        pbStart->setChecked(false);
        pbStart->setEnabled(false);
        return;
    }

    static unsigned char flag;

    if(pbStart->isChecked()) 
        if(D129_IsConnect(numDevice)) 
            if(D129_DataReadyForRead(numDevice, flag)) 
            {
                D129_ReadData(numDevice);
                displays[currentChannelMeasure]->DrawData();
                currentChannelMeasure = (currentChannelMeasure == 0) ? 1 : 0;
                StartAltera(currentChannelMeasure);
            }

    timer->start();
}

bool Voltmeter_Window::SetComPort(int _indexPort) 
{
    if((_indexPort < 0) ||(_indexPort > (cbPort->count() - 1))) 
        return false; 
    
    D129_Stop(numDevice);
    bool created = false;
    int counter = 0;

    do 
    {
        created = D129_ConnectDeviceToPort(numDevice, ports[_indexPort]);
        counter++;
    } while ((counter < 5) && (!created));

    pbStart->setEnabled(created);
    D129_SetSettings(numDevice);

    for(size_t i = 0; i < displays.size(); i++) 
        displays[i]->ConnectToDevice(created);
    
    return created;
}

void Voltmeter_Window::SetU0(int _u) 
{
    D129_Stop(numDevice);
    int range = voltArray[_u].range;
    D129_SetRange(numDevice, 0, range);
    D129_CalibrateZero(numDevice);
    if(pbStart->isChecked())
        D129_Start(numDevice, true);
}

void Voltmeter_Window::SetU1(int _u) 
{
    D129_Stop(numDevice);
    int range = voltArray[_u].range;
    D129_SetRange(numDevice, 1, range);
    D129_CalibrateZero(numDevice);
    if(pbStart->isChecked())
        D129_Start(numDevice, true);
}

void Voltmeter_Window::SetSinchroChannel(int _channel) 
{
    D129_SetSinchroChannel(numDevice, _channel);
}

void Voltmeter_Window::SetDC_Mode0(bool _dc0) 
{
    pbAC0->setText(_dc0 ? tr("ПС") : tr("ВЧ"));
    if(_dc0)
        D129_SetModeInput(numDevice, 0, ModeDC);
    else
        D129_SetModeInput(numDevice, 0, ModeAC);
}

void Voltmeter_Window::SetDC_Mode1(bool _dc1) 
{
    pbAC1->setText(_dc1 ? tr("ПС") : tr("ВЧ"));
    if(_dc1)
        D129_SetModeInput(numDevice, 1, ModeDC);
    else
        D129_SetModeInput(numDevice, 1, ModeAC);
}

void Voltmeter_Window::SetColorBackground() 
{
    QColor color = QColorDialog::getColor();
    if(color.isValid()) 
    {
        displays[0]->SetColorBackground(color);
        displays[1]->SetColorBackground(color);
    }
}

void Voltmeter_Window::SetColorSignal() 
{
    QColor color = QColorDialog::getColor();
    if(color.isValid()) 
    {
        displays[0]->SetColorSignal(color);
        displays[1]->SetColorSignal(color);
    }
}

void Voltmeter_Window::SetColorGrid() 
{
    QColor color = QColorDialog::getColor();
    if(color.isValid()) 
    {
        displays[0]->SetColorGrid(color);
        displays[1]->SetColorGrid(color);
    }
}

void Voltmeter_Window::SetColorFont() 
{
    QColor color = QColorDialog::getColor();
    if(color.isValid()) 
    {
        displays[0]->SetColorFont(color);
        displays[1]->SetColorFont(color);
    }
}

void Voltmeter_Window::Calibrate() 
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
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
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}

void Voltmeter_Window::closeEvent(QCloseEvent * /* e */) 
{
    SaveSettings();
    delete timer;
    D129_DestroyDevice(numDevice);
    for(size_t i = 0; i < displays.size(); i++) 
        delete displays[i];
}

void Voltmeter_Window::UpdatePorts() 
{
    for(size_t i = 0; i < displays.size(); i++)
        displays[i]->ConnectToDevice(false);

    disconnect(cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetComPort(int)));
    cbPort->clear();

    numPorts = D129_GetPorts(ports, 256);
    if(numPorts <= 0)
        return;

    for(int i = 0; i < numPorts; i++)
        cbPort->addItem(QString(tr("COM")) + QString("").setNum(ports[i], 10));

    cbPort->setCurrentIndex(numPorts - 1);
    SetComPort(numPorts - 1);
    connect(cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetComPort(int)));
}

void Voltmeter_Window::ConnectingSlots() 
{
    connect(cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetComPort(int)));
    connect(pbUpdate, SIGNAL(pressed()), this, SLOT(UpdatePorts()));
    connect(pbStart, SIGNAL(toggled(bool)), this, SLOT(Start(bool)));
    connect(cbU0, SIGNAL(currentIndexChanged(int)), this, SLOT(SetU0(int)));
    connect(cbU1, SIGNAL(currentIndexChanged(int)), this, SLOT(SetU1(int)));
    connect(pbAC0, SIGNAL(toggled(bool)), this, SLOT(SetDC_Mode0(bool)));
    connect(pbAC1, SIGNAL(toggled(bool)), this, SLOT(SetDC_Mode1(bool)));
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}

void Voltmeter_Window::ConfigureWidgets() 
{
    QTextCodec *rusCodec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(rusCodec);

	setWindowTitle(tr("Вольтметр"));
    QFrame *mainFrame = new QFrame;
    setCentralWidget(mainFrame);

    QFrame *frame0 = new QFrame;
    QFrame *frame1 = new QFrame;
    frame0->setFrameShadow(QFrame::Sunken);
    frame0->setFrameShape(QFrame::StyledPanel);
    frame1->setFrameShadow(QFrame::Sunken);
    frame1->setFrameShape(QFrame::StyledPanel);

    displays.push_back(new Display(frame0, 0));
    displays.push_back(new Display(frame1, 1));

    QHBoxLayout *mainLayout = new QHBoxLayout;

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(frame0);
    leftLayout->addWidget(frame1);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    cbPort = new QComboBox;
    QLabel *lPort = new QLabel(tr("Порт"));
    pbUpdate = new QPushButton(tr("Обновить"));
    hLayout1->addWidget(cbPort);
    hLayout1->addWidget(lPort);
    hLayout1->addWidget(pbUpdate);
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    QSpacerItem *spacer1 = new QSpacerItem(110, 2, QSizePolicy::Minimum, QSizePolicy::Maximum);
    pbStart = new QPushButton(tr("Старт"));
    pbStart->setCheckable(true);
    hLayout2->addSpacerItem(spacer1);
    hLayout2->addWidget(pbStart);

    QGroupBox *gb1 = new QGroupBox(tr("Канал 1"));
    QHBoxLayout *hLayout3 = new QHBoxLayout;
    QLabel *labelFreq1 = new QLabel(tr("Диап f1"));
    labelFreq1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    cbFreq0 = new QComboBox;
    //QSpacerItem *spacer2 = new QSpacerItem(100, 2, QSizePolicy::Minimum, QSizePolicy::Maximum);
    //hLayout3->addSpacerItem(spacer2);
    hLayout3->addWidget(labelFreq1);
    hLayout3->addWidget(cbFreq0);
    QHBoxLayout *hLayout4 = new QHBoxLayout;
    pbAC0 = new QPushButton(tr("ВЧ"));
    pbAC0->setMaximumWidth(24);
    QLabel *labelU1 = new QLabel(tr("Диап U1"));
    labelU1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    cbU0 = new QComboBox;
    hLayout4->addWidget(pbAC0);
    hLayout4->addWidget(labelU1);
    hLayout4->addWidget(cbU0);
    QVBoxLayout *vLayout1 = new QVBoxLayout;
    vLayout1->addLayout(hLayout3);
    vLayout1->addLayout(hLayout4);
    gb1->setLayout(vLayout1);

    QGroupBox *gb2 = new QGroupBox(tr("Канал 2"));
    QHBoxLayout *hLayoutFreq2 = new QHBoxLayout;
    QLabel *labelFreq2 = new QLabel(tr("Диап f2"));
    labelFreq2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    cbFreq1 = new QComboBox;
    hLayoutFreq2->addWidget(labelFreq2);
    hLayoutFreq2->addWidget(cbFreq1);
    QHBoxLayout *hLayoutU2 = new QHBoxLayout;
    pbAC1 = new QPushButton(tr("ВЧ"));
    pbAC1->setMaximumWidth(24);
    QLabel *labelU2 = new QLabel(tr("Диап U2"));
    labelU2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    cbU1 = new QComboBox;
    hLayoutU2->addWidget(pbAC1);
    hLayoutU2->addWidget(labelU2);
    hLayoutU2->addWidget(cbU1);
    QVBoxLayout *vLayoutCh2 = new QVBoxLayout;
    vLayoutCh2->addLayout(hLayoutFreq2);
    vLayoutCh2->addLayout(hLayoutU2);
    gb2->setLayout(vLayoutCh2);
    
    rightLayout->addLayout(hLayout1);
    rightLayout->addLayout(hLayout2);
    QSpacerItem *emptySpacer = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);
    rightLayout->addSpacerItem(emptySpacer);
    rightLayout->addWidget(gb1);
    rightLayout->addWidget(gb2);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    centralWidget()->setLayout(mainLayout);
    setMaximumWidth(100);
    setMaximumHeight(100);

    static int widthCombo = 60;
    cbFreq0->setMaximumWidth(widthCombo);
    cbFreq1->setMaximumWidth(widthCombo);
    cbU0->setMaximumWidth(widthCombo);
    cbU1->setMaximumWidth(widthCombo);

    static int widthButton  = 70;
    pbUpdate->setMaximumWidth(widthButton);
    pbStart->setMaximumWidth(widthButton);
    pbStart->setMinimumWidth(widthButton);
	pbStart->setEnabled(false);

    freqArray.push_back(FreqSet(TBase_20ms, tr("100Hz")));
    freqArray.push_back(FreqSet(TBase_2ms, tr("1kHz")));
    freqArray.push_back(FreqSet(TBase_200us, tr("10kHz")));
    freqArray.push_back(FreqSet(TBase_20us, tr("100kHz")));
    freqArray.push_back(FreqSet(TBase_2us, tr("1MHz")));
    freqArray.push_back(FreqSet(TBase_200ns, tr("10MHz")));

    voltArray.push_back(VoltSet(Range_20mv, tr("40mV")));
    voltArray.push_back(VoltSet(Range_50mv, tr("100mV")));
    voltArray.push_back(VoltSet(Range_100mv, tr("200mV")));
    voltArray.push_back(VoltSet(Range_200mv, tr("400mV")));
    voltArray.push_back(VoltSet(Range_500mv, tr("1V")));
    voltArray.push_back(VoltSet(Range_1v, tr("2V")));
    voltArray.push_back(VoltSet(Range_2v, tr("4V")));
    voltArray.push_back(VoltSet(Range_5v, tr("10V")));
    
    size_t size = freqArray.size();
    for(size_t i = 0; i < size; i++) {
        cbFreq0->addItem(freqArray[i].string);
        cbFreq1->addItem(freqArray[i].string);
    }

    size = voltArray.size();
    for(size_t i = 0; i < size; i++) 
    {
        cbU0->addItem(voltArray[i].string);
        cbU1->addItem(voltArray[i].string);
    }

    pbStart->setCheckable(true);
    pbAC0->setCheckable(true);
    pbAC1->setCheckable(true);
    CreateMenu();
}

void Voltmeter_Window::CreateMenu() 
{
    // Меню "Файл"
    QAction *actionExit = new QAction(tr("&Выход"), this);
    actionExit->setStatusTip(tr("Выход"));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    QMenu *menuFile = menuBar()->addMenu(tr("&Файл"));
    menuFile->addAction(actionExit);

    // Меню "Настройки"
    QMenu *menuSettings = menuBar()->addMenu(tr("&Настройки"));
    QMenu *menuColors = new QMenu(tr("&Цвет"));
    menuSettings->addMenu(menuColors);
    QAction *actionColBackground = new QAction(tr("Фон"), this);
    QAction *actionColSignal = new QAction(tr("Сигнал"), this);
    QAction *actionColGrid = new QAction(tr("Сетка"), this);
    QAction *actionFont = new QAction(tr("Текст"), this);
    menuColors->addAction(actionColBackground);
    menuColors->addAction(actionColSignal);
    menuColors->addAction(actionColGrid);
    menuColors->addAction(actionFont);
    connect(actionColBackground, SIGNAL(triggered()), this, SLOT(SetColorBackground()));
    connect(actionColSignal, SIGNAL(triggered()), this, SLOT(SetColorSignal()));
    connect(actionColGrid, SIGNAL(triggered()), this, SLOT(SetColorGrid()));
    connect(actionFont, SIGNAL(triggered()), this, SLOT(SetColorFont()));
    QAction *actionCalibrate = new QAction(tr("Калибровать"), this);
    menuSettings->addAction(actionCalibrate);
    connect(actionCalibrate, SIGNAL(triggered()), this, SLOT(Calibrate()));
}

void Voltmeter_Window::SetCommonSettings() 
{
    if(pbAC0->isChecked())
        D129_SetModeInput(numDevice, 0, ModeDC);
    else
        D129_SetModeInput(numDevice, 0, ModeAC);
    if(pbAC1->isChecked())
        D129_SetModeInput(numDevice, 1, ModeDC);
    else
        D129_SetModeInput(numDevice, 1, ModeAC);

    D129_SetTPos(numDevice, TPos_Left);
    D129_SetFiltr(numDevice, false);
    D129_SetCalibrator(numDevice, true);
    D129_SetRShiftRel(numDevice, 0, RShiftZero);
    D129_SetRShiftRel(numDevice, 1, RShiftZero);
    D129_SetTShiftRel(numDevice, TShiftZero);
    D129_SetTrigLevRel(numDevice, 0, TrigLevZero);
    D129_SetTrigLevRel(numDevice, 1, TrigLevZero);
}

void Voltmeter_Window::SetSettings() 
{
    D129_SetRange(numDevice, 0, voltArray[cbU0->currentIndex()].range);
    D129_SetRange(numDevice, 1, voltArray[cbU1->currentIndex()].range);
    D129_SetSettings(numDevice);
}

void Voltmeter_Window::StartAltera(int _channel) 
{
    QComboBox *cbFreq = (_channel == 0) ? cbFreq0 : cbFreq1;
    int tBase = freqArray[cbFreq->currentIndex()].tBase;
    D129_SetTBase(numDevice, tBase);
    D129_SetStartMode(numDevice, StartMode_Auto);
    D129_Start(numDevice, true);
}

void Voltmeter_Window::LoadSettings() 
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
    {
        //LOG("Calibrate file not found");
    }

    if(configurator->IsExist()) 
        LoadConfigSettings();
    else 
        LoadDefaultSettings();
}

void Voltmeter_Window::LoadDefaultSettings() 
{
    cbFreq0->setCurrentIndex(3);
    cbFreq1->setCurrentIndex(3);
    cbU0->setCurrentIndex(5);
    cbU1->setCurrentIndex(4);
}

void Voltmeter_Window::LoadConfigSettings() 
{
    D129_SetAdditionRShift(numDevice, 0, configurator->LoadParameter(P_AdditionRShift0));
    D129_SetAdditionRShift(numDevice, 1, configurator->LoadParameter(P_AdditionRShift1));
    cbU0->setCurrentIndex(configurator->LoadParameter(P_Range0));
    cbU1->setCurrentIndex(configurator->LoadParameter(P_Range1));

    int modeInput0 = configurator->LoadParameter(P_ModeInput0);
    int modeInput1 = configurator->LoadParameter(P_ModeInput1);

    pbAC0->setChecked(ModeAC != modeInput0);
    D129_SetModeInput(numDevice, 0, modeInput0);

    pbAC1->setChecked(ModeAC != modeInput1);
    D129_SetModeInput(numDevice, 1, modeInput1);

    cbFreq0->setCurrentIndex(configurator->LoadParameter(P_IndexCBFreq0));
    cbFreq1->setCurrentIndex(configurator->LoadParameter(P_IndexCBFreq1));

    //D129_SetSinchroType(numDevice, Sinchro_Back);
}

void Voltmeter_Window::SaveSettings() 
{
    configurator->CreateConfig();
    configurator->SaveParameter(P_AdditionRShift0, D129_GetAdditionRShift(numDevice, 0));
    configurator->SaveParameter(P_AdditionRShift1, D129_GetAdditionRShift(numDevice, 1));
    configurator->SaveParameter(P_Range0, cbU0->currentIndex());
    configurator->SaveParameter(P_Range1, cbU1->currentIndex());
    configurator->SaveParameter(P_ModeInput0, D129_GetModeInput(numDevice, 0));
    configurator->SaveParameter(P_ModeInput1, D129_GetModeInput(numDevice, 1));
    configurator->SaveParameter(P_IndexCBFreq0, cbFreq0->currentIndex());
    configurator->SaveParameter(P_IndexCBFreq1, cbFreq1->currentIndex());
    configurator->CloseConfig();

    ofstream file(CALIBRATE_NAME_FILE, ios::out);
    file << setprecision(15) << D129_GetKoeffCalibrate(numDevice, 0) << endl;
    file << D129_GetKoeffCalibrate(numDevice, 1) << endl;
    file << D129_GetAddr_Min(numDevice) << endl;
    file << D129_GetAddr_Max(numDevice) << endl;
    file.close();
}

void Voltmeter_Window::OnTimerCalibrateZero()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    bool running = D129_IsRunning(numDevice);
    D129_Stop(numDevice);
    D129_CalibrateZero(numDevice);
    if(running)
        D129_Start(numDevice, false);
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}