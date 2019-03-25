////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_I2C.C
/// @author   S Yi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE I2C BSP LAYER FUNCTIONS.
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
#define _BSP_I2C_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "hal_gpio.h"
#include "hal_i2c.h"

#include "drv.h"
#include "bsp.h"
#include "drv_i2c.h"
#include "bsp_i2c.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup I2C_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup I2C_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure I2C GPIO.
/// @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_I2C_GPIO_Configure(I2C_TypeDef* I2Cx, bool remapEn, u8 remapIdx)
{
/* -------------------------------------------------------------------------- */
#if defined (__REGISTER)         /* ----------- Register Access ------------- */
/* -------------------------------------------------------------------------- */

    switch (*(u32*)&I2Cx) {
//------------------------------------------------------------------------------
        case (u32)I2C1:
            if (!remapEn) {
                GPIOB_ClockEnable();
                GPIOB->CRL = (GPIOB->CRL & 0x00FFFFFF)
                        | (u32)(PORT_AFOD | PORT_20MHz) << PORT_BIT6
                        | (u32)(PORT_AFOD | PORT_20MHz) << PORT_BIT7;           // GPIOB_Pin6-AF_OD, GPIOB_Pin7-AF_OD
    #if defined (__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOB->AFRL = (GPIOB->AFRL & 0x00FFFFFF) | 0x11000000;          // 6-AF1, 7-AF1
    #endif
            }
            else {
                if (remapIdx == 0) {
                    GPIOB_ClockEnable();
                    GPIOB->CRH = (GPIOB->CRH & 0xFFFFFF00)
                            | (u32)(PORT_AFOD | PORT_20MHz) << PORT_BIT8
                            | (u32)(PORT_AFOD | PORT_20MHz) << PORT_BIT9;       // GPIOB_Pin8-AF_OD, GPIOB_Pin9-AF_OD
    #if defined (__MM3N1)
                    RCC->APB2ENR |= RCC_APB2ENR_EXTI;
                    EXTI->MAPR |= EXTI_MAPR_I2C1;
    #endif
    #if defined (__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                    GPIOB->AFRH = (GPIOB->AFRH & 0xFFFFFF00) | 0x00000011;      // 8-AF1, 9-AF1
    #endif
                }
    #if defined (__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                if (remapIdx == 1) {
                    GPIOB_ClockEnable();
                    GPIOB->AFRH = (GPIOB->AFRH & 0xFFFF00FF) | 0x00001100;      // 10-AF1, 11-AF1
                    GPIOB->CRH = (GPIOB->CRH & 0xFFFF00FF)
                            | (u32)(PORT_AFOD | PORT_20MHz) << PORT_BIT10
                            | (u32)(PORT_AFOD | PORT_20MHz) << PORT_BIT11;      // GPIOB_Pin10-AF_OD, GPIOB_Pin11-AF_OD
                }
    #endif
            }
            break;
//------------------------------------------------------------------------------
    #if defined (__MM3N1)
        case (u32)I2C2:
            if (!remapEn) {
                GPIOB_ClockEnable();
                GPIOB->CRH = (GPIOB->CRH & 0xFFFF00FF)
                        | (u32)(PORT_AFOD | PORT_20MHz) << PORT_BIT10
                        | (u32)(PORT_AFOD | PORT_20MHz) << PORT_BIT11;          // GPIOA_Pin10-AF_OD, GPIOA_Pin11-AF_OD
            }
            break;
    #endif
        default: break;
    }
/* -------------------------------------------------------------------------- */
#else       /* ----------- Hardware Abstraction Layer Access ------------- */
/* -------------------------------------------------------------------------- */

    switch (*(u32*)&I2Cx) {
//------------------------------------------------------------------------------
        case (u32)I2C1:
            if (!remapEn) {
                    GPIOB_ClockEnable();
                    GPIO_Mode_AF_OD_20MHz_Init(GPIOB, GPIO_Pin_6, NO_REMAP, GPIO_AF_1);
                    GPIO_Mode_AF_OD_20MHz_Init(GPIOB, GPIO_Pin_7, NO_REMAP, GPIO_AF_1);
            }
            else {
                if (remapIdx == 0) {
                    GPIOB_ClockEnable();
                    GPIO_Mode_AF_OD_20MHz_Init(GPIOB, GPIO_Pin_8, EXTI_MAPR_I2C1, GPIO_AF_1);
                    GPIO_Mode_AF_OD_20MHz_Init(GPIOB, GPIO_Pin_9, EXTI_MAPR_I2C1, GPIO_AF_1);
                }
    #if defined (__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                if (remapIdx == 1) {
                    GPIOB_ClockEnable();
                    GPIO_Mode_AF_OD_20MHz_Init(GPIOB, GPIO_Pin_10, NO_REMAP, GPIO_AF_1);
                    GPIO_Mode_AF_OD_20MHz_Init(GPIOB, GPIO_Pin_11, NO_REMAP, GPIO_AF_1);
                }
    #endif
            }
            break;
//------------------------------------------------------------------------------
    #if defined (__MM3N1)
        case (u32)I2C2:
            if (!remapEn) {
                    GPIOB_ClockEnable();
                    GPIO_Mode_AF_OD_20MHz_Init(GPIOB, GPIO_Pin_10, NO_REMAP, GPIO_AF_1);
                    GPIO_Mode_AF_OD_20MHz_Init(GPIOB, GPIO_Pin_11, NO_REMAP, GPIO_AF_1);
            }
            break;
    #endif
        default: break;
    }
/* -------------------------------------------------------------------------- */
#endif           /* ----------------- End Access  ------------------- */
/* -------------------------------------------------------------------------- */
}


/// @}

/// @}

/// @}
