//  [12/13/2009 Sasha]
#pragma once
#include "AbstractAltera.h"

class AlteraRegistrator : public AbstractAltera 
{
public:
    AlteraRegistrator();
    bool DataReady();
    double GetValue(int channel);	// ���������� ��������, ��������� ��� ������ ������� DataReady(). ���� DataReady() ������� false,
	                                // ������� ����� ��������� ��������� ��������

private:
	double data[2];
};