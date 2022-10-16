// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Painter/Text_.h"


struct TypeMeasure
{
    enum E
    {
        None,
        VoltageMax,
        VoltageMin,
        VoltagePic,
        VoltageMaxSteady,
        VoltageMinSteady,
        VoltageAmpl,
        VoltageAverage,
        VoltageRMS,
        VoltageVybrosPlus,
        VoltageVybrosMinus,
        Period,
        Freq,
        TimeNarastaniya,
        TimeSpada,
        DurationPlus,
        DurationMinus,
        SkvaznostPlus,
        SkvaznostMinus,
        DelayPlus,
        DelayMinus,
        PhazaPlus,
        PhazaMinus,
        Count
    };

    E value;

    TypeMeasure(E v = None) : value(v) {}

    operator E() { return value; }

    bool IsNone() const { return value == None; }

    bool Is(E v) const { return value == v; }
};


TypeMeasure::E &operator++(TypeMeasure::E &measure);


// ���� ���������.
namespace TableAutoMeasures
{ 
    char GetChar(TypeMeasure::E type);

    bool IsActive(int row, int col);

    void GetActive(int *row, int *col);

    void SetActive(int row, int col);

    int GetDY();

    int GetDX();

    Text Name(int row, int col);

    TypeMeasure::E Type(int row, int col);

    int NumRows();

    int NumCols();
    
    int GetTopTable();

    int GetDeltaGridLeft();

    int GetDeltaGridBottom();

    namespace Draw
    {
        // ���������� �������� ������ ���������.
        void PageChoice();

        // ������� �������� �������������� ���������.
        void All();
    }

    namespace Handler
    {
        // ������� ����� ���������
        void RegSet(int angle);

        // ���������� ��� �������� ������� �� ������ ������ ���������.
        void ButtonSettings();

        // ������� ������ "������"
        void OnButonMarker();
    }
}
