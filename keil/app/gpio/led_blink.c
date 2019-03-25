////////////////////////////////////////////////////////////////////////////////
/// @file     LED_BLINK.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE LED_BLINK EXAMPLE.
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

/* -----------------------------------------------------------------------------
  THE PROGRAM IS SUITABLE FOR MM32 MINIBOARD DEVELOPMENT BOARD AND SUPPORTS
  MM32F031/MM32L0XX/MM32F103 FOR REFERENCE IN NON BLOCKING AND BLOCKING
  PROGRAMMING.
  WHEN CHOOSING NON BLOCKING AND BLOCKING PROGRAMMING, USERS MUST DEFINE OR
  CANCEL THE DEFINITION OF NONBLOCK MACROS.

  Copyright(c) 2017-2018 MindMotion, MM32 Library Ver.2.00 beta.0
  Author : ZhangQ
----------------------------------------------------------------------------- */

// Define to prevent recursive inclusion  --------------------------------------
#define _LED_BLINK_C_

// Files includes  -------------------------------------------------------------
#include "types.h"
#include "system_mm32.h"
#include "led_blink.h"
#include "hal_rcc.h"
#include "bsp_led.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_GPIO
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup GPIO_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Functions
/// @{

/* ---- Macro definition --- */
#define NONBLOCK
/* ------------------------- */

/* ------------------------------------------------- */
/* ---   Non blocking LED Display demo program  ---- */
/* ------------------------------------------------- */



#ifdef NONBLOCK

#if (0)

void SysTick_Handler(void)
{
    if (tickCnt++ >= 500) {
        tickCnt  = 0;
        tickFlag = true;
    }
}

int main(void)
{
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
    MCUID = SystemInit(SYSCLK_HSI_96MHz, emSYSTICK_On);
#endif

#if defined(__MM0Q1)
    MCUID = SystemInit(SYSCLK_HSI_48MHz, emSYSTICK_On);
#endif

    BSP_LED_Configure();

    while (1) {
        if (tickFlag) {
            tickFlag = false;
            CloseLED();
            switch (pipeCnt) {
                case 0: LD6_on(); break;
                case 1: LD5_on(); break;
                case 2: LD4_on(); break;
                case 3: LD3_on(); pipeCnt = 0xFF;
                default: break;
            }
            pipeCnt++;
        }
        // User Program...
    }
}

#else

void SysTick_Handler(void)
{
    static u8 ledStatus = 4;
    if (tickCnt++ >= 500) {
        tickCnt  = 0;
        tickFlag = true;
    }
    if (ledStatus != pipeCnt) {
        ledStatus = pipeCnt;
        CloseLED();
        switch (ledStatus) {
            case 0: LD6_on(); break;
            case 1: LD5_on(); break;
            case 2: LD4_on(); break;
            case 3: LD3_on();
            default: break;
        }
    }
}

int main(void)
{
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
    MCUID = SystemInit(SYSCLK_HSI_96MHz, emSYSTICK_On);
#endif

#if defined(__MM0Q1)
    MCUID = SystemInit(SYSCLK_HSI_48MHz, emSYSTICK_On);
#endif

    BSP_LED_Configure();

    while (1) {
        if (tickFlag) {
            tickFlag = false;
            if (pipeCnt++ == 3)
                pipeCnt = 0;
        }
        // User Program...
    }
}

#endif

////////////////////////////////////////////////////////////////////////////////
#else
////////////////////////////////////////////////////////////////////////////////

/* ------------------------------------------------- */
/* ---   Blocking LED Display demo program Start --- */
/* ------------------------------------------------- */

void SysTick_Handler(void)
{
    if (tickCnt++ >= 500) {
        tickCnt  = 0;
        tickFlag = true;
    }
}

int main(void)
{
    SystemInit();
    CoreID = DBGMCU_GetDEVID();

    BSP_LED_Configure();

    while (1) {
        CloseLED();
        LD6_on();
        while (!tickFlag)
            ;
        tickFlag = false;

        CloseLED();
        LD5_on();
        while (!tickFlag)
            ;
        tickFlag = false;

        CloseLED();
        LD4_on();
        while (!tickFlag)
            ;
        tickFlag = false;

        CloseLED();
        LD3_on();
        while (!tickFlag)
            ;
        tickFlag = false;

        // Blocking !!!
    }
}

#endif /* NONBLOCK and Block  Demo Program*/

/* ------------------------------------------------- */
/* ---   Blocking LED Display demo program End   --- */
/* ------------------------------------------------- */

/// @}


/// @}

/// @}


