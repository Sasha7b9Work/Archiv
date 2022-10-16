//  [11/26/2009 Sasha]
#pragma warning(disable:4127)
#include <QTimer>
#include <QColorDialog>
#include <QTextCodec>
#pragma warning(default:4127)
#include <fstream>
#include <iostream>
#include <iomanip>
#include "WindowAnalyzer.h"
#include "Display.h"
#include "../common/GlobalFunctions.h"
#include "MathDisplayAnalyzer.h"
#include "../Device129Multi/src/Device129Multi.h"
#include "../common/TypesAltera.h"

using namespace std;

using namespace NS_TypesAltera;
using namespace NS_GlobalFunctions;
using namespace NS_Configurator;

static const char * const CONFIGURE_NAME = "analizer.cfg";

int numDevice = -1;

WindowAnalyzer::WindowAnalyzer(QMainWindow *parent):
    QMainWindow(parent),
    configurator(NULL),
    display(NULL),
    timer(NULL)
{
    setupUi(this);
    //setWindowIcon(QIcon(":/newPrefix/Spectro16.ico"));
    numDevice = D129_CreateDevice(ModeAnalyzer);
    freq[0] = freq[1] = 6;
    range[0] = range[1] = 3;
    display = new Display(frame);
    cursors = new Cursor(leFreq0, sbPos0, leFreq1, sbPos1, leDeltaFreq, leU0, leU1, leDeltaU, display->GetPointerSettings(), cbFreq, cbU);
    timer = new QTimer();
    timer->setInterval(50);
    ConfigureWidgets();
    ConnectiongSlots();
    configurator = new Configurator(CONFIGURE_NAME);
    LoadSettings();
    UpdatePorts();
    pbStart->setChecked(false);
    if(pbStart->isEnabled()) 
        pbStart->setChecked(true);
    AutoMeasures();
}

void WindowAnalyzer::Start(bool _start) 
{
    if(!pbStart->isEnabled())
        return;
  
    if(_start) 
    {
        SetCommonSettings();
        D129_SetRange(numDevice, 0, range[0]);
        D129_SetRange(numDevice, 1, range[1]);
        int channel = cbFreq->currentIndex();
        D129_SetFBase(numDevice, channel, freqArray[channel].fBase);
        D129_Start(numDevice, true);
        display->Reset();
        timer->start();
        AutoMeasures();
    }
    else 
    {
        D129_Stop(numDevice);
        timer->stop();
    }
    cbPort->setEnabled(!_start);
    pbUpdate->setEnabled(!_start);
    pbStart->setText(_start ? tr("Стоп") : tr("Старт"));
}

void WindowAnalyzer::OnTimer() 
{
    timer->stop();

    if(!D129_IsConnect(numDevice)) 
    {
        display->ConnectToDevice(false);
        pbStart->setChecked(false);
        pbStart->setEnabled(false);
        return;
    }

    if(pbStart->isChecked())
    {
        if(D129_IsRunning(numDevice)) 
        {
            unsigned char flag;
            if(D129_DataReadyForRead(numDevice, flag)) 
            {
                D129_ReadData(numDevice);
                cursors->SetData();
                display->DrawData();
                AutoMeasures();
                D129_Start(numDevice, false);
            }
        }
    }
    timer->start();
}

void WindowAnalyzer::SetNumberPort(int _indexPort) 
{
    if(_indexPort < 0)
        return;
    
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
    display->ConnectToDevice(created);
    display->Reset();
    cursors->UnselectAllLineEdits();
}

void WindowAnalyzer::Calibration() 
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

void WindowAnalyzer::SetFreq(int _freq) 
{
    int numChannel = cbCh->currentIndex();
    freq[numChannel] = _freq;
    D129_SetFBase(numDevice, numChannel, freqArray[_freq].fBase);
    display->Reset();
}

void WindowAnalyzer::SetU(int _u) 
{
    int numChannel = cbCh->currentIndex();
    range[numChannel] = _u;
    D129_SetRange(numDevice, numChannel, _u);
    display->Reset();
}

void WindowAnalyzer::SetActiveSpectr(int _spectr) 
{
    display->SetActiveSpectr(_spectr);
    D129_SetSinchroChannel(numDevice, _spectr);
    cbU->setCurrentIndex(range[_spectr]);
    cbFreq->setCurrentIndex(freq[_spectr]);
    cursors->ChangeChannel();
    display->Reset();
}

void WindowAnalyzer::SetWindow(int _window) 
{
    D129_SetWindow(numDevice, _window);
    display->Reset();
}

//  [12/1/2009 Sasha]
void WindowAnalyzer::EnableCursors(int _state) {
    bool enable = (_state == Qt::Checked);
    leFreq0->setEnabled(enable);
    sbPos0->setEnabled(enable);
    leFreq1->setEnabled(enable);
    sbPos1->setEnabled(enable);
    leDeltaFreq->setEnabled(enable);
    leU0->setEnabled(enable);
    leU1->setEnabled(enable);
    leDeltaU->setEnabled(enable);
    display->EnableUCurs(_state == Qt::Checked);
    display->Repaint();
    cursors->UnselectAllLineEdits();
}

void WindowAnalyzer::LoadSettings() 
{
    // Сначала загрузим колибровочные коэфффициенты, если есть файл с ними
    ifstream file(CALIBRATE_NAME_FILE);
    if(!file.fail()) {      // Если файл с калибровочными коэффициентами существует
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
        
        if(file >> value) 
            D129_SetAddr_Min(numDevice, value);
        else 
            //ERROR_LOG("Error input addr min for randomize from file")
            ;
     
        if(file >> value)
            D129_SetAddr_Max(numDevice, value);
        else
            //ERROR_LOG("Error input addr max for randomize from file");
            ;
        
        file.close();
    }
    else 
        //LOG("Calibrate file not found");
        ;
    
    if(configurator->IsExist()) 
        LoadConfigSettings();
    else
        LoadDefaultSettings();

    LoadDefaultColors();
}

void WindowAnalyzer::LoadDefaultColors() 
{
    display->SetColorBackground((QColor)Qt::black);
    display->SetColorCurs(0, Qt::blue);
    display->SetColorCurs(1, Qt::blue);
    display->SetColorSignal(0, Qt::white);
    display->SetColorSignal(1, Qt::yellow);
    display->SetColorGrid((QColor)Qt::yellow);
}

void WindowAnalyzer::LoadDefaultSettings() 
{
    cbCh->setCurrentIndex(1);
    cbCh->setCurrentIndex(0);
    cbWindow->setCurrentIndex(2);
    cbFreq->setCurrentIndex(3);
    cbU->setCurrentIndex(Range_1v);
    chbEnableCurs->setCheckState(Qt::Checked);
    chbEnableCurs->setCheckState(Qt::Unchecked);
    cbPort->setCurrentIndex(cbPort->count() - 1);
}

void WindowAnalyzer::LoadConfigSettings() 
{
    D129_SetAdditionRShift(numDevice, 0, configurator->LoadParameter(P_AdditionRShift0));
    D129_SetAdditionRShift(numDevice, 1, configurator->LoadParameter(P_AdditionRShift1));
    range[0] = configurator->LoadParameter(P_Range0);
    range[1] = configurator->LoadParameter(P_Range1);
    freq[0] = configurator->LoadParameter(P_IndexCBFreq0);
    freq[1] = configurator->LoadParameter(P_IndexCBFreq1);
    int channel = configurator->LoadParameter(P_SynhroChannel);
    cbCh->setCurrentIndex((channel + 1) % 2);
    cbCh->setCurrentIndex(channel);
    int window = configurator->LoadParameter(P_CBWindowAnalyzer);
    cbWindow->setCurrentIndex((window + 1) % 3);
    cbWindow->setCurrentIndex(window);
}

void WindowAnalyzer::SaveSettings() 
{
    configurator->CreateConfig();
    configurator->SaveParameter(P_AdditionRShift0, D129_GetAdditionRShift(numDevice, 0));
    configurator->SaveParameter(P_AdditionRShift1, D129_GetAdditionRShift(numDevice, 1));
    configurator->SaveParameter(P_Range0, range[0]);
    configurator->SaveParameter(P_Range1, range[1]);
    configurator->SaveParameter(P_IndexCBFreq0, freq[0]);
    configurator->SaveParameter(P_IndexCBFreq1, freq[1]);
    configurator->SaveParameter(P_SynhroChannel, cbCh->currentIndex());     // Здесь сохраняем номер канала, спектр которого выводится
    configurator->SaveParameter(P_CBWindowAnalyzer, cbWindow->currentIndex());
    configurator->CloseConfig();

    ofstream file(CALIBRATE_NAME_FILE, ios::out);
    file << setprecision(15) << D129_GetKoeffCalibrate(numDevice, 0) << endl;
    file << D129_GetKoeffCalibrate(numDevice, 1) << endl;
    file << D129_GetAddr_Min(numDevice) << endl;
    file << D129_GetAddr_Max(numDevice) << endl;
    file.close();
}

void WindowAnalyzer::UpdatePorts() 
{
    disconnect(cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetNumberPort(int)));
    cbPort->clear();
    cbPort->setEnabled(false);
    //update();
    
    numPorts = D129_GetPorts(ports, 256);
    if(numPorts <= 0)
        return;

    for(int i = 0; i < numPorts; i++)
        cbPort->addItem(QString(tr("COM")) + QString("").setNum(ports[i], 10));

    cbPort->setCurrentIndex(numPorts - 1);
    SetNumberPort(numPorts - 1);
    connect(cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetNumberPort(int)));
    cbPort->setEnabled(true);
}

//  [2/19/2010 Sasha]
void WindowAnalyzer::SetColorBackground() {
    QColor colorBackground = QColorDialog::getColor();
    if(colorBackground.isValid()) {
        display->SetColorBackground(colorBackground);
    }
}

//  [2/19/2010 Sasha]
void WindowAnalyzer::SetColorGrid() {
    QColor colorGrid = QColorDialog::getColor();
    if(colorGrid.isValid()) {
        display->SetColorGrid(colorGrid);
    }
}

//  [2/19/2010 Sasha]
void WindowAnalyzer::SetColorSignal() {
    QColor colorSignal = QColorDialog::getColor();
    if(colorSignal.isValid()) {
        display->SetColorSignal(0, colorSignal);
    }
}

void WindowAnalyzer::MovedCursor(int _cursor, int _position) 
{
    QSpinBox *sbPosCur = (_cursor == 0) ? sbPos0 : sbPos1;
    sbPosCur->setValue(_position);
}

void WindowAnalyzer::close() 
{
    emit terminate();
    SaveSettings();
    D129_DestroyDevice(numDevice);
    SAFE_DELETE(display);
    SAFE_DELETE(timer);
    SAFE_DELETE(configurator);
    SAFE_DELETE(cursors);
    QMainWindow::close();
}

void WindowAnalyzer::closeEvent(QCloseEvent *) 
{
    close();
}

void WindowAnalyzer::ConnectiongSlots() 
{
    connect(cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(SetNumberPort(int)));
    connect(pbStart, SIGNAL(toggled(bool)), this, SLOT(Start(bool)));
    connect(cbFreq, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFreq(int)));
    connect(cbU, SIGNAL(currentIndexChanged(int)), this, SLOT(SetU(int)));
    connect(cbCh, SIGNAL(currentIndexChanged(int)), this, SLOT(SetActiveSpectr(int)));
    connect(cbWindow, SIGNAL(currentIndexChanged(int)), this, SLOT(SetWindow(int)));
    connect(chbEnableCurs, SIGNAL(stateChanged(int)), this, SLOT(EnableCursors(int)));
    connect(pbUpdate, SIGNAL(clicked()), this, SLOT(UpdatePorts()));
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    connect(actionExit, SIGNAL(changed()), this, SLOT(close()));
    connect(aColorBackground, SIGNAL(triggered()), this, SLOT(SetColorBackground()));
    connect(aColorGrid, SIGNAL(triggered()), this, SLOT(SetColorGrid()));
    connect(aColorSignal, SIGNAL(triggered()), this, SLOT(SetColorSignal()));
    connect(cursors, SIGNAL(Repaint()), display, SLOT(update()));
    connect(reinterpret_cast<QObject*>(display->signalScreen), SIGNAL(MovedCursor(int, int)), this, SLOT(MovedCursor(int, int)));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(aCalibrate, SIGNAL(triggered()), this, SLOT(Calibration()));
}

void WindowAnalyzer::ConfigureWidgets() 
{
    QTextCodec *rusCodec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(rusCodec);

    freqArray.push_back(FreqSet(FBase_250Hz, tr("250Hz")));
    freqArray.push_back(FreqSet(FBase_1250Hz, tr("1.25kHz")));
    freqArray.push_back(FreqSet(FBase_2500Hz, tr("2.5kHz")));
    freqArray.push_back(FreqSet(FBase_12500Hz, tr("12.5kHz")));
    freqArray.push_back(FreqSet(FBase_25kHz, tr("25kHz")));
    freqArray.push_back(FreqSet(FBase_125kHz, tr("125kHz")));
    freqArray.push_back(FreqSet(FBase_250kHz, tr("250kHz")));
    freqArray.push_back(FreqSet(FBase_1250kHz, tr("1.25MHz")));
    freqArray.push_back(FreqSet(FBase_2500kHz, tr("2.5MHz")));
    freqArray.push_back(FreqSet(FBase_12500kHz, tr("12.5MHz")));
    freqArray.push_back(FreqSet(FBase_25MHz, tr("25MHz")));

    voltArray.push_back(VoltSet(Range_5mv, tr("25mV")));
    voltArray.push_back(VoltSet(Range_10mv, tr("50mV")));
    voltArray.push_back(VoltSet(Range_20mv, tr("100mV")));
    voltArray.push_back(VoltSet(Range_50mv, tr("250mV")));
    voltArray.push_back(VoltSet(Range_100mv, tr("500mV")));
    voltArray.push_back(VoltSet(Range_200mv, tr("1V")));
    voltArray.push_back(VoltSet(Range_500mv, tr("2.5V")));
    voltArray.push_back(VoltSet(Range_1v, tr("5V")));
    voltArray.push_back(VoltSet(Range_2v, tr("10V")));
    voltArray.push_back(VoltSet(Range_5v, tr("25V")));

    for(size_t i = 0; i < freqArray.size(); i++)
        cbFreq->addItem(freqArray[i].string);

    for(size_t i = 0; i < voltArray.size(); i++)
        cbU->addItem(voltArray[i].string);

    sbPos0->setRange(0, 250);
    sbPos1->setRange(0, 250);
    int pos0 = display->GetPointerSettings()->GetPosCur(0, 0);
    int pos1 = display->GetPointerSettings()->GetPosCur(0, 1);
    sbPos0->setValue(pos0);
    sbPos1->setValue(pos1);
}

void WindowAnalyzer::SetCommonSettings() 
{
    D129_SetModeInput(numDevice, 0, ModeAC);
    D129_SetStartMode(numDevice, StartMode_Auto);
    D129_SetTPos(numDevice, TPos_Left);
    D129_SetFiltr(numDevice, false);
    D129_SetCalibrator(numDevice, true);
    D129_SetRShiftRel(numDevice, 0, RShiftZero);
    D129_SetRShiftRel(numDevice, 1, RShiftZero);
    D129_SetTShiftRel(numDevice, TShiftZero);
    D129_SetTrigLevRel(numDevice, 0, TrigLevZero);
    D129_SetTrigLevRel(numDevice, 1, TrigLevZero);
}

void WindowAnalyzer::AutoMeasures() 
{
    static MathDisplayAnalyzer mathDisplay;
    if(D129_DataReadyForDraw(numDevice)) 
    {
        char textBuffer[256];
        int numChannel = cbCh->currentIndex();
        D129_GetU0(numDevice, numChannel, textBuffer, 255);
        leU_DC->setText(tr(textBuffer));
        D129_GetFUmax(numDevice, numChannel, textBuffer, 255);
        leFUmax->setText(tr(textBuffer));
        D129_GetUmax(numDevice, numChannel, textBuffer, 255);
        leUmax->setText(tr(textBuffer));
    }
    else 
    {
        leU_DC->setText(EMPTY_STRING);
        leFUmax->setText(EMPTY_STRING);
        leUmax->setText(EMPTY_STRING);
    }
}

