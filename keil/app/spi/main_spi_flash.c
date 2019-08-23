////////////////////////////////////////////////////////////////////////////////
/// @file     MAIN_SPI_FLASH.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE MAIN_SPI_FLASH EXAMPLE.
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
#define _MAIN_SPI_FLASH_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "spi_flash.h"

#include "main_spi_flash.h"

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
int main(void)
{
// Step 1:  Setting System Clock     ------------------>>>>>
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
//  MCUID = SetSystemClock(emSYSTICK_On, NULL);

    while(!spiFlashInit());

    for (u8 i = 0; i < sizeof(newTxBuffer); i++) {
        newTxBuffer[i] = i + 1;
    }

    spiFlashSectorErase(0x00);
    spiFlashBufferRead(0x00, newRxBuffer, sizeof(newRxBuffer));

    spiFlashPageWrite(0x00, newTxBuffer, sizeof(newTxBuffer));
    spiFlashBufferRead(0x00, newRxBuffer, sizeof(newRxBuffer));



    while(1){
    }
}
