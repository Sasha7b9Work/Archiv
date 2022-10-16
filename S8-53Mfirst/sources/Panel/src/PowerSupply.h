// 2021/05/05 15:03:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace PowerSupply
{
    // ���������� true ������ � ��� ������, ���� � ������ ������ ��������� ���������
    bool AttemptToTurnOn(const KeyboardEvent &event);

    void TurnOff();

    bool IsEnabled();
};
