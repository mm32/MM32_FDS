////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_COMMUNICATION.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE SPI_COMMUNICATION EXAMPLE.
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
#define _SPI_COMMUNICATION_C_
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "MM32.h"
#include <string.h>

#include "hal_rcc.h"
#include "hal_gpio.h"

#include "hal_spi.h"

#include "spi.h"

////////////////////////////////////////////////////////////////////////////////
// SPI communication using interrupt
// Need to short MISO and MOSI
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(void)
{
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
    MCUID = SystemInit(SYSCLK_HSI_96MHz, emSYSTICK_On);
#endif

#if defined(__MM0Q1)
    MCUID = SystemInit(SYSCLK_HSI_48MHz, emSYSTICK_On);
#endif

    // parameter initial-------------------------------------------------------//
    SPI_datalength = 16;
    SPI_TX_count   = 0;
    SPI_RX_count   = 0;
    // initial tx data--------------------------------------------------------//
    u8 i;
    for (i = 0; i < SPI_datalength; i++)
        txbyte[i] = i;
    // initial rx data--------------------------------------------------------//
    memset(rxbyte, 0xFF, SPI_datalength);

    // initial spi    --------------------------------------------------------//
    SPI_InitTypeDef SPI_InitStructure;

    // SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth         = 8;    //SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;              // mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;            // mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;              //
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;  // SPI Maximum frequency 40MHz
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_LSB;

#if defined(SPI_MASTER_MODE)
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

#elif defined(SPI_SLAVE_MODE)
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
#endif

    SPI_Configure(SPI2, &SPI_InitStructure);  // use SPI2

    // tx and rx start---------------------------------------------------------//
    SPI_Cmd(SPI2, ENABLE);
    SPI_CSInternalSelected(SPI2, ENABLE);

#if defined(SPI_MASTER_MODE)
    SPI_SendData(SPI2, txbyte[SPI_TX_count++]);
#endif

#if defined(SPI_MASTER_MODE)
    while (!TX_finish_flag) {
        ;
    }  // wait
//  //SPI_ClearITPendingBit(SPI2, SPI_IT_TXEPT);
//  //for(int i = 0; i< 100 ;i++){;}
//  SPI_ITConfig(SPI2,SPI_IT_TXEPT,DISABLE);
//  SPI_CSInternalSelected(SPI2,DISABLE);
#elif defined(SPI_SLAVE_MODE)
    while (!RX_finish_flag) {
        ;
    }  // wait //wait
    SPI_ITConfig(SPI2, SPI_IT_RX, DISABLE);
#endif

    // SPI_Cmd(SPI2,DISABLE);
    // End---------------------------------------------------------------------//
    while (1) {}
}
