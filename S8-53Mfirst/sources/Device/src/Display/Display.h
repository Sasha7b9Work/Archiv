// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tables.h"
#include "common/Display/Colors_.h"


namespace Display
{
    struct DrawMode { enum E {
        Default,        // ����������� ��������� � �����
        Timer           // ��������� �� ������� �������� �������
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

    // ������ ������ row � ����� pixels
    void ReadRow(int row, uint8 pixels[320]);

    namespace Message
    {
        void Show(pchar textRu, pchar textEn);

        void Hide();
    }

    // ����� ����� �������� ������ � ���������
    namespace Sender
    {
        void SendPalette();

        void SendFrame();

        void Update();
    };
};
