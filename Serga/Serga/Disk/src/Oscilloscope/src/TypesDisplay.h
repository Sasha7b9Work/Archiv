//  [10/24/2009 Sasha]
#pragma once 

namespace NS_TypesDisplay {

    // Общие для обоих режимов
    static const int SCR_W = 501;
    static const int SCR_W_TR_LEV = 5;
    static const int SCR_W_RSHIFT = 5;
    static const int SCR_W_TSHIFT = SCR_W + SCR_W_TR_LEV + SCR_W_RSHIFT;
    static const int SCR_H_TSHIFT = 5;

    // Для режима совместного вывода сигналов
    static const int SCR_BG_H = 501;                       // Высота экрана
    static const int SCR_BG_H_RSHIFT = SCR_BG_H;     // Высота окна для рисования смещения по напряжению
    static const int SCR_BG_H_TR_LEV = SCR_BG_H; // Высота окна для рисования уровня синхронизации

    // Для режима раздельного вывода сигналов
    static const int SCR_SM_H = 251;
    static const int SCR_SM_H_RSHIFT = SCR_SM_H;
    static const int SCR_SM_H_TR_LEV = SCR_SM_H;
    static int DISPLAY_W = 2 + 2 + SCR_W_TR_LEV + SCR_W_RSHIFT + SCR_W;
    static int DISPLAY_BG_H = 2 + 2 + SCR_H_TSHIFT + SCR_BG_H;
    static int DISPLAY_SM_H = 2 + 2 + SCR_H_TSHIFT + 2 * SCR_SM_H;

    static const int RShiftMinScreen = 1000;
    static const int RShiftMaxScreen = 3000;

    enum TypeDisplay {
        TypeDisplay_Together,        // Выводить сигналы, математическую функцию и спектр вместе
        TypeDisplay_Separetly,       // Выводить сигналы в верхнем экрана, а математическую функцию и спектр в нижнем
    };

    enum TypeGrid {
        TypeGrid_Base,	
        TypeGrid_Simple,			
        TypeGrid_None		
    };

    // Чем выводить сигнал - линиями или точками
    enum DrawMode {
        DrawMode_Points,
        DrawMode_Lines
    };

    // В какам виде выводить сигналы - 
    enum ModeAccumulation {
        ModeAccumulation_Disable,
        ModeAccumulation_NoLimitation,  // Не ограничивать накопления
        ModeAccumulation_Limitation     // Ограничивать накопления
    };
}