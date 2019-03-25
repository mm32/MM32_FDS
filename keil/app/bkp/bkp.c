////////////////////////////////////////////////////////////////////////////////
/// @file     BKP.C
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE BKP EXAMPLE.
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
#define _BKP_C_

// Includes  -------------------------------------------------------------------
#include <string.h>

#include "system_mm32.h"
#include "drv.h"

#include "bkp.h"
#include "resource.h"

u32 bkpBuff[10] = {0x0123, 0x4567,0x89ab, 0xcdef};
u32 value = 0;
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_BKP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BKP_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if (tickCnt++ >= 20) {
        tickCnt  = 0;
        tickFlag = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Main function, program entry.
/// @param  None.
/// @retval int
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
    HANDLE hBKP = CreateFile(emIP_BKP);
    if (hBKP == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_BKP_DCB dcb = {
        .hSub       = emFILE_BKP                    // BKP
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hBKP, (void*)&dcb))       while(1);

    ReadFile(hBKP, emFILE_BKP, (u8*)bkpBuff, sizeof(bkpBuff));

    bkpBuff[3] = bkpBuff[0];
    bkpBuff[0] = bkpBuff[1];
    bkpBuff[1] = bkpBuff[2];
    bkpBuff[2] = bkpBuff[3];

    WriteFile(hBKP, emFILE_BKP, (u8*)bkpBuff, sizeof(bkpBuff));

    ReadFile(hBKP, emFILE_BKP, (u8*)bkpBuff, sizeof(bkpBuff));

    outp((u32*)(*hBKP->instance + 4), 0x33cc);
    value =inp((u32*) (*hBKP->instance + 4));
    outp((u32*)(*hBKP->instance + 8), 0x55aa);
    outp((u32*)(*hBKP->instance + 12), 0x00ff);
    value = 0;
    ReadFile(hBKP, emFILE_BKP, (u8*)bkpBuff, sizeof(bkpBuff));

    CloseFile(hBKP);

    while (1) {
        if (SysKeyboard(&vkKey)) {
            switch  (vkKey) {
                case  VK_K0:    value++;            break;
                case  VK_K1:    value--;            break;
                case  VK_K2:    vdLED = 0x01;       break;
                case  VK_K3:    vdLED = 0x02;       break;
                default:                            break;
            }
        }
#if defined(__SYS_RESOURCE_POLLING)
        if (tickFlag) {
            tickFlag = false;
            SysDisplay((u8*)&vdLED);
        }
#endif
    }}

/// @}


/// @}

/// @}
