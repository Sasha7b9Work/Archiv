#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

void NMI_Handler()
{
}

void HardFault_Handler()
{
    while (1)
    {      
    }
}

void MemManage_Handler()
{
  while (1)
  {
  }
}

void BusFault_Handler()
{
  while (1)
  {
  }
}

void UsageFault_Handler()
{
  while (1)
  {
  }
}

void SVC_Handler()
{
}

void DebugMon_Handler()
{
}

void PendSV_Handler()
{
}

void SysTick_Handler()
{
  HAL_IncTick();
}

void ADC_IRQHandler()
{
}

void OTG_HS_IRQHandler()
{
    HAL_HCD_IRQHandler(&handleHCD);
}

void DMA1_Stream5_IRQHandler()
{
}

#ifdef __cplusplus
}
#endif
