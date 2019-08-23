////////////////////////////////////////////////////////////////////////////////
/// @file     UART_BAUDRATERX.C
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE UART_BAUDRATERX EXAMPLE.
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
#define _UART_BAUDRATERX_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "drv_uart_baudrate.h"

//#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
///@addtogroup EXAMPLE_UART
///@{


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Constants
/// @{

#define RX_SIZE 16
u8 rxBuffer[RX_SIZE];

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

    Baudrate_IO_Config(baudrateTIM);
    TIM_EXTI_Config();
    TIM_Config();

    while(1){
        baudrateReceive(rxBuffer, RX_SIZE, 9600);
    }
}

/// @}


/// @}

/// @}
