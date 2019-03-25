////////////////////////////////////////////////////////////////////////////////
/// @file     PVD_SYNC.C
/// @author   MY Zhang
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE PVD_SYNC EXAMPLE.
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

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_PWR
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWR_Exported_Variables
/// @{


//EM_EDGE pvdStatus = emEDGE_Falling;

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
//  @brief  PVD Falling of power voltage completed callback
//  @param  PVD Handle
//  @note   This example shows a simple way to set the voltage falling edge.
//
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void FallingCallback()
{
    pvdStatus = emEDGE_Falling; //  Detection of voltage fall edge

    vdLED = 0x01;
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  PVD  Rising of power voltage completed callback
//  @param  PVD Handle
//  @note   This example shows a simple way to set the voltage rising edge.
//
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void RisingCallback()
{
    pvdStatus = emEDGE_Rising;  //  Detection of voltage rise edge

    vdLED = 0x02;
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
        .hSub       = emFILE_PVD_F,                                         // emPOWER_PVD_R, emPOWER_PVD_F

        .sync       = emTYPE_Sync,                                              // emTYPE_Sync or emTYPE_ASync
        .voltage    = 4,                                                        // PWR_CR_PLS_3V0
        .cb1        = (u32)&FallingCallback,                                    // (u32)&FallingCallback,
        .cb2        = (u32)&RisingCallback,                                     // (u32)&RisingCallback,
        .enable     = true
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hPVD, (void*)&dcb))   while(1);

    while(1) {
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
