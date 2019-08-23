////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_DMA_SLAVE_TX.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE SPI_DMA_SLAVE_TX EXAMPLE.
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
#define _SPI_DMA_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "spi_dma.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_SPI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{

#define BUFFER_SIZE 200
u8 newTxBuffer[BUFFER_SIZE];
u8 newRxBuffer[BUFFER_SIZE];


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  : None.
/// @retval : None.
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
//  @retval None
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
//  @retval None
////////////////////////////////////////////////////////////////////////////////
void RxCallback()
{
    /* Set transmission flag: transfer complete */
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles compare.
/// @param  pBuffer: Pointer points to a buffer.
/// @param  pBuffer: Pointer points to another buffer.
/// @param  BufferLength: buffer length.
/// @retval : status.
////////////////////////////////////////////////////////////////////////////////
u8 Buffercmp(const u8* pBuffer, u8* pBuffer1, u16 BufferLength)
{
    u16 i = 0;

    for(i = 0; i < BufferLength; i++){
        if(*pBuffer != *pBuffer1){
            return i;
        }
        pBuffer++;
        pBuffer1++;
    }
    return 0;
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
        .hSub               = emFILE_SPI2,
        .type               = emTYPE_DMA,           // emTYPE_Polling,emTYPE_IT,emTYPE_DMA,
        .block              = emTYPE_Block,         // emTYPE_Blocking, emTYPE_NonBlocking

// Callback function
        .sync               = emTYPE_Sync,          // Sync, ASync
        .cbTx               = (u32)&TxCallback,    //tx callback function access address
        .cbRx               = (u32)&RxCallback,    //rx callback function access address

// operation mode
        .remapEn            = false,                // Disable : 0 ,Enable : 1
        .remapIdx           = 0,                    // u8 value

// SPI parameter
        .prescaler          = 4,
        .fastMode           = true,
        .mode               = emSPI_MODE_0,
        .hardNss            = false,
        .firstLsb           = false,
        .master             = false
    };

    if (!OpenFile(hSPI, (void*)&dcb))       while(1);

    for (u8 i = 0; i < sizeof(newTxBuffer); i++) {
        newTxBuffer[i] = i + 1;
    }

    while(1){
        if(!success && WriteFile(hSPI, emFILE_SPI2, newTxBuffer, BUFFER_SIZE)){
            success = true;
        }
    }
}
