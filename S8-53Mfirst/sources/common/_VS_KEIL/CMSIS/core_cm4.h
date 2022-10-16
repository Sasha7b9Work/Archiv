#pragma once

#define __sizeof_ptr 4
#define __FPU_USED 0

#include <stdint.h>

#ifndef __IO
#define __IO
#endif

#ifdef WIN32
#else
    #ifndef __STATIC_INLINE
        #define __STATIC_INLINE
    #endif
    
    inline uint32_t __RBIT(uint32_t value)
    {
        return value;
    }
    
    #ifndef __CLZ
        #define __CLZ  (uint8_t)__builtin_clz
    #endif
#endif

void NVIC_SystemReset(void);
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
void __disable_irq(void);
void __set_MSP(uint32_t);
void __enable_irq(void);


typedef struct
{
    uint32_t VTOR;
} _SCB;


extern _SCB *SCB;
