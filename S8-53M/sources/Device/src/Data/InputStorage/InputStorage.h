// 2021/03/18 16:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/Buffer_.h"
#include "Data/DataSettings.h"
#include "Data/InputStorage/ReadingFrame.h"


/*
*  ����� ���������� �� ���������� ������������ �������� ��� ��������� ������, ������ � ������� �������� ���.
*  � ���������� addressFirstRecord �������� ����� ������ ������. ������ ������������ ����� ������� ���� ReadingFrame.
*  ������ ������ ����������� ������ ��������� �� ��������� ������ ���������� ������.
*  ��� ���������� ����� ��� ��������� ������ (����� ������) �������, � �� ��� ����� ������������ ��������� (�����
*  �����. �� ������ ���������� ��������� �� ������ �
*/

namespace InputStorage
{
    // ����������� ��� ������ ����� ������
    void PrepareForNewCycle();

    // �������� ������ ������
    void AppendDataFrame(const ReadingFrame &data);

    // ������� ��������� ���������� ������
    bool ExtractLastFrame(ReadingFrame &data);

    // ������� ������, ����������� �� "����������" from_end �� ��������� ������ (��� from_end ���������� ���������
    // ���������� ������)
    bool ExtractFrame(uint from_end, ReadingFrame &data);

    // ���������� ���������� ������� � ���������
    uint NumFrames();

    // ���������� true, ���� ��������� � from-end-� ������ � data ��������� (����� ��� �������������)
    bool SettingsIsEquals(uint from_end, const ReadingFrame &data);

    // ����� ����� ������ (�������� ���������)
    StorageRecord *Newest();
};
