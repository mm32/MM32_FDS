////////////////////////////////////////////////////////////////////////////////
/// @file     ADC_DAC_ONOFFTRIG.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE M_SCAN_DMA_TIME EXAMPLE.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _ADC_C_
#define _DAC_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "adc.h"
#include "dac.h"
#include "adc_common.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_ADC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
bool tickFlag2 = false;
void AppTaskTick(void)
{
    if (tickCnt++ >= 200) {
        tickCnt  = 0;
        tickFlag = SET;
        tickFlag2 = SET;
    }
    samFlag1 = true;
    samFlag2 = true;
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  ADC trigger setting
//  @param  trig: if emTRIGGER_Ext7 then setting EXTI Line 11
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_TrigerSetting(EM_TRIGGER trig)         //tim3_cc1
{
    HANDLE hTIM = CreateFile(emIP_TIM);
    if (hTIM == NULL)       while(1);

    tAPP_TIM_DCB dcb = {
        .hSub       = emFILE_TIM3,
        .mode       = emTIM_Master,
        .trgo       = emTIM_CC1,
        .cntFreq    = 441000,
        .period     = 10,
        .pulse      = 5
    };

    if (!OpenFile(hTIM, (void*)&dcb))       while(1);
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  ADC completed callback
//  @param  ADCHandle
//  @note   This example shows a simple way to report end of IT or DMA , and
//          you can add your own implementation.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void Callback1(void* ptr, u16 len)
{
    /* Set Conversion flag: Conversion complete */
    ADC1_Filter((u32*)ptr, 16);                                 // IIR filter
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  The main function initializes channel 0 of adc1, uses systick
///         interrupt, triggers a single sampling with software every 1ms,
///         the result of sampling is passed to the array adcValue through
///         adc interrupt, and then the result is filtered and stored in
///         the array adcResult.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u16 adcOnOff = 0;
u16 dacOnOff = 0;
int main(void)
{
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);

    HANDLE hADC = CreateFile(emIP_ADC);
    if (hADC == NULL)       while(1);

    tAPP_ADC_DCB dcb = {
        .hSub   = emFILE_ADC1,                                  // EM_FILE_ADC
        .type   = emTYPE_DMA,                                   // polling, interrupt, dma
        .mode   = emADC_Scan,                                   // ADC mode: emADC_Imm, emADC_Scan, emADC_Continue, emADC_WindowComp
        .sync   = emTYPE_Sync,                                  // emTYPE_Sync, emTYPE_ASync
        .trig   = emTRIGGER_Ext6,                               // Software Start & Trigger enum
        .chs    = LEFT_SHIFT_BIT(0) | LEFT_SHIFT_BIT(1),                            // Channels: ADC_CH_n
        .temp   = false,                                        // Temperature measurement:0(DISABLE),1(ENABLE)
        .vRef   = false,                                        // Reference voltage:0(DISABLE),1(ENABLE)
        .cb     = (u32)&Callback1,                              // ADC callback
    };

    if (!OpenFile(hADC, (void*)&dcb))       while(1);

    ADC_TrigerSetting(dcb.trig);

    HANDLE hDAC = CreateFile(emIP_DAC);
    if (hDAC == NULL)       while(1);

    tAPP_DAC_DCB dcb2 = {
        .type       = emTYPE_DMA,           // Data transmission method emTYPE_DMA or NULL
        .trig       = true,                 // External trigger enable:0(software trigger), 1(Hardware trigger)
        .hSub       = emFILE_DAC_CH1,
        .wave       = 0                     // DAC generated wave: 0(Not generated), 1(Noise), 2(Triangle)
    };

    if (!OpenFile(hDAC, (void*)&dcb2))      while(1);

    WriteFile(hDAC, 0, (u8*)sineWave2, 32);                                 //Output a sine wave

    while (1) {
        if (tickFlag2) {
            tickFlag2 = false;
            adcOnOff++;
            dacOnOff++;
            if (adcOnOff == 25) {
                CloseFile(hADC);
            }
            if (adcOnOff == 50) {
                adcOnOff = 0;
                if (!OpenFile(hADC, (void*)&dcb))       while(1);
            }
            if (dacOnOff == 30) {
                CloseFile(hDAC);
            }
            if (dacOnOff == 60) {
                dacOnOff = 0;
                if (!OpenFile(hDAC, (void*)&dcb2))      while(1);
            }
        }

        if (SysKeyboard(&vkKey)) {
            switch (vkKey) {
                case VK_K0:
                case VK_K1:
                case VK_K2:
                case VK_K3:
                default: break;
            }
        }
#if defined(__SYS_RESOURCE_POLLING)
        if (tickFlag) {
            tickFlag = false;
            SysDisplay((u8*)&vdLED);
            if (vdLED == 0) vdLED = 1;
            else vdLED <<= 1;
            if (vdLED >= 0x10) vdLED = 0x0001;
        }
#endif
    }
}

/// @}


/// @}

/// @}
