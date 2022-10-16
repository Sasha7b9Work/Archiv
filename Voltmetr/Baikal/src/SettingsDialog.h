//  [4/6/2010 Sasha]
#pragma once
#pragma warning(disable:4127)
#include "ui_settingsdialog.h"
#pragma warning(default:4127)

class SettingsDialog:public QDialog, public Ui::SettingsDialog {
    Q_OBJECT
public:
    SettingsDialog(QDialog *parent = 0);

    void Update();      // Обновить состояние диалога в соответствии с к новыми данными

public slots:
    void show();
};