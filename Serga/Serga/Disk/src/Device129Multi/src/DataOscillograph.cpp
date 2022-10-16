#include "DataOscillograph.h"
#include "../../common/TypesAltera.h"
#include "../../common/GeometryMath.h"

using namespace NS_TypesAltera;
using namespace NS_GeometryMath;

const double DataOscillograph::FRACTION_FOR_ADDITION_DATA = 0.5;

DataOscillograph::DataOscillograph(int /*_n*/):
    AbstractData(), 
    mathFunction(false),
    rangeMath(0)
{
    Clear();
}

void DataOscillograph::Clear() 
{
    for(int i = 0; i < NS_TypesAltera::DB_LENGTH; i++) 
    {
        readPoint[i] = false;
        drawPoints[i] = false;
        data[0][i] = AVE_VALUE;
        data[1][i] = AVE_VALUE;
    }
}

void DataOscillograph::AdditionData() 
{
    {
        AdditionData(0);
        AdditionData(1);
        /*
        AdditionInterpolationData(0);
        AdditionInterpolationData(1);
        */
        
    }
}

void DataOscillograph::AdditionInterpolationData(int _channel)
{
    double *data = dataAbs[_channel];
    for(int i = 0; i < DB_SIGNED_LENGTH; i++)
    {
        if(readPoint[i])    // ���� ����� �������, ������������ ��������� �� ����
            continue;

        data[i] = 0.0;
        for(int point = 0; point < DB_SIGNED_LENGTH; point++)
        {
            if(readPoint[point] && point != i)
            {
                double s = data[point];
                double x = 2 * asin(1.0) * 2.0 * 0.5 * (i - point);
                double fi = sin(x) / x;
                if(x != 0)
                    data[i] += s * fi;
            }
        }
    }
}

void DataOscillograph::AdditionData(int _channel) 
{
    int startX = 0;
    int endX = 0;

    // �������� ������ �� ������� ����� �� ������ �����, ���������� �� �������������
    while(drawPoints[startX] == false) 
        startX++;
    
    // ���� ������� ������ �����, ���������� �� �������������, �� �������
    if(startX > 0) 
    {
        // ������� ������� ������ �����, ���������� �� �������������
        endX = startX + 1;
        while(drawPoints[endX] == 0) 
            endX++;
       
        // ������ �������� ����� ����� ������ ��� ����� �������������. �� ��� ����� ������� ����������� ����� �� 0-� �������
        Line line(startX, dataAbs[_channel][startX], endX, dataAbs[_channel][endX]);
        for(int pos = 0; pos < endX; pos++) 
        {
            Line vertLine(pos, 1.0, pos, 2.0);
            dataAbs[_channel][pos] = line.GetIntersection(vertLine).y;
        }
        startX = endX;
    }

    // ������� ������ �������� �����, ���������� �� �������������
#pragma warning(disable:4127)
    while(true) 
    {
#pragma warning(default:4127)
        while(drawPoints[startX] == 0) 
        {
            startX++;
            if(startX > DB_SIGNED_LENGTH - 1) 
                break;
        }
        if(startX > DB_SIGNED_LENGTH - 1) 
            break;
        
        endX = startX + 1;
        while(drawPoints[endX] == 0) 
        {
            endX++;
            if(endX > DB_SIGNED_LENGTH - 1) 
                break;
        }
        if(endX > DB_SIGNED_LENGTH - 1) 
            break;
        
        if(endX == startX + 1) 
        {
            startX = endX;
            continue;
        }

        Line line(startX, dataAbs[_channel][startX], endX, dataAbs[_channel][endX]);
        for(int pos = startX + 1; pos < endX; pos++) 
        {
            Line vertLine(pos, 1.0, pos, 2.0);
            dataAbs[_channel][pos] = line.GetIntersection(vertLine).y;
        }
        startX = endX;
    }

    // ������ �������� ��������� �����, ���� ��������� �����, ���������� �� �������������, ����� �������, ������� DB_SIGNED_LENGTH - 1
    endX = DB_SIGNED_LENGTH - 1;
    while(drawPoints[endX] == 0) 
        endX--;

    if(endX < (DB_SIGNED_LENGTH - 1)) 
    {
        startX = endX - 1;
        while(drawPoints[startX] == 0) 
            startX--;
        
        Line line(startX, dataAbs[_channel][startX], endX, dataAbs[_channel][endX]);
        for(int pos = endX + 1; pos < DB_SIGNED_LENGTH; pos++) 
        {
            Line vertLine(pos, 1.0, pos, 2.0);
            if((pos >= endX + 1) && (pos < DB_SIGNED_LENGTH) && pos >= 0)
                dataAbs[_channel][pos] = line.GetIntersection(vertLine).y;
        }
    }
}

DataOscillograph& DataOscillograph::operator /(int n) 
{
    for(int i = 0; i < DB_LENGTH; i++) 
    {
        dataAbs[0][i] /= n;
        dataAbs[1][i] /= n;
        channelMath[i] /= n;
        channelDraw[0][i] /= n;
        channelDraw[1][i] /= n;
    }
    return *this;
}

DataOscillograph& DataOscillograph::operator +=(DataOscillograph &rVal) 
{
    if(&rVal != this) 
    {
        for(int i = 0; i < DB_LENGTH; i++) 
        {
            dataAbs[0][i] += rVal.dataAbs[0][i];
            dataAbs[1][i] += rVal.dataAbs[1][i];
            //channelMath[i] += rVal.channelMath[i];
            //channelDraw[0][i] += rVal.channelDraw[0][i];
            //channelDraw[1][i] += rVal.channelDraw[1][i];
            //readyRand[i] += rVal.readyRand[i];
        }
    }
    return *this;
}