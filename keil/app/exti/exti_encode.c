////////////////////////////////////////////////////////////////////////////////
/// @file     EXTI_ENCODE.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE EXTI_ENCODE EXAMPLE.
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
#define _EXTI_DECODE_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "resource.h"
#include "exti_encode.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_EXTI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXTI_Exported_Functions
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
/// @brief  Orthogonal decoding
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Orth_Decode()
{
    u8 sum = 0;

    PhaseA_Pre_Status = PhaseA_Status;
    PhaseB_Pre_Status = PhaseB_Status;

#if defined(__MM3N1)
    PhaseA_Status = inp16((u16*)0x40010C00 + 0x0C) & 1 << 10;        // Key3(); GPIOB:10
    PhaseB_Status = inp16((u16*)0x40010C00 + 0x0C) & 1 << 11;        // Key4(); GPIOB:11
#else
    PhaseA_Status = inp16((u16*)0x48000400 + 0x0C) & 1 << 10;        // Key3(); GPIOB:10
    PhaseB_Status = inp16((u16*)0x40010C00 + 0x0C) & 1 << 11;        // Key4(); GPIOB:11
#endif
    //--------------------------------------------------------------------------
    //      A pre   B pre   A now   B now   status      sum +   -   err
    //      0       0       0       0       no_change   //
    //      0       0       0       1       -1          //      1
    //      0       0       1       0       +1          //  1
    //      0       0       1       1       [-2 or err] //          2-
    //      0       1       0       0       +1          //  1
    //      0       1       0       1       no_change   //
    //      0       1       1       0       [+2 or err] //          2+
    //      0       1       1       1       -1          //      3
    //      1       0       0       0       -1          //      1
    //      1       0       0       1       [+2 or err] //          2+
    //      1       0       1       0       no_change   //
    //      1       0       1       1       +1          //  3
    //      1       1       0       0       [-2 or err] //          2-
    //      1       1       0       1       +1          //  3
    //      1       1       1       0       -1          //      3
    //      1       1       1       1       no_change   //

    sum = PhaseA_Pre_Status + PhaseB_Pre_Status + PhaseA_Status + PhaseB_Status;

    if ((sum == 1 && (PhaseA_Status == 1 || PhaseB_Pre_Status == 1)) ||
        (sum == 3 && (PhaseA_Status == 0 || PhaseB_Pre_Status == 0))) {
        if(PulseDir != 1) {
            PulseCount = 0;
            PulseDir = 1;
        }

        PulseCount++;
        Position++;

    }

    if ((sum == 1 && (PhaseB_Status == 1 || PhaseA_Pre_Status == 1)) ||
        (sum == 3 && (PhaseB_Status == 0 || PhaseA_Pre_Status == 0))) {
        if(PulseDir != -1) {
            PulseCount = 0;
            PulseDir = -1;
        }

        PulseCount++;
        Position--;
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTI callback.
/// @param  line (0 ~ [32])
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void cbExtiPhaseA(bool level)
{
    Orth_Decode();
}

void cbExtiPhaseB(bool level)
{
    Orth_Decode();
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
    HANDLE hEXTI = CreateFile(emIP_EXTI);
    if (hEXTI == NULL)      while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_EXTI_DCB dcb = {
        .sync   = emTYPE_Sync,              // emTYPE_Sync
        .mode   = emEXTI_IT,                // emEXTI_IT, emEXTI_Event
        .edge   = emEDGE_RisingFalling      // emEDGE_Rising, emEXTI_Falling, emEDGE_RisingFalling
    };

// Step 4:  Open File Device     ---------------------->>>>>
    dcb.port = emGPIOB;                     // emGPIOB
    dcb.hSub = emFILE_EXTI10;               // Line 10
    dcb.cb   = (u32)&cbExtiPhaseA;
    if (!OpenFile(hEXTI, (void*)&dcb))      while(1);

    dcb.hSub = emFILE_EXTI11;               // Line 11
    dcb.cb   = (u32)&cbExtiPhaseB;
    if (!OpenFile(hEXTI, (void*)&dcb))      while(1);


    PhaseA_Pre_Status = 0;
    PhaseB_Pre_Status = 0;
    PhaseA_Status = 0;
    PhaseB_Status = 0;

    PulseCount = 0;
    PulseDir = 0;

    Position = 0;

    while (1) {
        // Position
        // PulseCount
        // PulseDir

#if defined(__SYS_RESOURCE_POLLING)
        if (SysKeyboard(&vkKey)) {
            switch (vkKey) {
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
