#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Hardware
{
public:

    void Init();
private:
    void SystemClock_Config();
    void MX_GPIO_Init();
    void MX_ADC3_Init();
    void MX_DAC_Init();
    void MX_SPI4_Init();
    void MX_USART3_UART_Init();
    void MX_USB_OTG_FS_PCD_Init();
    void MX_USB_OTG_HS_PCD_Init();
};
