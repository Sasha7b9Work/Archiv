#pragma once

#ifdef WIN32

#include "defines.h"

#include "device.h"
#include "tables.h"

#include "Command.h"

#include "Data/Buffer.h"
#include "Data/DataSettings.h"
#include "Data/Reader.h"
#include "Data/Storage.h"

#include "Display/Console.h"
#include "Display/DisplayPrimitives.h"
#include "Display/Painter.h"
#include "Display/PainterMem.h"
#include "Display/Symbols.h"
#include "Display/font/Font.h"

#include "FlashDrive/FileManager.h"
#include "FlashDrive/FlashDrive.h"

#include "FPGA/AD9286.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAMath.h"

#include "Keyboard/Decoder.h"

#include "Hardware/AT25160N.h"
#include "Hardware/CPU.h"
#include "Hardware/EEPROM.h"
#include "Hardware/FSMC.h"
#include "Hardware/GPIO.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Hardware/stm32/Timer4XX.h"

#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/PageDebug.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Pages/Include/PageTime.h"
#include "Menu/Pages/Include/PageTrig.h"

#include "Osci/BottomPart.h"
#include "Osci/HiPart.h"
#include "Osci/MemoryWindow.h"
#include "Osci/PainterData.h"

#include "Settings/Settings.h"

#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include "Utils/StringUtils.h"

#include "libs/USBH/usbh_diskio.h"

#pragma warning(push)
#pragma warning(disable:4365 4710 4996)

#include <algorithm>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <cctype>

#pragma warning(pop)

#endif
