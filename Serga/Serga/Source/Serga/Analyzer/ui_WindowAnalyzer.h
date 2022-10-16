/********************************************************************************
** Form generated from reading ui file 'WindowAnalyzer.ui'
**
** Created: Wed Jun 27 09:42:50 2012
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WINDOWANALYZER_H
#define UI_WINDOWANALYZER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WindowAnalyzer
{
public:
    QAction *actionExit;
    QAction *aColorBackground;
    QAction *aColorGrid;
    QAction *aColorSignal;
    QAction *aCalibrate;
    QWidget *centralwidget;
    QFrame *frame;
    QComboBox *cbFreq;
    QLabel *label;
    QComboBox *cbU;
    QLabel *label_2;
    QComboBox *cbCh;
    QLabel *label_4;
    QGroupBox *groupBox;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *leU0;
    QLabel *label_7;
    QLineEdit *leU1;
    QLabel *label_8;
    QLineEdit *leDeltaFreq;
    QLabel *label_9;
    QLineEdit *leFreq1;
    QSpinBox *sbPos1;
    QLineEdit *leDeltaU;
    QCheckBox *chbEnableCurs;
    QLineEdit *leUmax;
    QLineEdit *leFUmax;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLineEdit *leU_DC;
    QLabel *label_13;
    QLineEdit *leFreq0;
    QSpinBox *sbPos0;
    QComboBox *cbPort;
    QPushButton *pbStart;
    QComboBox *cbWindow;
    QPushButton *pbUpdate;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuSettings;
    QMenu *menu;

    void setupUi(QMainWindow *WindowAnalyzer)
    {
        if (WindowAnalyzer->objectName().isEmpty())
            WindowAnalyzer->setObjectName(QString::fromUtf8("WindowAnalyzer"));
        WindowAnalyzer->setWindowModality(Qt::NonModal);
        WindowAnalyzer->resize(899, 345);
        WindowAnalyzer->setMinimumSize(QSize(899, 345));
        WindowAnalyzer->setMaximumSize(QSize(899, 345));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/newPrefix/Spectro16.ico"), QSize(), QIcon::Normal, QIcon::Off);
        WindowAnalyzer->setWindowIcon(icon);
        actionExit = new QAction(WindowAnalyzer);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        aColorBackground = new QAction(WindowAnalyzer);
        aColorBackground->setObjectName(QString::fromUtf8("aColorBackground"));
        aColorGrid = new QAction(WindowAnalyzer);
        aColorGrid->setObjectName(QString::fromUtf8("aColorGrid"));
        aColorSignal = new QAction(WindowAnalyzer);
        aColorSignal->setObjectName(QString::fromUtf8("aColorSignal"));
        aCalibrate = new QAction(WindowAnalyzer);
        aCalibrate->setObjectName(QString::fromUtf8("aCalibrate"));
        centralwidget = new QWidget(WindowAnalyzer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 10, 601, 304));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        cbFreq = new QComboBox(centralwidget);
        cbFreq->setObjectName(QString::fromUtf8("cbFreq"));
        cbFreq->setGeometry(QRect(680, 18, 65, 22));
        cbFreq->setMaxVisibleItems(100);
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(630, 20, 41, 16));
        cbU = new QComboBox(centralwidget);
        cbU->setObjectName(QString::fromUtf8("cbU"));
        cbU->setGeometry(QRect(680, 60, 65, 22));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(630, 62, 61, 16));
        cbCh = new QComboBox(centralwidget);
        cbCh->setObjectName(QString::fromUtf8("cbCh"));
        cbCh->setGeometry(QRect(680, 100, 65, 22));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(631, 104, 31, 16));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(630, 133, 261, 181));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 62, 21, 16));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 92, 21, 16));
        leU0 = new QLineEdit(groupBox);
        leU0->setObjectName(QString::fromUtf8("leU0"));
        leU0->setEnabled(false);
        leU0->setGeometry(QRect(170, 58, 61, 22));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(145, 60, 20, 20));
        leU1 = new QLineEdit(groupBox);
        leU1->setObjectName(QString::fromUtf8("leU1"));
        leU1->setEnabled(false);
        leU1->setGeometry(QRect(170, 87, 61, 22));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(144, 91, 21, 16));
        leDeltaFreq = new QLineEdit(groupBox);
        leDeltaFreq->setObjectName(QString::fromUtf8("leDeltaFreq"));
        leDeltaFreq->setEnabled(false);
        leDeltaFreq->setGeometry(QRect(50, 118, 61, 22));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 120, 21, 16));
        leFreq1 = new QLineEdit(groupBox);
        leFreq1->setObjectName(QString::fromUtf8("leFreq1"));
        leFreq1->setEnabled(false);
        leFreq1->setGeometry(QRect(50, 87, 61, 22));
        sbPos1 = new QSpinBox(groupBox);
        sbPos1->setObjectName(QString::fromUtf8("sbPos1"));
        sbPos1->setEnabled(false);
        sbPos1->setGeometry(QRect(111, 87, 18, 22));
        leDeltaU = new QLineEdit(groupBox);
        leDeltaU->setObjectName(QString::fromUtf8("leDeltaU"));
        leDeltaU->setEnabled(false);
        leDeltaU->setGeometry(QRect(170, 118, 61, 22));
        chbEnableCurs = new QCheckBox(groupBox);
        chbEnableCurs->setObjectName(QString::fromUtf8("chbEnableCurs"));
        chbEnableCurs->setGeometry(QRect(20, 20, 71, 19));
        leUmax = new QLineEdit(groupBox);
        leUmax->setObjectName(QString::fromUtf8("leUmax"));
        leUmax->setGeometry(QRect(170, 148, 61, 22));
        leFUmax = new QLineEdit(groupBox);
        leFUmax->setObjectName(QString::fromUtf8("leFUmax"));
        leFUmax->setGeometry(QRect(50, 148, 61, 22));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 150, 31, 20));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(145, 121, 21, 16));
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(134, 150, 31, 20));
        leU_DC = new QLineEdit(groupBox);
        leU_DC->setObjectName(QString::fromUtf8("leU_DC"));
        leU_DC->setGeometry(QRect(170, 20, 61, 22));
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(144, 22, 21, 20));
        leFreq0 = new QLineEdit(groupBox);
        leFreq0->setObjectName(QString::fromUtf8("leFreq0"));
        leFreq0->setEnabled(false);
        leFreq0->setGeometry(QRect(50, 60, 61, 22));
        sbPos0 = new QSpinBox(groupBox);
        sbPos0->setObjectName(QString::fromUtf8("sbPos0"));
        sbPos0->setEnabled(false);
        sbPos0->setGeometry(QRect(111, 60, 18, 22));
        cbPort = new QComboBox(centralwidget);
        cbPort->setObjectName(QString::fromUtf8("cbPort"));
        cbPort->setGeometry(QRect(760, 17, 61, 22));
        pbStart = new QPushButton(centralwidget);
        pbStart->setObjectName(QString::fromUtf8("pbStart"));
        pbStart->setGeometry(QRect(760, 60, 131, 24));
        pbStart->setCheckable(true);
        cbWindow = new QComboBox(centralwidget);
        cbWindow->setObjectName(QString::fromUtf8("cbWindow"));
        cbWindow->setGeometry(QRect(760, 100, 131, 22));
        pbUpdate = new QPushButton(centralwidget);
        pbUpdate->setObjectName(QString::fromUtf8("pbUpdate"));
        pbUpdate->setGeometry(QRect(831, 16, 61, 24));
        pbUpdate->setCheckable(false);
        WindowAnalyzer->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(WindowAnalyzer);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 899, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        menu = new QMenu(menuSettings);
        menu->setObjectName(QString::fromUtf8("menu"));
        WindowAnalyzer->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuSettings->menuAction());
        menuFile->addAction(actionExit);
        menuSettings->addAction(menu->menuAction());
        menuSettings->addAction(aCalibrate);
        menu->addAction(aColorBackground);
        menu->addAction(aColorGrid);
        menu->addAction(aColorSignal);

        retranslateUi(WindowAnalyzer);

        QMetaObject::connectSlotsByName(WindowAnalyzer);
    } // setupUi

    void retranslateUi(QMainWindow *WindowAnalyzer)
    {
        WindowAnalyzer->setWindowTitle(QApplication::translate("WindowAnalyzer", "\320\220\320\275\320\260\320\273\320\270\320\267\320\260\321\202\320\276\321\200 \321\201\320\277\320\265\320\272\321\202\321\200\320\260", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("WindowAnalyzer", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        aColorBackground->setText(QApplication::translate("WindowAnalyzer", "\320\244\320\276\320\275", 0, QApplication::UnicodeUTF8));
        aColorGrid->setText(QApplication::translate("WindowAnalyzer", "\320\241\320\265\321\202\320\272\320\260", 0, QApplication::UnicodeUTF8));
        aColorSignal->setText(QApplication::translate("WindowAnalyzer", "\320\241\320\270\320\263\320\275\320\260\320\273", 0, QApplication::UnicodeUTF8));
        aCalibrate->setText(QApplication::translate("WindowAnalyzer", "\320\232\320\260\320\273\320\270\320\261\321\200\320\276\320\262\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WindowAnalyzer", "\320\224\320\270\320\260\320\277 f", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("WindowAnalyzer", "\320\224\320\270\320\260\320\277 U", 0, QApplication::UnicodeUTF8));
        cbCh->clear();
        cbCh->insertItems(0, QStringList()
         << QApplication::translate("WindowAnalyzer", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowAnalyzer", "2", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("WindowAnalyzer", "\320\232\320\260\320\275", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("WindowAnalyzer", "\320\232\321\203\321\200\321\201\320\276\321\200\321\213", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("WindowAnalyzer", "f1", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("WindowAnalyzer", "f2", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("WindowAnalyzer", "U1", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("WindowAnalyzer", "U2", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("WindowAnalyzer", "df", 0, QApplication::UnicodeUTF8));
        chbEnableCurs->setText(QApplication::translate("WindowAnalyzer", "\320\222\320\272\320\273\321\216\321\207\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("WindowAnalyzer", "fumax", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("WindowAnalyzer", "dU", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("WindowAnalyzer", "Umax", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("WindowAnalyzer", "U0", 0, QApplication::UnicodeUTF8));
        pbStart->setText(QApplication::translate("WindowAnalyzer", "\320\241\321\202\320\260\321\200\321\202", 0, QApplication::UnicodeUTF8));
        cbWindow->clear();
        cbWindow->insertItems(0, QStringList()
         << QApplication::translate("WindowAnalyzer", "\320\236\320\272\320\275\320\276 \320\245\321\215\320\274\320\274\320\270\320\275\320\263\320\260", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowAnalyzer", "\320\236\320\272\320\275\320\276 \320\221\320\273\321\215\320\272\320\274\320\260\320\275\320\260", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("WindowAnalyzer", "\320\237\321\200\321\217\320\274\320\276\321\203\320\263\320\276\320\273\321\214\320\275\320\276\320\265", 0, QApplication::UnicodeUTF8)
        );
        pbUpdate->setText(QApplication::translate("WindowAnalyzer", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("WindowAnalyzer", "\320\244\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        menuSettings->setTitle(QApplication::translate("WindowAnalyzer", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("WindowAnalyzer", "\320\246\320\262\320\265\321\202", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WindowAnalyzer: public Ui_WindowAnalyzer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWANALYZER_H
