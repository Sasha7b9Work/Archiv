#include "defines.h"
#include "usbd_core.h"
#include "VCP/VCP.h"
#include "Hardware/Timer.h"
#include "Display/Display.h"
#include "Log.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern PCD_HandleTypeDef handlePCD;
extern USBD_HandleTypeDef handleUSBD;

/*******************************************************************************
                       PCD BSP Routines
*******************************************************************************/

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Configure USB FS GPIOs */
    __GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Enable USB FS Clocks */
    __USB_OTG_FS_CLK_ENABLE();

    /* Set USBFS Interrupt priority */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 1, 0);

    /* Enable USBFS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
    __USB_OTG_FS_CLK_DISABLE();
}

/*******************************************************************************
                       LL Driver Callbacks (PCD -> USB Device Library)
*******************************************************************************/
//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{

    USBD_LL_SetupStage((USBD_HandleTypeDef *)hpcd->pData, (uint8_t *)hpcd->Setup);

    USBD_HandleTypeDef *pdev = (USBD_HandleTypeDef*)(hpcd->pData);  //
    USBD_SetupReqTypedef request = pdev->request;                   //
                                                                    //
    static uint16 prevLength = 7;                                   //
                                                                    //
    if (request.wLength == 0)                                       //
    {                                                               //
        if (gBF.cableVCPisConnected == 1)                           //
        {                                                           //
            if (prevLength != 0)                                    //
            {                                                       //
                gBF.connectToHost = 1;                              // GOVNOCODE ??????????????? ????????? ???????????????????????????????????? ????????????????????? ??????????????????????????????, ????????? ??? ????????? ?????????????????????????????????????????? ???????????? (
            }                                                       //
            else                                                    //
            {                                                       //
                gBF.connectToHost = 0;                              //
                Settings_Save();                                    // ????????? ????????????????????????????????????????????? ??????????????????????????? ??????????????????????????? // WARN ????????????????????????, ????????? ?????? ??????????????? ??????????????????
            }                                                       //
            gBF.connectToHost = (prevLength != 0) ? 1 : 0;          // 
        }                                                           //
    }                                                               //
    prevLength = request.wLength;                                   //
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/**
  * @param  epnum: Endpoint Number
  */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBD_LL_DataOutStage((USBD_HandleTypeDef *)hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/**
  * @param  epnum: Endpoint Number
  */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBD_LL_DataInStage((USBD_HandleTypeDef *)hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
    
    // \todo WARN ??????????????? ?????????????????????????????? ??????????????? ?????????????????? ???????????????????????? ??????????????????. ??????????????? ???????????????????????? ??????????????????, ??????????????? ???????????????????????? ???????????????????????????.
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_SOF((USBD_HandleTypeDef *)hpcd->pData);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{ 
    USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

    /* Set USB Current Speed */
    switch(hpcd->Init.speed)
    {
    case PCD_SPEED_HIGH:
        speed = USBD_SPEED_HIGH;
        break;
    
    case PCD_SPEED_FULL:
        speed = USBD_SPEED_FULL;    
        break;
	
	default:
        speed = USBD_SPEED_FULL;    
        break;
    }
    USBD_LL_SetSpeed((USBD_HandleTypeDef *)hpcd->pData, speed);  
  
    /* Reset Device */
    USBD_LL_Reset((USBD_HandleTypeDef *)hpcd->pData);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_Suspend((USBD_HandleTypeDef *)hpcd->pData);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_Resume((USBD_HandleTypeDef *)hpcd->pData);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBD_LL_IsoOUTIncomplete((USBD_HandleTypeDef *)hpcd->pData, epnum);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBD_LL_IsoINIncomplete((USBD_HandleTypeDef *)hpcd->pData, epnum);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_DevConnected((USBD_HandleTypeDef *)hpcd->pData);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_DevDisconnected((USBD_HandleTypeDef *)hpcd->pData);
}

/*******************************************************************************
                       LL Driver Interface (USB Device Library --> PCD)
*******************************************************************************/

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef  USBD_LL_Init (USBD_HandleTypeDef *pdev)
{ 
    /* Change Systick prioity */
    NVIC_SetPriority (SysTick_IRQn, 0);  
  
    /*Set LL Driver parameters */
    handlePCD.Instance = USB_OTG_FS;
    handlePCD.Init.dev_endpoints = 4; 
    handlePCD.Init.use_dedicated_ep1 = 0;
    handlePCD.Init.ep0_mps = 0x40;  
    handlePCD.Init.dma_enable = 0;
    handlePCD.Init.low_power_enable = 0;
    handlePCD.Init.phy_itface = PCD_PHY_EMBEDDED; 
    handlePCD.Init.Sof_enable = 0;
    handlePCD.Init.speed = PCD_SPEED_FULL;
    handlePCD.Init.vbus_sensing_enable = 1;
    /* Link The driver to the stack */
    handlePCD.pData = pdev;
    pdev->pData = &handlePCD;
    /*Initialize LL Driver */
    HAL_PCD_Init(&handlePCD);
  
    HAL_PCD_SetRxFiFo(&handlePCD, 0x80);
    HAL_PCD_SetTxFiFo(&handlePCD, 0, 0x40);
    HAL_PCD_SetTxFiFo(&handlePCD, 1, 0x80); 

    return USBD_OK;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev)
{
    HAL_PCD_DeInit((PCD_HandleTypeDef *)pdev->pData);
    return USBD_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
    HAL_PCD_Start((PCD_HandleTypeDef *)pdev->pData);
    return USBD_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev)
{
    HAL_PCD_Stop((PCD_HandleTypeDef *)pdev->pData);
    return USBD_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev, uint8_t  ep_addr, uint8_t  ep_type, uint16_t ep_mps)
{
    HAL_PCD_EP_Open((PCD_HandleTypeDef *)pdev->pData, ep_addr, ep_mps, ep_type);
    return USBD_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_Close((PCD_HandleTypeDef *)pdev->pData, ep_addr);
    return USBD_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_Flush((PCD_HandleTypeDef *)pdev->pData, ep_addr);
    return USBD_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_SetStall((PCD_HandleTypeDef *)pdev->pData, ep_addr);
    return USBD_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_ClrStall((PCD_HandleTypeDef *)pdev->pData, ep_addr);  
    return USBD_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef *)pdev->pData; 
  
    if((ep_addr & 0x80) == 0x80)
    {
        return hpcd->IN_ep[ep_addr & 0x7F].is_stall; 
    }
    else
    {
        return hpcd->OUT_ep[ep_addr & 0x7F].is_stall; 
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, uint8_t dev_addr)   
{
    HAL_PCD_SetAddress((PCD_HandleTypeDef *)pdev->pData, dev_addr);
    return USBD_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, uint8_t  ep_addr, uint8_t  *pbuf, uint16_t  size)
{
    HAL_PCD_EP_Transmit((PCD_HandleTypeDef *)pdev->pData, ep_addr, pbuf, size);
    return USBD_OK;   
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, uint8_t  ep_addr, uint8_t  *pbuf, uint16_t  size)
{
    HAL_PCD_EP_Receive((PCD_HandleTypeDef *)pdev->pData, ep_addr, pbuf, size);
    return USBD_OK;   
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t  ep_addr)  
{
    return HAL_PCD_EP_GetRxCount((PCD_HandleTypeDef *)pdev->pData, ep_addr);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void  USBD_LL_Delay(uint32_t Delay)
{
    Timer::PauseOnTime(Delay);
}
