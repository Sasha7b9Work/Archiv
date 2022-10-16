/********************************************************************************
** Form generated from reading ui file 'WindowRegistrator.ui'
**
** Created: Fri Jun 22 10:01:55 2012
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WINDOWREGISTRATOR_H
#define UI_WINDOWREGISTRATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollBar>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WindowRegistrator
{
public:
    QAction *aExit;
    QAction *aColorBackground;
    QAction *aColorGrid;
    QAction *aColorSignal;
    QAction *aCalibrate;
    QWidget *centralwidget;
    QFrame *frame0;
    QComboBox *cbTime;
    QLabel *label;
    QComboBox *cbU0;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pbStart;
    QComboBox *cbPort;
    QPushButton *pbUpdatePorts;
    QFrame *frame1;
    QComboBox *cbU1;
    QScrollBar *hsbCh0;
    QScrollBar *hsbCh1;
    QPushButton *pbDir;
    QPushButton *pbOpen;
    QComboBox *cbLengthRecord;
    QLabel *label_5;
    QGroupBox *groupBox;
    QCheckBox *chbUCursCh0;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_6;
    QFormLayout *formLayout_5;
    QLineEdit *leUCur0Ch0;
    QSpinBox *sbUCur0Ch0;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_9;
    QFormLayout *formLayout_8;
    QLineEdit *leUCur1Ch0;
    QSpinBox *sbUCur1Ch0;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_7;
    QFormLayout *formLayout_6;
    QLineEdit *leDeltaUCurCh0;
    QGroupBox *groupBox_2;
    QCheckBox *chbUCursCh1;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_11;
    QFormLayout *formLayout_10;
    QLineEdit *leUCur0Ch1;
    QSpinBox *sbUCur0Ch1;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_10;
    QFormLayout *formLayout_9;
    QLineEdit *leUCur1Ch1;
    QSpinBox *sbUCur1Ch1;
    QWidget *layoutWidget5;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_8;
    QFormLayout *formLayout_7;
    QLineEdit *leDeltaUCurCh1;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;

    void setupUi(QMainWindow *WindowRegistrator)
    {
        if (WindowRegistrator->objectName().isEmpty())
            WindowRegistrator->setObjectName(QString::fromUtf8("WindowRegistrator"));
        WindowRegistrator->setWindowModality(Qt::NonModal);
        WindowRegistrator->resize(894, 700);
        WindowRegistrator->setMinimumSize(QSize(894, 700));
        WindowRegistrator->setMaximumSize(QSize(894, 700));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/newPrefix/Registro.ico"), QSize(), QIcon::Normal, QIcon::Off);
        WindowRegistrator->setWindowIcon(icon);
        aExit = new QAction(WindowRegistrator);
        aExit->setObjectName(QString::fromUtf8("aExit"));
        aColorBackground = new QAction(WindowRegistrator);
        aColorBackground->setObjectName(QString::fromUtf8("aColorBackground"));
        aColorGrid = new QAction(WindowRegistrator);
        aColorGrid->setObjectName(QString::fromUtf8("aColorGrid"));
        aColorSignal = new QAction(WindowRegistrator);
        aColorSignal->setObjectName(QString::fromUtf8("aColorSignal"));
        aCalibrate = new QAction(WindowRegistrator);
        aCalibrate->setObjectName(QString::fromUtf8("aCalibrate"));
        centralwidget = new QWidget(WindowRegistrator);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        frame0 = new QFrame(centralwidget);
        frame0->setObjectName(QString::fromUtf8("frame0"));
        frame0->setGeometry(QRect(10, 10, 601, 304));
        frame0->setFrameShape(QFrame::StyledPanel);
        frame0->setFrameShadow(QFrame::Sunken);
        cbTime = new QComboBox(centralwidget);
        cbTime->setObjectName(QString::fromUtf8("cbTime"));
        cbTime->setGeometry(QRect(780, 19, 74, 22));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(635, 22, 131, 16));
        cbU0 = new QComboBox(centralwidget);
        cbU0->setObjectName(QString::fromUtf8("cbU0"));
        cbU0->setGeometry(QRect(780, 51, 74, 22));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(635, 54, 121, 16));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(635, 86, 121, 16));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(636, 225, 41, 16));
        pbStart = new QPushButton(centralwidget);
        pbStart->setObjectName(QString::fromUtf8("pbStart"));
        pbStart->setEnabled(false);
        pbStart->setGeometry(QRect(630, 120, 71, 24));
        pbStart->setCheckable(true);
        cbPort = new QComboBox(centralwidget);
        cbPort->setObjectName(QString::fromUtf8("cbPort"));
        cbPort->setGeometry(QRect(679, 222, 74, 22));
        pbUpdatePorts = new QPushButton(centralwidget);
        pbUpdatePorts->setObjectName(QString::fromUtf8("pbUpdatePorts"));
        pbUpdatePorts->setGeometry(QRect(774, 220, 75, 24));
        pbUpdatePorts->setCheckable(false);
        frame1 = new QFrame(centralwidget);
        frame1->setObjectName(QString::fromUtf8("frame1"));
        frame1->setGeometry(QRect(10, 350, 601, 304));
        frame1->setFrameShape(QFrame::StyledPanel);
        frame1->setFrameShadow(QFrame::Sunken);
        cbU1 = new QComboBox(centralwidget);
        cbU1->setObjectName(QString::fromUtf8("cbU1"));
        cbU1->setGeometry(QRect(780, 82, 74, 22));
        hsbCh0 = new QScrollBar(centralwidget);
        hsbCh0->setObjectName(QString::fromUtf8("hsbCh0"));
        hsbCh0->setGeometry(QRect(10, 313, 601, 16));
        hsbCh0->setLayoutDirection(Qt::LeftToRight);
        hsbCh0->setMaximum(0);
        hsbCh0->setPageStep(500);
        hsbCh0->setOrientation(Qt::Horizontal);
        hsbCh1 = new QScrollBar(centralwidget);
        hsbCh1->setObjectName(QString::fromUtf8("hsbCh1"));
        hsbCh1->setGeometry(QRect(10, 653, 601, 16));
        hsbCh1->setLayoutDirection(Qt::LeftToRight);
        hsbCh1->setMaximum(0);
        hsbCh1->setPageStep(500);
        hsbCh1->setOrientation(Qt::Horizontal);
        pbDir = new QPushButton(centralwidget);
        pbDir->setObjectName(QString::fromUtf8("pbDir"));
        pbDir->setGeometry(QRect(716, 120, 71, 24));
        pbDir->setCheckable(false);
        pbOpen = new QPushButton(centralwidget);
        pbOpen->setObjectName(QString::fromUtf8("pbOpen"));
        pbOpen->setGeometry(QRect(800, 120, 71, 24));
        pbOpen->setCheckable(false);
        cbLengthRecord = new QComboBox(centralwidget);
        cbLengthRecord->setObjectName(QString::fromUtf8("cbLengthRecord"));
        cbLengthRecord->setGeometry(QRect(722, 158, 101, 20));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(629, 160, 81, 16));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(630, 270, 251, 95));
        chbUCursCh0 = new QCheckBox(groupBox);
        chbUCursCh0->setObjectName(QString::fromUtf8("chbUCursCh0"));
        chbUCursCh0->setGeometry(QRect(12, 24, 16, 20));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(39, 21, 92, 26));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_6);

        formLayout_5 = new QFormLayout();
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        formLayout_5->setHorizontalSpacing(0);
        leUCur0Ch0 = new QLineEdit(layoutWidget);
        leUCur0Ch0->setObjectName(QString::fromUtf8("leUCur0Ch0"));
        leUCur0Ch0->setEnabled(false);
        leUCur0Ch0->setMinimumSize(QSize(50, 22));
        leUCur0Ch0->setMaximumSize(QSize(50, 22));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, leUCur0Ch0);

        sbUCur0Ch0 = new QSpinBox(layoutWidget);
        sbUCur0Ch0->setObjectName(QString::fromUtf8("sbUCur0Ch0"));
        sbUCur0Ch0->setEnabled(false);
        sbUCur0Ch0->setMinimumSize(QSize(18, 22));
        sbUCur0Ch0->setMaximumSize(QSize(18, 22));
        sbUCur0Ch0->setMinimum(0);
        sbUCur0Ch0->setMaximum(0);
        sbUCur0Ch0->setSingleStep(1);

        formLayout_5->setWidget(0, QFormLayout::FieldRole, sbUCur0Ch0);


        horizontalLayout->addLayout(formLayout_5);

        layoutWidget1 = new QWidget(groupBox);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(146, 21, 92, 26));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(layoutWidget1);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_9);

        formLayout_8 = new QFormLayout();
        formLayout_8->setObjectName(QString::fromUtf8("formLayout_8"));
        formLayout_8->setHorizontalSpacing(0);
        leUCur1Ch0 = new QLineEdit(layoutWidget1);
        leUCur1Ch0->setObjectName(QString::fromUtf8("leUCur1Ch0"));
        leUCur1Ch0->setEnabled(false);
        leUCur1Ch0->setMinimumSize(QSize(50, 22));
        leUCur1Ch0->setMaximumSize(QSize(50, 22));

        formLayout_8->setWidget(0, QFormLayout::LabelRole, leUCur1Ch0);

        sbUCur1Ch0 = new QSpinBox(layoutWidget1);
        sbUCur1Ch0->setObjectName(QString::fromUtf8("sbUCur1Ch0"));
        sbUCur1Ch0->setEnabled(false);
        sbUCur1Ch0->setMinimumSize(QSize(18, 22));
        sbUCur1Ch0->setMaximumSize(QSize(18, 22));
        sbUCur1Ch0->setMinimum(0);
        sbUCur1Ch0->setMaximum(0);
        sbUCur1Ch0->setSingleStep(1);

        formLayout_8->setWidget(0, QFormLayout::FieldRole, sbUCur1Ch0);


        horizontalLayout_4->addLayout(formLayout_8);

        layoutWidget2 = new QWidget(groupBox);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(146, 55, 73, 26));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(layoutWidget2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_7);

        formLayout_6 = new QFormLayout();
        formLayout_6->setObjectName(QString::fromUtf8("formLayout_6"));
        formLayout_6->setHorizontalSpacing(0);
        leDeltaUCurCh0 = new QLineEdit(layoutWidget2);
        leDeltaUCurCh0->setObjectName(QString::fromUtf8("leDeltaUCurCh0"));
        leDeltaUCurCh0->setEnabled(false);
        leDeltaUCurCh0->setMinimumSize(QSize(50, 22));
        leDeltaUCurCh0->setMaximumSize(QSize(50, 22));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, leDeltaUCurCh0);


        horizontalLayout_2->addLayout(formLayout_6);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(630, 370, 251, 95));
        chbUCursCh1 = new QCheckBox(groupBox_2);
        chbUCursCh1->setObjectName(QString::fromUtf8("chbUCursCh1"));
        chbUCursCh1->setGeometry(QRect(13, 24, 16, 19));
        layoutWidget3 = new QWidget(groupBox_2);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(40, 21, 92, 26));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(layoutWidget3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(label_11);

        formLayout_10 = new QFormLayout();
        formLayout_10->setObjectName(QString::fromUtf8("formLayout_10"));
        formLayout_10->setHorizontalSpacing(0);
        leUCur0Ch1 = new QLineEdit(layoutWidget3);
        leUCur0Ch1->setObjectName(QString::fromUtf8("leUCur0Ch1"));
        leUCur0Ch1->setEnabled(false);
        leUCur0Ch1->setMinimumSize(QSize(50, 22));
        leUCur0Ch1->setMaximumSize(QSize(50, 22));

        formLayout_10->setWidget(0, QFormLayout::LabelRole, leUCur0Ch1);

        sbUCur0Ch1 = new QSpinBox(layoutWidget3);
        sbUCur0Ch1->setObjectName(QString::fromUtf8("sbUCur0Ch1"));
        sbUCur0Ch1->setEnabled(false);
        sbUCur0Ch1->setMinimumSize(QSize(18, 22));
        sbUCur0Ch1->setMaximumSize(QSize(18, 22));
        sbUCur0Ch1->setMinimum(0);
        sbUCur0Ch1->setMaximum(0);
        sbUCur0Ch1->setSingleStep(1);

        formLayout_10->setWidget(0, QFormLayout::FieldRole, sbUCur0Ch1);


        horizontalLayout_6->addLayout(formLayout_10);

        layoutWidget4 = new QWidget(groupBox_2);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(146, 21, 92, 26));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(layoutWidget4);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_10);

        formLayout_9 = new QFormLayout();
        formLayout_9->setObjectName(QString::fromUtf8("formLayout_9"));
        formLayout_9->setHorizontalSpacing(0);
        leUCur1Ch1 = new QLineEdit(layoutWidget4);
        leUCur1Ch1->setObjectName(QString::fromUtf8("leUCur1Ch1"));
        leUCur1Ch1->setEnabled(false);
        leUCur1Ch1->setMinimumSize(QSize(50, 22));
        leUCur1Ch1->setMaximumSize(QSize(50, 22));

        formLayout_9->setWidget(0, QFormLayout::LabelRole, leUCur1Ch1);

        sbUCur1Ch1 = new QSpinBox(layoutWidget4);
        sbUCur1Ch1->setObjectName(QString::fromUtf8("sbUCur1Ch1"));
        sbUCur1Ch1->setEnabled(false);
        sbUCur1Ch1->setMinimumSize(QSize(18, 22));
        sbUCur1Ch1->setMaximumSize(QSize(18, 22));
        sbUCur1Ch1->setMinimum(0);
        sbUCur1Ch1->setMaximum(0);
        sbUCur1Ch1->setSingleStep(1);

        formLayout_9->setWidget(0, QFormLayout::FieldRole, sbUCur1Ch1);


        horizontalLayout_5->addLayout(formLayout_9);

        layoutWidget5 = new QWidget(groupBox_2);
        layoutWidget5->setObjectName(QString::fromUtf8("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(146, 55, 73, 26));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget5);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(layoutWidget5);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_8);

        formLayout_7 = new QFormLayout();
        formLayout_7->setObjectName(QString::fromUtf8("formLayout_7"));
        formLayout_7->setHorizontalSpacing(0);
        leDeltaUCurCh1 = new QLineEdit(layoutWidget5);
        leDeltaUCurCh1->setObjectName(QString::fromUtf8("leDeltaUCurCh1"));
        leDeltaUCurCh1->setEnabled(false);
        leDeltaUCurCh1->setMinimumSize(QSize(50, 22));
        leDeltaUCurCh1->setMaximumSize(QSize(50, 22));

        formLayout_7->setWidget(0, QFormLayout::LabelRole, leDeltaUCurCh1);


        horizontalLayout_3->addLayout(formLayout_7);

        WindowRegistrator->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(WindowRegistrator);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 894, 21));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menu_2);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        WindowRegistrator->setMenuBar(menuBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(aExit);
        menu_2->addAction(menu_3->menuAction());
        menu_2->addAction(aCalibrate);
        menu_3->addAction(aColorBackground);
        menu_3->addAction(aColorGrid);
        menu_3->addAction(aColorSignal);

        retranslateUi(WindowRegistrator);

        QMetaObject::connectSlotsByName(WindowRegistrator);
    } // setupUi

    void retranslateUi(QMainWindow *WindowRegistrator)
    {
        WindowRegistrator->setWindowTitle(QApplication::translate("WindowRegistrator", "\320\240\320\265\320\263\320\270\321\201\321\202\321\200\320\260\321\202\320\276\321\200", 0, QApplication::UnicodeUTF8));
        aExit->setText(QApplication::translate("WindowRegistrator", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        aColorBackground->setText(QApplication::translate("WindowRegistrator", "\320\244\320\276\320\275", 0, QApplication::UnicodeUTF8));
        aColorGrid->setText(QApplication::translate("WindowRegistrator", "\320\241\320\265\321\202\320\272\320\260", 0, QApplication::UnicodeUTF8));
        aColorSignal->setText(QApplication::translate("WindowRegistrator", "\320\241\320\270\320\263\320\275\320\260\320\273", 0, QApplication::UnicodeUTF8));
        aCalibrate->setText(QApplication::translate("WindowRegistrator", "\320\232\320\260\320\273\320\270\320\261\321\200\320\276\320\262\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WindowRegistrator", "\320\232\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202 \321\200\320\260\320\267\320\262\321\221\321\200\321\202\320\272\320\270", 0, QApplication::UnicodeUTF8));
        cbU0->clear();
        cbU0->insertItems(0, QStringList()
         << QApplication::translate("WindowRegistrator", "5mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "10mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "20mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "50mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "100mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "200mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "500mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "1V/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "2V/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "5V/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("WindowRegistrator", "\320\224\320\270\320\260\320\277 \320\272\320\260\320\275 1", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("WindowRegistrator", "\320\224\320\270\320\260\320\277 \320\272\320\260\320\275 2", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("WindowRegistrator", "\320\237\320\276\321\200\321\202", 0, QApplication::UnicodeUTF8));
        pbStart->setText(QApplication::translate("WindowRegistrator", "\320\241\321\202\320\260\321\200\321\202", 0, QApplication::UnicodeUTF8));
        pbUpdatePorts->setText(QApplication::translate("WindowRegistrator", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        cbU1->clear();
        cbU1->insertItems(0, QStringList()
         << QApplication::translate("WindowRegistrator", "5mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "10mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "20mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "50mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "100mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "200mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "500mV/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "1V/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "2V/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowRegistrator", "5V/\320\264\320\265\320\273", 0, QApplication::UnicodeUTF8)
        );
        pbDir->setText(QApplication::translate("WindowRegistrator", "\320\232\320\260\321\202\320\260\320\273\320\276\320\263", 0, QApplication::UnicodeUTF8));
        pbOpen->setText(QApplication::translate("WindowRegistrator", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("WindowRegistrator", "\320\224\320\273\320\270\320\275\320\260 \320\267\320\260\320\277\320\270\321\201\320\270", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("WindowRegistrator", "\320\232\320\260\320\275\320\260\320\273 1", 0, QApplication::UnicodeUTF8));
        chbUCursCh0->setText(QString());
        label_6->setText(QApplication::translate("WindowRegistrator", "U1", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("WindowRegistrator", "U2", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("WindowRegistrator", "dU", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("WindowRegistrator", "\320\232\320\260\320\275\320\260\320\273 2", 0, QApplication::UnicodeUTF8));
        chbUCursCh1->setText(QString());
        label_11->setText(QApplication::translate("WindowRegistrator", "U1", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("WindowRegistrator", "U2", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("WindowRegistrator", "dU", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("WindowRegistrator", "\320\244\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("WindowRegistrator", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("WindowRegistrator", "\320\246\320\262\320\265\321\202", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WindowRegistrator: public Ui_WindowRegistrator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWREGISTRATOR_H
