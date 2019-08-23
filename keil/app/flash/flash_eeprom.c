////////////////////////////////////////////////////////////////////////////////
/// @file     FLASH_EEPROM.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE FLASH_EEPROM EXAMPLE.
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
#define _FLASH_EEPROM_C_

// Includes  -------------------------------------------------------------------
#include <string.h>

#include "types.h"
#include "system_mm32.h"

#include "drv.h"

#include "flash_eeprom.h"

#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_FLASH
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup FLASH_Exported_Functions
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
/// @brief  EEPROM example
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
    HANDLE hEEPROM = CreateFile(emIP_EEPROM);
    if (hEEPROM == NULL)        while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_EEPROM_DCB dcb = {
        .erase  = false                     // do not erase EEPROM,
    };

// Step 4:  Open File Device     ---------------------->>>>>
    dcb.hSub    = emFILE_EEPROM1;           // EEPROM1
    dcb.size    = 16;                       // block size is 16 bytes
    if (!OpenFile(hEEPROM, (void*)&dcb))    while(1);

    dcb.hSub    = emFILE_EEPROM2;           // EEPROM2
    dcb.size    = 4;                        // block size is 4 bytes
    if (!OpenFile(hEEPROM, (void*)&dcb))    while(1);

    dcb.hSub    = emFILE_EEPROM3;           // EEPROM3
    dcb.size    = 8;                        // block size is 8 bytes
    if (!OpenFile(hEEPROM, (void*)&dcb))    while(1);

    dcb.hSub    = emFILE_EEPROM4;           // EEPROM4
    dcb.size    = 12;                       // block size is 12 bytes

    if (!OpenFile(hEEPROM, (void*)&dcb))    while(1);

    u16 i = 0;
    while(1) {
        if (SysKeyboard(&vkKey)) {
            memset(bufferWR, i + 1, 64);
            switch (vkKey) {
                case VK_K0:
                    WriteFile(hEEPROM, emFILE_EEPROM1, (u8*)bufferWR, 16);
                    break;

                case VK_K1:
                    WriteFile(hEEPROM, emFILE_EEPROM2, (u8*)bufferWR, 4);
                    break;

                case VK_K2:
                    WriteFile(hEEPROM, emFILE_EEPROM3, (u8*)bufferWR, 8);
                    break;

                case VK_K3:
                    WriteFile(hEEPROM, emFILE_EEPROM4, (u8*)bufferWR, 12);
                    break;
                default: break;
            }
            if (++i == 64) i = 0;
            memset(bufferRD, 0x00, 64);
            ReadFile(hEEPROM, 1, (u8*)bufferRD, 64);
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



