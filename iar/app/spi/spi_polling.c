////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_POLLING.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE SPI_POLLING EXAMPLE.
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
#define _SPI_POLLING_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "spi_polling.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_SPI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{
#define MASTER_BOARD


#define BUFFER_SIZE 19
#define RXBUFFER_SIZE 3
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

// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hSPI = CreateFile(emIP_SPI);
    if (hSPI == NULL)       while(1);


// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_SPI_DCB dcb = {
// Base parameter
        .hSub               = emFILE_SPI2,
        .type               = emTYPE_IT,            // emTYPE_Polling,emTYPE_IT,emTYPE_DMA,
        .block              = emTYPE_Block,         // emTYPE_Blocking, emTYPE_NonBlocking

// Callback function
        .sync               = emTYPE_Sync,          // Sync, ASync
        .cbTx               = (u32)&TxCallback,    //tx callback function access address
        .cbRx               = (u32)&RxCallback,    //rx callback function access address

// operation mode
        .remapEn            = false,                // Disable : 0 ,Enable : 1
        .remapIdx           = 0,                    // u8 value

// SPI parameter
//      .dataWidth          = 8,                     // 1~32
        .baudRatePrescaler  = 64,

        .clkPolarity        = emSPI_POLARITY_LOW,
        .clkPhase           = emSPI_PHASE_1EDGE,
        .nss                = emSPI_NSS_Soft,
        .firstBit           = emSPI_MSB_FIRST,

#if defined(MASTER_BOARD)
        .spiMode            = emBOOL_Master
#else
        .spiMode            = emBOOL_Slave
#endif
    };

    if (!OpenFile(hSPI, (void*)&dcb))       while(1);

    for (u8 i = 0; i < sizeof(newTxBuffer); i++) {
        newTxBuffer[i] = 2 + i;
    }



//    memset(newTxBuffer, 0x9F, sizeof(newTxBuffer));


//    WriteFile(hSPI, emFILE_SPI2, newTxBuffer, BUFFER_SIZE);
//    ReadFile(hSPI, emFILE_SPI2, newRxBuffer, BUFFER_SIZE);

    while(1){
//        if(!success && WriteFile(hSPI, emFILE_SPI2, newTxBuffer, BUFFER_SIZE)){
//            success = true;
//        }

        if(!success && ReadFile(hSPI, emFILE_SPI2, newRxBuffer, BUFFER_SIZE)){
            success = true;
        }
    }




//    txSuccess = false;
//    rxSuccess = false;
//  bool toggle = false;


//    while (1) {
//// Step 4:  Write File Device  --------------------->>>>>
//      if (!txSuccess && WriteFile(hSPI, 1,(u8*)newTxBuffer, sizeof(newTxBuffer)/4)) {
//            txSuccess = true;
//            // Process Send packet
//        }
//
//// Step 5:  Read File Device  ---------------------->>>>>
//      int ret = ReadFile(hSPI, 1, (u8*)newRxBuffer, sizeof(newRxBuffer)/4);
//      if (ret == emRETURN_TimeOut) {
//          for (u32 i = 0; i < 200; i++){
//              for (u32 j = 0; j < 50000; j++);
//              ret = ReadFile(hSPI, 1, (u8*)newRxBuffer, sizeof(newRxBuffer));
//              if (ret >  0)
//                  break;
//          }
//          if (txSuccess) {
//              toggle    = !toggle;
//              (toggle) ? memset(newTxBuffer, 0x30, sizeof(newTxBuffer)) : memset(newTxBuffer, 0x38, sizeof(newTxBuffer));
//              txSuccess = false;
//          }
//      }
//      if (ret == sizeof(newRxBuffer)) {
//          if (txSuccess) {
//              toggle    = !toggle;
//              (toggle) ? memset(newTxBuffer, 0x32, sizeof(newTxBuffer)) : memset(newTxBuffer, 0x39, sizeof(newTxBuffer));
//              txSuccess = false;
//              // process receive packet;
//          }
//      }
////        if (KEYBOARD_ReadFile(&hSysKeyBoard, (u8*)&vkKey, 1)) {
////            switch  (vkKey) {
////            case  VK_K0:
////                KeyProcess_Key0();
////                break;
////            case  VK_K1:
////                KeyProcess_Key1();
////                break;
////            case  VK_K2:
////                KeyProcess_Key2();
////                break;
////            case  VK_K3:
////                KeyProcess_Key3();
////                break;
////            default:
////                break;
////            }
////        }
////
////#if defined(__SYS_RESOURCE_POLLING)
////        if (sysTickFlag) {
////            sysTickFlag = false;
////            DISPLAY_WriteFile(&hSysDisplay, (u8*)&g_LED_status, 1);
////            g_LED_status <<= 1;
////            if (g_LED_status >= 0x0010) g_LED_status = 0x0001;
////        }
////#endif
//        // user program
//        // .......
//        // .......
//    }
}
