////////////////////////////////////////////////////////////////////////////////
/// @file     M_SCAN_DMA_TIME.C
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

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "adc.h"
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
void AppTaskTick(void)
{
    if (tickCnt++ >= 200) {
        tickCnt  = 0;
        tickFlag = SET;
    }
    samFlag1 = true;
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  ADC trigger setting
//  @param  trig: if emTRIGGER_Ext7 then setting EXTI Line 11
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_TrigerSetting(EM_TRIGGER trig)
{
    if (trig == emTRIGGER_Ext4) {                               // tim3_trgo Trigger ADC
        HANDLE hTIM = CreateFile(emIP_TIM);
        if (hTIM == NULL)       while(1);

        tAPP_TIM_DCB dcb = {
        .hSub       = emFILE_TIM3,          // 0:TIM1
        .mode       = emTIM_Master,         // 6-step mode
        .trgo       = emTIM_CC1,
        .cntFreq    = 441000,               // unit: 1Hz ==> timer counter clock = 1MHz
        .period     = 10,                 // (due to use CenterAligned-Mode) period = 1 MHz * 1000 / 2 = 1KHz / 2
        .pulse      = 5                   // 500 / 1000 = 50% PWM dutycycle
    };

        if (!OpenFile(hTIM, (void*)&dcb))       while(1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  ADC completed callback
//  @param  ADCHandle
//  @note   This example shows a simple way to report end of IT or DMA , and
//          you can add your own implementation.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void Callback(void* ptr, u16 len)
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
int main(void)
{
// Step 1:  Setting System Clock     ------------------>>>>>
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
    //  MCUID = SetSystemClock(emSYSTICK_On, NULL);

// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hADC = CreateFile(emIP_ADC);
    if (hADC == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_ADC_DCB dcb = {
        .hSub   = emFILE_ADC1,                                  // EM_FILE_ADC
        .type   = emTYPE_DMA,                                   // polling, interrupt, dma
        .mode   = emADC_Scan,                                   // ADC mode: emADC_Imm, emADC_Scan, emADC_Continue, emADC_WindowComp
        .sync   = emTYPE_Sync,                                  // emTYPE_Sync, emTYPE_ASync
        .trig   = emTRIGGER_Ext4,                               // Software Start & Trigger enum
        .chs    = LEFT_SHIFT_BIT(0),                            // Channels: ADC_CH_n
        .temp   = true,                                     // Temperature measurement:0(DISABLE),1(ENABLE)
        .vRef   = false,                                        // Reference voltage:0(DISABLE),1(ENABLE)
        .cb     = (u32)&Callback,                               // ADC callback
    };

/*
    Method 1:   dcb.mode = emADC_Imm
                dcb.type = emTYPE_Polling
                dcb.sync = emTYPE_ASync or emTYPE_Sync
                ReadFile()

    Method 2:   dcb.mode = emADC_Imm
                dcb.type = emTYPE_IT
                dcb.sync = emTYPE_ASync or emTYPE_Sync
                WriteFile()
                Callback()

    Method 3:   dcb.mode = emADC_Imm
                dcb.type = emTYPE_DMA
                dcb.sync = emTYPE_ASync
                WriteFile()
                ReadFile()

    Method 4:   dcb.mode = emADC_Imm
                dcb.type = emTYPE_DMA
                dcb.sync = emTYPE_Sync
                WriteFile()
                Callback()
*/

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hADC, (void*)&dcb))       while(1);

    ADC_TrigerSetting(dcb.trig);

    while (1) {
        u32 temp[4];
        if (dcb.mode != emADC_Continue) {
            if ((dcb.mode == emADC_Imm) && (dcb.type == emTYPE_Polling)) {
                if (dcb.trig == emTRIGGER_Software) {
                    if (samFlag1) {
                        samFlag1 = false;
// Step 5:  Read File Device    ---------------------->>>>>
                        u16 n = ReadFile(hADC, emFILE_ADC1, (u8*)&temp, 16);    // Start ADC & Getting ADC1 Conversion Data
                        if (n > 0) ADC1_Filter((u32*)&temp, n);                 // IIR filter
                    }
                }
            }
            else {
                if (dcb.trig == emTRIGGER_Software) {
                    if (samFlag1) {
                        samFlag1 = false;
// Step 5:  Write File Device    --------------------->>>>>
                        WriteFile(hADC, emFILE_ADC1, 0, 0);                     // Software Start ADC1
                    }
                }

                if (dcb.sync == emTYPE_ASync ) {
// Step 6:  Read File Device    ---------------------->>>>>
                    u16 n = ReadFile(hADC, emFILE_ADC1, (u8*)&temp, 16);        // Start ADC & Getting ADC1 Conversion Data
                    if (n > 0) ADC1_Filter((u32*)&temp, n);                     // IIR filter
                }
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
