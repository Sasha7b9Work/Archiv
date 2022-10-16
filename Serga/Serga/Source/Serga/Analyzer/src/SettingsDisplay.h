//  [12/12/2009 Sasha]
#pragma once
#include "../../common/AbstractSettingsDisplay.h"
#include "../../common/defines.h"

struct SettingsDisplay : public AbstractSettingsDisplay {
    SettingsDisplay() : AbstractSettingsDisplay(),     activeSpectr(0), enableCurs(false)
    {
        posCurSpectr[0][0] = posCurSpectr[1][0] = 50;
        posCurSpectr[0][1] = posCurSpectr[1][1] = 200;
    }

    void SetPosCur(int _channel, int _numCur, int _pos)
    {
        if((_channel < 0) || (_channel > 1) || (_numCur < 0) || (_numCur > 1) || (_pos < 0) || (_pos > 250)) 
        {
            //LOG("Error in input parameter: channel = %d, numCur = %d, pos = %d", _channel, _numCur, _pos);
            return;
        }
        posCurSpectr[_channel][_numCur] = _pos;
    };

    int GetPosCur(int _channel, int _numCur)
    {
        if((_channel < 0) || (_channel > 1) || (_numCur < 0) || (_numCur > 1)) 
        {
            //ERROR_LOG("Error int input parameters: channel = %d, numCur = %d", _channel, _numCur);
            return 0;
        }
        return posCurSpectr[_channel][_numCur];
    };

    int activeSpectr;       // Какой спектр сейчас выводится на экран. 0 - первый канал, 1 - второй канал
    bool enableCurs;        // Индикатор включенного состояния курсоров
private:
    int posCurSpectr[2][2]; // Позиция курсора канала [канал][курсор]. Изменяется от 0 до 250
};