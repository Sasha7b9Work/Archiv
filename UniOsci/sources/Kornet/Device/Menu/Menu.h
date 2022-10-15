#pragma once
#include "MenuControls.h"
#include "Keyboard/Buttons.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Menu
{
public:
    Menu();
    void Init();
    void Update();
    void ButtonPress(Key button, TypePress typePress);
    void Draw();
    void SaveSettings();

private:
    void OnPressNone();
    void OnPressRangeLessA();
    void OnPressRangeMoreA();
    void OnPressRangeLessB();
    void OnPressRangeMoreB();
    void OnPressTBaseMore();
    void OnPressTBaseLess();
    void OnPressRShiftLessA();
    void OnPressRShiftMoreA();
    void OnPressRShiftLessB();
    void OnPressRShiftMoreB();
    void OnPressTShiftLess();
    void OnPressTShiftMore();
    void OnPressTrigLevLess();
    void OnPressTrigLevMore();
    void OnPressChannelA();
    void OnPressChannelB();
    void OnPressF1();
    void OnPressF2();
    void OnPressF3();
    void OnPressF4();
    void OnPressF5();
    void OnPressService();
    void OnPressStartStop();
    void OnPressTrig();
    void OnPressMenu();
    void OnPressFunction();
    void OnPressEnter();

    void VerifyOnDoubleClick(int src, int dir);

    /// \tode Здесь хранится время предыдущего нажатия кнопки для отлова двойного нажатия.
    /// 1 индекс 1канал/2канал/синхронизация/время, 2 индекс - вниз/вверх
    uint timePrevPress[4][2];
    /// Время двойного нажатия
    const uint timeDoubleClick;
    /// Здесь хранится тип нажатий нопки для функции ButtonPress
    TypePress typePress;
    /// Текущая обрабатываемая кнопка
    Key button;
    /// Если true, то кнопка нажата
    bool isPressed[NumButtons];
    /// Время последнего нажатия кнопки. Нужно для того, чтобы периодически сохранять настройки
    uint timeLastPressedButton;
    /// Установить меню в соответствие с текущим режимом
    void TuneOnDevice();
};


extern Menu menu;
