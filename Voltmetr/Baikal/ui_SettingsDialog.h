/********************************************************************************
** Form generated from reading ui file 'SettingsDialog.ui'
**
** Created: Wed Jun 16 12:19:21 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *pbDirPrograms;
    QLineEdit *leDirPrograms;
    QPushButton *pbDirData;
    QLineEdit *leDirData;
    QCheckBox *chbSaveData;
    QWidget *tab_2;
    QPushButton *pbUpdate;
    QComboBox *cbPort;
    QWidget *tab_3;
    QPushButton *pbColorDisplay;
    QPushButton *pbColorSimbols;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(399, 128);
        tabWidget = new QTabWidget(SettingsDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(4, 8, 391, 111));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        pbDirPrograms = new QPushButton(tab);
        pbDirPrograms->setObjectName(QString::fromUtf8("pbDirPrograms"));
        pbDirPrograms->setGeometry(QRect(10, 11, 75, 24));
        leDirPrograms = new QLineEdit(tab);
        leDirPrograms->setObjectName(QString::fromUtf8("leDirPrograms"));
        leDirPrograms->setGeometry(QRect(100, 14, 271, 21));
        leDirPrograms->setReadOnly(true);
        pbDirData = new QPushButton(tab);
        pbDirData->setObjectName(QString::fromUtf8("pbDirData"));
        pbDirData->setGeometry(QRect(10, 50, 75, 24));
        leDirData = new QLineEdit(tab);
        leDirData->setObjectName(QString::fromUtf8("leDirData"));
        leDirData->setGeometry(QRect(100, 50, 181, 21));
        leDirData->setReadOnly(true);
        chbSaveData = new QCheckBox(tab);
        chbSaveData->setObjectName(QString::fromUtf8("chbSaveData"));
        chbSaveData->setGeometry(QRect(295, 50, 81, 19));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        pbUpdate = new QPushButton(tab_2);
        pbUpdate->setObjectName(QString::fromUtf8("pbUpdate"));
        pbUpdate->setGeometry(QRect(90, 13, 61, 24));
        cbPort = new QComboBox(tab_2);
        cbPort->setObjectName(QString::fromUtf8("cbPort"));
        cbPort->setGeometry(QRect(13, 14, 61, 22));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        pbColorDisplay = new QPushButton(tab_3);
        pbColorDisplay->setObjectName(QString::fromUtf8("pbColorDisplay"));
        pbColorDisplay->setGeometry(QRect(10, 10, 91, 24));
        pbColorSimbols = new QPushButton(tab_3);
        pbColorSimbols->setObjectName(QString::fromUtf8("pbColorSimbols"));
        pbColorSimbols->setGeometry(QRect(10, 47, 91, 24));
        tabWidget->addTab(tab_3, QString());

        retranslateUi(SettingsDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0, QApplication::UnicodeUTF8));
        pbDirPrograms->setText(QApplication::translate("SettingsDialog", "\320\237\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213", 0, QApplication::UnicodeUTF8));
        pbDirData->setText(QApplication::translate("SettingsDialog", "\320\224\320\260\320\275\320\275\321\213\320\265", 0, QApplication::UnicodeUTF8));
        chbSaveData->setText(QApplication::translate("SettingsDialog", "\320\241\320\276\321\205\321\200\320\260\320\275\321\217\321\202\321\214", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SettingsDialog", "\320\232\320\260\321\202\320\260\320\273\320\276\320\263\320\270", 0, QApplication::UnicodeUTF8));
        pbUpdate->setText(QApplication::translate("SettingsDialog", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SettingsDialog", "\320\237\320\276\321\200\321\202", 0, QApplication::UnicodeUTF8));
        pbColorDisplay->setText(QApplication::translate("SettingsDialog", "\320\246\320\262\320\265\321\202 \320\264\320\270\321\201\320\277\320\273\320\265\321\217", 0, QApplication::UnicodeUTF8));
        pbColorSimbols->setText(QApplication::translate("SettingsDialog", "\320\246\320\262\320\265\321\202 \321\201\320\270\320\274\320\262\320\276\320\273\320\276\320\262", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("SettingsDialog", "\320\240\320\260\320\267\320\275\321\213\320\265", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SettingsDialog);
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
