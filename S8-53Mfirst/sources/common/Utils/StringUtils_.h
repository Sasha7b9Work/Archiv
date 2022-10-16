// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Word
{
    pchar address;
    int8  numSymbols;

    // ��� ������� ����� ����������� � �������� �������� �����
    // ���������� ����� ����� numWord �� ������ string � word (���� ����� �������, �������)
    bool GetWord(pchar string, const int numWord);

    bool WordEqualZeroString(char *string);

private:

    // "�������" �������. ���������� nullptr, ���� ����� ���������� - ������ ���������
    pchar ChooseSymbols(pchar string);

    // "�������" �������. ���������� nullptr, ���� ����� ���������� - ������ ���������
    pchar ChooseSpaces(pchar string);
};


namespace SU
{
    char ToUpper(char symbol);

    char ToLower(char symbol);

    char *DoubleToString(double value);

    bool StringToDouble(double *value, pchar buffer);

    bool IsDigit(char symbol);

    bool EqualsStrings(pchar str1, pchar str2, uint size);
};
