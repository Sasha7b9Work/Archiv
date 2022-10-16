//  [4/8/2010 Sasha]]
#include "defines.h"
#include "Configurator.h"
#include "GlobalVars.h"

using namespace std;

extern SettingsWindow setW;

//  [4/8/2010 Sasha]
Configurator::Configurator() {
    try {
        settings = new QSettings("MNIPI",  "Voltmetr");
    } catch(bad_alloc) {
        settings = NULL;
        ERROR_LOG("Error create QSettings");
    }
    if(NULL != settings) {
        QCoreApplication::setOrganizationName("MNIPI");
        QCoreApplication::setApplicationName("Voltmetr");
    }
}

//  [4/27/2010 Sasha]
Configurator::~Configurator() {
    SAFE_DELETE(settings);
}

//  [4/27/2010 Sasha]
void Configurator::SaveDirProgram() {
    settings->setValue("/Settings/DirPrograms", setW.dirPrograms);
}

//  [4/27/2010 Sasha]
void Configurator::SaveDirData() {
    settings->setValue("/Settings/DirData", setW.dirDatas);
}

//  [4/27/2010 Sasha]
void Configurator::SaveColorSymbols() {
    settings->setValue("/Settings/ColorSymbols", setW.colorSymbols.rgb());
}

//  [4/27/2010 Sasha]
void Configurator::SaveColorBackground() {
    settings->setValue("/Settings/ColorBackground", setW.colorDisplay.rgb());
}

//  [4/27/2010 Sasha]
bool Configurator::LoadDirProgram() {
    QString dir = settings->value("/Settings/DirPrograms", "").toString();
    if(dir != "") {
        setW.dirPrograms = dir;
        return true;
    }
    return false;
}

//  [4/27/2010 Sasha]
bool Configurator::LoadDirData() {
    QString dir = settings->value("/Settings/DirData", "").toString();
    if(dir != "") {
        setW.dirDatas = dir;
        return true;
    }
    return false;
}

//  [4/27/2010 Sasha]
bool Configurator::LoadColorSymbols() {
    bool isOk = false;;
    QRgb rgb = settings->value("/Settings/ColorSymbols", 0).toUInt(&isOk);
    if((true == isOk) && (rgb != 0)) {
        setW.colorSymbols = QColor(rgb);
        return true;
    }
    return false;
}

//  [4/27/2010 Sasha]
bool Configurator::LoadColorBackground() {
    bool isOk = false;
    QRgb rgb = settings->value("/Settings/ColorBackground", 0).toUInt(&isOk);
    if((true == isOk) && (rgb != 0)) {
        setW.colorDisplay = QColor(rgb);
        return true;
    }
    return false;
}
