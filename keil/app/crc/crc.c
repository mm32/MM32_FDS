////////////////////////////////////////////////////////////////////////////////
/// @file     CRC.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE CRC EXAMPLE.
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
#define _CRC_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "crc.h"
#include "resource.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_CRC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CRC_Exported_Functions
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
/// @brief  The main function will do the crc for the given value, and if the
///         value is equal to the expected value, the okCRC flag will be
///         erected.
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
    HANDLE hCRC = CreateFile(emIP_CRC);
    if (hCRC == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_CRC_DCB dcb = {
        .hSub       = emFILE_CRC                    /// Device
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hCRC, (void*)&dcb))       while(1);

    for (u8 i = 0; i < 10; i++) {
        okCRC = false;
        WriteFile(hCRC, emFILE_CRC, (u8*)dataBuffer, sizeof(dataBuffer));
        ReadFile(hCRC, emFILE_CRC, (u8*)&resultCRC, 4);
        okCRC = (resultCRC == EXPECTED_VALUE_64) ?  true : false;
    }
    CloseFile(hCRC);

    while(1){
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





