#include "defines.h"
#include "Display.h"
#include "Painter.h"
#include "Keyboard/Controls.h"
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Display display;

LTDC_LayerCfgTypeDef pLayerCfg;
LTDC_HandleTypeDef hltdc;
static uint8 frontBuffer[320 * 240];       // Это экран
static uint8 backBuffer[320 * 240];        // Зто задний буфер. В нём происходит отрисовка, и затем изображение копируется во frontBuffer


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    hltdc.Instance = LTDC;
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hltdc.Init.HorizontalSync = 0;          //
    hltdc.Init.VerticalSync = 0;            //
    hltdc.Init.AccumulatedHBP = 70;         //
    hltdc.Init.AccumulatedVBP = 13;         //
    hltdc.Init.AccumulatedActiveW = 390;
    hltdc.Init.AccumulatedActiveH = 253;
    hltdc.Init.TotalWidth = 408;            //
    hltdc.Init.TotalHeigh = 263;            //
    hltdc.Init.Backcolor.Blue = 255;
    hltdc.Init.Backcolor.Green = 255;
    hltdc.Init.Backcolor.Red = 255;
    if (HAL_LTDC_Init(&hltdc) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = 320;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = 240;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
    pLayerCfg.Alpha = 255;
    pLayerCfg.Alpha0 = 255;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    pLayerCfg.FBStartAdress = (uint32_t)frontBuffer;
    pLayerCfg.ImageWidth = 320;
    pLayerCfg.ImageHeight = 240;
    pLayerCfg.Backcolor.Blue = 255;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red = 0;
    if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    painter.SetPalette(0, 0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    painter.BeginScene(COLOR_BACK);
    painter.SetColor(COLOR_FILL);
    painter.EndScene();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ToggleBuffers()
{
    uint destination = (uint)frontBuffer;
    uint source = (uint)backBuffer;

    DMA2D_HandleTypeDef hDMA2D;

    hDMA2D.Init.Mode = DMA2D_M2M;
    hDMA2D.Init.ColorMode = DMA2D_INPUT_L8;
    hDMA2D.Init.OutputOffset = 0;

    hDMA2D.XferCpltCallback = 0;

    hDMA2D.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hDMA2D.LayerCfg[1].InputAlpha = 0xFF;
    hDMA2D.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
    hDMA2D.LayerCfg[1].InputOffset = 0;

    hDMA2D.Instance = DMA2D;

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDMA2D, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDMA2D, source, destination, 320, 240) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display::GetBuffer()
{
    return backBuffer;
}
