#include "SettingsAltera.h"
#include "../../common/defines.h"
#include "../common/TablesOfConverting.h"

using namespace NS_TypesAltera;

extern TablesOfConverting tables;

SettingsAltera::SettingsAltera():
    sinchro(Sinchro_Front),
    startMode(StartMode_Auto),
    modeADC(ModeADC_RealTime),
    sinchroChannel(0),
    tPos(TPos_Center),
    filtr(false),
    calibrator(true),
    //autoCalibrate(false),
    addr_Min(-1),
    addr_Max(-1)
{
    modeInput[0] = modeInput[1] = ModeAC;
    k[0] = k[1] = 1.0;
    additionRShift[0] = additionRShift[1] = 0;
}

StartMode SettingsAltera::GetStartMode() 
{
    return startMode;
}

ModeADC SettingsAltera::GetModeADC() {
    return modeADC;
}

TPos SettingsAltera::GetTPos() 
{
    return tPos;
}

bool SettingsAltera::SetRange(int _channel, int _range) 
{
    if(   ((_channel == 0) || (_channel == 1))   &&   ((_range >= 0) && (_range < RangeSize))   ) 
    {
        adv.rShift[_channel] = tables.GetRShiftRel(adv.rShiftAbs[_channel], _range);
        adv.trigLev[_channel] = tables.GetRShiftRel(adv.trigLevAbs[_channel], _range);
        adv.range[_channel] = (Range)_range;
        return true;
    }
    else
        return false;
}

int SettingsAltera::GetRange(int _channel) 
{
    if(_channel == 0 || _channel == 1) 
        return adv.range[_channel];
    else
        return 0;
}

bool SettingsAltera::SetRShift(int _channel, int _rShift) 
{
    if(((_channel == 0) || (_channel == 1))) 
    {
        if(_rShift > RShiftMax) 
            _rShift = RShiftMax;
        else if(_rShift < RShiftMin)
            _rShift = RShiftMin;

        adv.rShiftAbs[_channel] = tables.GetRShiftAbs(_rShift, adv.range[_channel]);
        adv.rShift[_channel] = _rShift;
        return true;
    }
    else 
    {
        //LOG("Error in input parameter: channel = %d", _channel);
        return false;
    }
}

int SettingsAltera::GetRShift(int _channel) 
{
    if(   (_channel == 0)||(_channel == 1)   ) 
        return adv.rShift[_channel];
    else 
    {
        //LOG("Error in input parameter: channel = %d", _channel);
        return 0;
    }
}

double SettingsAltera::GetRShiftAbs(int _channel) 
{
    if(   (_channel == 0)||(_channel == 1)   ) 
        return adv.rShiftAbs[_channel];
    else 
    {
        //LOG("Error in input parameter: channel = %d", _channel);
        return 0.0;
    }
}

bool SettingsAltera::SetTrigLev(int _channel, int _trigLev) 
{
    if( (_channel == 0) || (_channel == 1) ) 
    {
        if(_trigLev < TrigLevMin) 
            _trigLev = TrigLevMin;
        else if(_trigLev > TrigLevMax)
            _trigLev = TrigLevMax;

        adv.trigLevAbs[_channel] = tables.GetRShiftAbs(_trigLev, adv.range[_channel]);
        adv.trigLev[_channel] = _trigLev;
        return true;
    }
    else 
    {
        //LOG("Error in input parameters: channel = %d", _channel);
        return false;
    }
}

int SettingsAltera::GetTrigLev(int _channel) 
{
    if((_channel == 0)||(_channel == 1)) 
        return adv.trigLev[_channel];
    else 
    {
        //LOG("Error in input parameter: channel = %d", _channel);
        return 0;
    }
}

int SettingsAltera::GetSinchroChannel() 
{
    return sinchroChannel;
}

double SettingsAltera::GetTrigLevAbs(int _channel) 
{
    if((_channel == 0)||(_channel == 1))
        return adv.trigLevAbs[_channel];
    else 
    {
        //LOG("Error in input parameter: channel = %d", _channel);
        return 0.0;
    }
}

bool SettingsAltera::SetTBase(int _tBase) 
{
    if((_tBase >= 0) && (_tBase < TBaseSize)) 
    {
        adv.tBase = (TBase)_tBase;
        adv.tShift = tables.GetTShiftRel(adv.tShiftAbs, _tBase);
        if(_tBase < TBase_1us)
            modeADC = ModeADC_Randomize;
        else 
            modeADC = ModeADC_RealTime;
        return true;
    }
    else 
    {
        return false;
    }
}

int SettingsAltera::GetTBase() 
{
    return adv.tBase;
}

void SettingsAltera::SetTShift(int _tShift) 
{
    if(_tShift < TShiftMin) 
        _tShift = TShiftMin;
    else if(_tShift > TShiftMax) 
        _tShift = TShiftMax;

    adv.tShiftAbs = tables.GetTShiftAbs(_tShift, adv.tBase);
    adv.tShift = _tShift;
}

void SettingsAltera::SetAdditionRShift(int _channel, int _additionRShift) 
{
    if((_channel == 0) || (_channel == 1))
    {
        additionRShift[_channel] = _additionRShift;
        //cout << "addition rSift" << _channel << " = " << _additionRShift << endl;
    }
    else 
        //LOG("Error in input parameter: channel = %d", _channel);
        ;
}

int SettingsAltera::GetAdditionRShift(int _channel) 
{
    if((_channel == 0) || (_channel == 1)) 
        return additionRShift[_channel];
    else 
        //LOG("Error in input parameter: channel = %d", _channel);
        ;
    
    return 0;
}

void SettingsAltera::SetK(int _channel, double _k) 
{
    if((_channel == 0) || (_channel == 1))
    {
        k[_channel] = _k;
        //LOG(0, LOG_MSG("new k = %f", _k));
    }
}

double SettingsAltera::GetK(int _channel) 
{
    if((_channel == 0) || (_channel == 1)) 
        return k[_channel];
    else 
        //LOG("Error in input parameter: channel = %d", _channel);
        ;
   
    return 0.0;
}

int SettingsAltera::GetTShift() 
{
    return adv.tShift;
}

double SettingsAltera::GetTShiftAbs() 
{
    return adv.tShiftAbs;
}
