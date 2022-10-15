#pragma once
#include "defines.h"


typedef enum
{
    DModel,             // "������"
    DSoftware,          // "����������� ����������� :"
    DVersion,           // "������ %s"
    DInformation,       // "����������"
    DTrigLev,           // "�� ����� = "
    DTrig,              // "��"
    DMode,              // "�����"
    D1ch,               // "1�"
    D2ch,               // "2�"
    DBalanceChA,        // "������������ ������ 1"
    DBalanceChB,        // "������������ ������ 2"
    DDetectFlashDrive,  // "���������� ������������ ����������"
    DSaveFirmware,      // "�������� ��������"
    DStoredInMemory,    // "��������� � ������"
    DM,                 // "�"
    DABS,               // "���"
    DREL,               // "���"
    DDis,               // "���"
    DShift,             // "��"
    D10uV,              // "\�10���"
    D10mV,              // "\�10��"
    D10V,               // "\�10�"
    D10kV,              // "\�10��"
    Dns,                // "��"
    Dus,                // "���"
    Dms,                // "��"
    Ds,                 // "�"
    DMHz,               // "���"
    DkHz,               // "���"
    DHz,                // "��"
    DDeleteFromMemory,  // "������ ���������� ������"
    DEnable,            // "���"
    DDisable,           // "����"
    DNumWords
} DictWord;

#define DICT(word) (dictWords[word][LANG])
#define DICT_RU(word) (dictWords[word][0])
#define DICT_EN(word) (dictWords[word][1])

extern pString dictWords[DNumWords][2];
