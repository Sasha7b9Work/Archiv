#pragma once
#pragma warning(push)
#pragma warning(disable:4820)

#ifdef STM32F207xx

#include "STM32Cube_F2/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/usbd_cdc.h"

#else

#include "STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/usbd_cdc.h"

#endif

#pragma warning(pop)
