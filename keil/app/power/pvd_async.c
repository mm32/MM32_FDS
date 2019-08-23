////////////////////////////////////////////////////////////////////////////////
/// @file     PVD_ASYNC.C
/// @author   MY Zhang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE PVD_ASYNC EXAMPLE.
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
#define _PWR_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "system_mm32.h"

#include "drv.h"
#include "pwr.h"

#include "resource.h"

//#include "bsp_key.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_PWR
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWR_Exported_Variables
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
int main(void)
{
// Step 1:  Setting System Clock     ------------------>>>>>
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
    //  MCUID = SetSystemClock(emSYSTICK_On, NULL);

// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hPVD = CreateFile(emIP_POWER);
    if (hPVD == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_POWER_DCB dcb = {
        .hSub   = emFILE_PVD_R,                                                 // emPOWER_PVD_R, emPOWER_PVD_F

        .sync   = emTYPE_ASync,                                                 // emTYPE_Sync or emTYPE_ASync
        .cb1    = NULL,                                                         // (u32)&SYNC_PVD_FallingCallback,
        .cb2    = NULL,                                                         // (u32)&SYNC_PVD_RisingCallback,

        .voltage    = 4,                                                        // PWR_CR_PLS_3V0
        .enable     = true
    };

// Step 4:  Open File Device     --- ------------------->>>>>
    if (!OpenFile(hPVD, (void*)&dcb))   while(1);

    while(1) {
        // Asynchronous mode uses the following code
        if (dcb.sync == emTYPE_ASync) {
            if (ReadFile(hPVD, emFILE_PVD_R, (u8*)&pvdStatus, 1)) {
                if (pvdStatus == emEDGE_Rising) {
                    //  The rising of power voltage
                    vdLED = 0x01;
                }
                else {
                    //  The falling of power voltage
                    vdLED = 0x02;
                }
            }
        }

        if (SysKeyboard(&vkKey)) {
            switch  (vkKey) {
            case  VK_K0:
                KeyProcess_Key0();
                break;
            case  VK_K1:
                KeyProcess_Key1();
                break;
            case  VK_K2:
                KeyProcess_Key2();
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
        }
#endif
    }
}

/// @}


/// @}

/// @}
