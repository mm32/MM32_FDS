////////////////////////////////////////////////////////////////////////////////
/// @file     UART_PRINTF_HAL.C
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE UART_PRINTF_HAL EXAMPLE.
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
#define _UART_PRINTF_HAL_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include <stdio.h>

#include "types.h"
#include "system_mm32.h"

#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_uart.h"

#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
///@defgroup EXAMPLE_UART
///@{

// IAR: Options->General Options -> Library Configuration -> Library: Full
// Keil: Options for Target -> Target -> Code Generation: Use MicroLIB

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE* f)
#endif /* __GNUC__ */

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
/// @brief  Retargets the C library printf function to the UART1.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
PUTCHAR_PROTOTYPE
{
    UART_SendData(UART1, (uint8_t)ch);
    while (UART_GetFlagStatus(UART1, UART_IT_TXIEN) == RESET)
        ;
    UART_ClearITPendingBit(UART1, UART_IT_TXIEN);
    return ch;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART GPIO config
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART_IO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;

#if defined(__MM3N1)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART config
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART_Config()
{
    UART_InitTypeDef UART_InitStructure;

    UART_IO_Config();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);

    UART_InitStructure.BaudRate            = 115200;
    UART_InitStructure.WordLength          = UART_WordLength_8b;
    UART_InitStructure.StopBits            = UART_StopBits_1;
    UART_InitStructure.Parity              = UART_Parity_No;
    UART_InitStructure.HWFlowControl       = UART_HWFlowControl_None;
    UART_InitStructure.Mode                = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(UART1, &UART_InitStructure);

    UART_Cmd(UART1, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Main program
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is not NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
//  MCUID = SetSystemClock(emSYSTICK_On, NULL);


    UART_Config();

    /* Output a message on Hyperterminal using printf function */
    printf("\n\r UART Printf Example: retarget the C library printf function to the UART\n\r");
    printf("** Test finished successfully. ** \n\r");

    while (1) {
        printf("success \n\r");
    }
}
/// @}

/// @}
