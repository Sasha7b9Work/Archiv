// 2021/03/18 16:06:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Data/DataSettings.h"
#include "Data/InputStorage/InputStorage.h"
#include "Data/InputStorage/StorageRecord.h"
#include <cstring>


namespace InputStorage
{
    // Создаёт запись во внешнем ОЗУ для сохраенения data
    static StorageRecord *Create(const DataSettings &data);

    // Удаляет самую старую запись
    static void DeleteOldest();

    // Удаляет самую новую запись (добавленную последней)
    static void DeleteNewest();

    // Возвращает false и пустое data (в котором оба канала выключены, остальные настройки не определены)
    static bool CreateNull(ReadingFrame &data);

    // Удалить поточечный фрейм, если таковой имеется
    static void DeleteFrameP2P();

    // Найти запись для случая, когда новая запись имеет больший адрес, чем старая
    static StorageRecord *FindForNewestMoreOldest(uint need_memory);

    // Записывает по address данные из buffer. Возвращает адрес первого байта после записи
    static uint8 *CopyTo(uint8 *address, const void *buffer, uint size);

    static StorageRecord *oldestRecord = nullptr;   // Здесь хранится адрес первой записи. Зная его, можно рассчитать все
                                                    // остальные адреса

    // Самая старая запись (записана первой)
    static StorageRecord *Oldest() { return oldestRecord; };
}


void StorageRecord::Fill(const ReadingFrame &data)
{
    DataSettings &ds = data.Settings();
    uint length_channel = (uint)ds.BytesInChannel();

    uint8 *address = (uint8 *)this + sizeof(StorageRecord);

    address = InputStorage::CopyTo(address, (void *)&ds, sizeof(DataSettings));

    if (ds.enabled[Chan::A])
    {
        address = InputStorage::CopyTo(address, ((ReadingFrame &)data).Data(Chan::A), length_channel);
    }

    if (ds.enabled[Chan::B])
    {
        InputStorage::CopyTo(address, ((ReadingFrame &)data).Data(Chan::B), length_channel);
    }

    isP2P = data.isP2P;
}


uint StorageRecord::Size() const
{
    return Size(Settings());
}


uint StorageRecord::Size(const DataSettings &data) const
{
    return sizeof(StorageRecord) + sizeof(data) + data.BytesInData();
}


uint8 *StorageRecord::End() const
{
    return Address() + Size();
}


StorageRecord *InputStorage::Newest()
{
    if (oldestRecord == nullptr) { return nullptr; }

    StorageRecord *record = oldestRecord;

    while (record->next) { record = record->next; }

    return record;
}


void InputStorage::PrepareForNewCycle()
{
    DeleteFrameP2P();

    if (TBASE_IS_P2P)
    {
        ReadingFrame frame;

        frame.Prepare();

        frame.isP2P = true;

        InputStorage::AppendDataFrame(frame);
    }
}


void InputStorage::AppendDataFrame(const ReadingFrame &data)
{
    DeleteFrameP2P();

    StorageRecord *record = Create(data.Settings());

    record->Fill(data);

    record->isP2P = data.isP2P;
    record->index = 0;
    record->totalBytes = 0;
}


StorageRecord *InputStorage::Create(const DataSettings &ds)
{
    StorageRecord *result = nullptr;

    if (oldestRecord == nullptr)
    {
        oldestRecord = (StorageRecord *)HAL_FMC::ADDR_RAM_BEGIN;
        result = oldestRecord;
        result->prev = nullptr;
        result->next = nullptr;
    }
    else
    {
        uint need_memory = Oldest()->Size(ds);

        if (Oldest() > Newest())
        {
            while (Oldest()->Address() - Newest()->End() < (int)need_memory)
            {
                DeleteOldest();

                if (Oldest() < Newest())
                {
                    break;
                }
            }
        }

        result = (Oldest() < Newest()) ? FindForNewestMoreOldest(need_memory) : (StorageRecord *)Newest()->End();

        result->prev = Newest();
        HAL_TIM2::DelayTicks(0);        // \todo Без этого почему-то виснет при переключении на 281 точку в релизе
        result->next = nullptr;
        Newest()->next = result;
    }

    return result;
}


void InputStorage::DeleteFrameP2P()
{
    StorageRecord *record = Newest();

    if (record != nullptr)
    {
        if (record->isP2P)
        {
            DeleteNewest();
        }
    }
}


bool InputStorage::ExtractFrame(uint from_end, ReadingFrame &data)
{
    if (NumFrames() == 0 || from_end >= NumFrames())
    {
        return CreateNull(data);
    }

    StorageRecord *record = Newest();

    while (from_end != 0)
    {
        if (record == nullptr)
        {
            LOG_ERROR("Ошибка");
            break;
        }

        record = record->prev;
        from_end--;
    }

    data.CreateFromRecord(record);

    return true;
}


StorageRecord *InputStorage::FindForNewestMoreOldest(uint need_memory)
{
    StorageRecord *result = Newest();

    if (result != nullptr)
    {
        uint8 *address = result->End();                                           // Это предполагаемый адрес нашей записи

        if (address + need_memory <= HAL_FMC::ADDR_RAM_END)                               // Если запись поместится в память
        {
            result = (StorageRecord *)address;
        }
        else                                                                          // Если запись не поместится в память
        {                                                                             // то будем записывать в начало памяти
            while (Oldest()->Address() - HAL_FMC::ADDR_RAM_BEGIN < (int)need_memory)
            {
                DeleteOldest();
            }

            result = (StorageRecord *)HAL_FMC::ADDR_RAM_BEGIN;
        }
    }

    return result;
}


void InputStorage::DeleteOldest()
{
    if (oldestRecord == nullptr)
    {
        return;
    }

    Oldest()->next->prev = nullptr;

    oldestRecord = Oldest()->next;
}


void InputStorage::DeleteNewest()
{
    StorageRecord *last = Newest();

    if (last != nullptr)
    {
        last = last->prev;       // Теперь last указывает на предпоследнюю запись (ту, которая была записана перед последней

        if (last == nullptr)        // Предыдущей записи нет - новейщая запись является одновременно и старейшей - в
        {                           // хранилище только одна запись
            oldestRecord = nullptr;
        }
        else
        {
            last->next = nullptr;
        }
    }
}


uint InputStorage::NumFrames()
{
    if (oldestRecord == nullptr)
    {
        return 0;
    }

    StorageRecord *record = Oldest();

    uint result = 1;

    while (record->next)
    {
        result++;
        
        record = record->next;
    }

    return result;
}


bool InputStorage::ExtractLastFrame(ReadingFrame &data)
{
    return ExtractFrame(0, data);
}


bool InputStorage::CreateNull(ReadingFrame &data)
{
    data.CreateNull();

    return false;
}


uint8 *InputStorage::CopyTo(uint8 *address, const void *buffer, uint size)
{
    std::memcpy(address, buffer, size);

    return address + size;
}


bool InputStorage::SettingsIsEquals(uint from_end, const ReadingFrame &sample)
{
    ReadingFrame data;

    if (ExtractFrame(from_end, data))
    {
        DataSettings &ss = sample.Settings();
        DataSettings &sd = data.Settings();

        return ((ss.tbase == sd.tbase) &&
            (ss.range == sd.range) &&
            (ss.tshift == sd.tshift) &&
            (ss.r_shift_a == sd.r_shift_a) &&
            (ss.r_shift_b == sd.r_shift_b) &&
            (ss.multiplier_a == sd.multiplier_a) &&
            (ss.multiplier_b == sd.multiplier_b) &&
            (ss.enum_points == sd.enum_points));
    }

    return false;
}
