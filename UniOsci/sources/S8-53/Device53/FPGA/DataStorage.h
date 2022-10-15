#pragma once


class Storage
{
public:
    /// �������� ���� ���������� ���������
    static void Clear();
    /// �������� ��������� ������. ��� ���� ����������� ��������� ������� ��������� �������.
    static void AddData(uint8 *data0, uint8 *data1, DataSettings dss);
    /// ���������� ����� ����������� ���������, ������� � ����������, � ������ �� �����������, ��� � ����������.
    static int NumElementsWithSameSettings();
    /// ���������� ����� ����������� ���������, ������� � ����������, � �������� ����������� �������
    static int NumElementsWithCurrentSettings();
    /// ���������� ���������� ���������� ���������
    static int NumElementsInStorage();
    /// �������� ��������� �� ������
    static bool GetDataFromEnd(int fromEnd, DataSettings **ds, uint8 **data0, uint8 **data1);

    static uint8* GetData(Channel chan, int fromEnd);
    /// �������� ���������� ������ �� ���������� ����������.
    static uint8* GetAverageData(Channel chan);
    /// ������� ����� ��������� ��������� � ������.
    static int AllDatas();
    /// �������� �������������� ����� ������� 0 - �����, 1 - ������.
    static uint8* GetLimitation(Channel chan, int direction);                                      

    static int NumberAvailableEntries();
private:

    static void CalculateSums();
    /// ���������� ���������� ��������� ������ � ������
    static int MemoryFree();
    /// ���������, ������� ������ ���������, ����� ��������� ��������� � ����������� dp
    static int SizeElem(DataSettings *dp);
    /// ������� ������ (����� ������) ���������
    static void RemoveFirstElement();
    /// ��������� ������
    static void PushData(DataSettings *dp, uint8 *data0, uint8 *data1);
    /// ���������� ��������� �� ���������, ��������� �� elem
    static DataSettings* NextElem(DataSettings *elem);
    /// ���������� ��������� �� ������, ��������� �� indexFromEnd o� ���������� �����������
    static DataSettings* FromEnd(int indexFromEnd);
    /// ���������� true, ���� ��������� ��������� � ��������� elemFromEnd0 � elemFromEnd1 ���������, � false � ���� ������.
    static bool SettingsIsIdentical(int elemFromEnd0, int elemFromEnd1);
    /// ��������� true, ���� ��������� � ����� ���������� ���������
    static bool SettingsIsEquals(DataSettings *dp0, DataSettings *dp1);
    /// ������� �������� ���, ���� � ����
    static void ClearLimitsAndSums();

    static void CalculateLimits(uint8 *data0, uint8 *data1, DataSettings *dss);

    static DataSettings* GetSettingsDataFromEnd(int fromEnd);
    /// �������� ������ ������ chan ��, ������������ ds, � ���� �� ���� ����� ������� dataImportRel. ������������ �������� false ��������, ��� ������ ����� ��������.
    static bool CopyData(DataSettings *ds, Channel chan, uint8 *datat);

    static void PrintElement(DataSettings *dp);
};
