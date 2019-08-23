////////////////////////////////////////////////////////////////////////////////
/// @file     COMP_PORT.C
/// @author   PX Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE COMP_PORT EXAMPLE.
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
#define _COMP_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "comp.h"

#include "resource.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_COMP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup COMP_Exported_Functions
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
//  @brief  COMP callback
//  @param  COMP channal number
//  @note   This example shows a simple way to report end of IT , and  you can
//          add your own implementation.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void COMP_Callback(u8 ch, bool val)
{
    /* Set Conversion flag: Conversion complete */
    tickCnt++;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  main function.
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
    HANDLE hCOMP = CreateFile(emIP_COMP);
    if (hCOMP == NULL)      while(1);

/*  __MM3N1 / __MM0N1 ---------------------------------------------------
        cpmpare 1:
            OUT:    PA0,  PA6,  PA11
            INP:    PA 0 : 7
            INM:    PA4, PA5, PA0, PA6
        cpmpare 2:
            OUT:    PA2,  PA7,  PA12
            INP:    PA 0 : 7
            INM:    PA4, PA5, PA2, PA6
-----------------------------------------------------------------------*/

/*  __MM0P1 -------------------------------------------------------------
        cpmpare 1:
            OUT:    PA0, PA6, PA11
            INP:    PD3,  PA6, PB2, PB10
            INM:    PB12, PA8, PD2

        cpmpare 2:
            OUT:    PA2, PA7, PA12, PC10
            INP:    PA11, PA6, PB2, PB10
            INM:    PB12, PA8, PA12

        cpmpare 3:
            OUT:    PA14£¬ PD4
            INP:    PB8
            INM:    PB9

        cpmpare 4:
            OUT:    PA13,  PD0,  PD5
            INP:    PD7
            INM:    PC13

        cpmpare 5:
            OUT:    PD1, PD 6
            INP:    PC14
            INM:    PC15
-----------------------------------------------------------------------*/

/*  __MM0Q1 -------------------------------------------------------------
        cpmpare 1:
            OUT:    PA0,  PA6,  PA11
            INP:    PA 1 : 4
            INM:    PA 5 : 7
-----------------------------------------------------------------------*/

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_COMP_DCB dcb = {
        .hSub           = emFILE_COMP1,         // emFILE_COMP1,emFILE_COMP2, ...,emFILE_COMP5
        .sync           = emTYPE_Sync,          // emTYPE_Sync, emTYPE_ASync
        .cb             = (u32)&COMP_Callback,  // CallBack function
        .type           = emTYPE_Polling,       // emTYPE_Polling, emTYPE_IT
        .inm            = 7,                    // bsp in mode 0..7:  invert input selection
        .inp            = 1,                    // bsp in mode 0..7:  noninvert input selection
        .out            = 0,                    // bsp out mode 0..3:  output selection
        .pol            = 0,                    //  0, 1: output polarity
        .mode           = 0,                    // 0,1,2,3
        .crv            = 0,                    // 0-15 : external reterence voltage select
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hCOMP, (void*)&dcb))      while(1);

    while (1) {
        vdLED =(ReadFile(hCOMP, emFILE_COMP1, 0, 0)) ?  0x01 : 0x02;

//      if (SysKeyboard(&vkKey)) {
//          switch (vkKey) {
//              case VK_K0:
//              case VK_K1:
//              case VK_K2:
//              case VK_K3:
//              default: break;
//          }
//      }

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
