#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PI 3.141592653589793f

#define LIMIT_BELOW(x, min)                 if((x) < (min)) { x = (min); }

#define LIMIT_ABOVE(x, max)                 if((x) > (max)) { x = (max); }

#define SET_MIN_IF_LESS(x, min)             if((x) < (min)) { (min) = (x); }

#define SET_MAX_IF_LARGER(x, max)           if((x) > (max)) { (max) = (x); }

#define LIMITATION_BOUND(var, min, max)     if((var) < (min)) { (min) = (var); }; if((var) > (max)) { (max) = (var); };
/// \todo ��� ��������� ����������� �������� � �������������� �������
#define ROUND(type, x) ((type)(x + 0.5f))

#define SET_IF_LARGER(x, max, newValue)     if((x) > (max)) { (x) = (newValue); }

#define LIMITATION_ABOVE(var, value, max)   var = (value); if((var) > (max)) { (var) = (max); }

#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); };

#define IN_RANGE(x, min, max)               ((x) >= (min) && (x) <= (max))


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Math
{
public:
    /// ���������� ����� �������� ����, ������� �������
    static int LowSignedBit(uint value);
    /// \brief ���������� ��� �����. ���������� true, ���� ����� ���������� ���� �� ����� �� �����, ��� �� epsilonPart. ��� ���� ��� ������� epsilonPart 
    /// ������������ ������� � ������ ������ �����.
    static bool FloatsIsEquals(float value0, float value1, float epsilonPart);

    static bool IsEquals(float x, float y);

    static float RandFloat(float min, float max);

    static void Smoothing(uint8 *data, int numPoints, int numSmooth);
    
    static int MinFrom2Int(int val0, int val1);

    static uint8 MaxFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint);

    static uint8 MinFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint);

    static uint8 MaxFromArray(const uint8 *data, int firstPoint, int lastPoint);

    static uint8 MinFromArray(const uint8 *data, int firstPoint, int lastPoint);

    static uint8 MaxFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);

    static uint8 MinFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);
    /// \brief ���������� ���������� x ����������� �����, ���������� ����� (x0, y0), (x1, y1), � �������������� ������, ���������� ����� ����� � 
    /// ��������� yHorLine.
    static float GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);
    
    static void CalculateMathFunction(float *dataAandResult, float *dataB, int numPoints);

    static uint8 CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing);

    static void CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing);
    /// ����� ������ ������� �������, �� �������������� ���������
    static int FindAnotherElement(uint8 *data, uint8 value, int numElements);

    static int DigitsInIntPart(float value);
    /// \brief ��������� ����� � ��������� ������. numDigits - ������ ����� ������, �� ������� ������������ ����������.
    /// ��������, RoundFloat(12.37137, 4) �������� �� 12.40)
    static float RoundFloat(float value, int numDigits);
    /// ���������� 10**pow.
    static int Pow10(int pow);
    /// ���������� ������� ���������� ������ �� ������� value0 � value1
    template<class T> static void Swap(T *value0, T *value1);
    /// ��������� ���������� value0 � value1 � ������� �����������
    template<class T> static void Sort(T *value0, T *value1);

    template<class T> static int  Sign(T x);

    template<class T> static T    Abs(T x);

    template<class T> static T    Min(T x1, T x2);

    template<class T> static T    Max(T x1, T x2);
    /// ���������� ������������ �������� �� ���
    template<class T> static T    Max(T val1, T val2, T val3);

    template<class T> static void CircleIncrease(T *value, T min, T max);

    template<class T> static void CircleDecrease(T *value, T min, T max);
    /// ���������������� *value, �� �� ������, ��� max
    template<class T> static void LimitationIncrease(T *value, T max);
    /// ���������������� *value, �� �� ������, ��� min
    template<class T> static void LimitationDecrease(T *value, T min);

    template<class T> static void Limitation(T *value, T min, T max);

    template<class T> static T    LimitationRet(T value, T min, T max);
    /// ��������� � *value term � ����������, ���� ��������� ������� �� ������� [min, max]
    template<class T> static void AdditionThisLimitation(T *value, int term, T min, T max);
};

#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))


#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)
