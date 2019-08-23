////////////////////////////////////////////////////////////////////////////////
/// @file     RTC_TAMPER.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE RTC_TAMPER EXAMPLE.
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
#define _RTC_TAMPER_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include <time.h>

#include "system_mm32.h"
#include "drv.h"

#include "rtc.h"
#include "rtc_app.h"
#include "resource.h"


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
/// @brief  open RTC example
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{

    while(1);
}

/// @}

/// @}

/// @}
