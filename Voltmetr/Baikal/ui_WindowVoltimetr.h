/********************************************************************************
** Form generated from reading ui file 'WindowVoltimetr.ui'
**
** Created: Wed Jun 16 12:19:21 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WINDOWVOLTIMETR_H
#define UI_WINDOWVOLTIMETR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WindowVoltimetr
{
public:
    QAction *aExit;
    QAction *aManualMode;
    QAction *aAutoMode;
    QAction *aSettings;
    QAction *aDeleteRow;
    QAction *aInsertRow;
    QWidget *centralwidget;
    QGroupBox *groupBox_2;
    QFrame *frameIMeas;
    QFrame *frameUMeas;
    QGroupBox *groupBox_3;
    QFrame *frameUSet;
    QSpinBox *sbHolderI;
    QSpinBox *sbHolderU;
    QPushButton *pbExactly;
    QFrame *frameISet;
    QFrame *frameHolderU;
    QFrame *frameHolderI;
    QPushButton *pbStop;
    QPushButton *pbStart;
    QPushButton *pbEnable;
    QFrame *frame;
    QPushButton *pbSaveProgram;
    QPushButton *pbLoadPorgram;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_3;
    QMenu *menuMode;

    void setupUi(QMainWindow *WindowVoltimetr)
    {
        if (WindowVoltimetr->objectName().isEmpty())
            WindowVoltimetr->setObjectName(QString::fromUtf8("WindowVoltimetr"));
        WindowVoltimetr->resize(600, 500);
        WindowVoltimetr->setMinimumSize(QSize(600, 500));
        WindowVoltimetr->setMaximumSize(QSize(600, 500));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/newPrefix/1.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        WindowVoltimetr->setWindowIcon(icon);
        aExit = new QAction(WindowVoltimetr);
        aExit->setObjectName(QString::fromUtf8("aExit"));
        aManualMode = new QAction(WindowVoltimetr);
        aManualMode->setObjectName(QString::fromUtf8("aManualMode"));
        aManualMode->setEnabled(false);
        aAutoMode = new QAction(WindowVoltimetr);
        aAutoMode->setObjectName(QString::fromUtf8("aAutoMode"));
        aAutoMode->setEnabled(false);
        aSettings = new QAction(WindowVoltimetr);
        aSettings->setObjectName(QString::fromUtf8("aSettings"));
        aDeleteRow = new QAction(WindowVoltimetr);
        aDeleteRow->setObjectName(QString::fromUtf8("aDeleteRow"));
        aInsertRow = new QAction(WindowVoltimetr);
        aInsertRow->setObjectName(QString::fromUtf8("aInsertRow"));
        centralwidget = new QWidget(WindowVoltimetr);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 90, 391, 71));
        groupBox_2->setAlignment(Qt::AlignCenter);
        frameIMeas = new QFrame(groupBox_2);
        frameIMeas->setObjectName(QString::fromUtf8("frameIMeas"));
        frameIMeas->setGeometry(QRect(200, 20, 180, 40));
        frameIMeas->setFrameShape(QFrame::StyledPanel);
        frameIMeas->setFrameShadow(QFrame::Sunken);
        frameUMeas = new QFrame(groupBox_2);
        frameUMeas->setObjectName(QString::fromUtf8("frameUMeas"));
        frameUMeas->setGeometry(QRect(10, 20, 180, 40));
        frameUMeas->setFrameShape(QFrame::StyledPanel);
        frameUMeas->setFrameShadow(QFrame::Sunken);
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 10, 581, 71));
        groupBox_3->setAlignment(Qt::AlignCenter);
        frameUSet = new QFrame(groupBox_3);
        frameUSet->setObjectName(QString::fromUtf8("frameUSet"));
        frameUSet->setGeometry(QRect(10, 20, 180, 40));
        frameUSet->setFrameShape(QFrame::StyledPanel);
        frameUSet->setFrameShadow(QFrame::Sunken);
        sbHolderI = new QSpinBox(groupBox_3);
        sbHolderI->setObjectName(QString::fromUtf8("sbHolderI"));
        sbHolderI->setGeometry(QRect(280, 30, 21, 22));
        sbHolderU = new QSpinBox(groupBox_3);
        sbHolderU->setObjectName(QString::fromUtf8("sbHolderU"));
        sbHolderU->setGeometry(QRect(350, 30, 16, 22));
        pbExactly = new QPushButton(groupBox_3);
        pbExactly->setObjectName(QString::fromUtf8("pbExactly"));
        pbExactly->setGeometry(QRect(270, 25, 41, 31));
        frameISet = new QFrame(groupBox_3);
        frameISet->setObjectName(QString::fromUtf8("frameISet"));
        frameISet->setGeometry(QRect(390, 20, 180, 40));
        frameISet->setFrameShape(QFrame::StyledPanel);
        frameISet->setFrameShadow(QFrame::Sunken);
        frameHolderU = new QFrame(groupBox_3);
        frameHolderU->setObjectName(QString::fromUtf8("frameHolderU"));
        frameHolderU->setGeometry(QRect(210, 20, 41, 41));
        frameHolderU->setFrameShape(QFrame::StyledPanel);
        frameHolderU->setFrameShadow(QFrame::Raised);
        frameHolderI = new QFrame(groupBox_3);
        frameHolderI->setObjectName(QString::fromUtf8("frameHolderI"));
        frameHolderI->setGeometry(QRect(330, 20, 41, 41));
        frameHolderI->setFrameShape(QFrame::StyledPanel);
        frameHolderI->setFrameShadow(QFrame::Raised);
        pbStop = new QPushButton(centralwidget);
        pbStop->setObjectName(QString::fromUtf8("pbStop"));
        pbStop->setEnabled(false);
        pbStop->setGeometry(QRect(475, 99, 51, 25));
        pbStart = new QPushButton(centralwidget);
        pbStart->setObjectName(QString::fromUtf8("pbStart"));
        pbStart->setEnabled(false);
        pbStart->setGeometry(QRect(410, 99, 51, 25));
        pbEnable = new QPushButton(centralwidget);
        pbEnable->setObjectName(QString::fromUtf8("pbEnable"));
        pbEnable->setEnabled(false);
        pbEnable->setGeometry(QRect(540, 99, 51, 25));
        pbEnable->setCheckable(true);
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 180, 581, 291));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        pbSaveProgram = new QPushButton(centralwidget);
        pbSaveProgram->setObjectName(QString::fromUtf8("pbSaveProgram"));
        pbSaveProgram->setEnabled(true);
        pbSaveProgram->setGeometry(QRect(410, 134, 81, 25));
        pbLoadPorgram = new QPushButton(centralwidget);
        pbLoadPorgram->setObjectName(QString::fromUtf8("pbLoadPorgram"));
        pbLoadPorgram->setEnabled(true);
        pbLoadPorgram->setGeometry(QRect(510, 134, 81, 25));
        WindowVoltimetr->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WindowVoltimetr);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 600, 19));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menuMode = new QMenu(menu_3);
        menuMode->setObjectName(QString::fromUtf8("menuMode"));
        menuMode->setEnabled(true);
        WindowVoltimetr->setMenuBar(menubar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu->addAction(aExit);
        menu_3->addAction(menuMode->menuAction());
        menu_3->addAction(aSettings);
        menuMode->addAction(aManualMode);
        menuMode->addAction(aAutoMode);

        retranslateUi(WindowVoltimetr);

        QMetaObject::connectSlotsByName(WindowVoltimetr);
    } // setupUi

    void retranslateUi(QMainWindow *WindowVoltimetr)
    {
        WindowVoltimetr->setWindowTitle(QApplication::translate("WindowVoltimetr", "\320\230\321\201\321\202\320\276\321\207\320\275\320\270\320\272 \320\277\320\270\321\202\320\260\320\275\320\270\321\217", 0, QApplication::UnicodeUTF8));
        aExit->setText(QApplication::translate("WindowVoltimetr", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        aManualMode->setText(QApplication::translate("WindowVoltimetr", "\320\240\321\203\321\207\320\275\320\276\320\271", 0, QApplication::UnicodeUTF8));
        aAutoMode->setText(QApplication::translate("WindowVoltimetr", "\320\220\320\262\321\202\320\276\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\271", 0, QApplication::UnicodeUTF8));
        aSettings->setText(QApplication::translate("WindowVoltimetr", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0, QApplication::UnicodeUTF8));
        aDeleteRow->setText(QApplication::translate("WindowVoltimetr", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \321\201\321\202\321\200\320\276\320\272\321\203", 0, QApplication::UnicodeUTF8));
        aInsertRow->setText(QApplication::translate("WindowVoltimetr", "\320\222\321\201\321\202\320\260\320\262\320\270\321\202\321\214 \321\201\321\202\321\200\320\276\320\272\321\203", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("WindowVoltimetr", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("WindowVoltimetr", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\260", 0, QApplication::UnicodeUTF8));
        pbExactly->setText(QApplication::translate("WindowVoltimetr", "\320\223\321\200\321\203\320\261\320\276", 0, QApplication::UnicodeUTF8));
        pbStop->setText(QApplication::translate("WindowVoltimetr", "\320\241\321\202\320\276\320\277", 0, QApplication::UnicodeUTF8));
        pbStart->setText(QApplication::translate("WindowVoltimetr", "\320\241\321\202\320\260\321\200\321\202", 0, QApplication::UnicodeUTF8));
        pbEnable->setText(QApplication::translate("WindowVoltimetr", "\320\222\320\272\320\273", 0, QApplication::UnicodeUTF8));
        pbSaveProgram->setText(QApplication::translate("WindowVoltimetr", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        pbLoadPorgram->setText(QApplication::translate("WindowVoltimetr", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("WindowVoltimetr", "\320\234\320\265\320\275\321\216", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("WindowVoltimetr", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0, QApplication::UnicodeUTF8));
        menuMode->setTitle(QApplication::translate("WindowVoltimetr", "\320\240\320\265\320\266\320\270\320\274", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WindowVoltimetr: public Ui_WindowVoltimetr {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWVOLTIMETR_H
