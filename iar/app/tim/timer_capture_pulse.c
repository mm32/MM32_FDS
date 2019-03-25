////////////////////////////////////////////////////////////////////////////////
/// @file     TIMER_CAPTURE_PULSE.C
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE TIMER_CAPTURE_PULSE EXAMPLE.
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
#define _TIMER_C_

// Includes  -------------------------------------------------------------------
#include <string.h>

#include "system_mm32.h"

#include "drv.h"
#include "timer.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_TIM
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer update event callback
//  @param  TIMHandle: TIM handle.
//  @note   This example shows a simple way to use timer update event, and you
//          can add your own implementation.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void UpdateCallback(void* fPtr)
{
    if (tickCnt > 0)
        tickCnt++;
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer Compare/Capture event callback
//  @param  UartHandle: TIM handle
//  @note   This example shows a simple way to use timer Compare/Capture event,
//          and you can add your own implementation.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void CCCallback(void* fPtr)
{
    if (tickCnt > 0)
        tickCnt++;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if (tickCnt++ >= 500) {
        tickCnt  = 0;
        tickFlag = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Main function, program entry.
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
    HANDLE hTIM = CreateFile(emIP_TIM);
    if (hTIM == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_TIM_DCB dcb = {
// Base parameter
        .hSub       = emFILE_TIM3,          // 0:TIM1 1:TIM2 2:TIM3 ...
        .mode       = emTIM_CapturePulse,   // emPWM, emCapturePulse, emCapturePWM

// Timer base count configure
        .cntFreq    = 1000000,              // unit: 1Hz ==> timer counter clock = 1MHz
        .period     = 0xFFFF + 1,           // Auto reload value = period - 1

// Timer channel configure
        .ch         = emTIM_CH2,            // Now only support ch2

// Callback function
        .cbUp    = (u32)&UpdateCallback,    // NULL
        .cbCc    = (u32)&CCCallback,        // NULL

// Operation mode
        .type       = emTYPE_IT,            // emTYPE_IT, emTYPE_DMA
        .sync       = emTYPE_Sync,          // emTYPE_Sync, emTYPE_ASync
        .remapEn    = false,                // 0, 1
        .remapIdx   = 0,                    // u8 value
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if(!OpenFile(hTIM, (void*)&dcb))    while(1);

    pulseWidth = 0;

    while (1) {

        ReadFile(hTIM, emFILE_TIM3, (u8*)&pulseWidth, sizeof(pulseWidth));

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
