////////////////////////////////////////////////////////////////////////////////
/// @file     FLASH_PROTECT.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE FLASH_PROTECT EXAMPLE.
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
#define _FLASH_PROTECT_C_

// Includes  -------------------------------------------------------------------
#include <string.h>

#include "system_mm32.h"
#include "drv.h"
#include "flash_protect.h"

#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_FLASH
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup FLASH_Exported_Functions
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
/// @brief  FLASH PROTECT example
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
    HANDLE hPROTECT = CreateFile(emIP_PROTECT);
    if (hPROTECT == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_PROTECT_DCB  dcb = {
        .hSub       = 0                 // Protect
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hPROTECT, (void*)&dcb))   while(1);

    while(1) {
        if (SysKeyboard(&vkKey)) {
            switch  (vkKey) {
            case  VK_K0:
                KeyProcess_Key0();
                break;
            case  VK_K1:
                ReadFile(hPROTECT, 0, NULL, 1);
                break;
            case  VK_K2:
                WriteFile(hPROTECT, 0, NULL, 1);
                break;
            case  VK_K3:
                KeyProcess_Key3();
                break;
            default:
                break;
            }
        }

#if defined(__SYS_RESOURCE_POLLING)
        if (tickFlag) {
            tickFlag = false;
            SysDisplay((u8*)&vdLED);
            vdLED <<= 1;
            if (vdLED >= 0x10) vdLED = 0x0001;
        }
#endif
    }
}

/// @}

/// @}

/// @}
////////////////////////////////////////////////////////////////////////////////

