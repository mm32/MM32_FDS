////////////////////////////////////////////////////////////////////////////////
/// @file     DAC_TRIANGLEWAVE.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE DAC_TRIANGLEWAVE EXAMPLE.
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
#define _DAC_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "dac.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_DAC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DAC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick(void)
{
    if (tickCnt++ >= 2) {
        tickCnt  = 0;
        tickFlag = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  The main function initializes dac channels 1 and 2, and the two
///         channels are triggered by TIM to output triangle wave.
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
    HANDLE hDAC = CreateFile(emIP_DAC);
    if (hDAC == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_DAC_DCB dcb = {
        .type       = emTYPE_Polling,       // Data transmission method emTYPE_DMA or NULL
        .trig   = true,                 // External trigger enable:0(software trigger), 1(Hardware trigger)
        .wave       = 2                     // DAC generated wave: 0(Not generated), 1(Noise), 2(Triangle)
    };

// Step 4:  Open File Device     ---------------------->>>>>
    dcb.hSub = emFILE_DAC_CH1;                          // DAC ch0
    if (!OpenFile(hDAC, (void*)&dcb))       while(1);

    dcb.hSub = emFILE_DAC_CH2;                          // DAC ch1
    if (!OpenFile(hDAC, (void*)&dcb))       while(1);

    WriteFile(hDAC, 0, 0, 0);               // Generate and output a triangle wave
    WriteFile(hDAC, 1, 0, 0);               // Generate and output a triangle wave

    while (1) {
        if (SysKeyboard(&vkKey)) {
            switch (vkKey) {
                case VK_K0:
                case VK_K1:
                case VK_K2:
                case VK_K3:
                default: break;
            }
        }

        SysDisplay((u8*)&vdLED);
    }
}

/// @}


/// @}

/// @}
