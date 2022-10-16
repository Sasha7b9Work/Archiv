// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once 


// ���������� ���� ��� ��� ����������� ����� ������.
namespace FM
{
    void Init();
    
    void Draw();
    
    void PressLevelUp();
    
    void PressLevelDown();
    
    void RotateRegSet(int angle);
    
    bool GetNameForNewFile(char name[255]);

    void PressTab();

    extern uint needRedrawFileManager;  // ���� 1, �� ����-�������� ��������� � ������ �����������
                                        // ���� 2, �� ������������ ������ ��������
                                        // ���� 3, �� ������������ ������ �����
}
