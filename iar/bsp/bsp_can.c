////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_CAN.C
/// @author
/// @version
/// @date
/// @brief    THIS FILE PROVIDES ALL THE CAN BSP LAYER FUNCTIONS.
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
#define _BSP_CAN_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_gpio.h"
#include "hal_can.h"

#include "drv.h"
#include "bsp.h"
#include "drv_can.h"
#include "bsp_can.h"


#if defined(__MM3N1) || defined(__MM0N1)
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CAN_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CAN_Exported_Functions
/// @{

/* -----------------------------------------------------------------------------
----------  P e r i p h e r a l s    i n i t i a l i z a t i o n   -------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  CAN GPIO Config
/// @param  CANx:where x can be 1 to select the CAN peripheral.
/// @param  reampEn: Select the remap status.
/// @param  remap: Select the AFIO status.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_CAN_GPIO_Configure(CAN_Peli_TypeDef* CANx, bool remapEn, u8 remapIdx)
{
/* -------------------------------------------------------------------------- */
#if defined(__REGISTER)          /* ----------- Register Access ------------- */
/* -------------------------------------------------------------------------- */
    if (!remapEn) {
        GPIOA_ClockEnable();

        GPIOA->CRH = (GPIOA->CRH & 0xFFFFF00F)
                    | (u32)PORT_INWUD               << PORT_BIT11
                    | (u32)(PORT_AFPP | PORT_20MHz) << PORT_BIT12;              // 0x000008A0;  // GPIOA_Pin11-IPU, GPIOA_Pin12-AF_PP
        GPIOA->BSRR |= 1 << 12;                                                 // IPU,      IPD:   GPIOA->BSR  |= 1 << 11;
    }
    else {
        if (remapIdx == 0) {
            GPIOB_ClockEnable();
            GPIOB->CRL = (GPIOB->CRL & 0x00FFFFFF)
                    | (u32)PORT_INWUD               << PORT_BIT8
                    | (u32)(PORT_AFPP | PORT_20MHz) << PORT_BIT9;               // GPIOB_Pin8-IPU, GPIOB_Pin9-AF_PP
            GPIOB->BSRR |= 1 << 9;
            }
    }
    RCC->APB2ENR |= RCC_APB2ENR_EXTI;
#if defined(__MM3N1)
    EXTI->MAPR   |= EXTI_MAPR_CAN;                                              // 1 << 2;
#endif
/* ------------------------------------------------------------------------- */
#else
/* ---------- Hardware Abstraction Layer Access ------------- */
/* ------------------------------------------------------------------------- */
    if (!remapEn) {
            GPIOA_ClockEnable();
            GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_12,  NO_REMAP,       GPIO_AF_1);
            GPIO_Mode_IPU_Init        (GPIOA, GPIO_Pin_11, NO_REMAP,        GPIO_AF_1);
    }
    else {
        if (remapIdx == 0) {
            GPIOB_ClockEnable();
            GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_9, EXTI_MAPR_CAN,    GPIO_AF_0);
            GPIO_Mode_IPU_Init        (GPIOB, GPIO_Pin_8, EXTI_MAPR_CAN,    GPIO_AF_0);
        }
    }
    RCC->APB2ENR |= RCC_APB2ENR_EXTI;
#if defined(__MM3N1)
    EXTI->MAPR   |= EXTI_MAPR_CAN;                          // 1 << 2;
#endif
//------------------------------------------------------------------------------
/* -------------------------------------------------------------------------- */
#endif                   /* ----------------- End Access  ------------------- */
/* -------------------------------------------------------------------------- */
}
/// @}

/// @}

/// @}
#endif
