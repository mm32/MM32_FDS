////////////////////////////////////////////////////////////////////////////////
/// @file     CONTINUE_DMA_SYNC.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE CONTINUE_DMA_SYNC EXAMPLE.
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
    if (samFlag) {
        samFlag = false;
        ADC_Filter((u32*)ptr, 16);                                              // IIR filter
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  The main function initializes the four channels 0, 1, 2, and 3 of
///         adc1, and uses software to trigger continuous sampling. The result
///         of continuous sampling is passed to the array adcValue through dma,
///         and the result is filtered into the array adcResult by dma interrupt.
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

    memset((void*)&item, 0x00, sizeof(item));
    samFlag = false;
    samCnt = 0;


// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hADC = CreateFile(emIP_ADC);
    if (hADC == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_ADC_DCB dcb = {
        .hSub   = emFILE_ADC1,                  // EM_FILE_ADC
        .type   = emTYPE_DMA,                   // polling, interrupt, dma
        .mode   = emADC_Continue,               // Conversion mode: emADC_Imm,emADC_Scan,emADC_Continue
        .sync   = emTYPE_Sync,                  // emTYPE_Sync, emTYPE_ASync
        .trig   = emTRIGGER_Software,           // Software Start & Trigger enum
        .chs    =   LEFT_SHIFT_BIT(0) |
                    LEFT_SHIFT_BIT(1) |
                    LEFT_SHIFT_BIT(2) |
                    LEFT_SHIFT_BIT(3),          // channels: ADC_CH_n
        .temp   = false,                        // Temperature measurement:0(DISABLE),1(ENABLE)
        .vRef   = false,                        // Reference voltage:0(DISABLE),1(ENABLE)
        .cb     = (u32)&Callback,               // adc callback
    };


    if (!OpenFile(hADC, (void*)&dcb))       while(1);                           // Initia ADC & Start ADC

// Step 6:  Read File Device    ---------------------->>>>>
    while (1) {
        if ((dcb.mode == emADC_Continue) && (dcb.sync == emTYPE_ASync)) {
            if (samFlag) {
                samFlag = false;
                u32 temp[4];
                u16 n = ReadFile(hADC, emFILE_ADC1, (u8*)&temp, 16);            // Getting ADC1 Conversion Data
                if (n > 0) {
                    ADC_Filter((u32*)&temp, n);                                 // IIR filter
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
