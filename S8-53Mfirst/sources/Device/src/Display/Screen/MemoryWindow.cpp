// 2021/03/30 15:51:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Utils/Math_.h"
#include "common/Utils/Containers/Buffer_.h"
#include "Data/DataSettings.h"
#include "Data/InputStorage/InputStorage.h"
#include "Display/Screen/Grid.h"
#include "Display/Screen/MemoryWindow.h"
#include "FPGA/TypesFPGA.h"
#include "Settings/Settings.h"


namespace MemoryWindow
{
    static void DrawDataInRectangle(int width, const Chan &ch, ReadingFrame &data);

    static void DrawTPos(int left_x, int right_x);

    static void DrawTShift(int left_x, int right_x);

    // Возвращает точку в экранной координате. Если точка не считана (Value::NONE), возвращает -1.
    static int Ordinate(uint8 x, float scale);

    static void DrawDataInRectangle(const Chan &ch, int x, const int *min, const int *max, int width);

    static void Calculate(Buffer<uint8> &min, Buffer<uint8> &max, int bytes_in_channel, const uint8 *data);
}


void MemoryWindow::Draw(ReadingFrame &data)
{
    static const int rightXses[3] = { 276, 285, 247 };

    int right = rightXses[set.memory.mode_work];

    if (Cursors::NecessaryDraw())
    {
        right = 68;
    }

    DrawDataInRectangle(right + 2, Chan::FirstDrawing(), data);
    DrawDataInRectangle(right + 2, Chan::SecondDrawing(), data);

    int left = 3;

    float scale_x = (float)(right - left + 1) / EnumPointsFPGA::PointsInChannel();
    const int x_rect = left + (int)(SHIFT_IN_MEMORY_IN_POINTS * scale_x);

    int width = (int)((right - left) * (282.0f / EnumPointsFPGA::PointsInChannel()));

    Rectangle(width, Grid::Top() - 2).Draw(x_rect, 0, Color::FILL);

    DrawTPos(left, right);

    DrawTShift(left, right);
}


void MemoryWindow::DrawDataInRectangle(int width, const Chan &ch, ReadingFrame &data_channel)
{
    DataSettings &ds = data_channel.Settings();

    if (!ds.enabled[ch])
    {
        return;
    }

    Buffer<uint8> buffer_min(width);
    Buffer<uint8> buffer_max(width);

    Buffer<int> buffer_mines(width);
    Buffer<int> buffer_maxes(width);

    Calculate(buffer_min, buffer_max, ds.BytesInChannel(), data_channel.Data(ch));

    int *mines = buffer_mines.Data();   // Массив для максимальных значений в каждом столбике
    int *maxes = buffer_maxes.Data();   // Массив для минимальных значений в каждом столбике

    float scale = 17.0f / (ValueFPGA::MAX - ValueFPGA::MIN);

    uint8 *min = buffer_min.DataU8();
    uint8 *max = buffer_max.DataU8();

    mines[0] = Ordinate(max[0], scale);
    maxes[0] = Ordinate(min[0], scale);

    int counter = 0;

    for (int i = 1; i < width; i++)
    {
        maxes[i] = Ordinate((uint8)((max[i] < min[i - 1]) ? min[i - 1] : max[i]), scale);
        mines[i] = Ordinate((uint8)((min[i] > max[i - 1]) ? max[i - 1] : min[i]), scale);

        if (mines[i] < 0)
        {
            counter++;
//            LOG_WRITE("mines[i] = %d", mines[i]);
        }

        if (maxes[i] < 0)
        {
            counter++;
//            LOG_WRITE("maxes[i] = %d", maxes[i]);
        }
    }

    if (counter != 0)
    {
        LOG_WRITE("%d неправильных значений", counter);
    }

    // Теперь определим количество точек, которые нужно нарисовать
    int numPoints = 0;
    for (int i = 0; i < width; ++i)
    {
        if (maxes[i] != -1 && mines[i] != -1) { numPoints++; }
    }

    numPoints--;

    // Теперь определим, с какой позиции выводить точки (если сигнал сжат по горизонтали, то вначале будет пустое место
    int x = 1;
    for (int i = 0; i < width; ++i)
    {
        if (maxes[i] == -1 && mines[i] == -1) { ++x; }
        else { break; }
    }

    if (numPoints > 1)
    {
        DrawDataInRectangle(ch, x, mines + x, maxes + x, numPoints);
    }
}


void MemoryWindow::Calculate(Buffer<uint8> &buffer_min, Buffer<uint8> &buffer_max, int bytes_in_channel, const uint8 *data)
{
    uint8 *min = buffer_min.Data();
    uint8 *max = buffer_max.Data();

    int width = buffer_max.Size();

    float elems_in_column = (float)bytes_in_channel / width;

    if (PEAK_DET_IS_ENABLED)                                 // Если пик. дет. включен
    {
        uint8 *mini = &min[0];
        uint8 *maxi = &max[0];

        for (int col = 0; col < width; col++, mini++, maxi++)
        {
            uint first = (uint)(col * elems_in_column);
            uint last = (uint)(first + elems_in_column - 1);
            *mini = data[first];
            *maxi = data[first];

            for (uint elem = first + 1; elem <= last; elem++)
            {
                uint8 d = data[elem];
                if (d < *mini) { *mini = d; }
                if (d > *maxi) { *maxi = d; }
            }
        }
    }
    else                                                                // Если пик. дет. выключён
    {
        for (int col = 0; col < width; col++)
        {
            uint first = (uint)(col * elems_in_column);
            uint last = (uint)(first + elems_in_column - 1);
            min[col] = data[first];
            max[col] = data[first];

            for (uint elem = first + 1; elem <= last; elem++)
            {
                uint8 d = data[elem];
                if (d < min[col]) { min[col] = d; }
                if (d > max[col]) { max[col] = d; }
            }
        }
    }
}


void MemoryWindow::DrawTPos(int leftX, int rightX)
{
    int x[] = { leftX, (rightX - leftX) / 2 + leftX, rightX };
    int x0 = x[TPOS];

    TPos::Draw(x0, 10);
}


void MemoryWindow::DrawTShift(int left, int right)
{
    EnumPointsFPGA::E enum_points = ENUM_POINTS_FPGA;
    int num_points = EnumPointsFPGA::PointsInChannel();

    float scale = (float)(right - left + 1) / num_points;

    int x = (int)(1.5f + (TPos::InPoints(enum_points, TPOS) - TSHIFT) * scale) - 1;

    if (PEAK_DET_IS_ENABLED && TPOS_IS_RIGHT)
    {
        --x;
    }

    if (enum_points == EnumPointsFPGA::_512)
    {
        ++x;                           /// \todo Костыль
    }

    Math::LimitAbove(&x, right - 2);

    int x_rect = Math::Limitation(x, 1, right);

    Rectangle(7, 6).Draw(x_rect - 1, 1, Color::BACK);
    Region(5, 5).Fill(x_rect, 1, Color::FILL);

    if (x < left - 3)
    {
        VLine(3).Draw(x_rect + 3, 2, Color::BACK);
        HLine(2).Draw(x_rect + 1, 3);
    }
    else if (x > right - 1)
    {
        VLine(3).Draw(x_rect + 1, 2, Color::BACK);
        HLine(2).Draw(x_rect + 2, 3);
    }
    else
    {
        HLine(3).Draw(x_rect + 1, 2, Color::BACK);
        VLine(2).Draw(x_rect + 2, 3);
    }
}


int MemoryWindow::Ordinate(uint8 x, float scale)
{
    Math::Limitation<uint8>((uint8)(x - ValueFPGA::MIN), 0, (ValueFPGA::MAX - ValueFPGA::MIN));

    return Math::Limitation<int>((int)((17.0f - scale * x) + 0.5f), 0, 16);
}


void MemoryWindow::DrawDataInRectangle(const Chan &ch, int x, const int *min, const int *max, int width)
{
    Buffer<uint8> buffer(width * 2);

    uint8 *points = buffer.DataU8();

    for (int i = 0; i < width; i++)
    {
        points[i * 2] = (uint8)max[i];
        points[i * 2 + 1] = (uint8)(min[i] < 0 ? 0 : min[i]);
    }

    VLineArray().Draw(x, (int)width, points, ch.GetColor());
}
