#pragma once
#pragma warning(push)
#pragma warning(disable : 4820)

#ifdef STM32F207xx

#include "STM32Cube_F2/Middlewares/Third_Party/FatFs/src/ff_gen_drv.h"

#else

#include "STM32Cube_F4/Middlewares/Third_Party/FatFs/src/ff_gen_drv.h"

#endif

#pragma warning(pop)
