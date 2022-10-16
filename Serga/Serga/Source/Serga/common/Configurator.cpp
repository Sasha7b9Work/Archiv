#include "Configurator.h"
#include <Windows.h>
#include <fstream>
#include <string>

using namespace NS_Configurator;

Configurator::Configurator(const char * const _nameFile) : nameFile(_nameFile) 
{

}

bool Configurator::IsExist() 
{
    std::ifstream file(nameFile, std::ios::in);
    if(file.is_open()) 
    {
        file.read((char*)data, sizeof(int) * P_NumberParameters);
   
        for(int i = 0; i < CP_NumberColorParameters; i++)
            file.read((char*)(&(colors[i])), sizeof(QColor));

        return true;
    }

    return false;
}

int Configurator::LoadParameter(PARAMETER _param) 
{
    return data[_param];
}

bool Configurator::CreateConfig() 
{
    ZeroMemory(data, sizeof(int)*P_NumberParameters);
    return true;
}

bool Configurator::SaveParameter(PARAMETER _param, int _value) 
{
    data[(int)_param] = _value;
    return true;
}

bool Configurator::SaveParameter(PARAMETER _param, bool _value) 
{
    data[(int)_param] = _value;
    return true;
}

bool Configurator::SaveColor(COLOR_PARAMETER _param, const QColor& _color)
{
    colors[_param] = _color;
    return true;
}

QColor Configurator::LoadColor(COLOR_PARAMETER _param)
{
    return colors[_param];
}

bool Configurator::CloseConfig() 
{
    std::ofstream file(nameFile, std::ios::out);
    file.write((char*)data, sizeof(int) * P_NumberParameters);

    for(int i = 0; i < CP_NumberColorParameters; i++)
        file.write((char*)(&(colors[i])), sizeof(QColor));

    file.close();
    return true;
}
