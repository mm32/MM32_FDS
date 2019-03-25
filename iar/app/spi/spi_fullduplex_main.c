////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_FULLDUPLEX_MAIN.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE SPI_FULLDUPLEX_MAIN EXAMPLE.
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
#define _SPI_FULLDUPLEX_MAIN_C_
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "MM32.h"
#include <string.h>

#include "hal_rcc.h"
#include "hal_gpio.h"

#include "spi.h"
////////////////////////////////////////////////////////////////////////////////
// Need to short MOSI and MISO
////////////////////////////////////////////////////////////////////////////////
u32 rxbyte[16];
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
    MCUID = SystemInit(SYSCLK_HSI_96MHz, emSYSTICK_On);
#endif

#if defined(__MM0Q1)
    MCUID = SystemInit(SYSCLK_HSI_48MHz, emSYSTICK_On);
#endif

    // Recieve data initial----------------------------------------------------//
    static u32 i;
    memset(rxbyte, 0xFF, 16);

    // spi init ---------------------------------------------------------------//
    SPIM_Init(SPI2, SPI_BaudRatePrescaler_32);

    //------------------------------------------------------------------------//
    for (i = 0xF0; i < 0x100; i++)
        rxbyte[i] = SPIMReadWriteByte(SPI2, i);

    while (1) {}
}
