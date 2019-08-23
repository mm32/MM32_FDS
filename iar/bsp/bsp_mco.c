////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_MCO.C
/// @author   S Yi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE MCO BSP LAYER FUNCTIONS.
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
#define _BSP_MCO_C_

// Files includes  -------------------------------------------------------------
#include "hal_gpio.h"
#include "hal_rcc.h"

#include "bsp.h"
#include "bsp_mco.h"

#include "common.h"
#include "drv_mco.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MCO_BSP
/// @brief RCC driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RCC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure RCC GPIO.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_RCC_GPIO_Configure(void)
{
    #if defined(__REGISTER)
        GPIOA_ClockEnable();
        RCC->APB2ENR |= RCC_APB2ENR_EXTI;
        GPIOA->CRH = (GPIOA->CRH & 0xFFFFFFF0) | (PORT_AFPP | PORT_50MHz) << PORT_BIT8; // GPIOA_Pin8-AF_PP
        #if defined (__MM0N1) || defined(__MM0P1)  || defined(__MM0Q1)
            GPIOA->AFRH &= 0xFFFFFFF0;                                                  // 8-AF0
        #endif
    #else
        GPIOA_ClockEnable();
        COMMON_EnableIpClock(emCLOCK_EXTI);
        GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_8, NO_REMAP, GPIO_AF_0);
    #endif
}

/// @}

/// @}

/// @}
