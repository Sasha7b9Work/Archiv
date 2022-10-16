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
    BYTE data[2][NS_TypesAltera::DB_LENGTH];                // ���� ����������� ������ �� ������� � ������� �������
    double dataAbs[2][NS_TypesAltera::DB_LENGTH];           // ����� ���������� �������� ��� �������������� ��������
    double dataDraw[2][NS_TypesAltera::DB_LENGTH];       // ����� ���������� � 0...1 �������� ��� ������ �� �����
    bool isReady;
};