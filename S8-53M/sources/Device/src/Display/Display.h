// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tables.h"
#include "common/Display/Colors_.h"


namespace Display
{
    struct DrawMode { enum E {
        Default,        // Стандартная отрисовка в цикле
        Timer           // Отрисовка по таймеру заданной функции
    }; };

    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    void Init();

    uint8 *GetBuffer();

    uint8 *GetBufferEnd();

    void BeginFrame(const Color &color);

    void EndFrame();

    void Update();

    void RotateTrigLev();

    void Redraw();

    void SetDrawMode(DrawMode::E mode, pFuncVV func = nullptr);

    void Clear();

    void ShiftScreen(int delta);

    void ChangedRShiftMarkers();

    void DisableShowLevelTrigLev();

    void OnRShiftMarkersAutoHide();

    // Читает строку row в буфер pixels
    void ReadRow(int row, uint8 pixels[320]);

    namespace Message
    {
        void Show(pchar textRu, pchar textEn);

        void Hide();
    }

    // Класс будет посылать данные в интерфейс
    namespace Sender
    {
        void SendPalette();

        void SendFrame();

        void Update();
    };
};
