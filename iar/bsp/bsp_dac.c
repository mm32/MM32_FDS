////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_DAC.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE DAC BSP LAYER FUNCTIONS.
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
#define _BSP_DAC_C_

// Files includes  -------------------------------------------------------------
#include "hal_gpio.h"
#include "hal_rcc.h"

#include "bsp.h"
#include "bsp_dac.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DAC_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DAC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  DAC GPIO initialization
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM3N1)
void BSP_DAC_GPIO_Configure(u8 idx)
{
    #if defined(__REGISTER)
        GPIOA_ClockEnable();
        if (idx == 0)   GPIOA->CRL = (GPIOA->CRL & 0xFFF0FFFF);                 // GPIOA_Pin4~5-Analog
        else            GPIOA->CRL = (GPIOA->CRL & 0xFF0FFFFF);
    #else
        GPIOA_ClockEnable();
        if (idx == 0)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_AIN ,NO_REMAP, NO_REMAP);
        else            GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_AIN ,NO_REMAP, NO_REMAP);
    #endif
}
#endif

/// @}

/// @}

/// @}
