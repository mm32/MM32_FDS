////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_DMA.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE SPI_DMA EXAMPLE.
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

////////////////////////////////////////////////////////////////////////////////
#define _SPI_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "spi.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_SPI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{


#define TX_DATA_SIZE 16
#define RX_DATA_SIZE 16


u32 newTxBuffer[TX_DATA_SIZE];
u32 newRxBuffer[RX_DATA_SIZE];


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
//SPI data send example ,polling ,interruput, DMA
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  @brief  Tx Transfer completed callback
//  @param  fPtr:
//  @note   This example shows a simple way to report end of IT or DMA Tx transfer, and
//          you can add your own implementation.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void TxCallback()
{
    /* Set transmission flag: transfer complete */
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Rx Transfer completed callback
//  @param  fPtr:
//  @note   This example shows a simple way to report end of IT or DMA Rx transfer, and
//          you can add your own implementation.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void RxCallback()
{
    /* Set transmission flag: transfer complete */
}

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
    HANDLE hSPI = CreateFile(emIP_SPI);
    if (hSPI == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_SPI_DCB dcb = {
// Base parameter
        .hSub       = emFILE_SPI2,
        .type       = emTYPE_DMA,               // emTYPE_Polling,emTYPE_IT,emTYPE_DMA,

// Callback function
        .cbTx       = (u32)&TxCallback,         //tx callback function access address
        .cbRx       = (u32)&RxCallback,         //rx callback function access address

// operation mode
        .block      = emTYPE_Block,             // emTYPE_Blocking, emTYPE_NonBlocking
        .sync       = emTYPE_Sync,              // Sync, ASync
        .remapEn    = false,                    // Disable : 0 ,Enable : 1
        .remapIdx   = 0,                        // u8 value
        .timeOut    = 100,                      // u16 value

// SPI parameter
        .width      = 32,                       // 1~32
        .speed      = 64,
        .mode       = emSPI_MODE_0,
        .softNss    = true,
        .firstMSB   = false,
        .master     = true,
        .txRx       = emTXRX_TxRx
    };

    OpenFile(hSPI, (void*)&dcb);

    memset(newTxBuffer, 0x30, sizeof(newTxBuffer));
    memset(newRxBuffer, 0x00, sizeof(newRxBuffer));


    txSuccess = false;
    rxSuccess = false;
    bool toggle = false;


    while (1) {
// Step 4:  Write File Device    --------------------->>>>>
        if (!txSuccess && WriteFile(hSPI, emFILE_SPI2,(u8*)newTxBuffer, sizeof(newTxBuffer)/4)) {
            txSuccess = true;
            // Process Send packet
        }

// Step 5:  Read File Device    ---------------------->>>>>
        int ret = ReadFile(hSPI, emFILE_SPI2, (u8*)newRxBuffer, sizeof(newRxBuffer)/4);
        if (ret == emRETURN_TimeOut) {
            for (u32 i = 0; i < 200; i++){
                for (u32 j = 0; j < 50000; j++);
                ret = ReadFile(hSPI, emFILE_SPI2, (u8*)newRxBuffer, sizeof(newRxBuffer));
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
