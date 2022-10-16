// 2021/03/30 9:44:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "Display/Screen/Grid.h"
#include "Display/Screen/TopPart.h"
#include "FPGA/FPGA.h"
#include <cmath>


namespace TopPart
{
    // ������� �������� �������� ��������� ���������.
    static void WriteCursors();

    static void DrawHiRightPart();
}


void TopPart::Draw()
{
    WriteCursors();
    DrawHiRightPart();
}


void TopPart::WriteCursors()
{
    int startX = 43;

    if (set.memory.mode_work.IsDirect())
    {
        startX += 29;
    }

    if (Cursors::NecessaryDraw())
    {
        int x = startX;
        int y1 = 0;
        int y2 = 9;

        VLine().Draw(x, 1, Grid::Top() - 2, Color::FILL);
        x += 3;
        Chan::E source = CURS_SOURCE;

        Color colorText = Chan(source).GetColor();

        if (!CURS_U_CONTROL_IS_DISABLED(source))
        {
            Text("1:").Draw(x, y1, colorText);
            Text("2:").Draw(x, y2);
            x += 7;
            Cursors::GetVoltage(source, 0).ToText().Draw(x, y1);
            Cursors::GetVoltage(source, 1).ToText().Draw(x, y2);
            x = startX + 49;

            float pos0 = Cursors::GetVoltage
                (Cursors::GetPosU(source, 0), RANGE(source), RSHIFT(source));

            float pos1 = Cursors::GetVoltage
                (Cursors::GetPosU(source, 1), RANGE(source), RSHIFT(source));

            float delta = std::fabsf(pos1 - pos0);
            Text(":dU=").Draw(x, y1);
            Voltage(delta).ToText(false).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            Cursors::GetPercentsU(source).Draw(x + 10, y2);
        }

        x = startX + 101;
        VLine().Draw(x, 1, Grid::Top() - 2, Color::FILL);
        x += 3;

        if(!CURS_T_CONTROL_IS_DISABLED(source))
        {
            colorText.SetAsCurrent();
            Text("1:").Draw(x, y1);
            Text("2:").Draw(x, y2);
            x += 7;
            Cursors::GetTime(source, 0).ToText().Draw(x, y1);
            Cursors::GetTime(source, 1).ToText().Draw(x, y2);
            x = startX + 153;
            float pos0 = Cursors::GetTime(CURS_T_POS(source, 0), TBASE);
            float pos1 = Cursors::GetTime(CURS_T_POS(source, 1), TBASE);
            float delta = std::fabsf(pos1 - pos0);
            Text(":dT=").Draw(x, y1);
            Time(delta).ToText(false).Draw(x + 17, y1);
            Text(":").Draw(x, y2);
            Cursors::GetPercentsT(source).Draw(x + 8, y2);

            if (CURS_SHOW_FREQ)
            {
                int width = 65;
                int x0 = Grid::Right() - width;
                Rectangle(width, 12).Draw(x0, Grid::Top(), Color::FILL);
                Region(width - 2, 10).Fill(x0 + 1, Grid::Top() + 1, Color::BACK);
                Text("1/dT=").Draw(x0 + 1, Grid::Top() + 2, colorText);
                Text text = (std::fabsf(delta - 0.0F) < 1e-12f) ? Text("---.---") : Frequency(1.0F / delta).ToText();
                text.Draw(x0 + 25, Grid::Top() + 2);
            }
        }
    }
}


void TopPart::DrawHiRightPart()
{
    VLine line(18);
    line.Draw(278, 0, Color::FILL);
    line.Draw(300, 0);

    // �������������
    int y = 2;

    static const int xses[3] = { 280, 271, 251 };
    int x = xses[set.memory.mode_work];

    if (!set.memory.mode_work.IsLatest())
    {
        HLine().Draw(x, 1, Grid::Top() - 2, Color::FILL);

        x += 2;

        if (SyncPulse::is_fired)
        {
            Region(17, 13).Fill(x - 1, y + 1);
            Text(LANG_RU ? "��" : "Tr").Draw(x + 2, y + 3, Color::BACK);
        }
    }

    // ����� ������
    static pchar strings_[][2] =
    {
        {"���",     "MEAS"},
        {"����",    "LAST"},
        {"����",    "INT"}
    };

    if (!set.memory.mode_work.IsDirect())
    {
        x += 18;
        HLine().Draw(x, 1, Grid::Top() - 2, Color::FILL);
        x += 2;
        Text(LANG_RU ? "�����" : "mode").Draw(LANG_RU ? x : x + 3, -1);
        Text(strings_[set.memory.mode_work][LANG]).DrawInCenterRect(x + 1, 9, 25, 8);
    }
    else
    {
        x -= 9;
    }

    if (!set.memory.mode_work.IsLatest())
    {
        x += 27;
        HLine().Draw(x, 1, Grid::Top() - 2, Color::FILL);

        x += 2;
        y = 1;

        if (StateWorkFPGA::IsWork())
        {
            Char(Symbol::S8::PLAY).Draw4SymbolsInRect(x, 1);
        }
        else if (StateWorkFPGA::IsStop())
        {
            Region(10, 10).Fill(x + 3, y + 3);
        }
        else if (StateWorkFPGA::IsWait())
        {
            int w = 4;
            int h = 14;
            int delta = 4;
            x = x + 2;
            Region(w, h).Fill(x, y + 1);
            Region(w, h).Fill(x + w + delta, y + 1);
        }
    }
}
