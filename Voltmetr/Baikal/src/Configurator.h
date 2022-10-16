//  [4/8/2010 Sasha]
#pragma once
#include "defines.h"
/*
Сохраняемые настройки:
    1. Каталог для сохранения программ
    2. Каталог для сохранения данных
    3. Цвет символов
    4. Цвет фона
*/

class Configurator {
public:
    Configurator();
    ~Configurator();

    void SaveDirProgram();
    void SaveDirData();
    void SaveColorSymbols();
    void SaveColorBackground();

    bool LoadDirProgram();
    bool LoadDirData();
    bool LoadColorSymbols();
    bool LoadColorBackground();

private:
    QSettings *settings;
};