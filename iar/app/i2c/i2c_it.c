////////////////////////////////////////////////////////////////////////////////
/// @file     I2C_IT.C
/// @author   S Yi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE I2C_IT EXAMPLE.
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
#define _I2C_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "system_mm32.h"

#include "drv.h"
#include "i2c.h"

#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_I2C
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup I2C_Exported_Functions
/// @{

#define TX_DATA_SIZE 128
#define RX_DATA_SIZE 128

u8 newTxBuffer[TX_DATA_SIZE] = {
//    0xa0,       // Slave Address
//    0x00,       // SubAddress

    0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x11,
    0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99
};

u8 newRxBuffer[RX_DATA_SIZE];

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if (tickCnt++ >= 10) {
        tickCnt  = 0;
        tickFlag = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Tx Transfer completed callback
//  @param  fPtr: I2C handle.
//  @note   This example shows a simple way to report end of IT or DMA Tx transfer, and
//          you can add your own implementation.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void TxCallback(void* fPtr)
{
    // Set transmission flag: transfer complete
    if (tickCnt > 0)
        tickCnt++;
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Rx Transfer completed callback
//  @param  fPtr: I2C handle
//  @note   This example shows a simple way to report end of IT or DMA Rx transfer, and
//          you can add your own implementation.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void RxCallback(void* fPtr)
{
    // Set transmission flag: transfer complete
    if (tickCnt > 10)
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
    HANDLE hI2C= CreateFile(emIP_I2C);
    if (hI2C == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_I2C_DCB dcb = {
// Base parameter
        .hSub       = emFILE_I2C1,
        .type       = emTYPE_IT,            // emTYPE_Polling, emTYPE_IT or emTYPE_DMA
// Callback function
        .cbTx       = (u32)&TxCallback,
        .cbRx       = (u32)&RxCallback,

        .block      = emTYPE_Block,         // emTYPE_Block or emTYPE_Non_Block
        .sync       = emTYPE_Sync,          // emTYPE_Sync or emTYPE_ASync
        .remapEn    = true,                 // GPIO remap or not
        .remapIdx   = 0,                    // GPIO remap index
        .timeOut    = 0,
// I2C parameter
        .fast       = false,                // fast or standard speed mode
        .master     = true,                 // master or slave mode
        .ownaddr    = 0x20,                 // I2C Own Address
        .slave      = 0xa0,                 // slave address
        .subAddr    = 0x00,                 // SubAddress
        .subSize    = 1                     // SubAddress size
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hI2C, (void*)&dcb))       while(1);

    while(1){
        if (!txSuccess && i2cReady && WriteFile(hI2C, emFILE_I2C1, newTxBuffer, 10)) {
            txSuccess = true;
            i2cReady = false;
        }

        if (tickFlag) {
            tickFlag = false;
            i2cReady = true;
        }

        if (!rxSuccess && i2cReady && ReadFile(hI2C, emFILE_I2C1, newRxBuffer, 5)) {
            rxSuccess = true;
            i2cReady = false;
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
