//  [4/6/2010 Sasha]
#include "defines.h"
#include "SettingsDialog.h"
#include "GlobalVars.h"

extern QPushButton *g_pbUpdate;
extern QComboBox *g_cbPort;
extern SettingsWindow setW;

//  [4/8/2010 Sasha]
SettingsDialog::SettingsDialog(QDialog *_parent /* = 0 */) : QDialog(_parent) {
    setupUi(this);

    g_pbUpdate = pbUpdate;
    g_cbPort = cbPort;
}

//  [4/8/2010 Sasha]
void SettingsDialog::Update() {
    leDirPrograms->setText(setW.dirPrograms);
    leDirData->setText(setW.dirDatas);
}

//  [4/8/2010 Sasha]
void SettingsDialog::show() {
    Update();
    QDialog::show();
}
