//  [12/13/2009 Sasha]
#pragma once
#include "AbstractAltera.h"

class AlteraRegistrator : public AbstractAltera 
{
public:
    AlteraRegistrator();
    bool DataReady();
    double GetValue(int channel);	// Возвращает значение, считанное при вызове функции DataReady(). Если DataReady() вернула false,
	                                // функция вернёт последнее считанное значение

private:
	double data[2];
};