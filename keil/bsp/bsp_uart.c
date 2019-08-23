////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_UART.C
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE UART BSP LAYER FUNCTIONS.
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
#define _BSP_UART_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_gpio.h"
#include "hal_uart.h"

#include "drv.h"
#include "bsp.h"
#include "drv_uart.h"
#include "bsp_uart.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Functions
/// @{

/* -----------------------------------------------------------------------------
----------  P e r i p h e r a l s    i n i t i a l i z a t i o n   -------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART GPIO Config
/// @param  UARTx: Select the UART or the UART peripheral.
/// @param  reampEn: Select the remap status.
/// @param  remap: Select the AFIO status.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_UART_GPIO_Configure(UART_TypeDef* UARTx, bool remapEn, u8 remapIdx)
{
/* -------------------------------------------------------------------------- */
#if defined(__REGISTER)          /* ----------- Register Access ------------- */
/* -------------------------------------------------------------------------- */
        switch (*(u32*)&UARTx) {
//------------------------------------------------------------------------------
            case (u32)UART1:
            if (!remapEn) {
                GPIOA_ClockEnable();

                // IPU & AF_PP
                GPIOA->CRH = (GPIOA->CRH & 0xFFFFF00F)
                            | (u32)PORT_INWUD               << PORT_BIT10
                            | (u32)(PORT_AFPP | PORT_20MHz) << PORT_BIT9;       // 0x000008A0;  // GPIOA_Pin10-IPU, GPIOA_Pin9-AF_PP
                GPIOA->BSRR |= 1 << 10;                                         // IPU,      IPD:   GPIOA->BSR  |= 1 << 10;

    #if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOA->AFRH = (GPIOA->AFRH & 0xFFFFF00F) | 0x00000110;          // 10-AF1, 9-AF1
    #endif
            }
            else {
                if (remapIdx == 0) {
                    GPIOB_ClockEnable();
                    GPIOB->CRL = (GPIOB->CRL & 0x00FFFFFF)
                            | (u32)PORT_INWUD               << PORT_BIT7
                            | (u32)(PORT_AFPP | PORT_20MHz) << PORT_BIT6;       // GPIOB_Pin7-IPU, GPIOB_Pin6-AF_PP
                    GPIOB->BSRR |= 1 << 6;

    #if defined(__MM3N1)
                    RCC->APB2ENR |= RCC_APB2ENR_EXTI;
                    EXTI->MAPR   |= EXTI_MAPR_UART1;                            // 1 << 2;
    #endif
    #if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                    GPIOB->AFRL = (GPIOB->AFRL & 0x00FFFFFF) | 0x00000000;      // 7-AF0, 6-AF0
    #endif
                }
                if (remapIdx == 1) {
                    GPIOA_ClockEnable();
                    GPIOA->CRH = (GPIOA->CRH & 0x00FFFFFF)
                            | (u32)PORT_INWUD               << PORT_BIT9
                            | (u32)(PORT_AFPP | PORT_20MHz) << PORT_BIT10;      // GPIOA_Pin9-IPU, GPIOA_Pin10-AF_PP
                    GPIOA->BSRR |= 1 << 10;

    #if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                    GPIOA->AFRH = (GPIOA->AFRH & 0xFFFFF00F) | 0x00000330;      // 9-AF0, 10-AF0
    #endif
                }
        }
        break;
//------------------------------------------------------------------------------
        case (u32)UART2:
            if (!remapEn) {
                GPIOA_ClockEnable();
                GPIOA->CRL = (GPIOA->CRL & 0xFFFF00FF)
                            | (u32)PORT_INWUD               << PORT_BIT3
                            | (u32)(PORT_AFPP | PORT_20MHz) << PORT_BIT2;       // 0x00008A00;  // GPIOA_Pin3-IPU, GPIOA_Pin2-AF_PP,
                GPIOA->BSRR |= 1 << 3;

    #if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOA->AFRL = (GPIOA->AFRL & 0xFFFF00FF) | 0x00001100;          // Pin3-AF1, Pin2-AF1,
    #endif
            }
            else {
                if (remapIdx == 0) {
                    GPIOA_ClockEnable();
                    GPIOA->CRH = (GPIOA->CRH & 0xFFFF00FF)
                                | (u32)PORT_INWUD               << PORT_BIT15
                                | (u32)(PORT_AFPP | PORT_20MHz) << PORT_BIT14;  // GPIOA_Pin15-IPU, GPIOA_Pin14-AF_PP,
                    GPIOA->BSRR |= 1 << 2;

    #if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                    GPIOA->AFRH = (GPIOA->AFRH & 0x00FFFFFF) | 0x11000000;      // Pin3-AF1, Pin2-AF1,
    #endif
                }
                if (remapIdx == 1) {
                    GPIOC_ClockEnable();
                    GPIOC->CRL = (GPIOC->CRL & 0xFFFF00FF)
                                | (u32)PORT_INWUD               << PORT_BIT5
                                | (u32)(PORT_AFPP | PORT_20MHz) << PORT_BIT4;   // GPIOC_Pin5-IPU, GPIOC_Pin4-AF_PP,
                    GPIOC->BSRR |= 1 << 4;

    #if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                    GPIOC->AFRH = (GPIOC->AFRH & 0xFF00FFFF) | 0x00330000;      // Pin5-AF3, Pin4-AF3,
    #endif
                }
            }
            break;

//------------------------------------------------------------------------------
    #if defined(__MM3N1)
        case (u32)UART3:
            if (!remapEn) {
                GPIOB_ClockEnable();
                GPIOB->CRH = (GPIOB->CRH & 0xFFFF00FF)
                        | (u32)PORT_INWUD               << PORT_BIT11
                        | (u32)(PORT_AFPP | PORT_20MHz) << PORT_BIT10;          // 0x00008A00;  // GPIOB_Pin11-IPU, GPIOB_Pin10-AF_PP
                GPIOB->BSRR |= 1 << 11;
            }
            else {
                if (remapIdx == 0) {
                    GPIOC_ClockEnable();
                    GPIOC->CRH = (GPIOC->CRH & 0xFFFF00FF)
                            | (u32)PORT_INWUD               << PORT_BIT11
                            | (u32)(PORT_AFPP | PORT_20MHz) << PORT_BIT10;      // 0x00008A00;  // GPIOC_Pin11-IPU, GPIOC_Pin10-AF_PP
                    GPIOC->BSRR |= 1 << 11;

                    RCC->APB2ENR |= RCC_APB2ENR_EXTI;
                    EXTI->MAPR   |= EXTI_MAPR_UART3;        //1 << 4;
                }
            }
            break;
    #endif
            default: break;
        }
/* -------------------------------------------------------------------------- */
#else          /* ----------- Hardware Abstraction Layer Access ------------- */
/* -------------------------------------------------------------------------- */
        switch (*(u32*)&UARTx) {
//------------------------------------------------------------------------------
            case (u32)UART1:
            if (!remapEn) {
                    GPIOA_ClockEnable();
                    GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_9,  NO_REMAP,        GPIO_AF_1);
                    GPIO_Mode_IPU_Init        (GPIOA, GPIO_Pin_10, NO_REMAP,        GPIO_AF_1);
            }
            else {
                if (remapIdx == 0) {
                    GPIOB_ClockEnable();
                    GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_6, EXTI_MAPR_UART1,  GPIO_AF_0);
                    GPIO_Mode_IPU_Init        (GPIOB, GPIO_Pin_7, EXTI_MAPR_UART1,  GPIO_AF_0);
                }



    #if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                else if (remapIdx == 1) {
                    GPIOA_ClockEnable();
                    GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_10, NO_REMAP,        GPIO_AF_3);
                    GPIO_Mode_IPU_Init        (GPIOA, GPIO_Pin_9,  NO_REMAP,        GPIO_AF_3);
                }
    #endif
            }
            break;

//------------------------------------------------------------------------------
        case (u32)UART2:
            if (!remapEn) {
                    GPIOA_ClockEnable();
                    GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_2, NO_REMAP,         GPIO_AF_1);
                    GPIO_Mode_IPU_Init        (GPIOA, GPIO_Pin_3, NO_REMAP,         GPIO_AF_1);
            }
            else {
    #if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                if (remapIdx == 0) {
                    GPIOA_ClockEnable();
                    GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_14, NO_REMAP,        GPIO_AF_1);
                    GPIO_Mode_IPU_Init        (GPIOA, GPIO_Pin_15, NO_REMAP,        GPIO_AF_1);
                }
    #endif

    #if defined(__MM0P1) || defined(__MM0Q1)
                else if (remapIdx == 1) {
                    GPIOC_ClockEnable();
                    GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_4, NO_REMAP,         GPIO_AF_3);
                    GPIO_Mode_IPU_Init        (GPIOC, GPIO_Pin_5, NO_REMAP,         GPIO_AF_3);
                }
    #endif
            }
            break;

//------------------------------------------------------------------------------
    #if defined(__MM3N1)
        case (u32)UART3:
            if (!remapEn) {
                    GPIOB_ClockEnable();
                    GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_10, NO_REMAP,        NO_FUNCAF);
                    GPIO_Mode_IPU_Init        (GPIOB, GPIO_Pin_11, NO_REMAP,        NO_FUNCAF);
            }
            else {
                if (remapIdx == 0) {
                    GPIOC_ClockEnable();
                    GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_10, EXTI_MAPR_UART3, NO_FUNCAF);
                    GPIO_Mode_IPU_Init        (GPIOC, GPIO_Pin_11, EXTI_MAPR_UART3, NO_FUNCAF);
                }
            }
            break;
    #endif
        default: break;
    }
/* -------------------------------------------------------------------------- */
#endif                   /* ----------------- End Access  ------------------- */
/* -------------------------------------------------------------------------- */
}
/// @}

/// @}

/// @}
