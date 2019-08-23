////////////////////////////////////////////////////////////////////////////////
/// @file     TIMER_PWM_DMA.C
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE TIMER_PWM_DMA EXAMPLE.
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
        .hSub       = emFILE_TIM3,          // 1:TIM1 2:TIM2 2:TIM3 ...
        .mode       = emTIM_PWM,            // emPWM, emCapturePulse, emCapturePWM

// Timer base count configure
        .cntFreq    = 1000000,              // unit: 1Hz ==> timer counter clock = 1MHz
        .period     = 1000,                 // period = 1 MHz * 1000 = 1KHz

// Timer channel configure
        .ch         = emTIM_CH1,            // select config channel
        .pulse      = 500,                  // 500 / 1000 = 50% PWM dutycycle

// Callback function
        .cbUp    = (u32)&UpdateCallback,    //NULL
        .cbCc    = (u32)&CCCallback,        //NULL

// Operation mode
        .type       = emTYPE_DMA,           // emTYPE_IT, emTYPE_DMA
        .sync       = emTYPE_Sync,          // emTYPE_Sync, emTYPE_ASync
        .remapEn    = false,                // 0, 1
        .remapIdx   = 0,                    // u8 value
    };

// Step 3:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hTIM, (void*)&dcb))       while(1);


    u16 pwmValue = dcb.pulse;                // set 50% PWM dutyCycle

    while (1) {
        if (SysKeyboard(&vkKey)) {
            if (vkKey == VK_K2) {
                pwmValue += 10;       // Press K3
            }

            if (vkKey == VK_K3) {
                pwmValue -= 10;       // Press K4
            }
        }

        if (pwmValue > dcb.period) pwmValue = 10;   // set TIM CCR1 value < TIM ARR value

        if (tickFlag) {
            WriteFile(hTIM, emFILE_TIM3, (u8*)&pwmValue, 1);
            tickFlag = false;
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
