////////////////////////////////////////////////////////////////////////////////
/// @file     CRS.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE CRS EXAMPLE.
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
#define _CRS_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "crs.h"

#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_CRS
/// @brief CRS driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CRS_Exported_Functions
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
    HANDLE hCRS = CreateFile(emIP_CRS);
    if (hCRS == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_CRS_DCB dcb = {
        .hSub       = emFILE_CRS,               // emFILE_CRS
        .edge       = emEDGE_Rising,            // emEDGE_Rising, emEDGE_Falling
        .src        = emCRS_GPIO,               // emCRS_GPIO
        .hsiFreq    = 48000000,                 // HSI48_VALUE,
        .srcFreq    = 1000                      // CRS_SOURCE_FREQUENCY,
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hCRS, (void*)&dcb))       while(1);

    while (1) {
//      if (SysKeyboard(&vkKey)) {
//          switch (vkKey) {
//              case VK_K0:
//              case VK_K1:
//              case VK_K2:
//              case VK_K3:
//              default: break;
//          }
//      }
    u8 value = NULL;
#if defined(__SYS_RESOURCE_POLLING)
        if (tickFlag) {
            tickFlag = false;

            if (ReadFile(hCRS, 0, (u8*)&value, 1)) {
                vdLED <<= 1;
                if (vdLED >= 0x10) vdLED = 0x01;
            }
            else {
                vdLED >>= 1;
                if (vdLED == 0x00) vdLED = 0x08;
            }
            SysDisplay((u8*)&vdLED);
        }
#endif
    }
}

/// @}


/// @}

/// @}
