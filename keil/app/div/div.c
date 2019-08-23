////////////////////////////////////////////////////////////////////////////////
/// @file     DIV.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE DIV EXAMPLE.
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
#define _DIV_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "system_mm32.h"
#include "resource.h"

#include "drv.h"
#include "div.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_DIV
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DIV_Private_Variables
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DIV_Exported_Functions
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
    HANDLE hDIV = CreateFile(emIP_DIV);
    if (hDIV == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_DIV_DCB dcb = {
        .hSub       = emFILE_DIV,
        .usign      = false,
        .zero       = false
    };

// Step 4:  Open File Device     ---------------------->>>>>
    OpenFile(hDIV, (void*)&dcb);

    while (1){
// Step 5:  Write File Device    --------------------->>>>>
        for (int i = 0; i < 30; i++) {
            memcpy(&div.source, dataBuffer_1 + 4 * i, 8);
            if (!WriteFile(hDIV, emFILE_DIV, (u8*)&div.source, 4)) {
                while(1);       // Divisor zero error
            }

            quotient = div.result.quotr;    // Division quotient
            residue  = div.result.rmdr;     // Residue of division
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
            vdLED <<= 1;
            if (vdLED >= 0x10) vdLED = 0x0001;
        }
#endif

    }
}

/// @}


/// @}

/// @}

