#include "../../Device129Multi/src/Device129Multi.h"
#include "../../common/defines.h"
#pragma warning(disable:4127)
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QDate>
#include <QFileDialog>
#include <QMessageBox>
#pragma warning(default:4127)
#include <iostream>
#include <iomanip>
#include <fstream>
#include "WindowOsci.h"
#include "RunScreen.h"
#include "Display.h"
#include "../../common/TypesAltera.h"
#include "Tuner.h"
#include "../../common/Configurator.h"
#include "TrigScreen.h"
#include "Slider.h"
#include "../../common/MathDisplayOscilloscope.h"
#include "../../common/Timer.h"
#include "../../common/BinaryMathematics.h"
#include "ThreadAutoSearch.h"

using namespace NS_TypesAltera;
using namespace NS_Configurator;
using namespace NS_BinaryMathematics;

using std::fstream;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;
using std::setprecision;
//using std::cout;
using std::endl;

extern MathDisplayOscilloscope mathDisplay;

static const char * const CONFIGURE_FILE = "oscilloscope.cfg";

int numDevice = -1;

WindowOsci::WindowOsci(QMainWindow *_parent /* = 0 */) : QMainWindow(_parent), 
    trigScreen(0), 
    runScreen(0), 
    display(0), 
    numPorts(0),
    configurator(0),
    timer(0)
{
    setupUi(this);

    CreateWidgets();
    ConfigurationWidgets();
    CreateMenu();

    tuner = new Tuner(this);

    ConnectingSlots();

    numDevice = D129_CreateDevice(ModeOscilloscope);

    configurator = new Configurator(CONFIGURE_FILE);

    LoadSettings();

    threadSearch = new ThreadAutoSearch(this);

    timer = new QTimer();
    timer->setInterval(1);
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));

    timerCalibrateZero = new QTimer();
    timerCalibrateZero->setInterval(1000 * 120);
    timerCalibrateZero->start();

    UpdatePorts();
    pbStart->setChecked(false);
    if(pbStart->isEnabled())
        pbStart->setChecked(true);

    if(tabWidget->currentIndex() == 0 && cbMeasures->currentIndex() >= 5)
        connect(timerCalibrateZero, SIGNAL(timeout()), this, SLOT(OnCalibrateZero()));
    else
        disconnect(timerCalibrateZero, SIGNAL(timeout()), this, SLOT(OnCalibrateZero()));

    OnCalibrateZero();
}

WindowOsci::~WindowOsci()
{
    SAFE_DELETE(configurator);
}

void WindowOsci::CreateWidgets()
{
    display = new Display(frame);

    runScreen = new RunScreen(groupBox_3);
    runScreen->move(10, 25);

    trigScreen = new TrigScreen(groupBox_3);
    trigScreen->move(10, 60);

    vsTrigLev = new Slider(Qt::Vertical);
    vsTrigLev->setParent(centralwidget);
    vsTrigLev->move(5, 10);
    vsTrigLev->resize(16, 506);

    vsRShift0 = new Slider(Qt::Vertical);
    vsRShift0->setParent(centralwidget);
    vsRShift0->move(550, 10);
    vsRShift0->resize(16, 251);

    vsRShift1 = new Slider(Qt::Vertical);
    vsRShift1->setParent(centralwidget);
    vsRShift1->move(550, 270);
    vsRShift1->resize(16, 251);
}

void WindowOsci::ConfigurationWidgets() 
{
    sbTrigLev->setRange(TrigLevMin, TrigLevMax);
    vsTrigLev->setRange(TrigLevMin, TrigLevMax);

    sbTrigLev->setSingleStep(singleStep);
    vsTrigLev->setSingleStep(singleStep);
    vsTrigLev->setPageStep(singleStep * 10);

    sbRShift0->setRange(RShiftMin, RShiftMax);
    vsRShift0->setRange(RShiftMin, RShiftMax);
    sbRShift0->setSingleStep(singleStep);
    vsRShift0->setSingleStep(singleStep);
    vsRShift0->setPageStep(singleStep * 10);

    sbRShift1->setRange(RShiftMin, RShiftMax);
    vsRShift1->setRange(RShiftMin, RShiftMax);
    sbRShift1->setSingleStep(singleStep);
    vsRShift1->setSingleStep(singleStep);
    vsRShift1->setPageStep(singleStep * 10);

    sbTShift->setRange(TShiftMin, TShiftMax);
    hsTShift->setRange(TShiftMin, TShiftMax);
    sbTShift->setSingleStep(1);
    hsTShift->setSingleStep(1);
    hsTShift->setPageStep(25);

    cbMeasures->addItem(QString(tr("Частота")));
    cbMeasures->addItem(QString(tr("Период")));
    cbMeasures->addItem(QString(tr("Длительность импульса")));
    cbMeasures->addItem(QString(tr("Длительность паузы")));
    cbMeasures->addItem(QString(tr("Скважность")));
    cbMeasures->addItem(QString(tr("Минимальное значение напряжения")));
    cbMeasures->addItem(QString(tr("Максимально значение напряжения")));
    cbMeasures->addItem(QString(tr("Размах напряжения")));
    cbMeasures->addItem(QString(tr("Действующее напряжение")));

    chbTestSignalEnable->setVisible(false);
    cbTestSignalType->setVisible(false);
    dsbTestSignalFreq->setVisible(false);
    dsbTestSignalAmplitude->setVisible(false);
}

void WindowOsci::CreateMenu() 
{
    // Меню "Файл"
    QMenu *menuFile = menuBar()->addMenu(tr("Файл"));
    QAction *actionSave = new QAction(tr("Сохранить"), this);
    menuFile->addAction(actionSave);
    connect(actionSave, SIGNAL(triggered()), this, SLOT(SaveScreen()));
    QAction *actionExit = new QAction(tr("Выход"), this);
    actionExit->setStatusTip(tr("Выход"));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    menuFile->addAction(actionExit);

    // Меню "Настройки"
    QMenu *menuSettings = menuBar()->addMenu(tr("Настройки"));
    QMenu *menuScreen = new QMenu(tr("Экран"));
    menuSettings->addMenu(menuScreen);
    QMenu *menuColors = new QMenu(tr("Цвет"));
    menuScreen->addMenu(menuColors);
    QAction *actionColBackground = new QAction(tr("Фон"), this);
    menuColors->addAction(actionColBackground);
    connect(actionColBackground, SIGNAL(triggered()), display, SLOT(SetColorBackground()));
    QAction *actionColGrid = new QAction(tr("Сетка"), this);
    menuColors->addAction(actionColGrid);
    connect(actionColGrid, SIGNAL(triggered()), display, SLOT(SetColorGrid()));
    QAction *actionColSignal0 = new QAction(tr("Канал 1"), this);
    menuColors->addAction(actionColSignal0);
    connect(actionColSignal0, SIGNAL(triggered()), display, SLOT(SetColorSignal0()));
    QAction *actionColSignal1 = new QAction(tr("Канал 2"), this);
    menuColors->addAction(actionColSignal1);
    connect(actionColSignal1, SIGNAL(triggered()), display, SLOT(SetColorSignal1()));
    QAction *actionColMath = new QAction(tr("Мат сигнал"), this);
    menuColors->addAction(actionColMath);
    connect(actionColMath, SIGNAL(triggered()), display, SLOT(SetColorMath()));
    QAction *actionColTPos = new QAction(tr("TPos"), this);
    menuColors->addAction(actionColTPos);
    connect(actionColTPos, SIGNAL(triggered()), display, SLOT(SetColorTPos()));
    QAction *actionColTShift = new QAction(tr("TShift"), this);
    menuColors->addAction(actionColTShift);
    connect(actionColTShift, SIGNAL(triggered()), display, SLOT(SetColorTShift()));
    QAction *actionColCurs0 = new QAction(tr("Курс кан 1"), this);
    menuColors->addAction(actionColCurs0);
    connect(actionColCurs0, SIGNAL(triggered()), display, SLOT(SetColorCurs0()));
    QAction *actionColCurs1 = new QAction(tr("Курс кан 2"), this);
    menuColors->addAction(actionColCurs1);
    connect(actionColCurs1, SIGNAL(triggered()), display, SLOT(SetColorCurs1()));

    QMenu *menuTypeGrid = new QMenu(tr("Тип сетки"));
    menuScreen->addMenu(menuTypeGrid);
    QAction *actionGridBase = new QAction(tr("Основная"), this);
    menuTypeGrid->addAction(actionGridBase);
    connect(actionGridBase, SIGNAL(triggered()), display, SLOT(SetTypeGridBase()));
    QAction *actionGridSimple = new QAction(tr("Простая"), this);
    menuTypeGrid->addAction(actionGridSimple);
    connect(actionGridSimple, SIGNAL(triggered()), display, SLOT(SetTypeGridSimple()));
    QAction *actionGridNone = new QAction(tr("Отсутствует"), this);
    menuTypeGrid->addAction(actionGridNone);
    connect(actionGridNone, SIGNAL(triggered()), display, SLOT(SetTypeGridNone()));
    QAction *actionAntialiasing = new QAction(tr("Сглаживать"), this);
    menuScreen->addAction(actionAntialiasing);
    actionAntialiasing->setCheckable(true);
    connect(actionAntialiasing, SIGNAL(toggled(bool)), display, SLOT(EnableAntialiasing(bool)));

    QMenu *menuAltera = new QMenu(tr("Модуль"));
    menuSettings->addMenu(menuAltera);
    QMenu *menuModeCalibrator = new QMenu(tr("Режим калибратора"));
    menuAltera->addMenu(menuModeCalibrator);
    QAction *actionModeCalibrator_4V_0Hz = new QAction(tr("+4 В"), this);
    menuModeCalibrator->addAction(actionModeCalibrator_4V_0Hz);
    connect(actionModeCalibrator_4V_0Hz, SIGNAL(triggered()), this, SLOT(SetModeCalibrator_4V_0Hz()));
    QAction *actionModeCalibrator_4V_5kHz = new QAction(tr("4 В 5 кГц"), this);
    menuModeCalibrator->addAction(actionModeCalibrator_4V_5kHz);
    connect(actionModeCalibrator_4V_5kHz, SIGNAL(triggered()), this, SLOT(SetModeCalibrator_4V_5kHz()));
    QAction *actionCalibrate = new QAction(tr("Калибровать"), this);
    menuAltera->addAction(actionCalibrate);
    connect(actionCalibrate, SIGNAL(triggered()), this, SLOT(Calibrate()));
    QAction *actionResetSettings = new QAction(tr("Сброс настроек"), this);
    menuSettings->addAction(actionResetSettings);
    connect(actionResetSettings, SIGNAL(triggered()), this, SLOT(LoadDefaultSettings()));

    // Меню "Дополнительно"
    QMenu *menuAdditional = menuBar()->addMenu(tr("Дополнительно"));
    QAction *actionAutoSearch = new QAction(tr("Поиск сигнала"), this);
    menuAdditional->addAction(actionAutoSearch);
    connect(actionAutoSearch, SIGNAL(triggered()), this, SLOT(AutoSearchEnter()));
}

void WindowOsci::ConnectingSlots()
{
    // Установка активного порта
    connect(pbUpdatePorts, SIGNAL(clicked()), this, SLOT(UpdatePorts()));
    connect(cbPorts, SIGNAL(currentIndexChanged(int)), this, SLOT(SetNumberPort(int)));

    // Запуск альтеры
    connect(pbStart, SIGNAL(toggled(bool)), this, SLOT(Start(bool)));

    // Уровень синхронизации
    connect(vsTrigLev, SIGNAL(valueChanged(int)), sbTrigLev, SLOT(setValue(int)));
    connect(vsTrigLev, SIGNAL(sliderMoved(int)), vsTrigLev, SLOT(SetValue(int)));
    connect(sbTrigLev, SIGNAL(valueChanged(int)), vsTrigLev, SLOT(SetValue(int)));
    connect(pbResetTrigLev, SIGNAL(pressed()), tuner, SLOT(ResetTrigLev()));
    connect(vsTrigLev, SIGNAL(valueChanged(int)), tuner, SLOT(SetTrigLev(int)));

    // Фильтр по синхронизации
    connect(pbFiltrSinchro, SIGNAL(clicked(bool)), tuner, SLOT(SetFiltr(bool)));

    // Смещение по напряжению первого канала
    connect(vsRShift0, SIGNAL(valueChanged(int)), sbRShift0, SLOT(setValue(int)));
    connect(vsRShift0, SIGNAL(sliderMoved(int)), vsRShift0, SLOT(SetValue(int)));
    connect(sbRShift0, SIGNAL(valueChanged(int)), vsRShift0, SLOT(SetValue(int)));
    connect(pbResetRShift0, SIGNAL(pressed()), tuner, SLOT(ResetRShift0()));
    connect(vsRShift0, SIGNAL(valueChanged(int)), tuner, SLOT(SetRShift0(int)));

    // Смещение по напряжению второго канала
    connect(vsRShift1, SIGNAL(valueChanged(int)), sbRShift1, SLOT(setValue(int)));
    connect(vsRShift1, SIGNAL(sliderMoved(int)), vsRShift1, SLOT(SetValue(int)));
    connect(sbRShift1, SIGNAL(valueChanged(int)), vsRShift1, SLOT(SetValue(int)));
    connect(pbResetRShift1, SIGNAL(pressed()), tuner, SLOT(ResetRShift1()));
    connect(vsRShift1, SIGNAL(valueChanged(int)), tuner, SLOT(SetRShift1(int)));

    // Смещение по времени
    connect(hsTShift, SIGNAL(valueChanged(int)), sbTShift, SLOT(setValue(int)));
    connect(hsTShift, SIGNAL(sliderMoved(int)), hsTShift, SLOT(setValue(int)));
    connect(sbTShift, SIGNAL(valueChanged(int)), hsTShift, SLOT(setValue(int)));
    connect(pbResetTShift, SIGNAL(pressed()), tuner, SLOT(ResetTShift()));
    connect(hsTShift, SIGNAL(valueChanged(int)), tuner, SLOT(SetTShift(int)));

    // Установка синхронизации по каналу
    connect(rbTrigCh0, SIGNAL(toggled(bool)), tuner, SLOT(SetSinchroChannel(bool)));

    // Установка TPos
    connect(cbTPos, SIGNAL(currentIndexChanged(int)), tuner, SLOT(SetTShift0(int)));

    // Масштабы по напряжению
    connect(cbRangeChannel0, SIGNAL(currentIndexChanged(int)), tuner, SLOT(SetRange0(int)));
    connect(cbRangeChannel1, SIGNAL(currentIndexChanged(int)), tuner, SLOT(SetRange1(int)));

    // Масштаб по времени
    connect(cbTBase, SIGNAL(currentIndexChanged(int)), tuner, SLOT(SetTBase(int)));

    // Установка синхронизации по фронту или спаду
    connect(pbSinchro, SIGNAL(toggled(bool)), tuner, SLOT(SetSinchroType(bool)));

    // Коннектим фильтр постоянной составляющей
    connect(rbAC_Channel0, SIGNAL(pressed()), tuner, SLOT(SetAC0()));
    connect(rbAC_Channel1, SIGNAL(pressed()), tuner, SLOT(SetAC1()));
    connect(rbDC_Channel0, SIGNAL(pressed()), tuner, SLOT(SetDC0()));
    connect(rbDC_Channel1, SIGNAL(pressed()), tuner, SLOT(SetDC1()));

    // Коннектим заземление входов
    connect(rbGND_Channel0, SIGNAL(pressed()), tuner, SLOT(SetGround0()));
    connect(rbGND_Channel1, SIGNAL(pressed()), tuner, SLOT(SetGround1()));

    // Чем рисовать сигнал
    connect(cbDrawMode, SIGNAL(currentIndexChanged(int)), display, SLOT(SetDrawMode(int)));

    // Изменение режима запуска - автоматический, ждущий или ручной
    connect(cbStartMode, SIGNAL(currentIndexChanged(int)), tuner, SLOT(SetStartMode(int)));

    /*
    Матемтические функции
    */

    // Ch1 + Ch2, Ch1 * Ch2
	connect(chbEnableMath, SIGNAL(stateChanged(int)), tuner, SLOT(EnableMath(int)));
    connect(cbMathFunc, SIGNAL(currentIndexChanged(int)), tuner, SLOT(ChangeMathFunction(int)));

    // Режим сколязящего усреднения
    connect(pbFiltrPalm0, SIGNAL(toggled(bool)), tuner, SLOT(SetModePalm0(bool)));
    connect(pbFiltrPalm1, SIGNAL(toggled(bool)), tuner, SLOT(SetModePalm1(bool)));
    connect(sbPalm, SIGNAL(valueChanged(int)), tuner, SLOT(SetPointsPalm(int)));

    // Отлючение вывода сигнала
    connect(rbDIS_Ch0, SIGNAL(toggled(bool)), tuner, SLOT(DisableCh0(bool)));
    connect(rbDIS_Ch1, SIGNAL(toggled(bool)), tuner, SLOT(DisableCh1(bool)));
    connect(rbPOS_Ch0, SIGNAL(toggled(bool)), tuner, SLOT(EnableCh0(bool)));
    connect(rbPOS_Ch1, SIGNAL(toggled(bool)), tuner, SLOT(EnableCh1(bool)));
    connect(rbNEG_Ch0, SIGNAL(toggled(bool)), tuner, SLOT(InvertCh0(bool)));
    connect(rbNEG_Ch1, SIGNAL(toggled(bool)), tuner, SLOT(InvertCh1(bool)));

    // Режим накопления дисплея
    connect(chbEnableAccumulation, SIGNAL(stateChanged(int)), tuner, SLOT(EnableAccumulation(int)));
    connect(sbAccumulation, SIGNAL(valueChanged(int)), tuner, SLOT(SetCountAccumulation(int)));
    connect(chbDisableAccumulationLimit, SIGNAL(stateChanged(int)), tuner, SLOT(DisableAccumulationLimit(int)));
    connect(pbResetAccumulation, SIGNAL(pressed()), display, SLOT(ResetAccumulation()));

    // Режим усреднения
    connect(chbEnableAverage, SIGNAL(stateChanged(int)), tuner, SLOT(SetModeAverage(int)));
    connect(sbNumAverage, SIGNAL(valueChanged(int)), tuner, SLOT(SetNumberAverage(int)));

    // Расчёт спектра
    connect(chbEnableSpectr, SIGNAL(stateChanged(int)), tuner, SLOT(EnableSpectr(int)));
    connect(cbSpectr, SIGNAL(currentIndexChanged(int)), tuner, SLOT(ChangeChannelSpectr(int)));

    // При переключении комбобокса автоматических измерений нужно установить признак того, что 
    // вывод результата измерения нужно производить, не ожидая очередного срабатывания таймера измерений
    connect(cbMeasures, SIGNAL(currentIndexChanged(int)), this, SLOT(NewOutMeasure(int)));

    // Установка таймеров на реальный сигнал и на эмулируемый.
    connect(chbTestSignalEnable, SIGNAL(stateChanged(int)), this, SLOT(EnableTestSignal(int)));

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(OnChangeTab(int)));
    connect(cbMeasures, SIGNAL(currentIndexChanged(int)), this, SLOT(OnChangeMeasure(int)));
}

void WindowOsci::LoadSettings()
{
    // Сначала восстанавливаем калибровочные коэффициенты и постоянные рандомизатора
    ifstream file(CALIBRATE_NAME_FILE);
    if(!file.fail()) 
    {
        double value = 0.0;
        if(file >> value) 
            D129_SetKoeffCalibrate(numDevice, 0, value);
        if(file >> value) 
            D129_SetKoeffCalibrate(numDevice, 1, value);
        if(file >> value) 
            D129_SetAddr_Min(numDevice, value);
        if(file >> value) 
            D129_SetAddr_Max(numDevice, value);
        file.close();
    }

    if(configurator->IsExist()) 
    {
        LoadConfigSettings();
    }
    else 
    {
        LoadDefaultSettings();
        LoadDefaultColors();
    }
}

void WindowOsci::LoadConfigSettings()
{
    // Теперь остальные настройки
    D129_SetAdditionRShift(numDevice, 0, configurator->LoadParameter(P_AdditionRShift0));
    D129_SetAdditionRShift(numDevice, 1, configurator->LoadParameter(P_AdditionRShift1));
    int range0 = configurator->LoadParameter(P_Range0);
    if(cbRangeChannel0->currentIndex() != range0) 
        cbRangeChannel0->setCurrentIndex(configurator->LoadParameter(P_Range0));
    else
        tuner->SetRange0(range0);

    int range1 = configurator->LoadParameter(P_Range1);
    if(cbRangeChannel1->currentIndex() != range1)
        cbRangeChannel1->setCurrentIndex(configurator->LoadParameter(P_Range1));
    else
        tuner->SetRange1(range1);

    int rShift0 = configurator->LoadParameter(P_RShift0);
    if(sbRShift0->value() != rShift0)
        sbRShift0->setValue(configurator->LoadParameter(P_RShift0));
    else 
        tuner->SetRShift0(rShift0);

    int rShift1 = configurator->LoadParameter(P_RShift1);
    if(sbRShift1->value() != rShift1)
        sbRShift1->setValue(configurator->LoadParameter(P_RShift1));
    else 
        tuner->SetRShift1(rShift1);

    // Настраиваем уровни синхронизации
    rbTrigCh0->setChecked(true);
    int trigLev0 = configurator->LoadParameter(P_TRIG_LEV0);
    if(sbTrigLev->value() != trigLev0) 
        sbTrigLev->setValue(configurator->LoadParameter(P_TRIG_LEV0));
    else
        tuner->SetTrigLev(trigLev0);

    rbTrigCh1->setChecked(true);
    int trigLev1 = configurator->LoadParameter(P_TRIG_LEV1);
    if(sbTrigLev->value() != trigLev1) 
        sbTrigLev->setValue(configurator->LoadParameter(P_TRIG_LEV1));
    else 
        tuner->SetTrigLev(trigLev1);

    if(configurator->LoadParameter(P_SynhroChannel) == 0) 
        rbTrigCh0->setChecked(true);
    else 
        rbTrigCh1->setChecked(true);

    pbFiltrPalm0->setChecked((bool)configurator->LoadParameter(P_Palm0));
    pbFiltrPalm1->setChecked((bool)configurator->LoadParameter(P_Palm1));
    sbPalm->setValue(configurator->LoadParameter(P_PalmNum));
    D129_SetPointsPalm(numDevice, sbPalm->value());

    // Настраиваем фильтрацию постоянной составляющей и землю
    ModeInput mode0 = (ModeInput)configurator->LoadParameter(P_ModeInput0);
    ModeInput mode1 = (ModeInput)configurator->LoadParameter(P_ModeInput1);
    D129_SetModeInput(numDevice, 0, mode0);
    D129_SetModeInput(numDevice, 1, mode1);

    if(ModeAC == mode0)
        rbAC_Channel0->click();
    else if(ModeDC == mode0)
        rbDC_Channel0->click();
    else
        rbGND_Channel0->click();

    if(ModeAC == mode1)
        rbAC_Channel1->click();
    else if(ModeDC == mode1)
        rbDC_Channel1->click();
    else
        rbGND_Channel1->click();

    rbPOS_Ch0->setChecked((bool)configurator->LoadParameter(P_POS0));
    rbNEG_Ch0->setChecked((bool)configurator->LoadParameter(P_NEG0));
    rbDIS_Ch0->setChecked((bool)configurator->LoadParameter(P_DIS0));
    rbPOS_Ch1->setChecked((bool)configurator->LoadParameter(P_POS1));
    rbNEG_Ch1->setChecked((bool)configurator->LoadParameter(P_NEG1));
    rbDIS_Ch1->setChecked((bool)configurator->LoadParameter(P_DIS1));
    cbStartMode->setCurrentIndex(configurator->LoadParameter(P_START_MODE));
    if(pbStart->isEnabled()) 
        pbStart->setChecked((bool)configurator->LoadParameter(P_Start));
    
    if((bool)configurator->LoadParameter(P_FiltrSynhro))
        pbFiltrSinchro->click();
    //pbFiltrSinchro->setChecked((bool)configurator->LoadParameter(P_FiltrSynhro));

    int tBase = configurator->LoadParameter(P_TBase);
    if(cbTBase->currentIndex() != tBase) {
        cbTBase->setCurrentIndex(configurator->LoadParameter(P_TBase));
    }
    else {
        tuner->SetTBase(tBase);
    }

    int tPos = configurator->LoadParameter(P_TPos);
    if(cbTPos->currentIndex() != tPos) {
        cbTPos->setCurrentIndex(configurator->LoadParameter(P_TPos));
    }
    else {
        tuner->SetTShift0(tPos);
    }

    int tShift = configurator->LoadParameter(P_TShift);
    if(sbTShift->value() != tShift) 
        sbTShift->setValue(configurator->LoadParameter(P_TShift));
    else 
        tuner->SetTShift(tShift);

    cbSourceCurs->setCurrentIndex(configurator->LoadParameter(P_CurrentCursors));
    cbDrawMode->setCurrentIndex(configurator->LoadParameter(P_DrawMode));
    tabWidget->setCurrentIndex(configurator->LoadParameter(P_CurrentTab));

    QPoint point;
    point.rx() = configurator->LoadParameter(P_PositionWindowX);
    point.ry() = configurator->LoadParameter(P_PositionWindowY);

    display->SetColorBackground(configurator->LoadColor(CP_Background));
    display->SetColorGrid(configurator->LoadColor(CP_Grid));
    display->SetColorSignal(0, configurator->LoadColor(CP_Signal0));
    display->SetColorSignal(1, configurator->LoadColor(CP_Signal1));
    display->SetColorSignal(2, configurator->LoadColor(CP_Signal2));
    display->SetColorCurs(0, configurator->LoadColor(CP_Curs0));
    display->SetColorCurs(1, configurator->LoadColor(CP_Curs1));
    display->SetColorCurs(2, configurator->LoadColor(CP_Curs2));
    display->SetColorTPos(configurator->LoadColor(CP_TPos));
    display->SetColorTShift(configurator->LoadColor(CP_TShift));

    move(configurator->LoadParameter(P_PositionWindowX), configurator->LoadParameter(P_PositionWindowY));
}

void WindowOsci::LoadDefaultSettings()
{
    D129_SetAdditionRShift(numDevice, 0, 0);
    D129_SetAdditionRShift(numDevice, 1, 0);

    D129_SetRShiftRel(numDevice, 0, RShiftZero);
    D129_SetRShiftRel(numDevice, 1, RShiftZero);

    cbSourceCurs->setCurrentIndex(1);
    cbSourceCurs->setCurrentIndex(0);

    cbTPos->setCurrentIndex((int)TPos_Center);
    vsTrigLev->SetValue(TrigLevZero);
    vsRShift0->setValue(RShiftZero);
    vsRShift1->setValue(RShiftZero);
    hsTShift->setValue(TShiftZero);
    cbRangeChannel0->setCurrentIndex(Range_500mv);
    cbRangeChannel1->setCurrentIndex(Range_500mv);
    cbTBase->setCurrentIndex(TBase_50us);

    rbTrigCh0->setChecked(true);

    pbSinchro->setChecked(true);
    pbSinchro->setChecked(false);

    pbStart->setChecked(true);

    rbDC_Channel0->click();
    rbAC_Channel0->click();
    rbDC_Channel1->click();
    rbAC_Channel1->click();

    cbMeasures->setCurrentIndex(0);
    cbDrawMode->setCurrentIndex(1);

    // Режим синхронизации
    cbStartMode->setCurrentIndex(StartMode_Wait);
    cbStartMode->setCurrentIndex(StartMode_Auto);

    vsTrigLev->setValue(2000);

    /*
    Для математических функций
    */
    sbPalm->setValue(5);
    pbFiltrPalm0->setChecked(true);
    pbFiltrPalm0->setChecked(false);
    pbFiltrPalm1->setChecked(true);
    pbFiltrPalm1->setChecked(false);

    rbNEG_Ch0->setChecked(true);
    rbPOS_Ch0->setChecked(true);
    rbNEG_Ch1->setChecked(true);
    rbPOS_Ch1->setChecked(true);

    // Режим накопления дисплея
    chbEnableAccumulation->setChecked(true);
    chbEnableAccumulation->setChecked(false);

    // Режим усреднения
    sbNumAverage->setValue(16);

    // Управление курсорами
    chbUCurs->setChecked(true);
    chbUCurs->setChecked(false);

    chbTCurs->setChecked(true);
    chbTCurs->setChecked(false);

    // Выключаем режим математической функции
    chbEnableMath->setChecked(true);
    chbEnableMath->setChecked(false);

    int numPorts = cbPorts->count() - 1;

    cbPorts->setCurrentIndex(numPorts);

    leAccumulation->setText(tr("%1").arg(1, 0, 10));

    D129_SetKoeffCalibrate(numDevice, 0, 1.0);
    D129_SetKoeffCalibrate(numDevice, 1, 1.0);
    D129_ResetAddrMinMax(numDevice);
}

void WindowOsci::LoadDefaultColors()
{
    display->SetColorBackground((QColor)Qt::black);
    display->SetColorTPos((QColor)Qt::yellow);
    display->SetColorTShift((QColor)Qt::white);
    display->SetColorGrid((QColor)Qt::blue);
    display->SetColorSignal(0, Qt::white);
    display->SetColorCurs(0, Qt::white);
    display->SetColorSignal(1, Qt::yellow);
    display->SetColorCurs(1, Qt::yellow);
    display->SetColorSignal(2, Qt::white);
}

void WindowOsci::UpdatePorts()
{
    disconnect(cbPorts, SIGNAL(currentIndexChanged(int)), this, SLOT(SetNumberPort(int)));
    cbPorts->clear();
    cbPorts->setEnabled(false);
    update();

    numPorts = D129_GetPorts(ports, 256);
    if(numPorts <= 0)
        return;

    for(int i = 0; i < numPorts; i++) 
        cbPorts->addItem(QString(tr("COM")) + QString("").setNum(ports[i], 10));

    cbPorts->setCurrentIndex(numPorts - 1);
    SetNumberPort(numPorts - 1);
    connect(cbPorts, SIGNAL(currentIndexChanged(int)), this, SLOT(SetNumberPort(int)));
    cbPorts->setEnabled(true);
}

void WindowOsci::SetNumberPort(int _indexPort)
{
    // Защищаемся от вылета при очистке комбобокса порта
    if(_indexPort < 0) 
        return;

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
    display->ConnectToDevice(created);
    display->Reset();
}

void WindowOsci::close()
{
    SaveSettings();
    D129_DestroyDevice(numDevice);
    SAFE_DELETE(runScreen);
    SAFE_DELETE(trigScreen);
    SAFE_DELETE(display);
    SAFE_DELETE(vsTrigLev);
    SAFE_DELETE(vsRShift0);
    SAFE_DELETE(vsRShift1);
    SAFE_DELETE(tuner);
    SAFE_DELETE(configurator);
    SAFE_DELETE(threadSearch);
    SAFE_DELETE(timerCalibrateZero);
    SAFE_DELETE(timer);
    QMainWindow::close();
    //emit terminate();
}

void WindowOsci::closeEvent(QCloseEvent *)
{
    close();
}

void WindowOsci::SaveSettings()
{
    if(!configurator)
        return;

    configurator->CreateConfig();
    configurator->SaveParameter(P_AdditionRShift0, D129_GetAdditionRShift(numDevice, 0));
    configurator->SaveParameter(P_AdditionRShift1, D129_GetAdditionRShift(numDevice, 1));
    configurator->SaveParameter(P_SynhroChannel, D129_GetSinchroChannel(numDevice));
    configurator->SaveParameter(P_Range0, cbRangeChannel0->currentIndex());
    configurator->SaveParameter(P_Range1, cbRangeChannel1->currentIndex());
    configurator->SaveParameter(P_Palm0, pbFiltrPalm0->isChecked());
    configurator->SaveParameter(P_Palm1, pbFiltrPalm1->isChecked());
    configurator->SaveParameter(P_PalmNum, sbPalm->value());
    configurator->SaveParameter(P_RShift0, sbRShift0->value());
    configurator->SaveParameter(P_RShift1, sbRShift1->value());
    configurator->SaveParameter(P_ModeInput0, D129_GetModeInput(numDevice, 0));
    configurator->SaveParameter(P_ModeInput1, D129_GetModeInput(numDevice, 1));
    configurator->SaveParameter(P_POS0, rbPOS_Ch0->isChecked());
    configurator->SaveParameter(P_NEG0, rbNEG_Ch0->isChecked());
    configurator->SaveParameter(P_DIS0, rbDIS_Ch0->isChecked());
    configurator->SaveParameter(P_POS1, rbPOS_Ch1->isChecked());
    configurator->SaveParameter(P_NEG1, rbNEG_Ch1->isChecked());
    configurator->SaveParameter(P_DIS1, rbDIS_Ch1->isChecked());
    configurator->SaveParameter(P_TRIG_LEV0, D129_GetTrigLevRel(numDevice, 0));
    configurator->SaveParameter(P_TRIG_LEV1, D129_GetTrigLevRel(numDevice, 1));
    configurator->SaveParameter(P_START_MODE, cbStartMode->currentIndex());
    configurator->SaveParameter(P_Start, pbStart->isChecked());
    configurator->SaveParameter(P_SynhroFront, pbSinchro->isChecked());
    configurator->SaveParameter(P_FiltrSynhro, pbFiltrSinchro->isChecked());
    configurator->SaveParameter(P_TBase, cbTBase->currentIndex());
    configurator->SaveParameter(P_TPos, cbTPos->currentIndex());
    configurator->SaveParameter(P_TShift, sbTShift->value());
    configurator->SaveParameter(P_CurrentCursors, cbSourceCurs->currentIndex());
    configurator->SaveParameter(P_DrawMode, cbDrawMode->currentIndex());
    configurator->SaveParameter(P_CurrentTab, tabWidget->currentIndex());
    QPoint position = pos();
    configurator->SaveParameter(P_PositionWindowX, position.x());
    configurator->SaveParameter(P_PositionWindowY, position.y());

    configurator->SaveColor(CP_Background, display->GetColorBackground());
    configurator->SaveColor(CP_Grid, display->GetColorGrid());
    configurator->SaveColor(CP_Signal0, display->GetColorSignal(0));
    configurator->SaveColor(CP_Signal1, display->GetColorSignal(1));
    configurator->SaveColor(CP_Signal2, display->GetColorSignal(2));
    configurator->SaveColor(CP_Curs0, display->GetColorCurs(0));
    configurator->SaveColor(CP_Curs1, display->GetColorCurs(1));
    configurator->SaveColor(CP_Curs2, display->GetColorCurs(2));
    configurator->SaveColor(CP_TPos, display->GetColorTPos());
    configurator->SaveColor(CP_TShift, display->GetColorTShift());

    configurator->CloseConfig();

    // Теперь сохраним коэффициенты калибровки и аддр_мин, аддр_макс для рандомизатора
    ofstream file(CALIBRATE_NAME_FILE, ios::out);
    file << setprecision(15) << D129_GetKoeffCalibrate(numDevice, 0) << endl;
    file << D129_GetKoeffCalibrate(numDevice, 1) << endl;
    file << D129_GetAddr_Min(numDevice) << endl;
    file << D129_GetAddr_Max(numDevice) << endl;
    file.close();
}

void WindowOsci::AutoMeasures()
{
    static Timer timer;
    const long long timeUpdate = 500;
    static long long startTime = timer.GetTime().ToMSec();
    if(timer.GetTime().ToMSec() - startTime >= timeUpdate) 
    {
        static char buffer[16];
        D129_GetMeasure(numDevice, 0, cbMeasures->currentIndex(), buffer, 15);
        leMeasure1->setText(rbDIS_Ch0->isChecked() ? " " : tr(buffer));
        D129_GetMeasure(numDevice, 1, cbMeasures->currentIndex(), buffer, 15);
        leMeasure2->setText(rbDIS_Ch1->isChecked() ? " " : tr(buffer));
        startTime = timer.GetTime().ToMSec();
    }
}

bool WindowOsci::DataReady()
{
    unsigned char flag = 0;
    bool ready = D129_DataReadyForRead(numDevice, flag);
    trigScreen->SetTrig(GetBit(flag, 1));
    return ready;
}

void WindowOsci::SaveScreen() 
{
    QImage imageScreen = display->GetImage();
    QSize size(imageScreen.width() + 2, imageScreen.height() + 2 + 60);
    QImage image(size, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    painter.setBrush(QBrush(display->GetPointerSettings()->colorBackground, Qt::SolidPattern));
    painter.setPen(QPen(Qt::white));
    painter.drawRect(0, 0, image.size().width() - 1, image.size().height() - 1);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter.drawRect(0, 0, image.width() - 1, image.height() - 1);
    painter.drawImage(1, 1, imageScreen);
    int coordY0 = imageScreen.size().height() + 13;
    int coordX0 = 5;
    int coordX1 = imageScreen.size().width() / 2;
    int widthString = 14;
    if(!rbDIS_Ch0->isChecked()) {
        painter.setPen(QPen(display->GetPointerSettings()->colorSignal[0], Qt::SolidLine));
        painter.drawText(coordX0, coordY0, QString(tr("Range1 = ")) + mathDisplay.GetRangeString(cbRangeChannel0->currentIndex()));
        painter.drawText(coordX1, coordY0, QString(tr("Ypos1 = ")) + mathDisplay.GetRShiftAbs(sbRShift0->value(), cbRangeChannel0->currentIndex()));
    }
    if(!rbDIS_Ch1->isChecked()) {
        painter.setPen(QPen(display->GetPointerSettings()->colorSignal[1], Qt::SolidLine));
        painter.drawText(coordX0, coordY0 + widthString, QString(tr("Range2 = ")) + mathDisplay.GetRangeString(cbRangeChannel1->currentIndex()));
        painter.drawText(coordX1, coordY0 + widthString, QString(tr("Ypos2 = ")) + mathDisplay.GetRShiftAbs(sbRShift1->value(), cbRangeChannel1->currentIndex()));
    }
    QColor color;
    int red;
    int green;
    int blue;
    display->GetPointerSettings()->colorBackground.getRgb(&red, &green, &blue);
    painter.setPen(QPen(QColor(255 - red, 255 - green, 255 - blue), Qt::SolidLine));
    painter.drawText(coordX0, coordY0 + widthString * 2, QString(tr("TBase = ")) + mathDisplay.GetTBaseString(cbTBase->currentIndex()));
    painter.drawText(coordX1, coordY0 + widthString * 2, QString(tr("Xpos = ")) + mathDisplay.GetTShiftAbs(sbTShift->value(), cbTBase->currentIndex()));
    painter.drawText(coordX0, coordY0 + widthString * 3, QDate::currentDate().toString(QString(tr("dd.MM.yyyy"))) + QString(tr(" ")) + QTime::currentTime().toString(Qt::TextDate));

    static QString dir = "";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), dir, tr("*.bmp"));
    if((fileName.size() > 4) && (fileName.mid(fileName.size() - 4) != tr(".bmp"))) {
        fileName.append(tr(".bmp"));
    }
    dir = fileName;
    image.save(fileName, "BMP");
}

void WindowOsci::OnTimer()
{
    //disconnect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));

    // Сначала проверяем, есть ли связь с устройством
    if(!D129_IsConnect(numDevice))
    {
        display->ConnectToDevice(false);
        pbStart->setChecked(false);
        pbStart->setEnabled(false);
        return;
    }

    switch(cbStartMode->currentIndex()) 
    {
    case StartMode_Auto:
    case StartMode_Wait:
        if(D129_IsRunning(numDevice)) 
        {
            if(DataReady()) 
            {
                if(D129_ReadData(numDevice))
                {
                    display->DrawData();
                    AutoMeasures();
                    D129_Start(numDevice, false);
                }
            }
            else if(cbTBase->currentIndex() < TBase_1us) 
            {
                D129_Stop(numDevice);
                D129_Start(numDevice, false);
            }
        }
        break;
    case StartMode_Single:
        if(DataReady()) 
        {
            D129_ReadData(numDevice); // memcpy( Mass, "asdasd", 10 );
            display->DrawData();
            AutoMeasures();
            pbStart->setChecked(false);
        }
        break;
    default:
        break;
    }

    //connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}

void WindowOsci::Start(bool _start)
{
    if(!pbStart->isEnabled())
        return;

    trigScreen->SetTrig(false);

    if(_start) 
    {
        /*
        if(cbStartMode->currentIndex() != 2)
            D129_SetAutoCalibrate(numDevice, true);
            */

        D129_SetSettings(numDevice);
        if(cbStartMode->currentIndex() != 2)
            D129_CalibrateZero(numDevice);

        D129_Start(numDevice, true);
        display->Reset();
        timer->start();
    }
    else 
    {
        //D129_SetAutoCalibrate(numDevice, false);
        D129_Stop(numDevice);
        timer->stop();
    }
    cbPorts->setEnabled(!_start);
    pbUpdatePorts->setEnabled(!_start);
    pbStart->setText(_start ? tr("Стоп") : tr("Старт"));
    runScreen->SetState(_start);
}

void WindowOsci::SetModeCalibrator_4V_0Hz() 
{
    D129_SetCalibrator(numDevice, false);
}

void WindowOsci::SetModeCalibrator_4V_5kHz() 
{
    D129_SetCalibrator(numDevice, true);
}

void WindowOsci::Calibrate() 
{
    int running = D129_IsRunning(numDevice);
    D129_Stop(numDevice);

    uchar buffer[512];

    int rez = QMessageBox::information(NULL, tr("Важное сообщение"), tr("Подключите выход калибратора к первому каналу и нажмите кнопку Ок \
                                                                        для проведения калибровки или Cancel для отмены"), QMessageBox::Ok | QMessageBox::Cancel);
    if(rez == QMessageBox::Ok)
    {
        rez = D129_Calibrate(numDevice, 0, buffer);
        if(1 == rez)
            display->DrawPoints(0, buffer);
        else
            QMessageBox::information(NULL, tr("Важное сообщение"), tr("Первый канал не откалиброван"), QMessageBox::Ok);
    }

    rez = QMessageBox::information(NULL, tr("Важное сообщение"), tr("Подключите выход калибратора ко второму каналу и нажмите кнопку Ок \
                                                                    для проведения калибровки или Cancel для отмены"), QMessageBox::Ok | QMessageBox::Cancel);
    if(rez == QMessageBox::Ok)
    {
        rez = D129_Calibrate(numDevice, 1, buffer);
        if(1 == rez)
            display->DrawPoints(1, buffer);
        else
            QMessageBox::information(NULL, tr("Важное сообщение"), tr("Второй канал не откалиброван"), QMessageBox::Ok);
    }

    QMessageBox::information(NULL, tr("Важное сообщение"), tr("Калибровка завершена"), QMessageBox::Ok);

    if(running)
        D129_Start(numDevice, false);
}

void WindowOsci::AutoSearchEnter() 
{
    display->Reset();
    threadSearch->start(QThread::IdlePriority);
    disconnect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    connect(threadSearch, SIGNAL(AutoSearchExit(bool)), this, SLOT(AutoSearchExit(bool)));
    threadSearch->FindSignal();
    setEnabled(false);
}

void WindowOsci::AutoSearchExit(bool /*_isFind*/) 
{
    setEnabled(true);
    disconnect(threadSearch, SIGNAL(AutoSearchExit(bool)), this, SLOT(AutoSearchExit(bool)));
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}

void WindowOsci::NewOutMeasure(int) 
{
    leMeasure1->setText("");
    leMeasure2->setText("");
}

void WindowOsci::OnCalibrateZero()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    bool running = D129_IsRunning(numDevice);
    D129_Stop(numDevice);
    D129_CalibrateZero(numDevice);
    if(running)
        D129_Start(numDevice, false);
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}

void WindowOsci::OnChangeTab(int _index)
{
    if(0 == _index)
    {
        if(cbMeasures->currentIndex() >= 5)
            connect(timerCalibrateZero, SIGNAL(timeout()), this, SLOT(OnCalibrateZero()));
    }
    else
        disconnect(timerCalibrateZero, SIGNAL(timeout()), this, SLOT(OnCalibrateZero()));
}

void WindowOsci::OnChangeMeasure(int _index)
{
    if(_index >= 5)
        connect(timerCalibrateZero, SIGNAL(timeout()), this, SLOT(OnCalibrateZero()));
    else
        disconnect(timerCalibrateZero, SIGNAL(timeout()), this, SLOT(OnCalibrateZero()));
}