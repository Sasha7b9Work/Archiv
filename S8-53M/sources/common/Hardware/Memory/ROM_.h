// 2021/04/27 11:09:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/String_.h"


struct DataSettings;
struct SettingsNRST;


namespace ROM
{
    static const uint SIZE = 2 * 1024 * 1024;

    // ������� ��� ����������/�������� ��������
    namespace Settings
    {
        // ���������� ��������� �� ���������� � ROM �������������� ���������. nullptr � ������, ���� �������� ��� ���
        template<class T>
        T *Load();

        // ��������� ��������� �� ������ sett � ROM
        template<class T>
        void Save(T *sett);
    };

    // ������� ��� ����������/�������������� ������
    namespace Data
    {
        // \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� �����
        // ������� � ���� ����� ����� �����.
        static const uint MAX_NUM_SAVED_WAVES = 23;

        // ���� ������� ����, ��������������� ������� ������� ����� true.
        void GetInfo(bool existData[MAX_NUM_SAVED_WAVES]);

        void Save(uint numInROM, const DataSettings *ds);

        // �������� ������ � ������� ����� num. ���� ������� ���, � ds ������������ 0
        const DataSettings *Read(uint numInROM);

        void Erase(uint numInROM);

        void EraseAll();
    };
};
