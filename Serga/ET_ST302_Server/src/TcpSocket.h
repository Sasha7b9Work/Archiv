#pragma once
#include "common.h"

struct tcp_pcb;

static const int    DEFAULT_PORT = 9999,
                    POLICY_PORT = 843;

bool TCP_SOCKET_Init(void (*_funcConnect)(int _id), void (*_funcReciever)(int _id, const char *_buffer, uint _length));    // _funcConnect will be called every time a new connection
                                                                                                                   // _func
bool TCP_SOCKET_SendToAll(const char *_buffer, uint _length);

bool TCP_SOCKET_SendToOne(int _id, const char *_buffer, int _length);

int TCP_SOCKET_AllConnections(void);
