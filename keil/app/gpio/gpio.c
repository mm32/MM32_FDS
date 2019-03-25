////////////////////////////////////////////////////////////////////////////////
/// @file     GPIO.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE GPIO EXAMPLE.
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
#define _GPIO_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "gpio.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_GPIO
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup GPIO_Exported_Constants
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
/// @brief  GPIO example
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
// Step 1:  Setting System Clock     ------------------>>>>>
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    //MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
    MCUID = SetSystemClock(emSYSTICK_On, NULL);

// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hGPIO = CreateFile(emIP_GPIO);
    if (hGPIO == NULL)      while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_GPIO_DCB dcb;
    dcb.hSub    = emFILE_GPIOA;                                 // GPIOA
    dcb.dir     = 0x7FFF;                                       // 0: output  1: input
    dcb.mode    = 0x0001;                                       // when output mode,  0: pull-push,  1: od
                                                                // input mode, if inSta 0: ipd, 1: ipu
    dcb.state   = 0xFFFF;                                       // 0: HiZ, 1: ipd or ipu
// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hGPIO, (void*)&dcb))  while(1);

// Step 5:  Assignment DCB structure    --------------->>>>>
    dcb.hSub    = emFILE_GPIOB;         // GPIOB
    dcb.dir     = 0xFFC7;
    dcb.mode    = 0x0C07;
    dcb.state   = 0xFFFF;
// Step 6:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hGPIO, (void*)&dcb))  while(1);

    u16 out, in;
    while (1) {
        if (ReadFile(hGPIO, emFILE_GPIOB, (u8*)&out, 2) == 2) {
            in = out;

            if (in & 0x0400) {                                  // key3
                out = 0x0000;                                   // Bit 15: Red LED On
                WriteFile(hGPIO, emFILE_GPIOA, (u8*)&out, 2);
            }
            else {
                out = 0x8000;                                   // Bit 15: Red LED Off
                WriteFile(hGPIO, emFILE_GPIOA, (u8*)&out, 2);
            }

            if (in & 0x0800) {                                  // key4
                out = 0x0018;                                   // Bit 5: Green LED On
                WriteFile(hGPIO, emFILE_GPIOB, (u8*)&out, 2);
            }
            else {
                out = 0x0038;                                   // Bit 5: Green LED Off
                WriteFile(hGPIO, emFILE_GPIOB, (u8*)&out, 2);
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
        }
#endif
    }
}

/// @}


/// @}

/// @}


