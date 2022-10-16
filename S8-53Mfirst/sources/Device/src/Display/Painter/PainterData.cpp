// 2021/03/17 13:27:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Utils/Math_.h"
#include "Data/DataSettings.h"
#include "Display/Painter/PainterData.h"
#include "Data/ProcessingData.h"
#include "Display/Screen/MemoryWindow.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "common/Hardware/Timer_.h"
#include "Data/InputStorage/InputStorage.h"
#include <cstdlib>
#include <cstring>


namespace PainterData
{
    static void DrawSpectrum(DrawingFrame *);
    static void DrawSpectrum(const uint8 *dataIn, int numPoints, const Chan &ch);
    static void DrawSpectrumChannel(const float *spectrum, const Color &color);
    static void WriteParametersFFT(const Chan &, float freq0, float density0, float freq1, float density1);
    static void DrawCursor(int left, int pos, int top, int down, int y, int s);
}


void PainterData::DrawData()
{
#ifndef GUI

    DrawingFrame frame;

    InputStorage::ExtractLastFrame(frame.data);

    Processing::Process(frame.data);

    frame.PrepareForDrawing();

    frame.Draw();

    MemoryWindow::Draw(frame.data);

    DrawSpectrum(&frame);

#endif

    TableAutoMeasures::Draw::All();
}


DrawingFrame::DrawingFrame(const Buffer<uint8> &buffer)
{
    points[ChA] = buffer;
}


void DrawingFrame::PrepareForDrawing()
{
    PrepareChannel(ChA);
    PrepareChannel(ChB);
}


void DrawingFrame::Draw()
{
    DrawChannel(ChA);
    DrawChannel(ChB);
}


void DrawingFrame::PrepareChannel(const Chan &ch)
{
    points[ch].Free();

    if (!data.Settings().enabled[ch])
    {
        return;
    }

    int min_y = Grid::Top();
    int max_y = Grid::Channel::Bottom() - 1;
    float scale_y = (float)(max_y - min_y) / (ValueFPGA::MAX - ValueFPGA::MIN);

    int num_points = data.Settings().enabled_peak_det ? (281 * 2) : 281;

    points[ch].Realloc(num_points);

    BitSet64 p = SettingsDisplay::BytesOnDisplay();

    uint8 *in = &data.Data(ch)[p.first];
    uint8 *out = &points[ch][0];

    for (int i = p.second - p.first; i >= 0; i--, out++, in++)
    {
        int value = (int)(max_y - (*in - ValueFPGA::MIN) * scale_y);

        if (value < min_y)      *out = (uint8)min_y;
        else if (value > max_y) *out = (uint8)max_y;
        else                    *out = (uint8)value;
    }
}


void DrawingFrame::DrawChannel(const Chan &ch)
{
    if (!data.Settings().enabled[ch])
    {
        return;
    }

    if (points[ch].Size() == 0)
    {
        return;
    }

    ch.GetColor().SetAsCurrent();

    if (MODE_DRAW_SIGNAL_IS_LINES)
    {
        DrawChannelLined(Grid::Left(), points[ch]);
    }
    else
    {
        DrawChannelPointed(Grid::Left(), points[ch]);
    }

    Processing::markersHor[ch].Draw();
    Processing::markersVert[ch].Draw();
}


void DrawingFrame::DrawChannelPointed(int x, Buffer<uint8> &buffer)
{
    uint8 *d = buffer.DataU8();
    int size = buffer.Size();

    if (data.Settings().enabled_peak_det)
    {
        for (int i = 0; i < size; i += 2)
        {
            Point().Draw(x, *d++);
            Point().Draw(x, *d++);

            x++;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            Point().Draw(x++, *d++);
        }
    }
}


void DrawingFrame::DrawChannelLined(int x, Buffer<uint8> &buffer)
{
    if (data.Settings().enabled_peak_det)
    {
        int size = buffer.Size();
        uint8 *d = buffer.DataU8();

        for (int i = 0; i < size; i += 2)
        {
            int x1 = *d;
            d++;
            int x2 = *d;
            d++;

            VLine().Draw(x++, x1, x2);
        }
    }
    else
    {
        int size = buffer.Size() - 1;

        for (int i = 0; i < size; i++)
        {
            uint8 current = buffer[i];
            uint8 next = buffer[i + 1];

            if (current > next) { VLine().Draw(x++, next + 1, current); }
            else if (current < next) { VLine().Draw(x++, next - 1, current); }
            else { Point().Draw(x++, next); }
        }

        Point().Draw(x, buffer[buffer.Size() - 1]);
    }
}


void PainterData::DrawSpectrum(DrawingFrame *data)
{
    if (!FFT_IS_ENABLED || InputStorage::NumFrames() == 0)
    {
        return;
    }

    if (set.memory.mode_work.IsDirect())
    {
        int numPoints = data->data.Settings().BytesInChannel();

        if (numPoints > 1024)       // \todo Пока 8к и более не хочет считать
        {
            numPoints = 1024;
        }

        if (FFT_SOURCE_IS_A)
        {
            DrawSpectrum(data->data.Data(ChA), numPoints, ChA);
        }
        else if (FFT_SOURCE_IS_B)
        {
            DrawSpectrum(data->data.Data(ChB), numPoints, ChB);
        }
        else
        {
            Chan &first = LAST_AFFECTED_CHANNEL_IS_A ? ChB : ChA;
            Chan &second = first.IsA() ? ChB : ChA;

            DrawSpectrum(data->data.Data(first), numPoints, first);
            DrawSpectrum(data->data.Data(second), numPoints, second);
        }
    }
}


void PainterData::DrawCursor(int left, int pos, int top, int down, int y, int s)
{
    VLine(down - y).Draw(left + pos, y, Color::GRID);
    DashedVLine(3, 1).Draw(left + pos, top, y, 0);
    Rectangle(s * 2 - 1, s * 2 - 1).Draw(pos + left - s + 1, y - s, Color::FILL);
}


void PainterData::DrawSpectrum(const uint8 *dataIn, int numPoints, const Chan &ch)
{
    if (!ENABLED(ch))
    {
        return;
    }

#undef SIZE_BUFFER
#define SIZE_BUFFER (1024 * 4)

    if (numPoints > SIZE_BUFFER)
    {
        numPoints = SIZE_BUFFER;
    }

    float dataR[SIZE_BUFFER];

    float freq0 = 0.0F;
    float freq1 = 0.0F;
    float density0 = 0.0F;
    float density1 = 0.0F;
    int y0 = 0;
    int y1 = 0;

    float* spectrum = static_cast<float*>(std::malloc(numPoints * sizeof(float)));

    if (spectrum)
    {
        int s = 2;

        ValueFPGA::ToVoltage(dataIn, (int)numPoints, RANGE(ch), RSHIFT(ch), dataR);

        MathFPGA::CalculateFFT(dataR, numPoints, spectrum, &freq0, &density0, &freq1, &density1, &y0, &y1);

        DrawSpectrumChannel(spectrum, ch.GetColor());

        WriteParametersFFT(ch, freq0, density0, freq1, density1);

        DrawCursor(Grid::Left(), FFT_POS_CUR_0, Grid::Channel::Bottom(), Grid::Math::Bottom(), y0, s);
        DrawCursor(Grid::Left(), FFT_POS_CUR_1, Grid::Channel::Bottom(), Grid::Math::Bottom(), y1, s);

        std::free(spectrum);
    }
}


void PainterData::DrawSpectrumChannel(const float *spectrum, const Color &color)
{
    color.SetAsCurrent();
    int gridLeft = Grid::Left();
    int gridBottom = Grid::Math::Bottom();
    int gridHeight = Grid::Math::Height();

    for (int i = 0; i < 256; i++)
    {
        int height = static_cast<int>(gridHeight * spectrum[i]); //-V2563 //-V2564

        VLine(height).Draw(gridLeft + i, gridBottom - height);
    }
}


void PainterData::WriteParametersFFT(const Chan &ch, float freq0, float density0, float freq1, float density1)
{
    if (MENU_SHOWN && Item::Opened() != PageService::PageMath::PageFFT::PageCursors::self)
    {
        return;
    }

    int x = Grid::Left() + 259;
    int y = Grid::Channel::Bottom() + 5;
    int dY = 10;

    Frequency(freq0).ToText().Draw(x, y, Color::FILL);

    y += dY;

    Frequency(freq1).ToText().Draw(x, y);

    y += dY + 2;

    if (ch.IsB())
    {
        y += dY * 2 + 2;
    }

    Text(FFT_SCALE_IS_LOG ? Decibel(density0).ToText(4) : Float(density0).ToText(false, 4)).
        Draw(x, y, ch.GetColor());

    y += dY;

    Text(FFT_SCALE_IS_LOG ? Decibel(density1).ToText(4) :  Float(density1).ToText(false, 4)).Draw(x, y);
}
