//  [12/9/2009 Sasha]
#pragma once
#include "../../common/defines.h"
#include "../../common/TypesAltera.h"

struct AbstractData 
{

friend class AlteraAnalyzer;

    AbstractData() : isReady(false) 
    {
        Clear();
    };

    void Clear()
    {
        for(int i = 0; i < NS_TypesAltera::DB_LENGTH; i++) 
        {
            data[0][i]    = data[1][i]    = NS_TypesAltera::AVE_VALUE;
            dataAbs[0][i] = dataAbs[1][i] = 0.0;
        }
        isReady = false;
    };
    BYTE data[2][NS_TypesAltera::DB_LENGTH];                // Сюда считываются данные из первого и второго каналов
    double dataAbs[2][NS_TypesAltera::DB_LENGTH];           // Здесь абсолютные значения для математических расчётов
    double dataDraw[2][NS_TypesAltera::DB_LENGTH];       // Здесь приведённые к 0...1 значения для вывода на экран
    bool isReady;
};