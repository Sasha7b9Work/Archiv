// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once 


// Вызывается один раз при обнаружении новой флешки.
namespace FM
{
    void Init();
    
    void Draw();
    
    void PressLevelUp();
    
    void PressLevelDown();
    
    void RotateRegSet(int angle);
    
    bool GetNameForNewFile(char name[255]);

    void PressTab();

    extern uint needRedrawFileManager;  // Если 1, то файл-менеджер нуждается в полной перерисовке
                                        // Если 2, то перерисовать только каталоги
                                        // Если 3, то перерисовать только файлы
}
