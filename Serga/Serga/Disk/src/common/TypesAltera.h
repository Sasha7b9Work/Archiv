//  [9/24/2009 Sasha]
#pragma once

namespace NS_TypesAltera 
{

    static const int RShiftMin = 0;
    static const int RShiftMax = 4000;
    static const int RShiftZero = 2000;
    static const int TrigLevMin = 0;
    static const int TrigLevMax = 4000;
    static const int TrigLevZero = 2000;

    static const int TShiftMin = 0;
    static const int TShiftMax = 0x3fff;
    static const int TShiftZero = 500;

    static const int DB_LENGTH = 512;           // ������ ������ ������������ � ������ ��� ������ ������
    static const int DB_SIGNED_LENGTH = 501;    // ���������� �����, ��������� �� �����
    static const int MIN_VALUE = 2;             // ����� �������� ������������� ����������� ����� �� �������
    static const int AVE_VALUE = 127;           // ����� �������� ������������� �������� 0.0� �� ������
    static const int MAX_VALUE = 252;           // ����� �������� ������������� ������������ ����� �� ������

    enum Range 
    {
        Range_5mv = 0, Range_10mv = 1, Range_20mv = 2, Range_50mv = 3, Range_100mv = 4, 
        Range_200mv = 5, Range_500mv = 6, Range_1v = 7, Range_2v = 8, Range_5v = 9, 
        RangeSize
    };

    enum TBase 
    {
        TBase_10ns = 0, TBase_20ns = 1, TBase_50ns = 2, TBase_100ns = 3, TBase_200ns = 4,
        TBase_500ns = 5, TBase_1us = 6, TBase_2us = 7, TBase_5us = 8,
        TBase_10us = 9, TBase_20us = 0x0a, TBase_50us = 0x0b,TBase_100us = 0x0c,
        TBase_200us = 0x0d, TBase_500us = 0x0e, TBase_1ms = 0x0f, TBase_2ms = 0x10,
        TBase_5ms = 0x11, TBase_10ms = 0x12, TBase_20ms = 0x13, TBase_50ms = 0x14,
        TBase_100ms = 0x15, TBase_200ms = 0x16, TBase_500ms = 0x17, TBase_1s = 0x18,
        TBase_2s = 0x19, TBaseSize
    };

    enum FBase  // ��� ������ ������������������
    {
        FBase_250Hz = 0,
        FBase_1250Hz = 1,
        FBase_2500Hz = 2,
        FBase_12500Hz = 3,
        FBase_25kHz = 4,
        FBase_125kHz = 5,
        FBase_250kHz = 6,
        FBase_1250kHz = 7,
        FBase_2500kHz = 8,
        FBase_12500kHz = 9,
        FBase_25MHz = 10,
        FBaseSize
    };

    // ��� ������������� - �� ������ ��� �����
    enum Sinchro 
    {
        Sinchro_Front, 
        Sinchro_Back
    };

    // ����� ������� - ������, �������������� ��� �����������
    enum StartMode 
    {
        StartMode_Auto, 
        StartMode_Wait, 
        StartMode_Single
    };

    // ����� ������ ��� - ������������, �������� ����� ��� ����������
    enum ModeADC 
    {
        ModeADC_Randomize, 
        ModeADC_RealTime,
        ModeADC_PointForPoint
    };

    enum TPos 
    {
        TPos_Left = 0, 
        TPos_Center = 1, 
        TPos_Right = 2
    };

    enum ModeInput
    {
        ModeAC,
        ModeDC,
        ModeGRND
    };

    enum ModeDevice
    {
        ModeOscilloscope = 0,
        ModeVoltmeter = 1,
        ModeAnalyzer = 2,
        ModeRegistrator = 3,
        NO_DEVICE
    };
}