////////////////////////////////////////////////////////////////////////////////
/// @file     UART_BAUDRATETX.C
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE UART_BAUDRATETX EXAMPLE.
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
#include "BSP_KEY.h"
#include "drv_uart_baudrate.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
///@addtogroup EXAMPLE_UART
///@{


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Constants
/// @{
#define TX_SIZE 16

u8 txBuffer[TX_SIZE];

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

    for(u8 i = 0; i < TX_SIZE; i++){
        txBuffer[i] = i;
    }

    BSP_KEY_Configure();
    Baudrate_IO_Config(baudrateUART);


    while(1){
        if (SysKeyboard(&vkKey)) {
            switch  (vkKey) {
            case  VK_K0:
                KeyProcess_Key0();
                break;
            case  VK_K1:
                KeyProcess_Key1();
                Autobaudrate(9600);
                for(u8 i = 0; i < TX_SIZE; i++){
                    txBuffer[i] = TX_SIZE - i - 1;
                }
                baudrateTransmit(txBuffer, TX_SIZE);
                break;
            case  VK_K2:
                KeyProcess_Key2();
                Autobaudrate(115200);
                for(u8 i = 0; i < TX_SIZE; i++){
                    txBuffer[i] = TX_SIZE - i - 1;
                }
                baudrateTransmit(txBuffer, TX_SIZE);
                break;
            case  VK_K3:
                KeyProcess_Key3();
                Autobaudrate(9700);
                for(u8 i = 0; i < TX_SIZE; i++){
                    txBuffer[i] = i;
                }
                baudrateTransmit(txBuffer, TX_SIZE);
                break;
            default:
                break;
            }
        }
    }
}

/// @}


/// @}

/// @}
