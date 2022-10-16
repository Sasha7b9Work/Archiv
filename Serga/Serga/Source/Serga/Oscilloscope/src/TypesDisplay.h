//  [10/24/2009 Sasha]
#pragma once 

namespace NS_TypesDisplay {

    // ����� ��� ����� �������
    static const int SCR_W = 501;
    static const int SCR_W_TR_LEV = 5;
    static const int SCR_W_RSHIFT = 5;
    static const int SCR_W_TSHIFT = SCR_W + SCR_W_TR_LEV + SCR_W_RSHIFT;
    static const int SCR_H_TSHIFT = 5;

    // ��� ������ ����������� ������ ��������
    static const int SCR_BG_H = 501;                       // ������ ������
    static const int SCR_BG_H_RSHIFT = SCR_BG_H;     // ������ ���� ��� ��������� �������� �� ����������
    static const int SCR_BG_H_TR_LEV = SCR_BG_H; // ������ ���� ��� ��������� ������ �������������

    // ��� ������ ����������� ������ ��������
    static const int SCR_SM_H = 251;
    static const int SCR_SM_H_RSHIFT = SCR_SM_H;
    static const int SCR_SM_H_TR_LEV = SCR_SM_H;
    static int DISPLAY_W = 2 + 2 + SCR_W_TR_LEV + SCR_W_RSHIFT + SCR_W;
    static int DISPLAY_BG_H = 2 + 2 + SCR_H_TSHIFT + SCR_BG_H;
    static int DISPLAY_SM_H = 2 + 2 + SCR_H_TSHIFT + 2 * SCR_SM_H;

    static const int RShiftMinScreen = 1000;
    static const int RShiftMaxScreen = 3000;

    enum TypeDisplay {
        TypeDisplay_Together,        // �������� �������, �������������� ������� � ������ ������
        TypeDisplay_Separetly,       // �������� ������� � ������� ������, � �������������� ������� � ������ � ������
    };

    enum TypeGrid {
        TypeGrid_Base,	
        TypeGrid_Simple,			
        TypeGrid_None		
    };

    // ��� �������� ������ - ������� ��� �������
    enum DrawMode {
        DrawMode_Points,
        DrawMode_Lines
    };

    // � ����� ���� �������� ������� - 
    enum ModeAccumulation {
        ModeAccumulation_Disable,
        ModeAccumulation_NoLimitation,  // �� ������������ ����������
        ModeAccumulation_Limitation     // ������������ ����������
    };
}