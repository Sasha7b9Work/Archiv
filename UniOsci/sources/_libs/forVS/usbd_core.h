#pragma once
#pragma warning(push)
#pragma warning(disable : 4820)

#ifdef STM32F207xx

#include "STM32Cube_F2/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_core.h"

#else

#include "STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_core.h"

#endif

#pragma warning(pop)
