////////////////////////////////////////////////////////////////////////////////
/// @file     EXTI.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE EXTI EXAMPLE.
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
#define _EXTI_KEY_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "exti.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_EXTI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXTI_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  System tick timer for counting Keys.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if (tickCnt++ >= 50) {
        tickCnt  = 0;
        tickFlag = true;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTI callback.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Callback(u32 ev, u32 pri)
{
    if (ev & (1 << emFILE_EXTI0))
        vdLED ^= 1 << 2;
    else if (ev & (1 << emFILE_EXTI10))
        vdLED ^= 1 << 1;
    else if (ev & (1 << emFILE_EXTI11))
        vdLED ^= 1 << 0;
    else if (ev & (1 << emFILE_EXTI13))
        vdLED ^= 1 << 3;
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
    HANDLE hEXTI = CreateFile(emIP_EXTI);
    if (hEXTI == NULL)      while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_EXTI_DCB dcb = {
        .sync   = emTYPE_Sync,                  // emTYPE_ASync
        .mode   = emEXTI_IT,                    // emEXTI_IT, emEXTI_Event
        .edge   = emEDGE_Falling,               // emEDGE_Rising, emEXTI_Falling, emEDGE_RisingFalling
        .cb     = (u32)&Callback
    };

// Step 4:  Open File Device     ---------------------->>>>>
    // PC13:    Key1
    dcb.port = emGPIOC;
    dcb.hSub = emFILE_EXTI13;           // line 13
    if (!OpenFile(hEXTI, (void*)&dcb))      while(1);

    // PB10:    Key3
    dcb.port = emGPIOB;
    dcb.hSub = emFILE_EXTI10;           // line 10
    if (!OpenFile(hEXTI, (void*)&dcb))      while(1);

    // PB11:    Key4
    dcb.port = emGPIOB;
    dcb.hSub = emFILE_EXTI11;           // line 11
    if (!OpenFile(hEXTI, (void*)&dcb))      while(1);

    // PA0:     Key2
    dcb.port = emGPIOA;
    dcb.hSub = emFILE_EXTI0;            // line 0
    dcb.edge = emEDGE_Rising;
    if (!OpenFile(hEXTI, (void*)&dcb))      while(1);

    while (1) {

#if defined(__SYS_RESOURCE_POLLING)
        if (tickFlag) {
            tickFlag = false;
            SysDisplay((u8*)&vdLED);
        }
#endif
    }
}

/// @}


/// @}

/// @}
