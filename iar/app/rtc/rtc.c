////////////////////////////////////////////////////////////////////////////////
/// @file     RTC.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE RTC EXAMPLE.
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
#define _RTC_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include <time.h>

#include "system_mm32.h"
#include "drv.h"

#include "rtc.h"
#include "rtc_app.h"
#include "resource.h"


u32 sTotalSecond;
u32 aTotalSecond;

u16 refreshCnt = 0;
bool refreshFlag = false;

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_RTC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RTC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if (vdLED > 0) {            // If the LED bright, Start counting
        if (tickCnt++ >= 200) {
            tickCnt  = 0;
            tickFlag = true;
            vdLED = 0;          // Time to close LED
        }
    }

    if (refreshCnt++ > 100) {
        refreshCnt = 0;
        refreshFlag = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  RTC second interrupt Callback function
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_CallbackSecond(u8 status)
{
    vdLED |= status;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  RTC alarm interrupt Callback function
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_CallbackAlarm(u8 status)
{
    vdLED |= status;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  open RTC example
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
 int main(void)
{
    struct tm tim = {
        .tm_sec     = 0,                            // second count, 0-59
        .tm_min     = 0,                            // minute count, 0-59
        .tm_hour    = 12,                           // hour count, 0-23
        .tm_mday    = 25,                           // day in month, 1-31
        .tm_mon     = 0,                            // month count, 0-11
        .tm_year    = 2019 - 1900,                  // year count, Actual year minus 1900
        .tm_wday    = 2,                            // week day, 0-6,0:Sunday, 1:Monday...
        .tm_yday    = 0,                            // day in year, do not need, it can be calculate by year, mon and mday automaticly
        .tm_isdst   = 0                             // Daylight saving time identifier, default is 0
    };

    sTotalSecond = mktime(&tim);


// Step 1:  Setting System Clock     ------------------>>>>>
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
    //  MCUID = SetSystemClock(emSYSTICK_On, NULL);

// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hRTC = CreateFile(emIP_RTC);
    if (hRTC == NULL)       while(1);


// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_RTC_DCB dcb = {
        .hSub           = emFILE_RTC,
//      .sync           = emTYPE_Sync,                          // emTYPE_Sync, emTYPE_ASync
        .sync           = emTYPE_ASync,                         // emTYPE_Sync, emTYPE_ASync
        .cbSecond       = (u32)&RTC_CallbackSecond,             // second interrupt call back function
        .cbAlarm        = (u32)&RTC_CallbackAlarm,              // alarm interrupt call back function
        .second         = sTotalSecond                          // Total seconds of preset time
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hRTC, (void*)&dcb))       while(1);

    /* ====  Setting curent Date / Time  ==== */
    hRTC->mode = emRTC_TimeCounter;
    WriteFile(hRTC, emFILE_RTC, (u8*)&sTotalSecond, sizeof(sTotalSecond));

    /* ====  Setting Alarm Counter  ==== */
    aTotalSecond = 3;
    hRTC->mode = emRTC_AlarmCounter;
    WriteFile(hRTC, emFILE_RTC, (u8*)&aTotalSecond, sizeof(aTotalSecond));

    while(1) {
        if ((dcb.sync == emTYPE_ASync) && (refreshFlag)) {
            refreshFlag = false;
            hRTC->mode = emRTC_TimeCalendar;
            int status = ReadFile(hRTC, emFILE_RTC, (u8*)&tim, sizeof(tim));

            vdLED = (u8)status;
            SysDisplay((u8*)&vdLED);

            if (status & 0x02) {
                hRTC->mode = emRTC_AlarmCounter;
                aTotalSecond = 2;
                WriteFile(hRTC, 0, (u8*)&aTotalSecond, sizeof(aTotalSecond));
            }
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
        }
#endif
    }
}

/// @}

/// @}

/// @}
