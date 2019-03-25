////////////////////////////////////////////////////////////////////////////////
/// @file     IMM_DMA.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE IMM_DMA EXAMPLE.
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
    samFlag = true;
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
    ADC_Filter((u32*)ptr, 16);                                                  // IIR filter
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  The main function initializes four channels 0, 1, 2, and 3 of adc1,
///         and uses hardware TIM2_TRGO to trigger adc single sampling. The re-
///         sult of sampling is passed to the array adcValue through dma, and
///         finally the result is filtered by dma interrupt mode and stored in
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
        .hSub       = emFILE_ADC1,                  // EM_FILE_ADC
        .type       = emTYPE_DMA,                   // polling, interrupt, dma
        .mode       = emADC_Imm,                    // Conversion mode: emADC_Imm,emADC_Scan,emADC_Continue
        .sync       = emTYPE_Sync,                  // emTYPE_Sync, emTYPE_ASync
        .trig       = emTRIGGER_Software,           // Software Start & Trigger enum
        .chs        = LEFT_SHIFT_BIT(1),            // channels: ADC_CH_n
        .temp       = false,                        // Temperature measurement:0(DISABLE),1(ENABLE)
        .vRef       = false,                        // Reference voltage:0(DISABLE),1(ENABLE)
        .cb         = (u32)&Callback,               // adc callback
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

    while (1) {
        u32 temp[4];
        if (dcb.mode != emADC_Continue) {
            if ((dcb.mode == emADC_Imm) && (dcb.type == emTYPE_Polling)) {
                if (dcb.trig == emTRIGGER_Software) {
                    if (samFlag) {
                        samFlag = false;
// Step 5:  Read File Device    ---------------------->>>>>
                        u16 n = ReadFile(hADC, emFILE_ADC1, (u8*)&temp, 16);    // Start ADC & Getting ADC1 Conversion Data
                        if (n > 0) ADC_Filter((u32*)&temp, n);                  // IIR filter
                    }
                }
            }
            else {
                if (dcb.trig == emTRIGGER_Software) {
                    if (samFlag) {
                        samFlag = false;
// Step 5:  Write File Device    --------------------->>>>>
                        WriteFile(hADC, emFILE_ADC1, 0, 0);                     // Software Start ADC1
                    }
                }

                if (dcb.sync == emTYPE_ASync ) {
// Step 6:  Read File Device    ---------------------->>>>>
                    u16 n = ReadFile(hADC, emFILE_ADC1, (u8*)&temp, 16);        // Start ADC & Getting ADC1 Conversion Data
                    if (n > 0) ADC_Filter((u32*)&temp, n);                      // IIR filter
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
