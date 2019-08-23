////////////////////////////////////////////////////////////////////////////////
/// @file     WWDG.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE WWDG EXAMPLE.
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
#define _WWDG_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "wwdg.h"

#include "resource.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_WWDG
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup WWDG_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
///  @brief  WWDG time out  callback
///  @param  None.
///  @note   This routine function is a callback function that Watchdog is about
///          to trigger system reset. The flag mayDay is processed by the
///          software main loop program.
///  @retval None.
////////////////////////////////////////////////////////////////////////////////
void Callback()
{
    mayDay = true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if ((vdLED == 0x0008) && (tickCnt >= 10)) {
        vdLED = 0;
    }

    if (tickCnt++ >= 15) {
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
    HANDLE hWWDG = CreateFile(emIP_WWDG);       // 11
    if (hWWDG == NULL)      while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_WWDG_DCB dcb = {
        .hSub       = emFILE_WWDG,          // emFILE_WWDG
        .sync       = emTYPE_ASync,         // emTYPE_ASync
        .cb         = (u32)&Callback,       // Callback function
        .pres       = 0x03,                 // RANGE: 3:0
        .cnt        = RELOAD_WWDG           // WindowValue: 0x40 ~ 0x7F
    };

    vdLED = 0x01;                           // Show Reset status
    u8 feedValue = WATCHDOG_WWDG;
    bool toggle = false;

    while (1) {
        if (SysKeyboard(&vkKey)) {
            switch  (vkKey) {
            case  VK_K0:
                KeyProcess_Key0();
                break;
            case  VK_K1:
// Step 4:  Open File Device     ---------------------->>>>>
                OpenFile(hWWDG, (void*)&dcb);
                toggle = true;
                vdLED = 0x0002;             // Show Start IWatchdog
                break;
            case  VK_K2:
                KeyProcess_Key2();
                break;
            case  VK_K3:
                toggle = false;
                vdLED = 0x0006;             // Close Feed, Wait for Watchdog Resae
                break;
            default:
                break;
            }
        }

// Step 5:  Write File to Device     ------------------>>>>>
        if (mayDay) {
            mayDay = false;
            WriteFile(hWWDG, emFILE_WWDG, &feedValue, 1);
            vdLED = 0x08;;
        }
        else if (tickFlag) {
            tickFlag = false;
            if (toggle && hWWDG) {
                WriteFile(hWWDG, emFILE_WWDG, &feedValue, 1);
                vdLED = 0x08;;
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
