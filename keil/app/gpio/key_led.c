////////////////////////////////////////////////////////////////////////////////
/// @file     KEY_LED.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE KEY_LED EXAMPLE.
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
#define _KEY_LED_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_MM32.h"
#include "key_led.h"

#include "bsp_led.h"
#include "bsp_key.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_GPIO
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup GPIO_Exported_Functions
/// @{

/* ---- Macro definition --- */
//#define ASYNCHRONOUS
/* ------------------------- */
// SYNCHRONIZATION




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

#ifdef ASYNCHRONOUS
////////////////////////////////////////////////////////////////////////////////
/// @brief  Get Key value
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GetKeyValueB()
{
    if (!KEYB.event[0] && Key1()) {
        if ((!KEYB.press[0]) && (KEYB.dlyCnt[0]++ >= 20)) {
            KEYB.event[0] = true;
            KEYB.press[0] = true;
        }
    }
    else if (!Key1()) {
        KEYB.dlyCnt[0] = 0;
        KEYB.press[0]  = false;
    }

    if (!KEYB.event[1] && Key2()) {
        if ((!KEYB.press[1]) && (KEYB.dlyCnt[1]++ >= 20)) {
            KEYB.event[1] = true;
            KEYB.press[1] = true;
        }
    }
    else if (!Key2()) {
        KEYB.dlyCnt[1] = 0;
        KEYB.press[1]  = false;
    }

    if (!KEYB.event[2] && Key3()) {
        if ((!KEYB.press[2]) && (KEYB.dlyCnt[2]++ >= 20)) {
            KEYB.event[2] = true;
            KEYB.press[2] = true;
        }
    }
    else if (!Key3()) {
        KEYB.dlyCnt[2] = 0;
        KEYB.press[2]  = false;
    }

    if (!KEYB.event[3] && Key4()) {
        if ((!KEYB.press[3]) && (KEYB.dlyCnt[3]++ >= 20)) {
            KEYB.event[3] = true;
            KEYB.press[3] = true;
        }
    }
    else if (!Key4()) {
        KEYB.dlyCnt[3] = 0;
        KEYB.press[3]  = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  asynchronous key led example
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
//  MCUID = SetSystemClock(emSYSTICK_On, NULL);


//  SystemInit();
//
//    CoreID = DBGMCU_GetDEVID();

    memset(&KEYB, 0x00, sizeof(KEYB));
    memset(&LED, 0x00, sizeof(LED));

    BSP_KEY_Configure();
    BSP_LED_Configure();


    LED.status[0] = true;
    LD6_on();

    while (1) {
        if (KEYB.event[0]) {
            KEYB.event[0]  = false;
            LED.status[0] = !LED.status[0];
            LED.status[0] ? LD6_on() : LD6_off();
        }

        if (KEYB.event[1]) {
            KEYB.event[1]  = false;
            LED.status[1] = !LED.status[1];
            LED.status[1] ? LD5_on() : LD5_off();
        }

        if (KEYB.event[2]) {
            KEYB.event[2]  = false;
            LED.status[2] = !LED.status[2];
            LED.status[2] ? LD4_on() : LD4_off();
        }

        if (KEYB.event[3]) {
            KEYB.event[3]  = false;
            LED.status[3] = !LED.status[3];
            LED.status[3] ? LD3_on() : LD3_off();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
#else  // SYNCHRONIZATION
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief  set delay time
/// @param  ms: delay time
/// @retval bool: true: delay ms time
////////////////////////////////////////////////////////////////////////////////
bool delay(u16 ms)
{
    static u16 cnt = 0;
    if (cnt == 0) {
        cnt = ms;
    }
    else if (sysTickFlag1mS) {
        sysTickFlag1mS = false;
        if (cnt > 0)
            cnt--;
        if (cnt == 0)
            return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  set GPIO Callback function
/// @param  fPtr
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SYNC_GPIO_PutCallback(void* fPtr)
{
    /* Set transmission flag: transfer complete */
    if (tickCnt > 0)
        tickCnt++;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  get GPIO Callback function
/// @param  fPtr
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SYNC_GPIO_GetCallback(void* fPtr)
{
    /* Set transmission flag: transfer complete */
    if (tickCnt > 10)
        tickCnt++;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Synchronize KEY_LED example
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
//  MCUID = SetSystemClock(emSYSTICK_On, NULL);

    memset(&KEYB, 0x00, sizeof(KEYB));
    memset(&LED, 0x00, sizeof(LED));

    BSP_KEY_Configure();
    BSP_LED_Configure();

    LED.status[0] = true;
    LD6_on();

    while (1) {
        if (!KEYB.event[0] && Key1()) {
            if ((delay(20))) {
                KEYB.event[0]  = true;
                LED.status[0] = !LED.status[0];
                LED.status[0] ? LD6_on() : LD6_off();
            }
        }
        else if (!Key1()) {
            KEYB.event[0] = false;
        }

        if (!KEYB.event[1] && Key2()) {
            if ((delay(20))) {
                KEYB.event[1]  = true;
                LED.status[1] = !LED.status[1];
                LED.status[1] ? LD5_on() : LD5_off();
            }
        }
        else if (!Key2()) {
            KEYB.event[1] = false;
        }

        if (!KEYB.event[2] && Key3()) {
            if ((delay(20))) {
                KEYB.event[2]  = true;
                LED.status[2] = !LED.status[2];
                LED.status[2] ? LD4_on() : LD4_off();
            }
        }
        else if (!Key3()) {
            KEYB.event[2] = false;
        }

        if (!KEYB.event[3] && Key4()) {
            if ((delay(20))) {
                KEYB.event[3]  = true;
                LED.status[3] = !LED.status[3];
                LED.status[3] ? LD3_on() : LD3_off();
            }
        }
        else if (!Key4()) {
            KEYB.event[3] = false;
        }
    }
}

#endif

/// @}

/// @}

/// @}
