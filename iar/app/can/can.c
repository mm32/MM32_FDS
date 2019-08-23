////////////////////////////////////////////////////////////////////////////////
/// @file     CAN.C
/// @author
/// @version
/// @date
/// @brief    THIS FILE PROVIDES ALL THE CAN EXAMPLE.
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
#define _CAN_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "types.h"
#include "system_mm32.h"
#include "drv.h"
#include "can.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_CAN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CAN_Exported_Functions
/// @{

#define BUFFER_SIZE 8

u8 newRxBuffer[BUFFER_SIZE];
u8 newTxBuffer[BUFFER_SIZE] = { 0x21, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87,0x98 };

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
///  @brief  Tx Transfer completed callback
///  @param  fPtr: CAN callback formal parameter.
///  @note   This example shows a simple way to report end of IT Tx transfer, and
///          you can add your own implementation.
///  @retval None.
////////////////////////////////////////////////////////////////////////////////
void TxCallback(void* fPtr)
{
    if (tickCnt > 0)
        tickCnt++;
    /* Set transmission flag: transfer complete */
}

////////////////////////////////////////////////////////////////////////////////
///  @brief  Rx Transfer completed callback
///  @param  fPtr: CAN callback formal parameter.
///  @note   This example shows a simple way to report end of IT Rx transfer, and
///          you can add your own implementation.
///  @retval None.
////////////////////////////////////////////////////////////////////////////////
void RxCallback(void* fPtr)
{
    if (tickCnt > 10)
        tickCnt++;
    /* Set transmission flag: transfer complete */
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Main program
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main (void)
{
// Step 1:  Setting System Clock     ------------------>>>>>
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
    //  MCUID = SetSystemClock(emSYSTICK_On, NULL);

// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hCAN = CreateFile(emIP_CAN);
    if (hCAN == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_CAN_DCB dcb = {
        .hSub       = emFILE_CAN1,                                              // emFILE_CAN1,emFILE_CAN1_PELI.
        .canType    = emCAN_Peli,                                               //emCAN_Basic,emCAN_Peli
        .type       = emTYPE_IT,                                                // emTYPE_IT, emTYPE_Polling
        .block      = emTYPE_Block,                                             // emTYPE_Block, emTYPE_Non_Block
        .sync       = emTYPE_Sync,                                              // Sync, ASync
        .remapEn    = 1,                                                        // 0, 1
        .remapIdx   = 0,                                                        // GPIO remap index
        .baudRate   = 100000,                                                   // baudRate
        .sample     = emTriple_Sample,                                          // emTriple_Sample,emSingle_Sample
        .testMode   = emCAN_SeftTestMode,                                       // CAN_SeftTestMode,CAN_ListenOnlyMode.

        .id         = 0x123,
        .cbTx       = (u32)&TxCallback,
        .cbRx       = (u32)&RxCallback
    };

// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hCAN, (void*)&dcb))       while(1);

    while(1) {
// Step 4:  Write File Device    --------------------->>>>>
        if (!txSuccess && WriteFile(hCAN, emFILE_CAN1, newTxBuffer, sizeof(newTxBuffer))) {
            rxSuccess = false;
            txSuccess = true;
// Step 5:  Read File Device    ---------------------->>>>>
            if (txSuccess &&ReadFile(hCAN, emFILE_CAN1, newTxBuffer, sizeof(newTxBuffer))) {
                rxSuccess = true;
            }
        }
    }
}


/// @}


/// @}

/// @}
