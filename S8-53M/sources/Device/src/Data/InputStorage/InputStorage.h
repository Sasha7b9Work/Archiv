// 2021/03/18 16:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/Buffer_.h"
#include "Data/DataSettings.h"
#include "Data/InputStorage/ReadingFrame.h"


/*
*  Класс организует из свободного пространства внешнего ОЗУ хранилище данных, данные в которым хранятся так.
*  В переменной addressFirstRecord хранится адрес первой записи. Запись представляет собой элемент типа ReadingFrame.
*  Начало каждой последующей записи находится за последним байтом предыдущей записи.
*  При достижении конца ОЗУ стирается первый (самый старый) элемент, и на его место записывается последний (самый
*  новый. Не забыть переписать указатель на первый э
*/

namespace InputStorage
{
    // Подготовить для нового цикла чтения
    void PrepareForNewCycle();

    // Добавить данные фрейма
    void AppendDataFrame(const ReadingFrame &data);

    // Извлечь последние положенные данные
    bool ExtractLastFrame(ReadingFrame &data);

    // Извлечь данные, находящиеся на "расстоянии" from_end от последней записи (при from_end возвращает последнюю
    // сохранённую запись)
    bool ExtractFrame(uint from_end, ReadingFrame &data);

    // Возвращает количество записей в хранилище
    uint NumFrames();

    // Возвращает true, если настройки у from-end-й записи и data совпадают (нужно для рандомизатора)
    bool SettingsIsEquals(uint from_end, const ReadingFrame &data);

    // Самая новая запись (записана последней)
    StorageRecord *Newest();
};
