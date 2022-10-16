// 2021/03/16 16:35:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Math_.h"
#include "Data/DataSettings.h"
#include "Data/InputStorage/InputStorage.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "FPGA/ReaderFPGA.h"
#include "Panel/Panel.h"
#include "Data/InputStorage/StorageRecord.h"
#include <cstring>


#define WRITE_AND_OR_INVERSE(addr, data, ch)


namespace ReaderFPGA
{
    // Инвертирует данные.
    static void InverseDataIsNecessary(ReadingFrame &data, const Chan &ch);

    Mutex mutex_read;
}


struct Recalculator
{
private:

    inline static uint8 CalculateADC(uint8 value, float _balance)
    {
        float result = Math::AddWithLimitation<float>(value, _balance, -255.0F, 255.0F);

        float delta = result - (float)ValueFPGA::AVE;

        delta *= stretch;

        result = ValueFPGA::AVE + delta;

        Math::Limitation<float>(&result, ValueFPGA::MIN, ValueFPGA::MAX);

        return (uint8)result;
    }

public:

    // Загрузить настройки для выполнения дальнейших расчётов
    static void LoadSettings(const Chan &ch);

    // То же самое, что и метод с 8-битным параметром, но пересчитывает сразу два значения
    inline static uint16 Calculate(uint16 value)
    {
        // Пересчитать считанное из АЦП1 значение в реальное (с учётом растяжения и сдвига по вертикали)
        uint16 byte0 = CalculateADC((uint8)(value & 0x00ff), 0);

        // Пересчитать считанное из АЦП2 значение в реальное (тут добавляется баланс АЦП)
        uint16 byte1 = CalculateADC((uint8)((value & 0xff00) >> 8), balance);

        return (uint16)(byte0 + (byte1 << 8));
    }

    inline static uint8 Calculate(uint8 value)
    {
        return CalculateADC(value, balance);
    }

    static float stretch;               // Растяжка читаемых данных
    static float balance;               // Баланс второго АЦП
    static float addShift;              // Дополнительное смещение
};


struct Read
{
    static Int CalculateShift();

    // Возвращает адрес, с которого нужно читать первую точку
    static uint16 CalculateAddressRead();

    struct Real
    {
        static void Channel(ReadingFrame &data, const Chan &ch);
    };

    struct Randomizer
    {
        static void Channel(ReadingFrame &data, const Chan &ch);

        // Рассчитывает индекс первой считываемой точки в массиве и также количество точек, которые нужно отбросить
        // при чтении
        // firts_out - в этом место нужно поставить первую считанную точку
        // skipped_out - перед чтением точек нужно отбросить столько точек
        static bool IndexFirstPoint(int *first_out, int *skipped_out);

        // Считать и выбрость num_words данных по адресу address
        static void UtilizeFirstBytes(const uint16 *const address, int num_words);

        // Рассчитывает первый адрес, в который нужно записывать считанные данные
        static uint8 *CalculateFirstAddressWrite(ReadingFrame &dr, const ::Chan &ch);
    };
};


float Recalculator::stretch = 1.0f;
float Recalculator::balance = 0.0f;
float Recalculator::addShift = 0.0f;


const uint16 *const addresses_ADC[2] = { RD_ADC_A, RD_ADC_B };


void ReaderFPGA::ReadData()
{
    mutex_read.Lock();

    ReadingFrame frame;

    frame.Prepare();

    if (TBASE_IS_RAND)
    {
        Read::Randomizer::Channel(frame, ChA);
        Read::Randomizer::Channel(frame, ChB);
    }
    else
    {
        Read::Real::Channel(frame, ChA);
        Read::Real::Channel(frame, ChB);
    }

    InputStorage::AppendDataFrame(frame);

    mutex_read.Unlock();
}


void ReaderFPGA::ReadPointsP2P()
{
    FPGA::flag.Read();

    if (FPGA::flag.IsPointReady())
    {
        uint16 dataB = *RD_ADC_B;

        uint16 dataA = *RD_ADC_A;

        dataB = Recalculator::Calculate(dataB);

        dataA = Recalculator::Calculate(dataA);

        InputStorage::Newest()->AppendDataP2P(dataA, dataB);
    }
}


void Read::Real::Channel(ReadingFrame &data, const ::Chan &ch)
{
    uint8 *p = data.Data(ch);
    const uint8 *end = data.Data(ch) + data.Settings().BytesInChannel();

    ReaderFPGA::ReadPoints(ch, p, end);

    if (!PEAK_DET_IS_ENABLED && FPGA::flag.IsFirstByte0())
    {
        MathFPGA::ShiftBuffer(data.Data(ch), data.Data(ch) + data.Settings().BytesInChannel(), -1);
    }
}


void ReaderFPGA::ReadPoints(const Chan &ch, uint8 *_first, const uint8 *_last)
{
    BUS_FPGA::Write(WR_PRED, Read::CalculateAddressRead(), false);
    BUS_FPGA::Write(WR_ADDR_READ, 0xffff, false);

    Recalculator::LoadSettings(ch);

    uint16 *first = (uint16 *)_first;
    const uint16 *last = (const uint16 *)_last;

    const uint16 *address = ADDRESS_READ(ch);

    while (first < last)
    {
        *first++ = Recalculator::Calculate(*address);
    }
}


void Recalculator::LoadSettings(const Chan &ch)
{
    stretch = setNRST.adc.stretch_enabled ? NRST_STRETCH(ch) : 1.0f;

    balance = NRST_BALANCE_ADC(ch);

    if (TBASE_IS_RAND || PEAK_DET_IS_ENABLED)
    {
        balance = 0;
    }

    addShift = NRST_RSHIFT(ch, RANGE(ch), 0);
}


bool Read::Randomizer::IndexFirstPoint(int *first_out, int *skipped_out)
{
    Int Tsm = CalculateShift();

    if (!Tsm.IsValid())
    {
        return false;
    }

    int step = TBase::StepRand();

    int index = Tsm - 2 * step;

    int num_skipped = 0;

    while (index < 0)
    {
        index += step;
        num_skipped++;
    }

    *first_out = index;
    *skipped_out = num_skipped;

    return true;
}


uint8 *Read::Randomizer::CalculateFirstAddressWrite(ReadingFrame &dr, const ::Chan &ch)
{
    int index = 0;
    int num_skipped = 0;

    if (!IndexFirstPoint(&index, &num_skipped))
    {
        return nullptr;
    }

    DataSettings &ds = dr.Settings();

    uint8 *awrite = dr.Data(ch);                                           // Сюда будем писать считываемые данны

    if (awrite != nullptr)
    {
        if (ds.is_clean == 1)
        {
            std::memset(awrite, ValueFPGA::NONE, (uint)ds.BytesInChannel());
            ds.is_clean = 0;
        }

        UtilizeFirstBytes(ADDRESS_READ(ch), num_skipped);

        awrite += index + LaunchFPGA::AdditionalOffsetIndexFirst();
    }

    return awrite;
}


void Read::Randomizer::Channel(ReadingFrame &dr, const ::Chan &ch)
{
    uint8 *awrite = CalculateFirstAddressWrite(dr, ch);       // Первый адрес сохранения читаемых данных

    if (awrite == nullptr)
    {
        return;
    }

    BUS_FPGA::Write(WR_PRED, Read::CalculateAddressRead(), false);
    BUS_FPGA::Write(WR_ADDR_READ, 0xffff, false);

    int bytesInChannel = dr.Settings().BytesInChannel();
    int numBytes = bytesInChannel / TBase::StepRand();

    const uint8 *awriteLast = awrite + bytesInChannel;

    if (awriteLast > dr.DataEnd(ch))
    {
        awriteLast = dr.DataEnd(ch);
    }

    const uint16 *const aread = ADDRESS_READ(ch);

    Recalculator::LoadSettings(ch);

    for (int i = 0; i < numBytes; i++)
    {
        if (awrite >= dr.Data(ch) && awrite < awriteLast)
        {
            *awrite = Recalculator::Calculate((uint8)*aread);
        }

        awrite += TBase::StepRand();
    }
}


void Read::Randomizer::UtilizeFirstBytes(const uint16 * const address, int num_words)
{
    volatile uint16 data;

    for (int i = 0; i < num_words; i++)
    {
        data = *address;
    }

#ifdef __linux__
    data = data;
#endif
}


void ReaderFPGA::InverseDataIsNecessary(ReadingFrame &data, const Chan &ch)
{
    if (ch.IsInversed())
    {
        uint8 *d = data.Data(ch);
        int size = data.Settings().BytesInChannel();

        for (int i = 0; i < size; i++)
        {
            d[i] = (uint8)((int)(2 * ValueFPGA::AVE) - Math::Limitation<uint8>((uint8)d[i], ValueFPGA::MIN, ValueFPGA::MAX));
        }
    }
}


// Возвращает true, если нужно отбросить значение рандомизатора
static bool NeedToDiscardValueRandomizer(uint16 rand, uint16 min, uint16 max)
{
    if (rand > max - setNRST.rand.gate_max * 10)
    {
        return true;
    }

    if (rand < min + setNRST.rand.gate_max * 10)
    {
        return true;
    }

    return false;
}


Int Read::CalculateShift()
{
    Uint16 rand = HAL_ADC1::GetValue();

    if (rand.IsValid())
    {
        uint16 min = 0;
        uint16 max = 0;

        if (::Randomizer::CalculateGate(rand, &min, &max))
        {
            if (!NeedToDiscardValueRandomizer(rand, min, max))
            {
                float tin = (float)(rand - min) / (max - min) * 10e-9F;

                return (int)(tin / 10e-9F * TBase::StepRand());
            }
        }
    }

    return InvalidInt();
}


uint16 Read::CalculateAddressRead()
{
    int shift = TBASE_IS_RAND ?
        EnumPointsFPGA::BytesInChannel() / TBase::StepRand() :
        EnumPointsFPGA::BytesInChannel() / 2;

    int result = HAL_FMC::Read(RD_ADDR_LAST_RECORD) - shift;

    return (uint16)(result + LaunchFPGA::DeltaReadAddress() - 1);
}
