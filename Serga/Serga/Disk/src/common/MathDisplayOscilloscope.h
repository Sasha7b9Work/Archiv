//  [6/24/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QString>
#pragma warning(default:4127)
#include <map>

class MathDisplayOscilloscope {
public:
    MathDisplayOscilloscope();
    ~MathDisplayOscilloscope();

    // Возвращает строку масштаба по напряжению
    QString GetRangeString(int range);

    // Возвращает строку масштаба по времени
    QString GetTBaseString(int tBase);

    // Возвращает значение напряжение, соответствующее верхней границе экрана для данного масштаба по напряжению. Нижней границе соответствует -GetMaxVoltage();
    double GetMaxVoltage(int range);

    // Возвращате значение времени, соответствующее правой границе экрана для данного масштаба по времени. Левой границе соответствует -GetMaxTime(). Само собой,
    // возвращаемое значение вычисляется в предположении, что TPos == center и tShift == 0.
    double GetMaxTime(int tBase);

    // Возвращает абсолютное значение смещения по напряжению в текстовом виде в готовом для вывода виде
    QString GetRShiftAbs(const int rShift, const int range);

    // Возвращает относительное значение смещения по напряжению для засылки в прибор после
    // изменения значения масштаба по напряжению
    int GetRShiftRel(const int rShiftPrev, const int prevRange, const int currentRange);

    // Возвращает абсолютное значение смещения по времени в текстовом виде в готовом для вывода виде
    QString GetTShiftAbs(const int tShift, const int tBase);

    // Возвращает относительное значение смещения по времени для засылки в прибор после
    // изменения значения масштаба по времени
    int GetTShiftRel(const int tShiftPrev, const int prevTBase, const int currentTBase);

    /*
    Курсоры
    */

    // Возвращает позицию курсора напряжения в текстовом виде
    QString GetPosUCur(int pos, int range);

    // Возвращает абсолютное значение курсора напряжения без учёта смещения
    double GetPosUCurAbs(int pos, int range);

    // Возвращает относительное значение курсора напряжения по абсолютному
    int GetPosUCurRel(double pos, int range);

    // Возвращает разность между позициями курсоров в текстовом виде
    QString GetDeltaU(int pos1, int pos2, int range);

    // Возвращает позицию курсора после изменения масштаба по напряжению
    int GetPosUCurNew(int prevPos, int prevRange, int currentRange);

    // Возвращает позицию курсора времени в абсолютном виде
    //QString GetPosTCur(int pos, int tBase, int tShift, TPos tPos = TPos_Center);

    // Возвращает абсолютное значение курсора времени без учёта смещения
    double GetPosTCurAbs(int pos, int tBase);

    // Возвращает относительное значение курсора времени по абсолютному
    int GetPosTCurRel(double pos, int tBase);

    // Возвращает разность между позициями курсоров в абсолютном виде
    QString GetDeltaT(int pos1, int pos2, int tBase);

    // Возвращает позицию курсора после изменения масштаба по времени
    int GetPosTCurNew(int prevPos, int prevTBase, int currentTBase);

private:
    std::map<int, QString> mapRangesString;
    std::map<int, QString> mapTBaseString;
};