////////////////////////////////////////////////////////////////////////////////
/// @file     RTC_STANDBY.C
/// @author   MY Zhang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE RTC_STANDBY EXAMPLE.
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
#include <time.h>

#include "drv.h"
#include "pwr.h"

#include "resource.h"

u32 sTotalSecond;
u32 aTotalSecond;
u16 cntKey2 = 0;

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{
//////
/// @addtogroup EXAMPLE_PWR
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWR_Exported_Functions
/// @{


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

    if (cntKey2) cntKey2++;
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

    // Open LED
    vdLED = 0x03;
    SysDisplay((u8*)&vdLED);

// Step 2:  Create Power and RTC File Device     ---------------------->>>>>
    HANDLE hSTANDBY = CreateFile(emIP_POWER);
    if (hSTANDBY == NULL)       while(1);

    HANDLE hRTC = CreateFile(emIP_RTC);
    if (hRTC == NULL)       while(1);

// Step 3:  Open RTC    --------------->>>>>
    struct tm tim = {
        .tm_sec     = 0,
        .tm_min     = 0,
        .tm_hour    = 12,
        .tm_mday    = 25,
        .tm_mon     = 0,
        .tm_year    = 2019 - 1900,
        .tm_wday    = 2,
        .tm_yday    = 0,
        .tm_isdst   = 0
    };
    sTotalSecond = mktime(&tim);

    tAPP_RTC_DCB dcb1 = {
        .hSub           = emFILE_RTC,
        .sync           = emTYPE_ASync,                                         // emTYPE_Sync, emTYPE_ASync
        .cbSecond       = NULL,                                                 // second interrupt call back function
        .cbAlarm        = NULL,                                                 // (u32)&RTC_CallbackAlarm
        .second         = sTotalSecond
    };
    if (!OpenFile(hRTC, (void*)&dcb1))      while(1);

// Step 4:  Open Low-power Standby Mode    --------------->>>>>
    tAPP_POWER_DCB dcb = {
        .hSub           = emFILE_STANDBY,                                       // emFILE_STANDBY
        .cb1            = NULL,
        .sync           = emTYPE_ASync                                          // emTYPE_Sync or emTYPE_ASync
    };
    if (!OpenFile(hSTANDBY, (void*)&dcb))       while(1);

    while(1) {
        if (user_Key2() || cntKey2 > 1000) {

            if (!cntKey2) cntKey2++;                                            // First entering, start counting
            if (cntKey2 > 1000) {                                               // Key2 has been pressed for more than 1 seconds

                vdLED = 0x01;                                                   // Close LED2
                SysDisplay((u8*)&vdLED);

                if (!user_Key2()) {                                             // If released Key2, close all Led, set RTC AlarmCounter and entering Standby mode
                    vdLED = 0x00;
                    SysDisplay((u8*)&vdLED);
                    /* ====  Setting Alarm Counter  ==== */
                    aTotalSecond = 5;
                    hRTC->mode = emRTC_AlarmCounter;
                    WriteFile(hRTC, emFILE_RTC, (u8*)&aTotalSecond, sizeof(aTotalSecond));

                    /* ====  Setting Standby  ==== */
                    WriteFile(hSTANDBY, emFILE_STANDBY, NULL, NULL);
                }
            }
        }
        else {
            cntKey2 = 0;
        }
//      if (SysKeyboard(&vkKey)) {
//          switch  (vkKey) {
//          case  VK_K0:
//              KeyProcess_Key0();
//              break;
//          case  VK_K1:
//              KeyProcess_Key1();
//              break;
//          case  VK_K2:
//              KeyProcess_Key2();
//              break;
//          case  VK_K3:
//              KeyProcess_Key3();
//              break;
//          default:
//              break;
//          }
//      }
//#if defined(__SYS_RESOURCE_POLLING)               // Define macros in project options
//      if (tickFlag) {
//          tickFlag = false;
//          SysDisplay((u8*)&vdLED);
//          vdLED <<= 1;
//          if (vdLED >= 0x10) vdLED = 0x0001;
//      }
//#endif
    }
}







/// @}


/// @}

/// @}
