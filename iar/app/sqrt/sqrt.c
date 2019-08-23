////////////////////////////////////////////////////////////////////////////////
/// @file     SQRT.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE SQRT EXAMPLE.
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
#define _SQRT_C_

// Includes  -------------------------------------------------------------------
#include <string.h>

#include "system_mm32.h"

#include "resource.h"

#include "drv.h"
#include "sqrt.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_SQRT
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SQRT_Private_Variables
/// @{


/// @}

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SQRT_Exported_Functions
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

u32 aaa;
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
    HANDLE hSQRT = CreateFile(emIP_SQRT);
    if (hSQRT == NULL)      while(1);
// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_SQRT_DCB dcb = {
        .hSub       = emFILE_SQRT
    };

// Step 4:  Open File Device     ---------------------->>>>>
    OpenFile(hSQRT, (void*)&dcb);

    while (1){
// Step 5:  Write File Device    --------------------->>>>>
        for (int i = 0; i < 8; i++) {
            unSqrt.root = (u32)sqrtDat[i];
            WriteFile(hSQRT, emFILE_SQRT, (u8*)&unSqrt.root, 4);
            // result: unSqrt.squareRoot
        }

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
            vdLED <<= 1;
            if (vdLED >= 0x10) vdLED = 0x0001;
        }
#endif
    }
}

/// @}


/// @}

/// @}

