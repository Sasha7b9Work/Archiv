// 2022/02/10 17:00:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/DataSettings.h"
#include "common/Utils/Containers/Buffer_.h"
#include "Data/InputStorage/ReadingFrame.h"
#include "Settings/Settings.h"
#include "Data/InputStorage/InputStorage.h"
#include "Data/InputStorage/StorageRecord.h"
#include <cstring>


ReadingFrame::ReadingFrame() : isP2P(false)
{
    CreateFromCurrentSettings();
}


void ReadingFrame::Prepare()
{
    if (InputStorage::SettingsIsEquals(0, *this))
    {
        return;
    }

    ReadingFrame last;

    if (InputStorage::ExtractLastFrame(last))
    {
        DataSettings &ds = Settings();

        if (ds.enabled[Chan::A])
        {
            std::memcpy(Data(ChA), last.Data(ChA), (uint)ds.BytesInChannel());
        }

        if (ds.enabled[Chan::B])
        {
            std::memcpy(Data(ChB), last.Data(ChB), (uint)ds.BytesInChannel());
        }

        ds.is_clean = 0;
    }
}


void ReadingFrame::CreateFromCurrentSettings()
{
    buffer.Realloc((int)sizeof(DataSettings) + EnumPointsFPGA::BytesForData());

    DataSettings &ds = *(DataSettings *)buffer.DataU8();

    ds.Fill();
}


void ReadingFrame::CreateNull()
{
    DataSettings ds;
    ds.enabled[Chan::A] = false;
    ds.enabled[Chan::B] = false;

    buffer.Realloc(sizeof(DataSettings));

    std::memcpy(buffer.DataU8(), &ds, sizeof(DataSettings));
}


DataSettings &ReadingFrame::Settings() const
{
    return (DataSettings &)*((ReadingFrame *)this)->buffer.DataU8();
}


uint8 *ReadingFrame::Data(const Chan &ch)
{
    const DataSettings &ds = Settings();

    if (!ds.enabled[ch])
    {
        return nullptr;
    }

    uint8 *result = buffer.DataU8() + sizeof(DataSettings);

    if (ch.IsB() && ds.enabled[Chan::A])
    {
        result += ds.BytesInChannel();
    }

    return result;
}


uint8 *ReadingFrame::DataEnd(const Chan &ch)
{
    uint8 *data = Data(ch);

    return (data == nullptr) ? data : data + Settings().BytesInChannel();
}


void ReadingFrame::CreateFromRecord(const StorageRecord *record)
{
    if (record == nullptr)
    {
        LOG_ERROR("Нулевой указатель");
    }
    else
    {
        DataSettings &ds = record->Settings();

        int size = (int)sizeof(DataSettings) + ds.BytesInData();

        buffer.Realloc(size);

        std::memcpy(buffer.DataU8(), &ds, (uint)size);
    }
}


uint ReadingFrame::Size()
{
    uint result = sizeof(DataSettings);

    DataSettings &ds = Settings();

    if (ds.enabled[Chan::A])
    {
        result += ds.BytesInChannel();
    }

    if (ds.enabled[Chan::B])
    {
        result += ds.BytesInChannel();
    }

    return result;
}
