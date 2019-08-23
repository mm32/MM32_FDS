////////////////////////////////////////////////////////////////////////////////
/// @file     POWER.C
/// @author   MY Zhang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE POWER EXAMPLE.
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

// Includes  -------------------------------------------------------------------
#include <string.h>

#include "system_mm32.h"

#include "drv.h"
#include "pwr.h"

#include "resource.h"


u16 cntKey2 = 0;
bool fKey2 = false;

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_PWR
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWR_Exported_Functions
/// @{


void SYNC_STOP_FallingCallback()
{
    if (tickCnt > 10)
        tickCnt++;
}

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

    cntKey2++;
    if (cntKey2 >= 10000)
        cntKey2 = 10000;
}

bool user_Key2()
{
#if defined(__MM3N1)
    return (inp16((u16*)(0x40010800 + 0x08)) & 0x0001) ? 1 : 0;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    return (inp16((u16*)(0x48000008)) & 0x0001) ? 1 : 0;
#endif
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
    HANDLE hSTOP = CreateFile(emIP_STOP);
    if (hSTOP == NULL)      while(1);


    while(1) {
        if(!fKey2 && user_Key2()) {
            if (cntKey2 >= 20)
                fKey2 = true;
        }
        else if(!user_Key2()) {
            cntKey2 = 0;
        }

        if  (fKey2 && user_Key2()) {
            cntKey2 = 0;

            while (user_Key2()){
                if (cntKey2 > 1000) {
                    vdLED = 0x00;
                    SysDisplay((u8*)&vdLED);
                }
            }

            if (cntKey2 > 1000) {
// Step 3:  Assignment DCB structure    --------------->>>>>
                tAPP_POWER_DCB dcb = {
                    .hSub       = 1,                                            // Device
                    .cb1        = (u32)&SYNC_STOP_FallingCallback,
                    .sync       = emTYPE_ASync
                };

// Step 4:  Open File Device     ---------------------->>>>>
                if (!OpenFile(hSTOP, (void*)&dcb))      while(1);
                WriteFile(hSTOP, 0, NULL, NULL);

                while(user_Key2());
                SysDisplay((u8*)&vdLED);
                SetSystemClock(emSYSTICK_On, NULL);
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
            vdLED <<= 1;
            if (vdLED >= 0x10) vdLED = 0x0001;
        }
#endif
    }
}

/// @}


/// @}

/// @}



