#pragma once


#ifdef __cplusplus
 extern "C" {
#endif 

void NMI_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();
void SVC_Handler();
void DebugMon_Handler();
void PendSV_Handler();
void SysTick_Handler();
void EXTI9_5_IRQHandler();
void ADC_IRQHandler();

#ifdef __cplusplus
}
#endif
