#pragma once
#include "AbstractAltera.h"
#include "DataAnalyzer.h"

class AlteraAnalyzer : public AbstractAltera 
{
public:
    AlteraAnalyzer();
    virtual bool Start(bool first = true);
    virtual bool DataReady(bool &isAnswer, bool &isSyhcro);
    DataAnalyzer* GetData();
    void SetFBase(int _fBase);
    int GetFBase();

private:
    DataAnalyzer data;
};