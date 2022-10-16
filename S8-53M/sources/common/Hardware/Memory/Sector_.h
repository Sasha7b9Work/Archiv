// 2021/04/27 11:09:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Hardware/Memory/ROM_.h"


#define STATE_FREE    0xFFFFFFFFU
#define STATE_ERASED  0x00000000U
#define STATE_VALID   0xFF00FF00U

#define TYPE_DATA     0U

#define SECTOR(i) HAL_ROM::sectors[i]
#define ADDR_SECTOR(i) (SECTOR(i).address)
#define SIZE_SECTOR(i) (SECTOR(i).size)
#define END_SECTOR(i) (ADDR_SECTOR(i) + SIZE_SECTOR(i))


struct Sector;
struct ReadingFrame;


struct PacketROM
{
    uint  state;    // ��������� ������:
                    // 0xFFFFFFFF - � ����� ������ �� �������������
                    // 0x00000000 - ����� ����
                    // 0xFF00FF00 - ����������� �����
    uint16 size;    // ������ ������ - sizeof(size) + sizeof(type) + sizeof(data)
    uint16 type;    // ��� ������

    bool IsData() const { return (state == STATE_VALID) && (type == TYPE_DATA); }
    bool IsFree() const { return (state == STATE_FREE); }
    bool IsErased() const { return (state == STATE_ERASED); }
    bool IsValid() const { return (state == STATE_VALID); }

    uint Address() const {
#ifdef __linux__
        return 0 ;
#else
        return (uint)this;
#endif
    };

    PacketROM *Next() const;
    
    const ReadingFrame *UnPack() const;
    
    int Size() const;
    
    // ������ ������� ������ ������ � ������ sector. � ������ ������� ���������� false
    bool WriteToSector(const Sector *sector) const;
    
    void Erase() const;
    
    // ���������� ���������� ����, ����������� ��� �������� ������
    static int GetPackedSize(const DataSettings *ds);

    void Log() const;
};


struct Sector
{
    enum E
    {
        _00_BOOT_1,     // 16k  + ��������� (48k)
        _01_BOOT_2,     // 16k  +   ��������� 2
        _02_BOOT_3,     // 16k  +   ��������� 3
        _03,            // 16k
        _04,            // 64k
        _05_FIRM_1,     // 128k + �������� �������� (384k)
        _06_FIRM_2,     // 128k +   �������� �������� 2
        _07_FIRM_3,     // 128k +   �������� �������� 3
        _08_FIRM_4,     // 128k
        _09_FIRM_5,     // 128k
        _10_SETTINGS_1, // 128k + ��������� (256k)
        _11_SETTINGS_2, // 128k + ��������� 2
        _12_NRST_1,     // 16k  + �������������� ���������
        _13_NRST_2,     // 16k  + �������������� ���������
        _14,            // 16k
        _15,            // 16k
        _16,            // 64k
        _17,            // 128k
        _18,            // 128k
        _19_DATA_1,     // 128k + ���������� ������ (640k)
        _20_DATA_2,     // 128k +   ���������� ������ 2
        _21_DATA_3,     // 128k +   ���������� ������ 3
        _22_DATA_4,     // 128k +   ���������� ������ 4
        _23_DATA_5,     // 128k +   ���������� ������ 5
        Count
    } number;

    uint address;

    uint size;

    // ����� ������� ����� �� ��������
    uint End() const;

    void Erase() const;
    
    // true ��������, ��� � ������ �� ������� �� ���� �����
    bool ExistPackets() const;

    const PacketROM *FirstFreePacket() const;

    const PacketROM *WriteData(uint numInROM, const DataSettings *ds) const;

    const ReadingFrame *ReadData(uint numInROM) const;

    const PacketROM *DeleteData(uint numInROM) const;
    
    // �������� ���������� � ��������� � ������� ������
    void GetDataInfo(bool existData[ROM::Data::MAX_NUM_SAVED_WAVES]) const;
    
    // ���������� ��������� �� ������ �����
    const PacketROM *FirstPacket() const;

    const PacketROM *FindValidPacket(uint numInROM) const;
    
    // ���������� ���������� ����, ���������� ������� ��������
    uint GetNumberWornBytes() const;
    
    // ���������� ��������� �� ������ ����� � ������� �� ������� sector � nullptr, ���� ������ � ������� ���
    const PacketROM *GetFirstPacketWithData() const;
    
    // ���������� ����� �������, �������� ����������� address
    static int Number(uint address);
    
    // ���������� ������ �� ������, �������� ����������� ������ �����
    static const Sector &Get(uint address);

    static const Sector &Get(Sector::E number);

    void Log() const;
private:
    static void TranslateAddressToROM(const DataSettings *ds, const PacketROM *packet);
};
