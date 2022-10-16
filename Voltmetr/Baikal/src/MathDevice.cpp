//  [3/23/2010 Sasha]
#include "MathDevice.h"

//  [3/23/2010 Sasha]
MathDevice::MathDevice() : modelDevice(0) {
    maxVoltageAbs[0][0] = maxVoltageAbs[0][1] = 30.0f;
    maxVoltageAbs[1][0] = maxVoltageAbs[1][1] = 100.0f;

    maxVoltageRel[0][0] = 3000;
    maxVoltageRel[0][1] = 3000;
    maxVoltageRel[1][0] = 4000;
    maxVoltageRel[1][1] = 4000;

    maxAmperageAbs[0][0] = maxAmperageAbs[0][1] = 10.0f;
    maxAmperageAbs[1][0] = maxAmperageAbs[1][1] = 3.0f;

    maxAmperageRel[0][0] = 4000;
    maxAmperageRel[0][1] = 4000;
    maxAmperageRel[1][0] = 3000;
    maxAmperageRel[1][1] = 3000;

    numDigitsBeforePointsU[0] = 2;
    numDigitsBeforePointsU[1] = 3;

	numDigitsAfterPointsU[0] = 2;
	numDigitsAfterPointsU[1] = 3;

    numDigitsBeforePointsI[0] = 2;
    numDigitsBeforePointsI[1] = 1;

	numDigitsAfterPointsI[0] = 4;
	numDigitsAfterPointsI[1] = 3;
}

//  [3/23/2010 Sasha]
void MathDevice::SetModel(int _modelDevice) {
    modelDevice = _modelDevice;
}

//  [3/24/2010 Sasha]
int MathDevice::GetModel() {
    return modelDevice;
}

//  [3/23/2010 Sasha]
float MathDevice::GetMaxVoltage(int _inOut) {
    if((_inOut != 0) && (_inOut != 1)) {
        ERROR_LOG("Error in input parameter");
        return 0.0f;
    }
    return maxVoltageAbs[modelDevice][_inOut];
}

//  [3/24/2010 Sasha]
float MathDevice::GetMaxAmperage(int _inOut) {
    if((_inOut != 0) && (_inOut != 1)) {
        ERROR_LOG("Error in input parameter");
        return 0.0f;
    }
    return maxAmperageAbs[modelDevice][_inOut];
}

//  [4/29/2010 Sasha]
int MathDevice::GetRangeU(int _inOut) {
    return (modelDevice < 2) ? maxVoltageRel[modelDevice][_inOut] : 0;
}

//  [4/29/2010 Sasha]
int MathDevice::GetRangeI(int _inOut) {
    return (modelDevice < 2) ? maxAmperageRel[modelDevice][_inOut] : 0;
}

//  [7/24/2009 Sasha]
QString MathDevice::VoltageToString(int _inputValue, int _inOut) {
	int numBefore = numDigitsBeforePointsU[modelDevice];
	int numAfter = numDigitsAfterPointsU[modelDevice];
    return DoubleToString(VoltageToAbs(_inputValue, _inOut), numBefore, numAfter);
}

//  [4/29/2010 Sasha]
QString MathDevice::VoltageToString(double _value) {
    return DoubleToString(_value, numDigitsBeforePointsU[modelDevice], numDigitsAfterPointsU[modelDevice]);
}

//  [3/4/2010 Sasha]
QString MathDevice::AmperageToString(int _inputValue, int _inOut) {
    static int numDigitsBeforePoints[2] = {2, 1};
    return DoubleToString(AmperageToAbs(_inputValue, _inOut), numDigitsBeforePoints[modelDevice], numDigitsAfterPointsI[modelDevice]);
}

//  [4/29/2010 Sasha]
QString MathDevice::AmperageToString(double _value) {
    return DoubleToString(_value, numDigitsBeforePointsI[modelDevice], numDigitsAfterPointsI[modelDevice]);
}

//  [4/27/2010 Sasha]
double MathDevice::VoltageToAbs(int _input, int _inOut) {
    return (double)_input / maxVoltageRel[modelDevice][_inOut] * maxVoltageAbs[modelDevice][_inOut];
}

//  [4/27/2010 Sasha]
double MathDevice::AmperageToAbs(int _input, int _inOut) {
    return (double)_input / maxAmperageRel[modelDevice][_inOut] * maxAmperageAbs[modelDevice][_inOut];
}

//  [3/24/2010 Sasha]
int MathDevice::VoltageToRel(float _voltage, int _inOut) {
    float step = maxVoltageRel[modelDevice][_inOut] / maxVoltageAbs[modelDevice][_inOut];     // Шаг - (относительных вольт) / (абсолютных вольт)
    int retValue = (int)(_voltage * step);
    return retValue;
}

//  [3/24/2010 Sasha]
int MathDevice::AmperageToRel(float _amperage, int _inOut) {
    float step = maxAmperageRel[modelDevice][_inOut] / maxAmperageAbs[modelDevice][_inOut];
    int retValue = (int)(_amperage * step);
    return retValue;
}

//  [7/16/2009 Sasha]
QString MathDevice::DoubleToString(double _value, int _beforePoint, int _afterPoint) {
    static char buffer[100];
    char *pBuffer = buffer;
    if(3 == _beforePoint) {
        if(_value < 99.9999) {
            sprintf_s(pBuffer, 100, "%s", "0");
            pBuffer++;
        }
    }
    if(_beforePoint > 1) {
        if(_value < 9.999) {
            sprintf_s(pBuffer, 100, "%s", "0");
            pBuffer++;
        }
    }
    if(4 == _afterPoint) {
        sprintf_s(pBuffer, 100, "%.4f", _value);
    }
    else if (3 == _afterPoint) {
        sprintf_s(pBuffer, 100, "%.3f", _value);
    }
	else if (2 == _afterPoint) {
		sprintf_s(pBuffer, 100, "%.2f", _value);
	}
	else {
		sprintf_s(pBuffer, 100, "%f", _value);
	}
    return QString(buffer);

}

//  [3/3/2010 Sasha]]
char* MathDevice::SequenceBytesToASCII(BYTE *_input, char *_output, int _numBytes, char _limitter) {
    char bufferByte[4];     // Временный буфер для хранения одного байта в символьном виде
    char *pBuffer = _output;
    for(int i = 0; i < _numBytes; i++) {
        ByteToASCII(*(_input + i), bufferByte);
        size_t sizeBuffer = strlen(bufferByte);
        memcpy(pBuffer, bufferByte, sizeBuffer);
        pBuffer += sizeBuffer;
        *pBuffer = _limitter;
        pBuffer++;
    }
    *pBuffer = 0;
    return _output;
}

//  [3/3/2010 Sasha]
char* MathDevice::ByteToASCII(BYTE _byte, char *_output) {
    _itoa_s((int)_byte, _output, 4, 16);
    return _output;
}