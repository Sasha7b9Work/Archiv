// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "Display/Display.h"
#include "Display/Screen/Grid.h"
#include "Menu/Items/MenuItemsDefs.h"


class Page;
class SmallButton;
class TimeItem;
class Governor;
class Choice;


#define MAX_NUM_CHOICE_SMALL_BUTTON 6    // Максимальное количество вариантов маленькой кнопки + 1

                                            
// Разные виды пунктов меню
struct TypeItem { enum E
{
    None,           
    Choice,        // Пункт выбора - позволяет выбрать одно из нескольких заданных значений.
    Button,        // Кнопка.
    Page,          // Страница.
    Governor,      // Регулятор - позволяет выбрать любое целое числовое значение из заранее заданного диапазаона.
    Time,          // Позволяет ввести время.
    IP,            // Позволяет ввести IP-адрес.
    SwitchButton,
    GovernorColor, // Позволяет выбрать цвет.
    Formula,       // Позволяет выбрать знак и коэффициенты для математической формулы (умножение и сложение)
    MAC,           // MAC-адрес
    ChoiceReg,     // Элемент выбора, в котором выбор осуществляется не кнопкой, а ручкой
    SmallButton,   // Кнопка для режима малых кнопок
    Count
};};


// Здесь ID всех представленных в меню страниц
struct NamePage { enum E
{
    Empty,

    MainPage,                   // Главная страница меню. Из неё растут все остальные

    Display,                    // ДИСПЕЙ
    DisplayAccumulation,        // ДИСПЛЕЙ - НАКОПЛЕНИЕ
    DisplayAverage,             // ДИСПЛЕЙ - УСРЕДНЕНИЕ
    DisplayGrid,                // ДИСПЛЕЙ - СЕТКА
    DisplayRefresh,             // ДИСПЛЕЙ - ОБНОВЛЕНИЕ
    DisplaySettings,            // ДИСПЛЕЙ - НАСТРОЙКИ

    ChannelA,                   // КАНАЛ 1

    ChannelB,                   // КАНАЛ 2

    Trig,                       // СИНХР.
    TrigAuto,                   // СИНХР - АВТ ПОИСК

    Time,                       // РАЗВЕРТКА

    Cursors,                    // КУРСОРЫ
    CursorsSet_SB,              // КУРСОРЫ - УСТАНОВИТЬ

    Memory,                     // ПАМЯТЬ
    MemoryLatest_SB,            // Открыта страница ПАМЯТЬ - Последние
    MemoryInt_SB,               // Открыта страница ПАМЯТЬ - Внутр ЗУ
    MemoryExt,                  // ПАМЯТЬ - ВНЕШН. ЗУ
    MemoryExtSetMask_SB,        // Открыта страница ПАМЯТЬ - ВНЕШН ЗУ - Маска
    MemoryExtSetName_SB,        // Появляется перед сохранением файла на флешку для задания имени файла при соответствующей
                                // опции ВНЕШН ЗУ - Имя файла
    MemoryExtFileManager_SB,    // Открыта страница ПАМЯТЬ - ВНЕШН ЗУ - Обзор

    Measures,                   // ИЗМЕРЕНИЯ
    MeasuresTune_SB,            // ИЗМЕРЕНИЯ - НАСТРОИТЬ

    Service,                    // СЕРВИС
    ServiceDisplayColors,       // СЕРВИС - ДИСПЛЕЙ - ЦВЕТА
    ServiceCalibrator,          // СЕРВИС - КАЛИБРАТОР
    ServiceEthernet,            // СЕРВИС - ETHERNET
    ServiceMath,                // СЕРВИС - МАТЕМАТИКА
    ServiceMathFFT,
    ServiceMathFFTCursors_SB,   // СЕРВИС - МАТЕМАТИКА - Курсоры
    ServiceInformation_SB,      // СЕРВИС - ИНФОРМАЦИЯ
    ServiceMathFunction_SB,     // СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ

    Help_SB,                    // ПОМОЩЬ

    Debug,                      // ОТЛАДКА
    DebugADC,                   // ОТЛАДКА - АЦП
    DebugADCbalance,            // ОТЛАДКА - АЦП - БАЛАНС
    DebugADCstretch,            // ОТЛАДКА - АЦП - РАСТЯЖКА
    DebugADCrShift,             // ОТЛАДКА - АЦП - ДОП. СМЕЩЕНИЕ
    DebugADCaltShift,           // ОТЛАДКА - АЦП - ДОП. СМЕЩ. ПАМ.
    DebugAltera,                // ОТЛАДКА - АЛЬТЕРА
    DebugConsole,               // ОТЛАДКА - КОНСОЛЬ
    DebugData,                  // ОТЛАДКА - ДАТА
    DebugInfo,                  // ОТЛАДКА - ИНФО
    DebugRandomizer,            // ОТЛАДКА - РАНД-ТОР
    DebugShowRegisters,         // ОТЛАДКА - РЕГИСТРЫ
    DebugSound,                 // ОТЛАДКА - ЗВУК
    DebugSerialNumber_SB,       // ОТАДКА - С/Н

    Count,
    NoPage
};};             // При добавлении страниц нужно изменять инициализацию SettingsMenu в SSettings.c


#define TITLE(item) ((item)->data->titleHint[LANG])
#define HINT(item) ((item)->data->titleHint[2 + LANG])


class StructHelpSmallButton
{
public:
    pFuncVII funcDrawUGO;    // Указатель на функцию отрисовки изображения варианта кнопки
    pchar    helpUGO[2];     // Подпись к данному изображению.
};

typedef StructHelpSmallButton arrayHints[MAX_NUM_CHOICE_SMALL_BUTTON];

#include "Menu/Items/Item.h"
#include "Menu/Items/ItemPage.h"
#include "Menu/Items/ItemButton.h"
#include "Menu/Items/ItemSmallButton.h"
#include "Menu/Items/ItemGovernor.h"
#include "Menu/Items/ItemIPaddress.h"
#include "Menu/Items/ItemMACaddress.h"
#include "Menu/Items/ItemFormula.h"
#include "Menu/Items/ItemGovernorColor.h"
#include "Menu/Items/ItemChoice.h"
#include "Menu/Items/ItemTime.h"
