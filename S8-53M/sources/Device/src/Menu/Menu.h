// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Painter/Text_.h"
#include "common/Panel/Controls_.h"
#include "common/Utils/Containers/String_.h"
#include "Menu/Items/MenuItems.h"


namespace Menu
{
    void Init();

    void Draw();

    // Функция должна вызываться в главном цикле.
    void Update();

    // Возвращает адрес элемента меню, соответствующего данной key.
    Item *ItemUnderKey(Key::E key);

    // Установить время автоматического сокрытия меню в соответствии с установками.
    void SetAutoHide(bool active);

    // Возвращает адрес пункта меню, находящегося под нажатой в данный момент кнопкой.
    Item *ItemUnderKey();

    // Возвращает путь к текущему пункту меню в текстовом виде, готовом к выводу на экран.
    Text StringNavigation();

    void OpenItemTime();

    // Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата
    // последней.
    const Item *CurrentItem();

    NamePage::E GetNameOpenedPage();

    // Закрыть открытый элемент меню.
    void CloseOpenedItem();

    // Скрыть меню
    void Hide();

    // Показать меню
    void Show();

    // Изменить "показанность" - скрыть если открыто и наоборот
    void ChangeShowing();

    // Если true - меню находится в дополнительном режиме.
    bool IsMinimize();

    // Повернуть ручку УСТАНОВКА на текущей странице малых кнопок.
    void CurrentPageSBregSet(int angle);

    void DrawStringNavigation();

    // true, если нужно выводить строку навигации меню
    bool NeedDrawStringNavigation();

    extern bool showHelpHints;      // Если 1, то при нажатии кнопки вместо выполнения её фунции выводится подсказка о
                                    // её назначении
    extern bool needClosePageSB;    // Если 1, нужно закрывать страницу малых кнопок

    extern Item* itemsForButtons[6];    // Здесь после отрисовки меню оказываются указатели на отрисованные итемы
                                        // для кнопок MENU (0), F1-F5 (1-5)
                                        // Обновляются после каждой отрисовки меню

    namespace Handlers
    {
        // Функция обработки короткого нажатия кнопки (менее 0.5 сек.).
        void ShortPressureKey(Key::E key);

        // Обработка длинного нажатия кнопки.
        void LongPressureKey(Key::E key);

        // Обработка опускания кнопки вниз.
        void PressKey(Key::E key);

        // Обработка поднятия кнопки вверх.
        void ReleaseKey(Key::E key);

        // Обработка поворота ручки УСТАНОВКА.
        void RegulatorSet(Action::E action);

        void KeyForHint(Key::E key);
    };
};
