////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_SPI.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE SPI BSP LAYER FUNCTIONS.
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
#define _BSP_SPI_C_

// Files includes  -------------------------------------------------------------
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_spi.h"

#include "drv.h"
#include "bsp.h"
#include "drv_spi.h"
#include "bsp_spi.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{

/* __MM3N1
|----------|----------|---------|---------|---------|---------|
|remapEn   |remapInd  |SPI1_NSS |SPI1_SCK |SPI1_MISO|SPI1_MOSI|
|----------|----------|---------|---------|---------|---------|
| false    | -        |  PA4    |  PA5    |  PA6    | PA7     |
|----------|----------|---------|---------|---------|---------|
| true     | 0        |  PA15   |  PB3    |  PB4    | PB5     |
|----------|----------|---------|---------|---------|---------|

|----------|----------|---------|---------|---------|---------|
|remapInd  |remapInd  |SPI2_NSS |SPI2_SCK |SPI2_MISO|SPI2_MOSI|
|----------|----------|---------|---------|---------|---------|
| false    |-         |  PB12   |  PB13   |  PB14   | PB15    |
|----------|----------|---------|---------|---------|---------|

*/

/* __MM0N1
|----------|----------|---------|---------|---------|---------|
|remapEN   |remapInd  |SPI1_NSS |SPI1_SCK |SPI1_MISO|SPI1_MOSI|
|----------|----------|---------|---------|---------|---------|
| false    |-         | PA4/AF0 | PA5/AF0 | PA6/AF0 | PA7/AF0 |
|----------|----------|---------|---------|---------|---------|
| true     |0         | PA15/AF0| PB3/AF0 | PB4/AF0 | PB5/AF0 |
|----------|----------|---------|---------|---------|---------|


|----------|----------|---------|---------|---------|---------|
|remapInd  |remapEn   |SPI2_NSS |SPI2_SCK |SPI2_MISO|SPI2_MOSI|
|----------|----------|---------|---------|---------|---------|
| false    |-         | PB12/AF0| PB13/AF0| PB14/AF0| PB15/AF0|
|----------|----------|---------|---------|---------|---------|
| true     |0         | PB9/AF5 | PB10/AF5| PB14/AF0| PB15/AF0|
|----------|----------|---------|---------|---------|---------|
*/

/* __MM0P1
|----------|----------|---------|---------|---------|---------|
|remapEN   |remapInd  |SPI1_NSS |SPI1_SCK |SPI1_MISO|SPI1_MOSI|
|----------|----------|---------|---------|---------|---------|
| false    |-         | PA4/AF0 | PA5/AF0 | PA6/AF0 | PA7/AF0 |
|----------|----------|---------|---------|---------|---------|
| true     |0         | PA15/AF0| PB3/AF0 | PB4/AF0 | PB5/AF0 |
|----------|----------|---------|---------|---------|---------|

|----------|----------|---------|---------|---------|---------|
|remapInd  |remapEn   |SPI2_NSS |SPI2_SCK |SPI2_MISO|SPI2_MOSI|
|----------|----------|---------|---------|---------|---------|
| false    |-         | PB12/AF0| PB13/AF0| PB14/AF0| PB15/AF0|
|----------|----------|---------|---------|---------|---------|
| true     |0         | PB9/AF5 | PB10/AF5| PB14/AF0| PB15/AF0|
|----------|----------|---------|---------|---------|---------|


*/

/* __MM0Q1
|----------|----------|---------|---------|---------|---------|
|remapInd  |remapEn   |SPI2_NSS |SPI2_SCK |SPI2_MISO|SPI2_MOSI|
|----------|----------|---------|---------|---------|---------|
| false    |-         | PA0/AF3 |PB14/AF3 | PA13/AF4| PB13/AF4|
|----------|----------|---------|---------|---------|---------|
*/


void BSP_SPI_NSS_Configure(SPI_TypeDef* SPIx, bool remapEn, u16 remapIdx, FunctionalState newState)
{
    switch ((u32)SPIx) {
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
    case (u32)SPI1:
        if (!remapEn)           (newState) ? GPIO_ResetBits(GPIOA, GPIO_Pin_4)  : GPIO_SetBits(GPIOA, GPIO_Pin_4);
        else if (remapIdx == 0) (newState) ? GPIO_ResetBits(GPIOA, GPIO_Pin_15) : GPIO_SetBits(GPIOA, GPIO_Pin_15);
        break;
    case (u32)SPI2:
        if (!remapEn)           (newState) ? GPIO_ResetBits(GPIOB, GPIO_Pin_12) : GPIO_SetBits(GPIOB, GPIO_Pin_12);
        else if (remapIdx == 0) (newState) ? GPIO_ResetBits(GPIOB, GPIO_Pin_15)  : GPIO_SetBits(GPIOB, GPIO_Pin_15);
        break;
#endif

#if defined(__MM0Q1)
    case (u32)SPI2:
        if (!remapEn)           (newState) ? GPIO_ResetBits(GPIOA, GPIO_Pin_0)  : GPIO_SetBits(GPIOA, GPIO_Pin_0);
        else {  /* remap index  */ }
        break;
#endif
    }
}

void BSP_SPI_GPIO_Configure(SPI_TypeDef* SPIx, bool remapEn, u16 remapIdx, bool master, bool nss)
{
    if((u32)SPIx == (u32)SPI1){
        if (remapEn == 0) {
            GPIOA_ClockEnable();
            GPIOB_ClockEnable();
            if (master) {
                if (!nss == SPI_NSS_Soft) {
                    GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_4,NO_REMAP, GPIO_AF_0);  // spi1_nss
                }
                else{
                    GPIO_SetBits(GPIOA, GPIO_Pin_4);
                    GPIO_AF_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_0);
                }
                GPIO_Mode_AF_PP_50MHz_Init  (GPIOA, GPIO_Pin_5, NO_REMAP, GPIO_AF_0);       // spi1_sck
                exGPIO_PinAFConfig(GPIOB, GPIO_Pin_4, NO_REMAP, GPIO_AF_3);
                GPIO_Mode_IPU_Init          (GPIOA, GPIO_Pin_6, NO_REMAP, GPIO_AF_0);       // spi1_miso_enable
                GPIO_Mode_AF_PP_50MHz_Init  (GPIOA, GPIO_Pin_7, NO_REMAP, GPIO_AF_0);       // spi1_mosi
            }
            else {
                GPIO_Mode_IPU_Init          (GPIOA, GPIO_Pin_4, NO_REMAP, GPIO_AF_0);       // spi1_nss
                GPIO_Mode_FLOATING_Init     (GPIOA, GPIO_Pin_5, NO_REMAP, GPIO_AF_0);       // spi1_sck
                GPIO_Mode_AF_PP_50MHz_Init  (GPIOA, GPIO_Pin_6, NO_REMAP, GPIO_AF_0);       // spi1_miso
                GPIO_Mode_IPU_Init          (GPIOA, GPIO_Pin_7, NO_REMAP, GPIO_AF_0);       // spi1_mosi
            }
        }
    }
    else if ((u32)SPIx == (u32)SPI2) {
        if (remapEn == 0) {
               GPIOB_ClockEnable();
            if (master) {
                if (!nss == SPI_NSS_Soft) {
                    GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_12,NO_REMAP, GPIO_AF_0);     // spi2_nss
                }
                else{
                    GPIO_SetBits(GPIOB, GPIO_Pin_12);
                    GPIO_AF_Init(GPIOB, GPIO_Pin_12, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_0);
                }
                GPIO_Mode_AF_PP_50MHz_Init  (GPIOB, GPIO_Pin_13, NO_REMAP, GPIO_AF_0);      // spi2_sck
                GPIO_Mode_IPU_Init          (GPIOB, GPIO_Pin_14, NO_REMAP, GPIO_AF_0);      // spi2_miso
                GPIO_Mode_AF_PP_50MHz_Init  (GPIOB, GPIO_Pin_15, NO_REMAP, GPIO_AF_0);      // spi2_mosi
            }
            else {
                GPIOB_ClockEnable();

                GPIO_Mode_IPU_Init          (GPIOB, GPIO_Pin_12, NO_REMAP, GPIO_AF_0);      // spi2_nss
                GPIO_Mode_FLOATING_Init     (GPIOB, GPIO_Pin_13, NO_REMAP, GPIO_AF_0);      // spi2_sck
                GPIO_Mode_AF_PP_50MHz_Init  (GPIOB, GPIO_Pin_14, NO_REMAP, GPIO_AF_0);      // spi2_miso
                GPIO_Mode_IPU_Init          (GPIOB, GPIO_Pin_15, NO_REMAP, GPIO_AF_0);      // spi2_mosi
            }
        }
        else{
            GPIOB_ClockEnable();
            if (master) {
                if (!nss == SPI_NSS_Soft) {
                    GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_15, NO_REMAP, GPIO_AF_4);    // spi2_nss
                }
                else{
                    GPIO_SetBits(GPIOB, GPIO_Pin_15);
                    GPIO_AF_Init(GPIOB, GPIO_Pin_15, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_4);
                }
                GPIO_Mode_AF_PP_50MHz_Init  (GPIOB, GPIO_Pin_14, NO_REMAP, GPIO_AF_3);      // spi2_sck
                GPIO_Mode_IPU_Init          (GPIOB, GPIO_Pin_12, NO_REMAP, GPIO_AF_4);      // spi2_miso
                GPIO_Mode_AF_PP_50MHz_Init  (GPIOB, GPIO_Pin_13, NO_REMAP, GPIO_AF_4);      // spi2_mosi
            }



            // Todo
        }
    }
}











//#if defined(REGISTER)
//// ------------------------------ Register Access ---------------------------
//  switch ((u32)pDcb->SPIx) {
//#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
//  case (u32)SPI1:
//      // no remap ------------------------------------------------------------
//      if (pDcb->remapEn) {
//          if (pDcb->msMode == SPI_Mode_Master) {
//              // master mode--------------------------------------------------
//              GPIOA_ClockEnable();
//
//              if (pDcb->nss == SPI_NSS_Hard) {
//                  GPIOA->CRL = (GPIOA->CRL & 0xFFF0FFFF)
//                           | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT4;   //spi1_nss
//              }
//              else{
//                  GPIOA->CRL = (GPIOA->CRL & 0xFFF0FFFF)
//                           | (u32)(PORT_50MHz) << PORT_BIT4;   //spi1_nss
//              }
//
//              GPIOA->CRL = (GPIOA->CRL & 0x000FFFFF)
//                           | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT5
//                           | (u32)(PORT_INWUD << PORT_BIT6)
//                           | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT7;      // spi1_sck, spi1_miso, spi1_mosi
//
//              GPIOA->BSRR |= 1 << 6;
//
//#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
//              GPIOA->AFRL = (GPIOA->AFRL & 0x0000FFFF) | 0x00000000;          // 7-AF0, 6-AF0 , 5-AF0 , 4-AF0
//#endif
//
//          }
//          else {
//              // slave mode---------------------------------------------------
//              GPIOA_ClockEnable();
//
//              GPIOA->CRL = (GPIOA->CRL & 0x000FFFFF)
//                           | (u32)(PORT_INWUD << PORT_BIT4)
//                           | (u32)(PORT_FIN << PORT_BIT5)
//                           | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT6
//                           | (u32)(PORT_INWUD   << PORT_BIT7);                // spi1_nss, spi1_sck, spi1_miso, spi1_mosi
//
//              GPIOA->BSRR |= (1 << 4) | (1 << 7);
//
//#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
//              GPIOA->AFRL = (GPIOA->AFRL & 0x0000FFFF) | 0x00000000;          // 7-AF0, 6-AF0 , 5-AF0 , 4-AF0
//#endif
//
//          }
//      }
//      // remap ---------------------------------------------------------------
//      else {
//          // remap index 0----------------------------------------------------
//          if (pDcb->remapIdx == 0) {
//              if (pDcb->msMode == SPI_Mode_Master) {
//                  // master mode----------------------------------------------
//                  GPIOA_ClockEnable();
//                  GPIOB_ClockEnable();
//
//                  if (pDcb->nss == SPI_NSS_Hard) {
//                      GPIOA->CRH = (GPIOA->CRH & 0x0FFFFFFF)
//                                  | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT15;      //spi1_nss
//                  }
//                  else{
//                      GPIOA->CRH = (GPIOA->CRH & 0x0FFFFFFF)
//                                  | (u32)PORT_50MHz << PORT_BIT15;      //spi1_nss
//                  }
//
//                  GPIOB->CRL = (GPIOB->CRL & 0xFF000FFF)
//                             | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT3
//                             | (u32)(PORT_INWUD << PORT_BIT4)
//                             | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT5;    // spi1_sck, spi1_miso, spi1_mosi
//
//                  GPIOA->BSRR |= 1 << 4;
//#if defined(__MM3N1)
//                  RCC->APB2ENR |= RCC_APB2ENR_EXTI;
//                  EXTI->MAPR   |= EXTI_MAPR_SPI1;
//#endif
//#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
//                  GPIOA->AFRH = (GPIOA->AFRH & 0x0FFFFFFF) | 0x00000000;      // 15-AF0
//                  GPIOB->AFRL = (GPIOB->AFRL & 0xFF000FFF) | 0x00000000;      // 5-AF0 , 4-AF0 , 3-AF0
//#endif
//
//              }
//              else {
//                  // slave mode-----------------------------------------------
//                  GPIOA_ClockEnable();
//                  GPIOB_ClockEnable();
//
//                  GPIOA->CRH = (GPIOA->CRH & 0x0FFFFFFF)
//                              | (u32)(PORT_INWUD << PORT_BIT15);                //spi1_nss
//
//                  GPIOB->CRL = (GPIOB->CRL & 0xFF000FFF)
//                              | (u32) (PORT_FIN  | PORT_50MHz) << PORT_BIT3
//                              | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT4
//                              | (u32)(PORT_INWUD << PORT_BIT5);                // spi1_sck, spi1_miso, spi1_mosi
//
//                  GPIOA->BSRR |= 1 << 15;
//                  GPIOB->BSRR |= 1 << 5;
//#if defined(__MM3N1)
//                  RCC->APB2ENR |= RCC_APB2ENR_EXTI;
//                  EXTI->MAPR   |= EXTI_MAPR_SPI1;
//#endif
//#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
//                  GPIOA->AFRH = (GPIOA->AFRH & 0x0FFFFFFF) | 0x00000000;      // 15-AF0
//                  GPIOB->AFRL = (GPIOB->AFRL & 0xFF000FFF) | 0x00000000;      // 5-AF0 , 4-AF0 , 3-AF0
//#endif
//              }
//              // remap index ...----------------------------------------------
//          }
//      }
//      break;
//#endif /* defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)*/
//  case (u32)SPI2:
//      // no remap ------------------------------------------------------------
//      if (!pDcb->remapEn) {
//          if (pDcb->msMode == SPI_Mode_Master) {
//              // master mode--------------------------------------------------
//#if   defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
//              GPIOB_ClockEnable();
//
//              if (pDcb->nss == SPI_NSS_Hard) {
//                  GPIOB->CRH = (GPIOB->CRH & 0xFFF0FFFF)
//                              | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT12;  // spi1_nss
//              }
//              else {
//                  GPIOB->CRH = (GPIOB->CRH & 0xFFF0FFFF)
//                              | (u32)PORT_50MHz << PORT_BIT12;                // spi1_nss
//              }
//
//              GPIOB->CRH = (GPIOB->CRH & 0x000FFFFF)
//                          | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT13
//                          | (u32)(PORT_INWUD << PORT_BIT14)
//                          | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT15;      // spi1_sck, spi1_miso, spi1_mosi
//
//              GPIOB->BSRR |= 1 << 14;
//
//#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
//              GPIOB->AFRH = (GPIOB->AFRH & 0x0000FFFF) | 0x00000000;
//#endif
//
//#endif
//
//#if defined(__MM0Q1)
//              GPIOA_ClockEnable();
//              GPIOB_ClockEnable();
//
//              if (pDcb->nss == SPI_NSS_Hard) {
//                  GPIOA->CRL = (GPIOA->CRL & 0xFFFFFFF0)
//                              | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT0;   // spi1_nss
//              }
//              else {
//                  GPIOA->CRL = (GPIOA->CRL & 0xFFFFFFF0)
//                              | (u32)PORT_50MHz << PORT_BIT0;                 // spi1_nss
//              }
//
//              GPIOB->CRH = (GPIOB->CRH & 0xF00FFFFF)
//                          | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT13
//                          | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT14 ;     // spi1_mosi, spi1_sck
//
//              GPIOA->CRH = (GPIOA->CRH & 0xFF0FFFFF)
//                          | (u32)(PORT_INWUD << PORT_BIT13);                    // spi1_miso
//
//              GPIOA->BSRR |= 1 << 13;
//
//              GPIOA->AFRL = (GPIOA->AFRL & 0xFFFFFFF0) | 0x00000003;          // 0-AF3
//              GPIOA->AFRH = (GPIOA->AFRH & 0xFF0FFFFF) | 0x00400000;          // 13-AF4
//
//              GPIOB->AFRH = (GPIOB->AFRH & 0xF00FFFFF) | 0x03400000;          // 14-AF3 13-AF4
//
//#endif
//          }
//          else {
//              // slave mode--------------------------------------------------
//#if  defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
//              GPIOB_ClockEnable();
//
//              GPIOB->CRH = (GPIOB->CRH & 0x0FFFFFFF)
//                          | (u32)(PORT_INWUD << PORT_BIT12);                   // spi1_nss
//
//              GPIOB->CRH = (GPIOB->CRH & 0xFF000FFF)
//                          | (u32)(PORT_FIN << PORT_BIT13)
//                          | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT14
//                          | (u32)(PORT_INWUD << PORT_BIT15);                   // spi1_sck, spi1_miso, spi1_mosi
//
//              GPIOB->BSRR |= (1 << 12) | (1 << 15);
//
//#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
//              GPIOB->AFRH = (GPIOB->AFRH & 0x0000FFFF) | 0x00000000;          // 15-AF0 , 14-AF0 , 13-AF0, 12-AF0
//#endif
//
//#endif
//
//#if defined(__MM0Q1)
//
//              GPIOA_ClockEnable();
//              GPIOB_ClockEnable();
//
//              GPIOA->CRL = (GPIOA->CRL & 0xFFFFFFF0)
//                          | (u32)(PORT_INWUD << PORT_BIT0);                    // spi1_nss
//
//
//              GPIOB->CRH = (GPIOB->CRH & 0xF00FFFFF)
//                          | (u32)(PORT_INWUD << PORT_BIT13)
//                          | (u32)(PORT_FIN   << PORT_BIT14);                  // spi1_mosi, spi1_sck
//
//              GPIOA->CRH = (GPIOA->CRH & 0xFF0FFFFF)
//                          | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT13;     // spi1_miso
//
//              GPIOA->BSRR |= 1 << 0;
//              GPIOB->BSRR |= 1 << 13;
//
//              GPIOA->AFRL = (GPIOA->AFRL & 0xFFFFFFF0) | 0x00000003;          //0-AF3
//              GPIOA->AFRH = (GPIOA->AFRH & 0xFF0FFFFF) | 0x00400000;          //13-AF4
//              GPIOB->AFRH = (GPIOB->AFRH & 0xF00FFFFF) | 0x03400000;          //14-AF3 13-AF4
//
//#endif
//          }
//      }
//      // remap ---------------------------------------------------------------
//      else {
//          // remap index 0----------------------------------------------------
//#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
//          if (pDcb->remapIdx == 0) {
//              if (pDcb->msMode == SPI_Mode_Master) {
//                  // master mode----------------------------------------------
//                  GPIOB_ClockEnable();
//
//                  if (pDcb->nss == SPI_NSS_Hard) {
//                      GPIOB->CRH = (GPIOB->CRH & 0xFFFFFF0F)
//                                  | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT9; // spi1_nss
//                  }
//                  else {
//                      GPIOB->CRH = (GPIOB->CRH & 0xFFFFFF0F)
//                          | (u32)PORT_50MHz << PORT_BIT9; // spi1_nss
//                  }
//
//                  GPIOB->CRH = (GPIOB->CRH & 0x00FFF0FF)
//                              | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT10
//                              | (u32)(PORT_INWUD << PORT_BIT14)
//                              | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT15;  // spi1_sck, spi1_miso, spi1_mosi
//
//                  GPIOB->BSRR |= 1 << 14;
//
//                  GPIOB->AFRH = (GPIOB->AFRH & 0x00FFF00F) | 0x00000550;
//
//              }
//              else {
//                  // slave mode-----------------------------------------------
//                  GPIOB_ClockEnable();
//
//                  GPIOB->CRH = (GPIOB->CRH & 0xFFFFFF0F)
//                              | (u32) PORT_INWUD << PORT_BIT9;                // spi1_nss
//
//                  GPIOB->CRH = (GPIOB->CRH & 0x00FFF0FF)
//                              | (u32)(PORT_FIN << PORT_BIT10)
//                              | (u32)(PORT_AFPP | PORT_50MHz) << PORT_BIT14
//                              | (u32)(PORT_INWUD << PORT_BIT15);               // spi1_sck, spi1_miso, spi1_mosi
//
//                  GPIOB->BSRR |= (1 << 9) | (1 << 15);
//
//
//                  GPIOB->AFRH = (GPIOB->AFRH & 0x00FFF00F) | 0x00000550;      // 15-AF0 , 14-AF0 , 10-AF0, 9-AF0
//
//              }
//          }
//#endif
//          // remap index 1----------------------------------------------------
//          if (pDcb->remapIdx == 1) {
//              //... ... //
//          }
//      }
//      break;
//  default:
//      break;
//  }
//
//
//#else

























//  // ------------------ Hardware Abstraction Layer Access ---------------------
//  switch ((u32)pDcb->SPIx) {
//#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
//  case (u32)SPI1:
//      // no remap ------------------------------------------------------------
//      if (!pDcb->remapEn) {
//          if (pDcb->msMode == SPI_Mode_Master) {
//              // master mode--------------------------------------------------
//              GPIOA_ClockEnable();
//
//              if (pDcb->nss == SPI_NSS_Hard) {
//                  GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_4,NO_REMAP, GPIO_AF_0); // spi1_nss
//              }
//              else {
//                  GPIO_AF_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_0);
//              }
//              GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_5, NO_REMAP, GPIO_AF_0);// spi1_sck
//              GPIO_Mode_IPU_Init(GPIOA, GPIO_Pin_6, NO_REMAP, GPIO_AF_0);        // spi1_miso
//              GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_7, NO_REMAP, GPIO_AF_0);// spi1_mosi
//
//          }
//          else {
//              // slave mode---------------------------------------------------
//              GPIOA_ClockEnable();
//
//              GPIO_Mode_IPU_Init(GPIOA, GPIO_Pin_4,NO_REMAP, GPIO_AF_0);         // spi1_nss
//              GPIO_Mode_FLOATING_Init(GPIOA, GPIO_Pin_5, NO_REMAP, GPIO_AF_0);// spi1_sck
//              GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_6, NO_REMAP, GPIO_AF_0);// spi1_miso
//              GPIO_Mode_IPU_Init(GPIOA, GPIO_Pin_7, NO_REMAP, GPIO_AF_0);        // spi1_mosi
//          }
//      }
//      // remap ---------------------------------------------------------------
//      else {
//          // remap index 0----------------------------------------------------
//          if (pDcb->remapIdx == 0) {
//              if (pDcb->msMode == SPI_Mode_Master) {
//                  // master mode--------------------------------------------------
//                  GPIOA_ClockEnable();
//                  GPIOB_ClockEnable();
//
//                  if (pDcb->nss == SPI_NSS_Hard) {
//                      GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_15,0, GPIO_AF_0);             // spi1_nss
//                  }
//                  else {
//                      GPIO_AF_Init(GPIOA, GPIO_Pin_15, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_0);
//                  }
//                  GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_3, 0, GPIO_AF_0);    // spi1_sck
//                  GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_4, 0, GPIO_AF_0);     // spi1_miso
//                  GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_5, 0, GPIO_AF_0);    // spi1_mosi
//
//              }
//              else {
//                  // slave mode---------------------------------------------------
//                  GPIOA_ClockEnable();
//                  GPIOB_ClockEnable();
//
//                  GPIO_Mode_IPU_Init(GPIOA, GPIO_Pin_15,0, GPIO_AF_0);         // spi1_nss
//                  GPIO_Mode_IN_FLOATING_Init(GPIOB, GPIO_Pin_3, 0, GPIO_AF_0); // spi1_sck
//                  GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_4, 0, GPIO_AF_0); // spi1_miso
//                  GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_5, 0, GPIO_AF_0);         // spi1_mosi
//              }
//              // remap index ...--------------------------------------------------
//          }
//      }       // remap
//      break;
//#endif
//  case (u32)SPI2:
//      // no remap ------------------------------------------------------------
//      if (!pDcb->remapEn) {
//          if (pDcb->msMode == SPI_Mode_Master) {
//              // master mode--------------------------------------------------
//#if   defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
//              GPIOB_ClockEnable();
//              if (pDcb->nss == SPI_NSS_Hard) {
//                  GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_12,NO_REMAP, GPIO_AF_0);             // spi1_nss
//              }
//              else{
//                  GPIO_AF_Init(GPIOB, GPIO_Pin_12, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_0);
//              }
//              GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_13, NO_REMAP, GPIO_AF_0);    // spi1_sck
//              GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_14, NO_REMAP, GPIO_AF_0);     // spi1_miso
//              GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_15, NO_REMAP, GPIO_AF_0);    // spi1_mosi
//#endif
//
//#if defined(__MM0Q1)
//              GPIOA_ClockEnable();
//              GPIOB_ClockEnable();
//
//              if (pDcb->nss == SPI_NSS_Hard) {
//                  GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_0,NO_REMAP, GPIO_AF_3); // spi1_nss
//              }
//              else {
//                  GPIO_AF_Init(GPIOA, GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_0);
//              }
//              GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_14, NO_REMAP, GPIO_AF_3);   // spi1_sck
//              GPIO_Mode_IPU_Init(GPIOA, GPIO_Pin_13, NO_REMAP, GPIO_AF_4);           // spi1_miso
//              GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_13, NO_REMAP, GPIO_AF_4);   // spi1_mosi
//#endif
//          }
//          else {
//              // slave mode---------------------------------------------------
//#if  defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1)
//              GPIOB_ClockEnable();
//
//              GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_12,NO_REMAP, GPIO_AF_0);        // spi1_nss
//              GPIO_Mode_IN_FLOATING_Init(GPIOB, GPIO_Pin_13, NO_REMAP, GPIO_AF_0);   // spi1_sck
//              GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_14, NO_REMAP, GPIO_AF_0);   // spi1_miso
//              GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_15, NO_REMAP, GPIO_AF_0);           // spi1_mosi
//#endif
//
//#if defined(__MM0Q1)
//              GPIOA_ClockEnable();
//              GPIOB_ClockEnable();
//
//              GPIO_Mode_IPU_Init(GPIOA, GPIO_Pin_0,NO_REMAP, GPIO_AF_3);         // spi1_nss
//              GPIO_Mode_IN_FLOATING_Init(GPIOB, GPIO_Pin_14, NO_REMAP, GPIO_AF_3);   // spi1_sck
//              GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_13, NO_REMAP, GPIO_AF_4);   // spi1_miso
//              GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_13, NO_REMAP, GPIO_AF_4);           // spi1_mosi
//#endif
//          }
//      }
//      // remap ---------------------------------------------------------------
//      else {
//          // remap index 0----------------------------------------------------
//#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
//          if (pDcb->remapIdx == 0) {
//              if (pDcb->msMode == SPI_Mode_Master) {
//                  // master mode----------------------------------------------
//                  GPIOB_ClockEnable();
//
//                  if (pDcb->nss == SPI_NSS_Hard) {
//                      GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_9,0, GPIO_AF_5);// spi1_nss
//                  }
//                  else {
//                      GPIO_AF_Init(GPIOB, GPIO_Pin_9, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_0);
//                  }
//                  GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_10, 0, GPIO_AF_5);  // spi1_sck
//                  GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_14, 0, GPIO_AF_0);          // spi1_miso
//                  GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_15, 0, GPIO_AF_0);  // spi1_mosi
//
//              }
//              else {
//                  // slave mode-----------------------------------------------
//                  GPIOB_ClockEnable();
//
//                  GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_9,0, GPIO_AF_5);           // spi1_nss
//                  GPIO_Mode_IN_FLOATING_Init(GPIOB, GPIO_Pin_10, 0, GPIO_AF_5); // spi1_sck
//                  GPIO_Mode_AF_PP_50MHz_Init(GPIOB, GPIO_Pin_14, 0, GPIO_AF_0); // spi1_miso
//                  GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_15, 0, GPIO_AF_0);         // spi1_mosi
//              }
//          }
//#endif
//          // remap index 1----------------------------------------------------
//          if (pDcb->remapIdx == 1) {
//              //... ... //
//          }
//      }
//      break;
//  default:
//      break;
//  }
//
//#endif
//  /* defined(REGISTER) */
//}

/// @}

/// @}

/// @}

