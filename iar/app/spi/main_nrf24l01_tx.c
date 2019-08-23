////////////////////////////////////////////////////////////////////////////////
/// @file     MAIN_NRF24L01_TX.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE MAIN_NRF24L01_TX EXAMPLE.
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
#define _SPI_FLASH_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "spi_flash.h"

#include "nrf24l01.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_SPI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{


#define BUFFER_SIZE 20
u8 newTxBuffer[BUFFER_SIZE];
u8 readIDBuffer[3];


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

    u8 buff[DATA_PACKET];

    while(!nrf_init())                  //init NRF24L01+
    {
    }


    for(u8 i = 0; i < DATA_PACKET; i++){
        buff[i] = i + 1;
    }

    while(1){
        if(nrf_tx(buff,DATA_PACKET) == 1 ) {          //send packet buff(inculde 32 bytes)

            while(nrf_tx_state() == NRF_TXING);         //wait tx end

            if( NRF_TX_OK == nrf_tx_state () )
            {
                buff[0] ++;
            }
        }
    }
}
