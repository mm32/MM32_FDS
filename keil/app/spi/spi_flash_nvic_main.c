////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_FLASH_NVIC_MAIN.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE SPI_FLASH_NVIC_MAIN EXAMPLE.
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
#define _SPI_FLASH_NVIC_MAIN_C_
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "MM32.h"
#include <string.h>

#include "hal_rcc.h"
#include "hal_spi.h"

#include "spi.h"

////////////////////////////////////////////////////////////////////////////////
// SPI Flash connect to SPI2
////////////////////////////////////////////////////////////////////////////////
u8 txdata[256];
u8 rxdata[256];
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    //------------------------------------------------------------------------//
    u16 i;
    for (i = 0; i < 256; i++) {
        txdata[i] = i;
    }
    memset(rxdata, 0xFF, 256);

    //------------------------------------------------------------------------//
    SPIM_Init(SPI2, SPI_BaudRatePrescaler_128);
    SPIM_ChipErase(SPI2);

    //------------------------------------------------------------------------//
    SPIM_PageProgram(SPI2, 0x00, txdata, 256);  // write
    SPIM_PageRead(SPI2, 0x00, rxdata, 256);     // read

    while (1) {
//        printf("test1\n");
//        printf("test2\n");
    }
}
