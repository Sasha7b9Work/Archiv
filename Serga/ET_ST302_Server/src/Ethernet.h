#pragma once
#include "common.h"


bool ETHERNET_Init(void (*_funcConnect)(int _id), void (*_funcReciever)(int _id, const char *_buffer, uint _length));
bool ETHERNET_ProcessingInputBuffer(void);


