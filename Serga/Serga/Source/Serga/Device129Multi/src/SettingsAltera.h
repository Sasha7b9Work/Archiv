#pragma once
#include "../../common/TypesAltera.h"

struct SettingsAltera {
    friend class AbstractAltera;
    friend class AlteraOscillograph;

public:
    NS_TypesAltera::StartMode   GetStartMode();
    NS_TypesAltera::ModeADC     GetModeADC();
    NS_TypesAltera::TPos        GetTPos();
    NS_TypesAltera::ModeInput   GetModeInput(int _channel) { return modeInput[_channel]; };
    int     GetRange(int channel);
    int     GetTBase();
    double  GetRShiftAbs(int channel);     
    double  GetTrigLevAbs(int channel);   
    int     GetTShift();
    double  GetTShiftAbs();
    int     GetRShift(int channel);
    int     GetTrigLev(int channel);
	int     GetSinchroChannel();
    // inline void SetAutoCalibrate(bool _autoCalibrate)   { autoCalibrate = _autoCalibrate;}
    // inline bool GetAutoCalibrate()                      { return autoCalibrate;          }
    void    SetAdditionRShift(int channel, int additionRShift);
    int     GetAdditionRShift(int channel);

private:
    SettingsAltera();

    bool SetRange(int channel, int range);
    bool SetTBase(int tBase);
    bool SetRShift(int channel, int rShift);
    bool SetTrigLev(int channel, int trigLev);
    void SetTShift(int tShift);

    void SetK(int channel, double k);
    double GetK(int channel);

    // bool autoCalibrate;                   // Производить автоматическую калибровку при изменении масштаба напряжения и канала синхронизации или нет
    int sinchroChannel;      
    NS_TypesAltera::Sinchro sinchro;
    NS_TypesAltera::StartMode startMode;
    NS_TypesAltera::ModeADC modeADC;
    NS_TypesAltera::TPos tPos;
    NS_TypesAltera::ModeInput modeInput[2];
    bool filtr;
    bool calibrator;

    double k[2];                // Коэффициент калибровки
    int additionRShift[2];      // Добавочное смещение к rShift для компенсации плавания нуля
    double addr_Min;            // addrMin' - используется в рандомизаторе. Нужно сохранять после каждого сеанса
    double addr_Max;            // addrMax' - используется в рандомизаторе. Нужно сохранять после каждого сеанса

    struct AdvancedSettingsAltera {
        friend struct SettingsAltera;
    private:
        AdvancedSettingsAltera():tShift(NS_TypesAltera::TShiftZero), tShiftAbs(0.0), tBase(NS_TypesAltera::TBase_10ms) {
            rShift[0]  = rShift[1]  = NS_TypesAltera::RShiftZero;
            rShiftAbs[0] = rShiftAbs[1] = 0.0;
            trigLev[0] = trigLev[1] = NS_TypesAltera::TrigLevZero;
            trigLevAbs[0] = trigLevAbs[1] = 0.0;
            range[0]   = range[1]   = NS_TypesAltera::Range_1v;
        };
        int rShift[2];
        double rShiftAbs[2];
        int trigLev[2];
        double trigLevAbs[2];
        int tShift;
        double tShiftAbs;
        NS_TypesAltera::TBase tBase;
        NS_TypesAltera::Range range[2];
    } adv;
};
