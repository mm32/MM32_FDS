////////////////////////////////////////////////////////////////////////////////
/// @file     IWDG.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE IWDG EXAMPLE.
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
#define _IWDG_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "iwdg.h"

#include "resource.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_IWDG
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup IWDG_Exported_Functions
/// @{


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
///  @brief  IWDG interrupt mode callback
///  @param  None.
///  @note   This routine function is a callback function for the watchdog.
///  @retval None.
////////////////////////////////////////////////////////////////////////////////
void IWDG_cbCallback()
{
    tickFlag = true;
    // Handling IWDG interrupt transactions...
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
    HANDLE hIWDG = CreateFile(emIP_IWDG);
    if (hIWDG == NULL)      while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_IWDG_DCB dcb = {
        .hSub       = emFILE_IWDG,          // emFILE_IWDG
        .sync       = emTYPE_ASync,         // Asynchronous mode
        .pres       = 4,                    // Range 0 : 7
        .cnt        = IWDG_RELOAD_VALUE,    // Reload value: 0 ~ 0xFFF
        .itEn       = 1                     // 1: interrupt,  0: reset
    };

    vdLED = 0x01;                           // Show Reset status
    bool status = false;

    while (1) {
        if ((dcb.sync == emTYPE_ASync) || dcb.itEn) {
            // Handling IWDG interrupt transactions...
        }

        if (tickFlag) {
            tickFlag = false;
            if (status) {
                WriteFile(hIWDG, emFILE_IWDG, NULL, NULL);
                vdLED = 0x08;               // Feed Watchdog
            }
        }

        if (SysKeyboard(&vkKey)) {
            switch (vkKey) {
            case  VK_K0:
                KeyProcess_Key0();
                break;

            case  VK_K1:
// Step 4:  Open File Device     ---------------------->>>>>
                OpenFile(hIWDG, (void*)&dcb);
                status = true;
                vdLED = 0x02;               // Show Start Watchdog
                break;
            case  VK_K2:
                status = true;
                break;
            case  VK_K3:
                status = false;
                vdLED = 0x06;               // Close Feed, Wait for Watchdog Reset
                break;

            default:
                break;
            }
        }

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
