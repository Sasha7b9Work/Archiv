#include "vs2008specific.h"
#include "ServerBSD.h"
#include "lwip/tcp.h"

#include "main.h"
//#include "stm32f2xx_it.h"

err_t CallBackOnConnect(void *_arg, struct tcp_pcb *_newPCB, err_t err) {
    STM_EVAL_LEDToggle(LED1);
    //GPIO_ToggleBits(GPIOC, GPIO_Pin_13);
    return 0;
}

void BSD_Create(void) {

    struct tcp_pcb *structPCD = tcp_new();
    //struct ip_addr ipaddr;
    //IP4_ADDR(&ipaddr, 192, 168, 1, 92);
    err_t codeError = tcp_bind(structPCD, IP_ADDR_ANY, 999);
    
    structPCD = tcp_listen(structPCD);

    tcp_accept(structPCD, CallBackOnConnect);

    //struct tcp_pcb *newStructPCD = tcp_listen(structPCD);
}
