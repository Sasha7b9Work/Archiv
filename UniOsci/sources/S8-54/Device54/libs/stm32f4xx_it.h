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

void EXTI0_IRQHandler();
void SPI1_IRQHandler();

void OTG_FS_IRQHandler();
    
void OTG_HS_IRQHandler();

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);     // ������������ ��� ������ ��� �������������

#ifdef __cplusplus
}
#endif
