// 2021/08/09 17:00:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct StorageRecord;

/*
*   ����� ������. ����� �������� ��������� ������ ��� ��� ����
*/
struct ReadingFrame
{
    friend struct StorageRecord;

    ReadingFrame();

    void Prepare();

    void CreateFromCurrentSettings();

    void CreateNull();

    DataSettings &Settings() const;

    // ���� � ������������ ��������� �� 8-������ ��������, ������� ����� �����, ��� � ������, ���� ������ �����������
    // �� ������� ���, ������ ������� �� 16 ���, �.�. ������ �� �������� ������� � ��� ��������
    uint8 *Data(const Chan &ch);

    // ���������� ��������� �� ��������� �� ��������� ����
    uint8 *DataEnd(const Chan &ch);

    void CreateFromRecord(const StorageRecord *record);

private:

    // � ������ �������� ��������������� - ���������, ������ ������� ������, ������ ������� ������
    Buffer<uint8> buffer;

    uint Size();

    /*
    *  �������������� ���� ��� ����������� ������
    */

public:

    bool isP2P;         // true ��������, ��� ��� ����� ����������� ������
};
