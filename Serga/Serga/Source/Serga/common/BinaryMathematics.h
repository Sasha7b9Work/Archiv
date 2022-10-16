//  [12/10/2009 Sasha]
#pragma once
#include "defines.h"

namespace NS_BinaryMathematics {
    inline void SetBit(BYTE &data, BYTE bit) 
    {
        data |= (BYTE(1)<<bit);
    };
    inline void ClearBit(BYTE &data, BYTE bit)
    {
        data &= (~((BYTE)1 << bit));
    };
    inline bool GetBit(WORD data, BYTE bit)
    {
        WORD temp = (data&(1<<bit));
        return (temp != 0);
    };
}