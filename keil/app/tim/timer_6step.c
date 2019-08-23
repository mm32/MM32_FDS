////////////////////////////////////////////////////////////////////////////////
/// @file     TIMER_6STEP.C
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE TIMER_6STEP EXAMPLE.
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

typedef struct {
    bool    start;
    int     step;
} TIM_6STEP_TypeDef;

bool direct = true, flag = true;
u32 speed = 20;

TIM_6STEP_TypeDef  stepMotor;

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if (tickCnt++ > speed) {
        tickCnt  = 0;
        tickFlag = true;

        if (stepMotor.start){
            if (direct) {
                if (++stepMotor.step == 6) {
                    stepMotor.step = 0;
                }
            }
            else {
                if (--stepMotor.step == -1) {
                    stepMotor.step = 6;
                }
            }
        }
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

// Step 2:  Assignment DCB structure    --------------->>>>>
    tAPP_TIM_DCB dcb = {
// Base parameter
        .hSub       = emFILE_TIM1,          // 0:TIM1
        .mode       = emTIM_6Step,          // 6-step mode

// Timer base count configure
        .cntFreq    = 1000000,              // unit: 1Hz ==> timer counter clock = 1MHz
        .period     = 1000,                 // (due to use CenterAligned-Mode) period = 1 MHz * 1000 / 2 = 1KHz / 2

// Timer channel configure
        .pulse      = 500                   // 500 / 1000 = 50% PWM dutycycle
    };

// Step 3:  Open File Device     ---------------------->>>>>
    HANDLE hTIM = CreateFile(emIP_TIM);
    if (hTIM == NULL)       while(1);

    OpenFile(hTIM, (void*)&dcb);

    while (1) {

        if (SysKeyboard(&vkKey)) {

            if (vkKey == VK_K1) {
                direct = !direct;
            }
            if (vkKey == VK_K2) {
                (flag) ? speed++ : speed--;
                if  (speed == 100)
                    flag = false;
                else if (speed == 5)
                    flag = true;
            }
            if (vkKey == VK_K3) {
                stepMotor.start = !stepMotor.start;
            }
        }

        if (tickFlag) {
            tickFlag = false;
            WriteFile(hTIM, 0, (u8*)&stepMotor, 8);
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
