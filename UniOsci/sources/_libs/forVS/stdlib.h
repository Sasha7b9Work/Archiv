#pragma once
#include "stdint.h"

void *malloc(int size);
void free(void *);

#define __disable_irq()
#define __set_MSP(x)
#define __enable_irq()

#define RAND_MAX 0x7fffffff

