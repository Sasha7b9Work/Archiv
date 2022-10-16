#include "vs2008specific.h"
#include "ServerBSD.h"
#include "stm32f2xx_it.h"
#include "netconf.h"
#include "main.h"
#include "ETHERNET.h"
#include "TcpSocket.h"
#include "httpd.h"
#include <stdio.h>
#include <string.h>
#include "Menu.h"

//*****************************************************************************************
void FuncConnect(int _id);
void FuncReciever(int _id, const char *_buffer, uint _length);

//****************************************************************************************
#define SYSTEMTICK_PERIOD_MS  10
uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */

//****************************************************************************************
int main(void) {
    ETHERNET_Init(FuncConnect, FuncReciever);

    STM_EVAL_LEDInit(LED1);
    TIM6_Config();
    DAC_Ch1_EscalatorConfig();
    
    Menu_Init();
    
    //DebugOut("Test string\n");
    
    while (true) {
        //printf("Test");
        //DebugOut("A");
        GPIO_ToggleBits(GPIOC, GPIO_Pin_13);
        ETHERNET_ProcessingInputBuffer();
    }
}

//****************************************************************************************
void FuncReciever(int _id, const char *_buffer, uint _length) {
    TCP_SOCKET_SendToOne(_id, _buffer, _length);
}

//****************************************************************************************
void FuncConnect(int _id) {
    static char message[100];
    message[0] = '\0';
    strcpy(message, "Connection ");
    IntToStrCat(message, _id);
    strcat(message, " is establishes. All ");
    IntToStrCat(message, TCP_SOCKET_AllConnections());
    strcat(message, " connections");
    TCP_SOCKET_SendToOne(_id, message, strlen(message));
}

//****************************************************************************************
void DebugOut(char *buffer) {
    while((*buffer) != '\0') {
        char symbol = *buffer;
        int sendValue = symbol;
        sendValue &= 0x000000ff;
        ITM_SendChar(sendValue);
        buffer++;
    }
}

//****************************************************************************************
void Time_Update(void) {
    LocalTime += SYSTEMTICK_PERIOD_MS;
}

//****************************************************************************************
#ifdef __cplusplus
extern "C" 
#endif
u32_t sys_now(void) {
    return LocalTime;
}
