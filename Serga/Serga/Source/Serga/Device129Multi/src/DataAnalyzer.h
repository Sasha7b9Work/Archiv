#pragma once
#include "AbstractData.h"

struct DataAnalyzer : public AbstractData 
{
    DataAnalyzer() : AbstractData() 
    {

    };

    double deltaF[2];
};