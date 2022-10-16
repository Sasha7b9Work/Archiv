/********************************************************************************
** Form generated from reading ui file 'WindowOsci.ui'
**
** Created: Wed Jun 27 09:43:27 2012
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WINDOWOSCI_H
#define UI_WINDOWOSCI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WindowOsci
{
public:
    QWidget *centralwidget;
    QFrame *frame;
    QSlider *hsTShift;
    QGroupBox *groupBoxChannel2;
    QGroupBox *groupBox_6;
    QRadioButton *rbPOS_Ch1;
    QRadioButton *rbNEG_Ch1;
    QRadioButton *rbDIS_Ch1;
    QLabel *label_8;
    QGroupBox *groupBox_5;
    QRadioButton *rbAC_Channel1;
    QRadioButton *rbDC_Channel1;
    QRadioButton *rbGND_Channel1;
    QComboBox *cbRangeChannel1;
    QLabel *labelRShift2;
    QPushButton *pbResetRShift1;
    QPushButton *pbFiltrPalm1;
    QWidget *layoutWidget;
    QFormLayout *formLayout_6;
    QLineEdit *leRShift1Abs;
    QSpinBox *sbRShift1;
    QGroupBox *groupBoxChannel1;
    QGroupBox *groupBox_7;
    QRadioButton *rbPOS_Ch0;
    QRadioButton *rbNEG_Ch0;
    QRadioButton *rbDIS_Ch0;
    QGroupBox *groupBox;
    QRadioButton *rbAC_Channel0;
    QRadioButton *rbDC_Channel0;
    QRadioButton *rbGND_Channel0;
    QComboBox *cbRangeChannel0;
    QLabel *labelRShift1;
    QLabel *label;
    QPushButton *pbResetRShift0;
    QPushButton *pbFiltrPalm0;
    QWidget *layoutWidget1;
    QFormLayout *formLayout_7;
    QLineEdit *leRShift0Abs;
    QSpinBox *sbRShift0;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLineEdit *leMeasure2;
    QLineEdit *leMeasure1;
    QComboBox *cbMeasures;
    QWidget *tabMath;
    QSpinBox *sbPalm;
    QLabel *label_44;
    QCheckBox *chbEnableAverage;
    QSpinBox *sbNumAverage;
    QComboBox *cbMathFunc;
    QCheckBox *chbEnableMath;
    QCheckBox *chbEnableSpectr;
    QComboBox *cbSpectr;
    QWidget *tabSignals;
    QLabel *label_3;
    QComboBox *cbDrawMode;
    QGroupBox *groupBoxAccumulation;
    QCheckBox *chbEnableAccumulation;
    QCheckBox *chbDisableAccumulationLimit;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *leAccumulation;
    QSpinBox *sbAccumulation;
    QPushButton *pbResetAccumulation;
    QWidget *tab_6;
    QComboBox *cbPorts;
    QLabel *label_7;
    QPushButton *pbUpdatePorts;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *chbTestSignalEnable;
    QComboBox *cbTestSignalType;
    QDoubleSpinBox *dsbTestSignalFreq;
    QDoubleSpinBox *dsbTestSignalAmplitude;
    QGroupBox *groupBox_3;
    QGroupBox *groupBox_4;
    QRadioButton *rbTrigCh0;
    QRadioButton *rbTrigCh1;
    QPushButton *pbResetTrigLev;
    QLabel *label_11;
    QPushButton *pbStart;
    QComboBox *cbStartMode;
    QLabel *label_12;
    QPushButton *pbSinchro;
    QPushButton *pbFiltrSinchro;
    QWidget *layoutWidget4;
    QFormLayout *formLayout_5;
    QLineEdit *leTrigLevAbs;
    QSpinBox *sbTrigLev;
    QGroupBox *groupBox_2;
    QCheckBox *chbUCursTogether;
    QLineEdit *leDeltaUAbs;
    QLabel *label_9;
    QLabel *label_15;
    QCheckBox *chbTCursTogether;
    QLabel *label_16;
    QLineEdit *leDeltaTAbs;
    QComboBox *cbSourceCurs;
    QLabel *label_17;
    QCheckBox *chbUCurs;
    QCheckBox *chbTCurs;
    QLineEdit *lePercentage;
    QPushButton *pbPercentage;
    QLabel *label_19;
    QWidget *layoutWidget5;
    QFormLayout *formLayout;
    QLineEdit *leUCur2Abs;
    QSpinBox *sbUCur2;
    QWidget *layoutWidget6;
    QFormLayout *formLayout_2;
    QLineEdit *leTCur2Abs;
    QSpinBox *sbTCur2;
    QWidget *layoutWidget7;
    QFormLayout *formLayout_3;
    QLineEdit *leUCur1Abs;
    QSpinBox *sbUCur1;
    QWidget *layoutWidget8;
    QFormLayout *formLayout_4;
    QLineEdit *leTCur1Abs;
    QSpinBox *sbTCur1;
    QLabel *label_18;
    QGroupBox *groupBox_9;
    QLabel *label_6;
    QComboBox *cbTPos;
    QPushButton *pbResetTShift;
    QSpinBox *sbTShift;
    QLabel *label_5;
    QLineEdit *leTShiftAbs;
    QLabel *label_4;
    QComboBox *cbTBase;

    void setupUi(QMainWindow *WindowOsci)
    {
        if (WindowOsci->objectName().isEmpty())
            WindowOsci->setObjectName(QString::fromUtf8("WindowOsci"));
        WindowOsci->setWindowModality(Qt::ApplicationModal);
        WindowOsci->resize(966, 583);
        WindowOsci->setMinimumSize(QSize(966, 583));
        WindowOsci->setMaximumSize(QSize(966, 583));
        WindowOsci->setFocusPolicy(Qt::TabFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Images/Osci.ico"), QSize(), QIcon::Normal, QIcon::Off);
        WindowOsci->setWindowIcon(icon);
        WindowOsci->setAnimated(false);
        centralwidget = new QWidget(WindowOsci);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(28, 10, 521, 516));
        frame->setFocusPolicy(Qt::TabFocus);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        hsTShift = new QSlider(centralwidget);
        hsTShift->setObjectName(QString::fromUtf8("hsTShift"));
        hsTShift->setGeometry(QRect(25, 526, 516, 16));
        hsTShift->setOrientation(Qt::Horizontal);
        groupBoxChannel2 = new QGroupBox(centralwidget);
        groupBoxChannel2->setObjectName(QString::fromUtf8("groupBoxChannel2"));
        groupBoxChannel2->setGeometry(QRect(711, 120, 241, 111));
        groupBox_6 = new QGroupBox(groupBoxChannel2);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(185, 15, 51, 86));
        rbPOS_Ch1 = new QRadioButton(groupBox_6);
        rbPOS_Ch1->setObjectName(QString::fromUtf8("rbPOS_Ch1"));
        rbPOS_Ch1->setGeometry(QRect(8, 10, 46, 19));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Images/POS.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        rbPOS_Ch1->setIcon(icon1);
        rbPOS_Ch1->setChecked(true);
        rbNEG_Ch1 = new QRadioButton(groupBox_6);
        rbNEG_Ch1->setObjectName(QString::fromUtf8("rbNEG_Ch1"));
        rbNEG_Ch1->setGeometry(QRect(8, 35, 41, 19));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Images/NEG.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        rbNEG_Ch1->setIcon(icon2);
        rbDIS_Ch1 = new QRadioButton(groupBox_6);
        rbDIS_Ch1->setObjectName(QString::fromUtf8("rbDIS_Ch1"));
        rbDIS_Ch1->setGeometry(QRect(8, 60, 41, 19));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Images/DISABLE.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        rbDIS_Ch1->setIcon(icon3);
        rbDIS_Ch1->setChecked(false);
        label_8 = new QLabel(groupBoxChannel2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 15, 51, 16));
        label_8->setAlignment(Qt::AlignCenter);
        groupBox_5 = new QGroupBox(groupBoxChannel2);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(130, 15, 51, 86));
        rbAC_Channel1 = new QRadioButton(groupBox_5);
        rbAC_Channel1->setObjectName(QString::fromUtf8("rbAC_Channel1"));
        rbAC_Channel1->setGeometry(QRect(8, 10, 36, 19));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Images/AC.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        rbAC_Channel1->setIcon(icon4);
        rbAC_Channel1->setChecked(true);
        rbDC_Channel1 = new QRadioButton(groupBox_5);
        rbDC_Channel1->setObjectName(QString::fromUtf8("rbDC_Channel1"));
        rbDC_Channel1->setGeometry(QRect(8, 35, 36, 19));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Images/DC.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        rbDC_Channel1->setIcon(icon5);
        rbGND_Channel1 = new QRadioButton(groupBox_5);
        rbGND_Channel1->setObjectName(QString::fromUtf8("rbGND_Channel1"));
        rbGND_Channel1->setGeometry(QRect(8, 60, 36, 19));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/Images/Ground.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        rbGND_Channel1->setIcon(icon6);
        rbGND_Channel1->setChecked(false);
        cbRangeChannel1 = new QComboBox(groupBoxChannel2);
        cbRangeChannel1->setObjectName(QString::fromUtf8("cbRangeChannel1"));
        cbRangeChannel1->setGeometry(QRect(15, 35, 61, 22));
        labelRShift2 = new QLabel(groupBoxChannel2);
        labelRShift2->setObjectName(QString::fromUtf8("labelRShift2"));
        labelRShift2->setGeometry(QRect(10, 61, 71, 16));
        labelRShift2->setAlignment(Qt::AlignCenter);
        pbResetRShift1 = new QPushButton(groupBoxChannel2);
        pbResetRShift1->setObjectName(QString::fromUtf8("pbResetRShift1"));
        pbResetRShift1->setGeometry(QRect(104, 81, 20, 20));
        pbResetRShift1->setMinimumSize(QSize(20, 20));
        pbResetRShift1->setMaximumSize(QSize(20, 20));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Images/null.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        pbResetRShift1->setIcon(icon7);
        pbFiltrPalm1 = new QPushButton(groupBoxChannel2);
        pbFiltrPalm1->setObjectName(QString::fromUtf8("pbFiltrPalm1"));
        pbFiltrPalm1->setEnabled(true);
        pbFiltrPalm1->setGeometry(QRect(100, 35, 25, 25));
        pbFiltrPalm1->setIconSize(QSize(19, 19));
        pbFiltrPalm1->setCheckable(true);
        pbFiltrPalm1->setChecked(false);
        layoutWidget = new QWidget(groupBoxChannel2);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(15, 80, 93, 24));
        formLayout_6 = new QFormLayout(layoutWidget);
        formLayout_6->setObjectName(QString::fromUtf8("formLayout_6"));
        formLayout_6->setHorizontalSpacing(0);
        formLayout_6->setContentsMargins(0, 0, 0, 0);
        leRShift1Abs = new QLineEdit(layoutWidget);
        leRShift1Abs->setObjectName(QString::fromUtf8("leRShift1Abs"));
        leRShift1Abs->setEnabled(true);
        leRShift1Abs->setMinimumSize(QSize(66, 22));
        leRShift1Abs->setMaximumSize(QSize(66, 22));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, leRShift1Abs);

        sbRShift1 = new QSpinBox(layoutWidget);
        sbRShift1->setObjectName(QString::fromUtf8("sbRShift1"));
        sbRShift1->setMinimumSize(QSize(18, 22));
        sbRShift1->setMaximumSize(QSize(18, 22));
        sbRShift1->setMouseTracking(true);

        formLayout_6->setWidget(0, QFormLayout::FieldRole, sbRShift1);

        groupBoxChannel1 = new QGroupBox(centralwidget);
        groupBoxChannel1->setObjectName(QString::fromUtf8("groupBoxChannel1"));
        groupBoxChannel1->setGeometry(QRect(711, 5, 241, 111));
        groupBox_7 = new QGroupBox(groupBoxChannel1);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(185, 15, 51, 86));
        rbPOS_Ch0 = new QRadioButton(groupBox_7);
        rbPOS_Ch0->setObjectName(QString::fromUtf8("rbPOS_Ch0"));
        rbPOS_Ch0->setGeometry(QRect(8, 10, 36, 19));
        rbPOS_Ch0->setIcon(icon1);
        rbPOS_Ch0->setChecked(true);
        rbNEG_Ch0 = new QRadioButton(groupBox_7);
        rbNEG_Ch0->setObjectName(QString::fromUtf8("rbNEG_Ch0"));
        rbNEG_Ch0->setGeometry(QRect(8, 35, 36, 19));
        rbNEG_Ch0->setIcon(icon2);
        rbDIS_Ch0 = new QRadioButton(groupBox_7);
        rbDIS_Ch0->setObjectName(QString::fromUtf8("rbDIS_Ch0"));
        rbDIS_Ch0->setGeometry(QRect(8, 60, 36, 19));
        rbDIS_Ch0->setIcon(icon3);
        rbDIS_Ch0->setChecked(false);
        groupBox = new QGroupBox(groupBoxChannel1);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(130, 15, 51, 86));
        rbAC_Channel0 = new QRadioButton(groupBox);
        rbAC_Channel0->setObjectName(QString::fromUtf8("rbAC_Channel0"));
        rbAC_Channel0->setGeometry(QRect(8, 10, 36, 19));
        rbAC_Channel0->setIcon(icon4);
        rbAC_Channel0->setChecked(true);
        rbDC_Channel0 = new QRadioButton(groupBox);
        rbDC_Channel0->setObjectName(QString::fromUtf8("rbDC_Channel0"));
        rbDC_Channel0->setGeometry(QRect(8, 35, 36, 19));
        rbDC_Channel0->setIcon(icon5);
        rbGND_Channel0 = new QRadioButton(groupBox);
        rbGND_Channel0->setObjectName(QString::fromUtf8("rbGND_Channel0"));
        rbGND_Channel0->setGeometry(QRect(8, 60, 36, 19));
        rbGND_Channel0->setIcon(icon6);
        rbGND_Channel0->setChecked(false);
        cbRangeChannel0 = new QComboBox(groupBoxChannel1);
        cbRangeChannel0->setObjectName(QString::fromUtf8("cbRangeChannel0"));
        cbRangeChannel0->setGeometry(QRect(15, 35, 61, 22));
        labelRShift1 = new QLabel(groupBoxChannel1);
        labelRShift1->setObjectName(QString::fromUtf8("labelRShift1"));
        labelRShift1->setGeometry(QRect(10, 62, 71, 16));
        labelRShift1->setAlignment(Qt::AlignCenter);
        label = new QLabel(groupBoxChannel1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(15, 16, 56, 14));
        label->setAlignment(Qt::AlignCenter);
        pbResetRShift0 = new QPushButton(groupBoxChannel1);
        pbResetRShift0->setObjectName(QString::fromUtf8("pbResetRShift0"));
        pbResetRShift0->setGeometry(QRect(104, 81, 20, 20));
        pbResetRShift0->setMinimumSize(QSize(20, 20));
        pbResetRShift0->setMaximumSize(QSize(20, 20));
        pbResetRShift0->setIcon(icon7);
        pbFiltrPalm0 = new QPushButton(groupBoxChannel1);
        pbFiltrPalm0->setObjectName(QString::fromUtf8("pbFiltrPalm0"));
        pbFiltrPalm0->setEnabled(true);
        pbFiltrPalm0->setGeometry(QRect(100, 35, 25, 25));
        pbFiltrPalm0->setIconSize(QSize(19, 19));
        pbFiltrPalm0->setCheckable(true);
        pbFiltrPalm0->setChecked(false);
        layoutWidget1 = new QWidget(groupBoxChannel1);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(15, 79, 93, 24));
        formLayout_7 = new QFormLayout(layoutWidget1);
        formLayout_7->setObjectName(QString::fromUtf8("formLayout_7"));
        formLayout_7->setHorizontalSpacing(0);
        formLayout_7->setContentsMargins(0, 0, 0, 0);
        leRShift0Abs = new QLineEdit(layoutWidget1);
        leRShift0Abs->setObjectName(QString::fromUtf8("leRShift0Abs"));
        leRShift0Abs->setEnabled(true);
        leRShift0Abs->setMinimumSize(QSize(66, 22));
        leRShift0Abs->setMaximumSize(QSize(66, 22));

        formLayout_7->setWidget(0, QFormLayout::LabelRole, leRShift0Abs);

        sbRShift0 = new QSpinBox(layoutWidget1);
        sbRShift0->setObjectName(QString::fromUtf8("sbRShift0"));
        sbRShift0->setMinimumSize(QSize(18, 22));
        sbRShift0->setMaximumSize(QSize(18, 22));
        sbRShift0->setMouseTracking(true);

        formLayout_7->setWidget(0, QFormLayout::FieldRole, sbRShift0);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(576, 425, 381, 131));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setElideMode(Qt::ElideNone);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        leMeasure2 = new QLineEdit(tab);
        leMeasure2->setObjectName(QString::fromUtf8("leMeasure2"));
        leMeasure2->setGeometry(QRect(295, 40, 71, 20));
        leMeasure1 = new QLineEdit(tab);
        leMeasure1->setObjectName(QString::fromUtf8("leMeasure1"));
        leMeasure1->setGeometry(QRect(295, 10, 71, 20));
        cbMeasures = new QComboBox(tab);
        cbMeasures->setObjectName(QString::fromUtf8("cbMeasures"));
        cbMeasures->setGeometry(QRect(10, 10, 276, 22));
        cbMeasures->setMaxVisibleItems(100);
        tabWidget->addTab(tab, QString());
        tabMath = new QWidget();
        tabMath->setObjectName(QString::fromUtf8("tabMath"));
        sbPalm = new QSpinBox(tabMath);
        sbPalm->setObjectName(QString::fromUtf8("sbPalm"));
        sbPalm->setGeometry(QRect(150, 70, 51, 22));
        sbPalm->setMinimum(5);
        sbPalm->setMaximum(15);
        sbPalm->setSingleStep(2);
        sbPalm->setValue(5);
        label_44 = new QLabel(tabMath);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(10, 68, 131, 26));
        chbEnableAverage = new QCheckBox(tabMath);
        chbEnableAverage->setObjectName(QString::fromUtf8("chbEnableAverage"));
        chbEnableAverage->setGeometry(QRect(60, 42, 81, 19));
        chbEnableAverage->setLayoutDirection(Qt::RightToLeft);
        sbNumAverage = new QSpinBox(tabMath);
        sbNumAverage->setObjectName(QString::fromUtf8("sbNumAverage"));
        sbNumAverage->setGeometry(QRect(150, 40, 51, 22));
        sbNumAverage->setMinimum(1);
        sbNumAverage->setMaximum(32);
        sbNumAverage->setSingleStep(1);
        sbNumAverage->setValue(3);
        cbMathFunc = new QComboBox(tabMath);
        cbMathFunc->setObjectName(QString::fromUtf8("cbMathFunc"));
        cbMathFunc->setGeometry(QRect(134, 10, 61, 22));
        chbEnableMath = new QCheckBox(tabMath);
        chbEnableMath->setObjectName(QString::fromUtf8("chbEnableMath"));
        chbEnableMath->setGeometry(QRect(61, 11, 70, 21));
        chbEnableSpectr = new QCheckBox(tabMath);
        chbEnableSpectr->setObjectName(QString::fromUtf8("chbEnableSpectr"));
        chbEnableSpectr->setGeometry(QRect(200, 12, 61, 20));
        chbEnableSpectr->setLayoutDirection(Qt::RightToLeft);
        cbSpectr = new QComboBox(tabMath);
        cbSpectr->setObjectName(QString::fromUtf8("cbSpectr"));
        cbSpectr->setGeometry(QRect(270, 10, 91, 22));
        tabWidget->addTab(tabMath, QString());
        tabSignals = new QWidget();
        tabSignals->setObjectName(QString::fromUtf8("tabSignals"));
        label_3 = new QLabel(tabSignals);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(220, 12, 51, 20));
        cbDrawMode = new QComboBox(tabSignals);
        cbDrawMode->setObjectName(QString::fromUtf8("cbDrawMode"));
        cbDrawMode->setGeometry(QRect(280, 10, 74, 22));
        groupBoxAccumulation = new QGroupBox(tabSignals);
        groupBoxAccumulation->setObjectName(QString::fromUtf8("groupBoxAccumulation"));
        groupBoxAccumulation->setGeometry(QRect(10, 10, 191, 71));
        chbEnableAccumulation = new QCheckBox(groupBoxAccumulation);
        chbEnableAccumulation->setObjectName(QString::fromUtf8("chbEnableAccumulation"));
        chbEnableAccumulation->setGeometry(QRect(10, 20, 81, 19));
        chbDisableAccumulationLimit = new QCheckBox(groupBoxAccumulation);
        chbDisableAccumulationLimit->setObjectName(QString::fromUtf8("chbDisableAccumulationLimit"));
        chbDisableAccumulationLimit->setGeometry(QRect(10, 45, 146, 19));
        layoutWidget2 = new QWidget(groupBoxAccumulation);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(100, 16, 76, 26));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_2->setSpacing(3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        leAccumulation = new QLineEdit(layoutWidget2);
        leAccumulation->setObjectName(QString::fromUtf8("leAccumulation"));
        leAccumulation->setMinimumSize(QSize(30, 22));
        leAccumulation->setMaximumSize(QSize(30, 22));

        horizontalLayout->addWidget(leAccumulation);

        sbAccumulation = new QSpinBox(layoutWidget2);
        sbAccumulation->setObjectName(QString::fromUtf8("sbAccumulation"));
        sbAccumulation->setMinimumSize(QSize(18, 22));
        sbAccumulation->setMaximumSize(QSize(18, 22));
        sbAccumulation->setMinimum(0);
        sbAccumulation->setMaximum(8);
        sbAccumulation->setValue(1);

        horizontalLayout->addWidget(sbAccumulation);


        horizontalLayout_2->addLayout(horizontalLayout);

        pbResetAccumulation = new QPushButton(layoutWidget2);
        pbResetAccumulation->setObjectName(QString::fromUtf8("pbResetAccumulation"));
        pbResetAccumulation->setMinimumSize(QSize(20, 20));
        pbResetAccumulation->setMaximumSize(QSize(20, 20));
        pbResetAccumulation->setIcon(icon7);

        horizontalLayout_2->addWidget(pbResetAccumulation);

        tabWidget->addTab(tabSignals, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        cbPorts = new QComboBox(tab_6);
        cbPorts->setObjectName(QString::fromUtf8("cbPorts"));
        cbPorts->setGeometry(QRect(60, 12, 81, 22));
        label_7 = new QLabel(tab_6);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 15, 31, 16));
        pbUpdatePorts = new QPushButton(tab_6);
        pbUpdatePorts->setObjectName(QString::fromUtf8("pbUpdatePorts"));
        pbUpdatePorts->setGeometry(QRect(150, 10, 81, 24));
        layoutWidget3 = new QWidget(tab_6);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 40, 258, 22));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        chbTestSignalEnable = new QCheckBox(layoutWidget3);
        chbTestSignalEnable->setObjectName(QString::fromUtf8("chbTestSignalEnable"));

        horizontalLayout_3->addWidget(chbTestSignalEnable);

        cbTestSignalType = new QComboBox(layoutWidget3);
        cbTestSignalType->setObjectName(QString::fromUtf8("cbTestSignalType"));

        horizontalLayout_3->addWidget(cbTestSignalType);

        dsbTestSignalFreq = new QDoubleSpinBox(layoutWidget3);
        dsbTestSignalFreq->setObjectName(QString::fromUtf8("dsbTestSignalFreq"));
        dsbTestSignalFreq->setDecimals(3);
        dsbTestSignalFreq->setMinimum(0.001);
        dsbTestSignalFreq->setSingleStep(0.001);

        horizontalLayout_3->addWidget(dsbTestSignalFreq);

        dsbTestSignalAmplitude = new QDoubleSpinBox(layoutWidget3);
        dsbTestSignalAmplitude->setObjectName(QString::fromUtf8("dsbTestSignalAmplitude"));
        dsbTestSignalAmplitude->setDecimals(3);
        dsbTestSignalAmplitude->setMinimum(0.001);
        dsbTestSignalAmplitude->setMaximum(50);
        dsbTestSignalAmplitude->setSingleStep(0.001);

        horizontalLayout_3->addWidget(dsbTestSignalAmplitude);

        tabWidget->addTab(tab_6, QString());
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(576, 5, 131, 226));
        groupBox_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox_4 = new QGroupBox(groupBox_3);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(51, 25, 71, 61));
        groupBox_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        rbTrigCh0 = new QRadioButton(groupBox_4);
        rbTrigCh0->setObjectName(QString::fromUtf8("rbTrigCh0"));
        rbTrigCh0->setGeometry(QRect(8, 10, 66, 19));
        rbTrigCh0->setChecked(true);
        rbTrigCh1 = new QRadioButton(groupBox_4);
        rbTrigCh1->setObjectName(QString::fromUtf8("rbTrigCh1"));
        rbTrigCh1->setGeometry(QRect(8, 35, 66, 19));
        pbResetTrigLev = new QPushButton(groupBox_3);
        pbResetTrigLev->setObjectName(QString::fromUtf8("pbResetTrigLev"));
        pbResetTrigLev->setGeometry(QRect(99, 113, 20, 20));
        pbResetTrigLev->setMinimumSize(QSize(20, 20));
        pbResetTrigLev->setMaximumSize(QSize(20, 20));
        pbResetTrigLev->setIcon(icon7);
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 92, 111, 16));
        label_11->setAlignment(Qt::AlignCenter);
        pbStart = new QPushButton(groupBox_3);
        pbStart->setObjectName(QString::fromUtf8("pbStart"));
        pbStart->setEnabled(false);
        pbStart->setGeometry(QRect(10, 190, 41, 25));
        pbStart->setIconSize(QSize(19, 19));
        pbStart->setCheckable(true);
        cbStartMode = new QComboBox(groupBox_3);
        cbStartMode->setObjectName(QString::fromUtf8("cbStartMode"));
        cbStartMode->setGeometry(QRect(10, 160, 111, 22));
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(15, 140, 101, 16));
        label_12->setAlignment(Qt::AlignCenter);
        pbSinchro = new QPushButton(groupBox_3);
        pbSinchro->setObjectName(QString::fromUtf8("pbSinchro"));
        pbSinchro->setEnabled(true);
        pbSinchro->setGeometry(QRect(60, 190, 25, 25));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/Images/FRONT.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        icon8.addFile(QString::fromUtf8(":/Images/END.bmp"), QSize(), QIcon::Normal, QIcon::On);
        icon8.addFile(QString::fromUtf8(":/Images/END.bmp"), QSize(), QIcon::Disabled, QIcon::On);
        pbSinchro->setIcon(icon8);
        pbSinchro->setIconSize(QSize(19, 19));
        pbSinchro->setCheckable(true);
        pbSinchro->setChecked(false);
        pbFiltrSinchro = new QPushButton(groupBox_3);
        pbFiltrSinchro->setObjectName(QString::fromUtf8("pbFiltrSinchro"));
        pbFiltrSinchro->setEnabled(true);
        pbFiltrSinchro->setGeometry(QRect(95, 190, 25, 25));
        pbFiltrSinchro->setIconSize(QSize(19, 19));
        pbFiltrSinchro->setCheckable(true);
        pbFiltrSinchro->setChecked(false);
        layoutWidget4 = new QWidget(groupBox_3);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(10, 112, 93, 24));
        formLayout_5 = new QFormLayout(layoutWidget4);
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        formLayout_5->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_5->setHorizontalSpacing(0);
        formLayout_5->setContentsMargins(0, 0, 0, 0);
        leTrigLevAbs = new QLineEdit(layoutWidget4);
        leTrigLevAbs->setObjectName(QString::fromUtf8("leTrigLevAbs"));
        leTrigLevAbs->setEnabled(true);
        leTrigLevAbs->setMinimumSize(QSize(66, 22));
        leTrigLevAbs->setMaximumSize(QSize(66, 22));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, leTrigLevAbs);

        sbTrigLev = new QSpinBox(layoutWidget4);
        sbTrigLev->setObjectName(QString::fromUtf8("sbTrigLev"));
        sbTrigLev->setMinimumSize(QSize(18, 22));
        sbTrigLev->setMaximumSize(QSize(18, 22));
        sbTrigLev->setMouseTracking(true);

        formLayout_5->setWidget(0, QFormLayout::FieldRole, sbTrigLev);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(576, 297, 381, 121));
        chbUCursTogether = new QCheckBox(groupBox_2);
        chbUCursTogether->setObjectName(QString::fromUtf8("chbUCursTogether"));
        chbUCursTogether->setEnabled(true);
        chbUCursTogether->setGeometry(QRect(347, 93, 24, 16));
        leDeltaUAbs = new QLineEdit(groupBox_2);
        leDeltaUAbs->setObjectName(QString::fromUtf8("leDeltaUAbs"));
        leDeltaUAbs->setEnabled(false);
        leDeltaUAbs->setGeometry(QRect(46, 89, 66, 22));
        leDeltaUAbs->setMinimumSize(QSize(66, 22));
        leDeltaUAbs->setMaximumSize(QSize(66, 22));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(119, 92, 16, 16));
        label_9->setAlignment(Qt::AlignCenter);
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(119, 48, 20, 20));
        label_15->setAlignment(Qt::AlignCenter);
        chbTCursTogether = new QCheckBox(groupBox_2);
        chbTCursTogether->setObjectName(QString::fromUtf8("chbTCursTogether"));
        chbTCursTogether->setEnabled(true);
        chbTCursTogether->setGeometry(QRect(346, 51, 20, 20));
        label_16 = new QLabel(groupBox_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(235, 52, 16, 16));
        label_16->setAlignment(Qt::AlignCenter);
        leDeltaTAbs = new QLineEdit(groupBox_2);
        leDeltaTAbs->setObjectName(QString::fromUtf8("leDeltaTAbs"));
        leDeltaTAbs->setEnabled(false);
        leDeltaTAbs->setGeometry(QRect(46, 49, 66, 22));
        leDeltaTAbs->setMinimumSize(QSize(66, 22));
        leDeltaTAbs->setMaximumSize(QSize(66, 22));
        cbSourceCurs = new QComboBox(groupBox_2);
        cbSourceCurs->setObjectName(QString::fromUtf8("cbSourceCurs"));
        cbSourceCurs->setGeometry(QRect(254, 17, 81, 22));
        label_17 = new QLabel(groupBox_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(186, 20, 61, 16));
        chbUCurs = new QCheckBox(groupBox_2);
        chbUCurs->setObjectName(QString::fromUtf8("chbUCurs"));
        chbUCurs->setGeometry(QRect(10, 91, 36, 19));
        chbUCurs->setLayoutDirection(Qt::LeftToRight);
        chbTCurs = new QCheckBox(groupBox_2);
        chbTCurs->setObjectName(QString::fromUtf8("chbTCurs"));
        chbTCurs->setGeometry(QRect(10, 51, 31, 20));
        chbTCurs->setLayoutDirection(Qt::LeftToRight);
        chbTCurs->setTristate(false);
        lePercentage = new QLineEdit(groupBox_2);
        lePercentage->setObjectName(QString::fromUtf8("lePercentage"));
        lePercentage->setGeometry(QRect(46, 18, 66, 22));
        lePercentage->setMinimumSize(QSize(66, 22));
        lePercentage->setMaximumSize(QSize(66, 22));
        pbPercentage = new QPushButton(groupBox_2);
        pbPercentage->setObjectName(QString::fromUtf8("pbPercentage"));
        pbPercentage->setGeometry(QRect(120, 19, 20, 20));
        pbPercentage->setMinimumSize(QSize(20, 20));
        pbPercentage->setMaximumSize(QSize(20, 20));
        pbPercentage->setIcon(icon7);
        label_19 = new QLabel(groupBox_2);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(230, 90, 21, 20));
        label_19->setAlignment(Qt::AlignCenter);
        layoutWidget5 = new QWidget(groupBox_2);
        layoutWidget5->setObjectName(QString::fromUtf8("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(255, 88, 91, 24));
        formLayout = new QFormLayout(layoutWidget5);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setHorizontalSpacing(0);
        formLayout->setContentsMargins(0, 0, 0, 0);
        leUCur2Abs = new QLineEdit(layoutWidget5);
        leUCur2Abs->setObjectName(QString::fromUtf8("leUCur2Abs"));
        leUCur2Abs->setEnabled(false);
        leUCur2Abs->setMinimumSize(QSize(66, 22));
        leUCur2Abs->setMaximumSize(QSize(66, 22));

        formLayout->setWidget(0, QFormLayout::LabelRole, leUCur2Abs);

        sbUCur2 = new QSpinBox(layoutWidget5);
        sbUCur2->setObjectName(QString::fromUtf8("sbUCur2"));
        sbUCur2->setEnabled(false);
        sbUCur2->setMinimumSize(QSize(18, 22));
        sbUCur2->setMaximumSize(QSize(18, 22));
        sbUCur2->setMaximum(500);

        formLayout->setWidget(0, QFormLayout::FieldRole, sbUCur2);

        layoutWidget6 = new QWidget(groupBox_2);
        layoutWidget6->setObjectName(QString::fromUtf8("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(255, 50, 93, 24));
        formLayout_2 = new QFormLayout(layoutWidget6);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setHorizontalSpacing(0);
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        leTCur2Abs = new QLineEdit(layoutWidget6);
        leTCur2Abs->setObjectName(QString::fromUtf8("leTCur2Abs"));
        leTCur2Abs->setEnabled(false);
        leTCur2Abs->setMinimumSize(QSize(66, 22));
        leTCur2Abs->setMaximumSize(QSize(66, 22));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, leTCur2Abs);

        sbTCur2 = new QSpinBox(layoutWidget6);
        sbTCur2->setObjectName(QString::fromUtf8("sbTCur2"));
        sbTCur2->setEnabled(false);
        sbTCur2->setMinimumSize(QSize(18, 22));
        sbTCur2->setMaximumSize(QSize(18, 22));
        sbTCur2->setMaximum(500);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, sbTCur2);

        layoutWidget7 = new QWidget(groupBox_2);
        layoutWidget7->setObjectName(QString::fromUtf8("layoutWidget7"));
        layoutWidget7->setGeometry(QRect(138, 89, 93, 24));
        formLayout_3 = new QFormLayout(layoutWidget7);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setHorizontalSpacing(0);
        formLayout_3->setContentsMargins(0, 0, 0, 0);
        leUCur1Abs = new QLineEdit(layoutWidget7);
        leUCur1Abs->setObjectName(QString::fromUtf8("leUCur1Abs"));
        leUCur1Abs->setEnabled(false);
        leUCur1Abs->setMinimumSize(QSize(66, 22));
        leUCur1Abs->setMaximumSize(QSize(66, 22));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, leUCur1Abs);

        sbUCur1 = new QSpinBox(layoutWidget7);
        sbUCur1->setObjectName(QString::fromUtf8("sbUCur1"));
        sbUCur1->setEnabled(false);
        sbUCur1->setMinimumSize(QSize(16, 22));
        sbUCur1->setMaximumSize(QSize(16, 22));
        sbUCur1->setMaximum(500);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, sbUCur1);

        layoutWidget8 = new QWidget(groupBox_2);
        layoutWidget8->setObjectName(QString::fromUtf8("layoutWidget8"));
        layoutWidget8->setGeometry(QRect(138, 49, 87, 24));
        formLayout_4 = new QFormLayout(layoutWidget8);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setHorizontalSpacing(0);
        formLayout_4->setContentsMargins(0, 0, 0, 0);
        leTCur1Abs = new QLineEdit(layoutWidget8);
        leTCur1Abs->setObjectName(QString::fromUtf8("leTCur1Abs"));
        leTCur1Abs->setEnabled(false);
        leTCur1Abs->setMinimumSize(QSize(66, 22));
        leTCur1Abs->setMaximumSize(QSize(66, 22));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, leTCur1Abs);

        sbTCur1 = new QSpinBox(layoutWidget8);
        sbTCur1->setObjectName(QString::fromUtf8("sbTCur1"));
        sbTCur1->setEnabled(false);
        sbTCur1->setMinimumSize(QSize(18, 22));
        sbTCur1->setMaximumSize(QSize(18, 22));
        sbTCur1->setMinimum(-3000);
        sbTCur1->setMaximum(3000);
        sbTCur1->setSingleStep(1);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, sbTCur1);

        label_18 = new QLabel(groupBox_2);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(22, 21, 21, 16));
        groupBox_9 = new QGroupBox(centralwidget);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        groupBox_9->setGeometry(QRect(576, 231, 276, 66));
        label_6 = new QLabel(groupBox_9);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(206, 13, 61, 20));
        label_6->setAlignment(Qt::AlignCenter);
        cbTPos = new QComboBox(groupBox_9);
        cbTPos->setObjectName(QString::fromUtf8("cbTPos"));
        cbTPos->setGeometry(QRect(207, 34, 61, 22));
        pbResetTShift = new QPushButton(groupBox_9);
        pbResetTShift->setObjectName(QString::fromUtf8("pbResetTShift"));
        pbResetTShift->setGeometry(QRect(176, 35, 20, 20));
        pbResetTShift->setMinimumSize(QSize(20, 20));
        pbResetTShift->setMaximumSize(QSize(20, 20));
        pbResetTShift->setIcon(icon7);
        sbTShift = new QSpinBox(groupBox_9);
        sbTShift->setObjectName(QString::fromUtf8("sbTShift"));
        sbTShift->setGeometry(QRect(151, 34, 18, 22));
        sbTShift->setMinimumSize(QSize(18, 22));
        sbTShift->setMaximumSize(QSize(18, 22));
        sbTShift->setMouseTracking(true);
        sbTShift->setFrame(true);
        sbTShift->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        label_5 = new QLabel(groupBox_9);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(86, 15, 66, 16));
        label_5->setAlignment(Qt::AlignCenter);
        leTShiftAbs = new QLineEdit(groupBox_9);
        leTShiftAbs->setObjectName(QString::fromUtf8("leTShiftAbs"));
        leTShiftAbs->setGeometry(QRect(86, 34, 66, 22));
        leTShiftAbs->setMinimumSize(QSize(66, 22));
        leTShiftAbs->setMaximumSize(QSize(66, 22));
        leTShiftAbs->setFocusPolicy(Qt::ClickFocus);
        label_4 = new QLabel(groupBox_9);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 10, 46, 20));
        label_4->setAlignment(Qt::AlignCenter);
        cbTBase = new QComboBox(groupBox_9);
        cbTBase->setObjectName(QString::fromUtf8("cbTBase"));
        cbTBase->setGeometry(QRect(15, 34, 61, 22));
        cbTBase->setMaxVisibleItems(33);
        WindowOsci->setCentralWidget(centralwidget);

        retranslateUi(WindowOsci);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(WindowOsci);
    } // setupUi

    void retranslateUi(QMainWindow *WindowOsci)
    {
        WindowOsci->setWindowTitle(QApplication::translate("WindowOsci", "\320\236\321\201\321\206\320\270\320\273\320\273\320\276\320\263\321\200\320\260\321\204", 0, QApplication::UnicodeUTF8));
        groupBoxChannel2->setTitle(QApplication::translate("WindowOsci", "\320\232\320\260\320\275\320\260\320\273 2", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QString());
        rbPOS_Ch1->setText(QString());
        rbNEG_Ch1->setText(QString());
        rbDIS_Ch1->setText(QString());
        label_8->setText(QApplication::translate("WindowOsci", "V/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QString());
        rbAC_Channel1->setText(QString());
        rbDC_Channel1->setText(QString());
        rbGND_Channel1->setText(QString());
        cbRangeChannel1->clear();
        cbRangeChannel1->insertItems(0, QStringList()
         << QApplication::translate("WindowOsci", "5 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "10 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "20 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "50 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "100 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "200 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "500 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "1 V", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "2 V", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "5 V", 0, QApplication::UnicodeUTF8)
        );
        labelRShift2->setText(QApplication::translate("WindowOsci", "Y \320\277\320\276\320\267", 0, QApplication::UnicodeUTF8));
        pbResetRShift1->setText(QString());
        pbFiltrPalm1->setText(QApplication::translate("WindowOsci", "\320\237\320\241", 0, QApplication::UnicodeUTF8));
        groupBoxChannel1->setTitle(QApplication::translate("WindowOsci", "\320\232\320\260\320\275\320\260\320\273 1", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QString());
        rbPOS_Ch0->setText(QString());
        rbNEG_Ch0->setText(QString());
        rbDIS_Ch0->setText(QString());
        groupBox->setTitle(QString());
        rbAC_Channel0->setText(QString());
        rbDC_Channel0->setText(QString());
        rbGND_Channel0->setText(QString());
        cbRangeChannel0->clear();
        cbRangeChannel0->insertItems(0, QStringList()
         << QApplication::translate("WindowOsci", "5 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "10 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "20 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "50 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "100 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "200 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "500 mV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "1 V", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "2 V", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "5 V", 0, QApplication::UnicodeUTF8)
        );
        labelRShift1->setText(QApplication::translate("WindowOsci", "Y \320\277\320\276\320\267", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WindowOsci", "V/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8));
        pbResetRShift0->setText(QString());
        pbFiltrPalm0->setText(QApplication::translate("WindowOsci", "\320\237\320\241", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("WindowOsci", "\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\270\321\217", 0, QApplication::UnicodeUTF8));
        label_44->setText(QApplication::translate("WindowOsci", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \321\202\320\276\321\207\320\265\320\272 \320\264\320\273\321\217 \n"
"\321\201\320\263\320\273\320\260\320\266\320\270\320\262\320\260\321\216\321\211\320\265\320\263\320\276 \321\204\320\270\320\273\321\214\321\202\321\200\320\260", 0, QApplication::UnicodeUTF8));
        chbEnableAverage->setText(QApplication::translate("WindowOsci", "\320\243\321\201\321\200\320\265\320\264\320\275\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        cbMathFunc->clear();
        cbMathFunc->insertItems(0, QStringList()
         << QApplication::translate("WindowOsci", "1 + 2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "1 * 2", 0, QApplication::UnicodeUTF8)
        );
        chbEnableMath->setText(QApplication::translate("WindowOsci", "\320\244\321\203\320\275\320\272\321\206\320\270\321\217", 0, QApplication::UnicodeUTF8));
        chbEnableSpectr->setText(QApplication::translate("WindowOsci", "\320\241\320\277\320\265\320\272\321\202\321\200", 0, QApplication::UnicodeUTF8));
        cbSpectr->clear();
        cbSpectr->insertItems(0, QStringList()
         << QApplication::translate("WindowOsci", "\320\232\320\260\320\275\320\260\320\273 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "\320\232\320\260\320\275\320\260\320\273 2", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tabMath), QApplication::translate("WindowOsci", "\320\234\320\260\321\202", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("WindowOsci", "\320\236\321\202\320\276\320\261\321\200\320\260\320\266", 0, QApplication::UnicodeUTF8));
        cbDrawMode->clear();
        cbDrawMode->insertItems(0, QStringList()
         << QApplication::translate("WindowOsci", "\320\242\320\276\321\207\320\272\320\270", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "\320\222\320\265\320\272\321\202\320\276\321\200", 0, QApplication::UnicodeUTF8)
        );
        groupBoxAccumulation->setTitle(QApplication::translate("WindowOsci", "\320\235\320\260\320\272\320\276\320\277\320\273\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        chbEnableAccumulation->setText(QApplication::translate("WindowOsci", "\320\222\320\272\320\273\321\216\321\207\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        chbDisableAccumulationLimit->setText(QApplication::translate("WindowOsci", "\320\222\321\213\320\272\320\273\321\216\321\207\320\270\321\202\321\214 \320\276\320\263\321\200\320\260\320\275\320\270\321\207\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        pbResetAccumulation->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabSignals), QApplication::translate("WindowOsci", "\320\241\320\270\320\263\320\275\320\260\320\273\321\213", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("WindowOsci", "\320\237\320\276\321\200\321\202", 0, QApplication::UnicodeUTF8));
        pbUpdatePorts->setText(QApplication::translate("WindowOsci", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        chbTestSignalEnable->setText(QApplication::translate("WindowOsci", "\320\222\320\272\320\273.", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("WindowOsci", "\320\224\320\276\320\277", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("WindowOsci", "\320\241\320\270\320\275\321\205\321\200\320\276\320\275\320\270\320\267\320\260\321\206\320\270\321\217", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QString());
        rbTrigCh0->setText(QApplication::translate("WindowOsci", "\320\232\320\260\320\275\320\260\320\273 1", 0, QApplication::UnicodeUTF8));
        rbTrigCh1->setText(QApplication::translate("WindowOsci", "\320\232\320\260\320\275\320\260\320\273 2", 0, QApplication::UnicodeUTF8));
        pbResetTrigLev->setText(QString());
        label_11->setText(QApplication::translate("WindowOsci", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214", 0, QApplication::UnicodeUTF8));
        pbStart->setText(QApplication::translate("WindowOsci", "\320\241\321\202\320\260\321\200\321\202", 0, QApplication::UnicodeUTF8));
        cbStartMode->clear();
        cbStartMode->insertItems(0, QStringList()
         << QApplication::translate("WindowOsci", "\320\220\320\262\321\202\320\276\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\271", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "\320\226\320\264\321\203\321\211\320\270\320\271", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "\320\236\320\264\320\275\320\276\320\272\321\200\320\260\321\202\320\275\321\213\320\271", 0, QApplication::UnicodeUTF8)
        );
        label_12->setText(QApplication::translate("WindowOsci", "\320\240\320\265\320\266\320\270\320\274 \320\267\320\260\320\277\321\203\321\201\320\272\320\260", 0, QApplication::UnicodeUTF8));
        pbSinchro->setText(QString());
        pbFiltrSinchro->setText(QApplication::translate("WindowOsci", "\320\237\320\241", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("WindowOsci", "\320\232\321\203\321\200\321\201\320\276\321\200\321\213", 0, QApplication::UnicodeUTF8));
        chbUCursTogether->setText(QString());
        label_9->setText(QApplication::translate("WindowOsci", "U1", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("WindowOsci", "T1", 0, QApplication::UnicodeUTF8));
        chbTCursTogether->setText(QString());
        label_16->setText(QApplication::translate("WindowOsci", "T2", 0, QApplication::UnicodeUTF8));
        cbSourceCurs->clear();
        cbSourceCurs->insertItems(0, QStringList()
         << QApplication::translate("WindowOsci", "\320\232\320\260\320\275\320\260\320\273 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "\320\232\320\260\320\275\320\260\320\273 2", 0, QApplication::UnicodeUTF8)
        );
        label_17->setText(QApplication::translate("WindowOsci", "\320\230\321\201\321\202\320\276\321\207\320\275\320\270\320\272", 0, QApplication::UnicodeUTF8));
        chbUCurs->setText(QApplication::translate("WindowOsci", "dU", 0, QApplication::UnicodeUTF8));
        chbTCurs->setText(QApplication::translate("WindowOsci", "dT", 0, QApplication::UnicodeUTF8));
        pbPercentage->setText(QString());
        label_19->setText(QApplication::translate("WindowOsci", "U2", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("WindowOsci", "%", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("WindowOsci", "\320\240\320\260\320\267\320\262\321\221\321\200\321\202\320\272\320\260", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("WindowOsci", "\320\235\320\260\321\207 \320\272\320\276\320\276\321\200\320\264", 0, QApplication::UnicodeUTF8));
        cbTPos->clear();
        cbTPos->insertItems(0, QStringList()
         << QApplication::translate("WindowOsci", "\320\233\320\265\320\262\320\276", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "\320\246\320\265\320\275\321\202\321\200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "\320\237\321\200\320\260\320\262\320\276", 0, QApplication::UnicodeUTF8)
        );
        pbResetTShift->setText(QString());
        label_5->setText(QApplication::translate("WindowOsci", "X \320\277\320\276\320\267", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("WindowOsci", "S/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8));
        cbTBase->clear();
        cbTBase->insertItems(0, QStringList()
         << QApplication::translate("WindowOsci", "10 ns", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "20 ns", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "50 ns", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "100 ns", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "200 ns", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "500 ns", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "1 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "2 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "5 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "10 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "20 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "50 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "100 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "200 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "500 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "1 ms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "2 ms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "5 ms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "10 ms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "20 ms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "50 ms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "100 ms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "200 ms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "500 ms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "1 s", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowOsci", "2 s", 0, QApplication::UnicodeUTF8)
        );
        Q_UNUSED(WindowOsci);
    } // retranslateUi

};

namespace Ui {
    class WindowOsci: public Ui_WindowOsci {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWOSCI_H
