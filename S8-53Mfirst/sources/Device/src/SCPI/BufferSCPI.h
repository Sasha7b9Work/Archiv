// 2022/01/21 16:38:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/String_.h"


class BufferSCPI
{
public:

    BufferSCPI();

    void Append(const uint8 *data, int length);

    // true, ���� ������������ ����������� - ������ ���� �������, ������� ����� ����������
    bool ExistDivider() const;

    String ExtractCommand();

    int Size() const { return size_data; }

    // ������� ����������� �� ������ ������
    void RemoveFirstDividers();

    void Free();

    String ToString();

private:

    uint8 *buffer;

    int size_buffer;    // ������ ���������� ������
    int size_data;      // ������ ������, ������� �������

    // ������� ������ num_bytes �� ������ ������
    void RemoveFirstBytes(int num_bytes);

    // �������� num_bytes ���� �� shfit ���� �����
    void ShiftToLeft(uint8 *data, int num_bytes, int shift);
};

