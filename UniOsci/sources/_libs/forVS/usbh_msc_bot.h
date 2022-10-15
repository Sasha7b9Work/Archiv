#pragma once
#pragma warning(push)
#pragma warning(disable : 4820)


#ifdef STM32F207xx

#include "../STM32Cube_F2/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_bot.h"

#else

#include "../STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_bot.h"

#endif

#pragma warning(pop)
