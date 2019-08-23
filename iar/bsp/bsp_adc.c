////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_ADC.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE ADC BSP LAYER FUNCTIONS.
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
#define _BSP_ADC_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_rcc.h"
#include "hal_gpio.h"

#include "bsp.h"
#include "bsp_adc.h"

#include "common.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief ADC GPIO initialization
/// @param None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_ADC_GPIO_Configure(ADC_TypeDef* ADCx, u32 chs)
{
    if (ADCx == ADC1) {
    #if defined(__REGISTER)
        COMMON_EnableIpClock(emCLOCK_GPIOA);
        GPIOA->CRL = (GPIOA->CRL & 0xFFFF0000);                                 // GPIOA_Pin0~3-Analog
    #else
        COMMON_EnableIpClock(emCLOCK_GPIOA);
        for (u8 i = 0; i < 8; i++) {
            if (chs & 1 << i)
                GPIO_Mode_IN_Init(GPIOA, 1 << i, GPIO_Mode_AIN ,NO_REMAP, NO_REMAP);    // PB0,PB1... be ignored
        }
    #endif
    }
    else {
    #if defined(__REGISTER)
        COMMON_EnableIpClock(emCLOCK_GPIOB);
        GPIOB->CRL = (GPIOB->CRL & 0xFFFFFF00);                                 // GPIOB_Pin0~1-Analog
    #else
        COMMON_EnableIpClock(emCLOCK_GPIOB);
        for (u8 i = 0; i < 8; i++) {
            if (chs & 1 << i)
                GPIO_Mode_IN_Init(GPIOB, 1 << i, GPIO_Mode_AIN ,NO_REMAP, NO_REMAP);    // PB0,PB1... be ignored
        }
    #endif
        // Todo
    }
}

/// @}

/// @}

/// @}
