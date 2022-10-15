#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DAC_HandleTypeDef handleDAC =
{
    DAC
};

HCD_HandleTypeDef handleHCD;


volatile BitField gBF =
{
    0,  // currentNumRAMSignal
    0,  // currentNumROMSignal
    0,  // showHelpHints
    0,  // needFinishDraw
    0,  // showDebugMenu
    0,  // needForSaveToFlashDrive
    0,  // exitFromModeSetNameTo
    0,  // temporaryShowStrNavi
    0,  // runningFPGAbeforeDrawButtons
    0   // exitFromROMtoRAM
};
