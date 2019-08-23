////////////////////////////////////////////////////////////////////////////////
/// @file     UART_IT_BLOCKING.C
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE UART_IT_BLOCKING EXAMPLE.
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
#define _UART_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "types.h"

#include "system_mm32.h"

#include "drv.h"
#include "uart.h"
#include "resource.h"



////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
///@addtogroup EXAMPLE_UART
///@{


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Functions
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
///  @brief  Tx Transfer completed callback
///  @param  fPtr: UART callback formal parameter.
///  @note   This example shows a simple way to report end of IT or DMA Tx transfer, and
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
///  @param  fPtr: UART callback formal parameter.
///  @note   This example shows a simple way to report end of IT or DMA Rx transfer, and
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
int main(void)
{
// Step 1:  Setting System Clock     ------------------>>>>>
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
    //  MCUID = SetSystemClock(emSYSTICK_On, NULL);

// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hUART = CreateFile(emIP_UART);
    if (hUART == NULL)      while(1);


    u8 newTxBuffer[TX_DATA_SIZE];
    u8 newRxBuffer[RX_DATA_SIZE];

    u16 txBufSize = sizeof(newTxBuffer);
    u16 rxBufSize = sizeof(newRxBuffer);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_UART_DCB dcb = {
// Base parameter
        .hSub       = emFILE_UART1,         // emFILE_UART1,emFILE_UART2 ..
        .type       = emTYPE_IT,            // emTYPE_polling, emTYPE_IT, emTYPE_DMA

// Callback function
        .cbTx       = (u32)&TxCallback,
        .cbRx       = (u32)&RxCallback,

        .block      = emTYPE_Block,         // emTYPE_Block, emTYPE_Non_Block
        .sync       = emTYPE_Sync,          // emTYPE_Sync, emTYPE_ASync
        .timeOut    = 0,                    // u16 value

        .remapEn    = false,                // 0, 1
        .remapIdx   = 0,                    // u8 value

// UART parameter
        .baudRate   = 115200,               // u32 value
        .width      = 8,                    // 5,6,7,8;
        .stop       = 0,                    // 0,1,2,3  : UART_StopBits_1,  2, 0_5, 1_5
        .parity     = emUART_PARITY_None,   // emUART_Parity_None, emUART_Parity_Even, emUART_Parity_Odd;
        .hardFlow   = 0,                    // 0,1,2,3   0: UART_HardwareFlowControl_None;
        .modeTxRx   = emTXRX_TxRx           // emTXRX_Tx,   emTXRX_Rx,,     emTXRX_TxRx,
    };



// Step 4:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hUART, (void*)&dcb))      while(1);


    memset(newTxBuffer, 0xAA, sizeof(newTxBuffer));
    memset(newRxBuffer, 0x00, sizeof(newRxBuffer));

    while (1) {
        memset(newTxBuffer, 0x55, sizeof(newTxBuffer));
        WriteFile(hUART, 0, newTxBuffer, txBufSize);

        memset(newTxBuffer, 0xAA, sizeof(newTxBuffer));
        WriteFile(hUART, 0, newTxBuffer, txBufSize);

        int ret = ReadFile(hUART, 0, newRxBuffer, rxBufSize);
        if (ret == emRETURN_TimeOut) {
            for (u32 i = 0; i < 200; i++){
                for (u32 j = 0; j < 50000; j++);
                ret = ReadFile(hUART, 0, newRxBuffer, sizeof(newRxBuffer));
                if (ret >  0)
                    break;
            }
            if (txSuccess) {
                toggle    = !toggle;
                (toggle) ? memset(newTxBuffer, 0x30, sizeof(newTxBuffer)) : memset(newTxBuffer, 0x38, sizeof(newTxBuffer));
                txSuccess = false;
            }
        }
        if (ret == sizeof(newRxBuffer)) {
            if (txSuccess) {
                toggle    = !toggle;
                (toggle) ? memset(newTxBuffer, 0x32, sizeof(newTxBuffer)) : memset(newTxBuffer, 0x39, sizeof(newTxBuffer));
                txSuccess = false;
                // process receive packet;
            }
        }
    }
}

/// @}


/// @}

/// @}
