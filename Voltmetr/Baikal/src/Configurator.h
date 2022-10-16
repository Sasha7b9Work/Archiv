//  [4/8/2010 Sasha]
#pragma once
#include "defines.h"
/*
����������� ���������:
    1. ������� ��� ���������� ��������
    2. ������� ��� ���������� ������
    3. ���� ��������
    4. ���� ����
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