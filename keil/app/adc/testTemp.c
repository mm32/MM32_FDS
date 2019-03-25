////////////////////////////////////////////////////////////////////////////////
/// @file     TESTTEMP.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE WINDOW_COMP EXAMPLE.
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
    if (tickCnt++ >= 500) {
        tickCnt  = 0;
        tickFlag = true;
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
    ADC_Filter((u32*)ptr, 16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  The main function initializes four channels of 0, 1, 2, 3 of adc1,
///         uses systick interrupt, triggers single sampling with software ev-
///         ery 1ms, the result of sampling is passed to the array adcValue
///         through adc interrupt, and then the result is filtered and stored
///         in the array adcResult.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u32 temp[4];
float temperate;
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
        .hSub    = emFILE_ADC1,                                                 // EM_FILE_ADC
        .type    = emTYPE_Polling,
        .mode    = emADC_Imm,                                                   // Conversion mode: emADC_Single,emADC_SingleScan,emADC_Continue
//      .sync    = emTYPE_Sync,                                                 // emTYPE_Sync, emTYPE_ASync
        .trig    = emTRIGGER_Software,                                          // Software Start & Trigger enum
//      .chs     = LEFT_SHIFT_BIT(8),                                           // channels: ADC_CH_n
        .temp    = true,
        .cb      = (u32)&Callback                                               // adc callback
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hADC, (void*)&dcb))       while(1);                           // start temp

    transFlag = 0;

    while (1) {
        if (samFlag) {
            samFlag = false;
            u16 n = ReadFile(hADC, emFILE_ADC1, (u8*)&temp, 16);    // Start ADC & Getting ADC1 Conversion Data
            if (n > 0) ADC_Filter((u32*)&temp, n);                  // IIR filter
        }
        if (transFlag) {
            transFlag = false;
#if defined(__MM3N1)
            float p;
            p = item.AdcResult[0] * (3.3 / 4096);
            temperate = (p - 1.5) / 0.004801 + 25;
#elif defined(__MM0N1)
            temperate = 27.0 + (item.AdcResult[0] - 1800) / 5.96;
#endif
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
