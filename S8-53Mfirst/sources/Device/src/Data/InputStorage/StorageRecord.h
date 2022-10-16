// 2021/08/09 16:57:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct StorageRecord
{
    /*
    *  Запись записывается так. Сначала идёт собственно StorageRecord, затем DataSettings, а затем данные первого и вто-
    *  рого каналов
    */

    friend struct ReadingFrame;

public:

    // Добавить данные поточечного режима
    void AppendDataP2P(uint16 dataA, uint16 dataB)
    {
        DataSettings &ds = Settings();

        if (index >= ds.BytesInChannel()) { index = 0; }

        if (ds.enabled[Chan::A]) { Data(ChA)[index / 2] = dataA; }

        if (ds.enabled[Chan::B]) { Data(ChB)[index / 2] = dataB; }

        index += 2;
        totalBytes += 2;
    }

    void Fill(const ReadingFrame &data);

    // Возвращает размер записи, когда в неё будут записаны данные data
    uint Size(const DataSettings &data) const;

    uint8 *Address() const { return (uint8 *)this; }

    // Возвращает адрес первого байта, следующего за записью
    uint8 *End() const;

private:

    uint Size() const;

    DataSettings &Settings() const
    {
        uint8 *address = Address() + sizeof(StorageRecord);

        return *(DataSettings *)address;
    }

    uint16 *Data(const Chan &ch)
    {
        DataSettings &ds = Settings();

        if (!ds.enabled[ch]) { return nullptr; }

        uint8 *address = Address() + sizeof(StorageRecord) + sizeof(DataSettings);

        if (ch.IsB() && ds.enabled[Chan::A]) { address += ds.BytesInChannel(); }

        return (uint16 *)address;
    }

    void AppendData(const Chan &ch, uint16 data)
    {
        DataSettings &ds = Settings();

        if (ds.enabled[ch])
        {
            if (index >= ds.BytesInChannel()) { index = 0; }

            uint16 *buffer = Data(ch);

            if (buffer == nullptr) { return; }

            buffer[index / 2] = data;

            index += 2;
            totalBytes += 2;
        }
    }

public:

    StorageRecord *prev;        // Адрес предыдущей (более старой) записи
    StorageRecord *next;        // Адрес следующей (более новой) записи

    bool isP2P;         // true означает, что это фрейм поточечного чтения - сюда нужно сохранять точки в поточечном
                        // режиме

    int index;          // Индекс байта, в который нужно производить следующую запись
    int totalBytes;     // Столько точек уже считано

    /*
    *  Поля для поточечного режима
    */
};
