////////////////////////////////////////////////////////////////////////////////
/// @file     UART_DMA.C
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE UART_DMA EXAMPLE.
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
/// @addtogroup EXAMPLE_UART
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Constants
/// @{

u8 newTxBuffer[TX_DATA_SIZE];
u8 newRxBuffer[RX_DATA_SIZE];
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
    if (tickCnt++ >= 200) {
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
    /* Set transmission flag: transfer complete */
    if (tickCnt > 0)
        tickCnt++;
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
    /* Set transmission flag: transfer complete */
    if (tickCnt > 10)
        tickCnt++;
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
    /* When the parameter is not NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);

    /* When the parameter is NULL, AppTaskTick function not be used */
    //  MCUID = SetSystemClock(emSYSTICK_On, NULL);

// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hUART = CreateFile(emIP_UART);
    if (hUART == NULL)      while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_UART_DCB dcb = {
// Base parameter
        .hSub       = emFILE_UART1,         // emFILE_UART1,emFILE_UART2 ..
        .type       = emTYPE_DMA,           // emTYPE_Polling, emTYPE_IT, emTYPE_DMA

// Callback function
        .cbTx       = (u32)&TxCallback,
        .cbRx       = (u32)&RxCallback,

// Operation mode
        .block      = emTYPE_Non_Block,     // emTYPE_Block, emTYPE_Non_Block
        .sync       = emTYPE_Sync,          // emTYPE_Sync, emTYPE_ASync
        .remapEn    = false,                // 0, 1
        .remapIdx   = 0,                    // u8 value
        .timeOut    = 0,                    // u16 value

// UART parameter
        .baudRate   = 115200,               // u32 value
        .width      = 8,                    // 5,6,7,8;
        .stop       = 0,                    // 0,1,2,3  : UART_StopBits_1,  2, 0_5, 1_5
        .parity     = emUART_PARITY_None,   // emUART_PARITY_None, emUART_PARITY_Even, emUART_PARITY_Odd;
        .hardFlow   = 0,                    // 0,1,2,3   0: UART_HardwareFlowControl_None;
        .modeTxRx   = emTXRX_TxRx           // emTXRX_Tx,   emTXRX_Rx,,     emTXRX_TxRx,
    };

// Step 4:  Open File Device     ---------------------->>>>>
#if defined(__MM3N1)
    dcb.hSub        = emFILE_UART3;
    if (!OpenFile(hUART, (void*)&dcb))      while(1);
#endif
    dcb.hSub        = emFILE_UART1;
    if (!OpenFile(hUART, (void*)&dcb))      while(1);

    dcb.hSub        = emFILE_UART2;
    if (!OpenFile(hUART, (void*)&dcb))      while(1);

    memset(newTxBuffer, 0x30, sizeof(newTxBuffer));
    memset(newRxBuffer, 0x00, sizeof(newRxBuffer));

    while (1) {
// Step 5:  Write File Device    --------------------->>>>>
        if (!txSuccess && WriteFile(hUART, emFILE_UART1, newTxBuffer, sizeof(newTxBuffer)) == sizeof(newTxBuffer)) {
            txSuccess = true;
            // Process Send packet
        }

// Step 6:  Read File Device    ---------------------->>>>>
        int ret = ReadFile(hUART, emFILE_UART1, newRxBuffer, sizeof(newRxBuffer));
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
            if (vdLED == 0) vdLED = 1;
            else vdLED <<= 1;
            if (vdLED >= 0x10) vdLED = 0x0001;
        }
#endif
    }
}

/// @}


/// @}

/// @}
