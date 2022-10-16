
#include "ETHERNET.h"
#include "TcpSocket.h"
#include "common.h"
#include "netconf.h"
#include "httpd.h"
#include "stm32f2x7_eth_bsp.h"
#include <stm32f2x7_eth.h>

extern volatile uint LocalTime;
void Delay(uint32_t nCount) {
    /* Capture the current local time */
    uint timingdelay = LocalTime + nCount;  

    /* wait until the desired delay finish */  
    while(timingdelay > LocalTime) {     
    }
}

bool ETHERNET_Init(void (*_funcConnect)(int _id), void (*_funcReciever)(int _id, const char *_buffer, uint _length)) {
    ETH_BSP_Config();
    LwIP_Init();
    httpd_init();
    Delay(50);
    return TCP_SOCKET_Init(_funcConnect, _funcReciever);
}

bool ETHERNET_ProcessingInputBuffer() {
    /* check if any packet received */
    if(ETH_CheckFrameReceived()) {
        /* process received ethernet packet */
        LwIP_Pkt_Handle();
    }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(LocalTime);
    return true;
}
