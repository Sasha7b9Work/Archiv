// 2021/05/13 13:53:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


template void ROM::Settings::Save< ::Settings>(::Settings *set);
template Settings *ROM::Settings::Load<Settings>();

template void ROM::Settings::Save<SettingsNRST>(SettingsNRST *sett);
template SettingsNRST *ROM::Settings::Load<SettingsNRST>();

namespace ROM
{
    template<class T>
    struct Packet
    {
        uint size;

        // ���������� ����� ���������� �� ���� ������
        Packet *Next();

        // ����� ������� ����� � ������
        uint Begin() { return (uint)this; }

        // ����� ������� ����� �� ���� �������
        uint End() { return (uint)Next(); }

        // ���������� true, ���� � ������ ��� ������ (����� ��������� �� ��������� ���������� �������)
        bool IsEmpty() const;

        // ������� �������� � ����� ��������� � �������
        bool SaveSettings(T *sett);
    };

    template<class T>
    struct StructSector
    {
        // ���������� ��������� �� ���������� ���������, ���� ������� ������� � nullptr � ��������� ������
        T *GetSaved() const;

        // ���������� ��������� �� �����, ������������� �� ������ �������
        Packet<T> *FirstPacket() const;

        // ���������� ��������� �� ��������� ���������� � ������� �����
        Packet<T> *LastPacket() const;

        // ������� �������� � ������ ��������� � �������
        bool SaveSettings(T *) const;

        static StructSector<T> &GetSector1();
        static StructSector<T> &GetSector2();

        const Sector &sector;
    };

    namespace Settings
    {
        template<class T>
        T *Load()
        {
            StructSector<T> &sector1 = StructSector<T>::GetSector1();
            StructSector<T> &sector2 = StructSector<T>::GetSector2();

            T *settings = sector2.GetSaved();

            if (settings)
            {
                return settings;
            }

            return sector1.GetSaved();
        }


        template<class T>
        void Save(T *settings)
        {
            StructSector<T> &sector1 = StructSector<T>::GetSector1();
            StructSector<T> &sector2 = StructSector<T>::GetSector2();

            if (!sector1.SaveSettings(settings))
            {
                if (!sector2.SaveSettings(settings))
                {
                    sector1.sector.Erase();
                    sector1.SaveSettings(settings);
                    sector2.sector.Erase();
                }
            }
        }
    }
}


template<class T>
bool ROM::StructSector<T>::SaveSettings(T *sett) const
{
    Packet<T> *last = LastPacket();

    if (last == nullptr)
    {
        last = (Packet<T> *)sector.address;

        return last->SaveSettings(sett);
    }

    return last->Next()->SaveSettings(sett);
}


template<class T>
ROM::Packet<T> *ROM::StructSector<T>::LastPacket() const
{
    Packet<T> *packet = FirstPacket();

    while (!packet->IsEmpty())
    {
        // ���������, ��� ����� �� ��������� �� ������� �������. ���������� ������ �����, � ������� ���������� ��-
        // ��������, ��� ��������� ����� ������
        if (packet->End() >= sector.End() - 4)
        {
            return packet;
        }

        if (packet->Next()->IsEmpty())
        {
            return packet;
        }

        packet = packet->Next();
    }

    return nullptr;
}


template<class T>
ROM::StructSector<T> &ROM::StructSector<T>::GetSector1()
{
    static StructSector<T> sectorNRST1   = { HAL_ROM::sectors[Sector::_12_NRST_1] };
    static StructSector<T> sectorCommon1 = { HAL_ROM::sectors[Sector::_10_SETTINGS_1] };

    return (sizeof(T) == sizeof(SettingsNRST)) ? sectorNRST1 : sectorCommon1;
}


template<class T>
ROM::StructSector<T> &ROM::StructSector<T>::GetSector2()
{
    static StructSector<T> sectorNRST2   = { HAL_ROM::sectors[Sector::_13_NRST_2] };
    static StructSector<T> sectorCommon2 = { HAL_ROM::sectors[Sector::_11_SETTINGS_2] };

    return (sizeof(T) == sizeof(SettingsNRST)) ? sectorNRST2 : sectorCommon2;
}


template<class T>
ROM::Packet<T> *ROM::StructSector<T>::FirstPacket() const
{
    return (Packet<T> *)sector.address;
}


template<class T>
T *ROM::StructSector<T>::GetSaved() const
{
    Packet<T> *last = LastPacket();

    return (last && last->IsEmpty()) ? nullptr : (T *)last;
}


template<class T>
bool ROM::Packet<T>::SaveSettings(T *sett)
{
    const Sector &sector = Sector::Get(Begin());

    if ((Begin() < sector.address) ||   // ��������� �� ����� �� ������ �������
        !IsEmpty()                 ||   // ��������� �� ��, ����� �� ����� ������ ������ �� ���� ��������
        (sizeof(T) + Begin() >= sector.End()))   // ��������� �� ����� �� ����� �������
    {
        return false;
    }

    sett->size = sizeof(T);

    HAL_ROM::WriteBufferBytes(Begin(), sett, sizeof(T));

    return true;
}


template<class T>
ROM::Packet<T> *ROM::Packet<T>::Next()
{
    if (size == (uint)(-1))
    {
        return nullptr;
    }

    return (Packet *)(Begin() + size);
}


template<class T>
bool ROM::Packet<T>::IsEmpty() const
{
    return (size == (uint)(-1));
}
