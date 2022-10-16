//  [3/23/2010 Sasha]
#pragma once
#include "defines.h"

/*
«десь описаны функции преобразовани€. «десь же хранитс€ текущее состо€ние прибора
ѕредполагаетс€, что этот класс будет один
*/

class MathDevice {
public:
    MathDevice();

    void SetModel(int modelDevice);
    int GetModel();

    float GetMaxVoltage(int inOut);
    float GetMaxAmperage(int inOut);

    int GetRangeU(int inOut);        // ћаксимальное относительное значение дл€ напр€жени€
    int GetRangeI(int inOut);        // -//- дл€ тока. «ависит от ранее установленного номера модели

    QString VoltageToString(int input, int inOut);  // inOut - 0 - напр€жение установочное, 1 - напр€жение считанное
    QString VoltageToString(double value);          // ѕеревести в строку с количеством нулей, соответствующим данной модели, заданной ранее функцией SetModel
    QString AmperageToString(int input, int inOut); // inOut - 0 - напр€жение установочное, 1 - напр€жение считанное
    QString AmperageToString(double value);

    double VoltageToAbs(int input, int inOut);
    double AmperageToAbs(int input, int inOut);

    int VoltageToRel(float voltage, int inOut);
    int AmperageToRel(float amperage, int inOut);

    QString DoubleToString(double value, int beforePoint, int afterPoint);
private:
    int modelDevice;        // «десь хранитс€ текуща€ модель устройства. ≈сли 2 - модель не определена
    float maxVoltageAbs[2][2];
    int maxVoltageRel[2][2];
    float maxAmperageAbs[2][2];
    int maxAmperageRel[2][2];
    int numDigitsBeforePointsU[2];
    int numDigitsBeforePointsI[2];
	int numDigitsAfterPointsU[2];
	int numDigitsAfterPointsI[2];
    char* SequenceBytesToASCII(BYTE *input, char* output, int numBytes, char limitter);
    char* ByteToASCII(BYTE byte, char *output);
};