// 2021/08/09 17:00:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct StorageRecord;

/*
*   Фрейм чтения. Здесь хранятся считанные данные как они есть
*/
struct ReadingFrame
{
    friend struct StorageRecord;

    ReadingFrame();

    void Prepare();

    void CreateFromCurrentSettings();

    void CreateNull();

    DataSettings &Settings() const;

    // Хотя и возвращается указатель на 8-битные значения, следует иметь ввиду, что в случае, если данные расположены
    // во внешнем ОЗУ, читать следует по 16 бит, т.к. доступ по нечётным адресам к ОЗУ запрещён
    uint8 *Data(const Chan &ch);

    // Возвращает указатель на следующий за последним байт
    uint8 *DataEnd(const Chan &ch);

    void CreateFromRecord(const StorageRecord *record);

private:

    // В буфере хранятся последовательно - настройки, данные первого канала, данные второго канала
    Buffer<uint8> buffer;

    uint Size();

    /*
    *  Дополнительные поля для поточечного режима
    */

public:

    bool isP2P;         // true означает, что это фрейм поточечного режима
};
