////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_CRS.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE CRS BSP LAYER FUNCTIONS.
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
#define _BSP_CRS_C_

// Files includes  -------------------------------------------------------------
#include "hal_crs.h"
#include "hal_gpio.h"
#include "hal_nvic.h"
#include "hal_rcc.h"

#include "bsp.h"
#include "bsp_led.h"
#include "bsp_crs.h"

#include "common.h"


#if defined(__MM0N1) || defined(__MM3O1)
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CRS_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CRS_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize CRS GPIO, if the sync source is GPIO.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_CRS_GPIO_Configure(void)
{
#if defined CRS_GPIO_PA8
    COMMON_EnableIpClock(emCLOCK_GPIOA);
    GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_8, NO_REMAP, GPIO_AF_4);
#endif
#if defined CRS_GPIO_PD0
    COMMON_EnableIpClock(emCLOCK_GPIOD);
    GPIO_Mode_AF_PP_50MHz_Init(GPIOD, GPIO_Pin_0, NO_REMAP, GPIO_AF_0);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize CRS USB, if the sync source is USB.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_CRS_USB_Configure(void)
{
    COMMON_EnableIpClock(emCLOCK_GPIOA);
    COMMON_EnableIpClock(emCLOCK_USB);

    GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_11, GPIO_Mode_AIN ,NO_REMAP, NO_FUNCAF);
    GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_12, GPIO_Mode_AIN ,NO_REMAP, NO_FUNCAF);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO MCO configuration.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_CRS_GPIO_MCO()
{
    COMMON_EnableIpClock(emCLOCK_GPIOA);
    COMMON_EnableIpClock(emCLOCK_EXTI);

#if defined CRS_MCO_PA8
    GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_8, NO_REMAP, GPIO_AF_0);
#endif
#if defined CRS_MCO_PA9
    GPIO_Mode_AF_PP_50MHz_Init(GPIOA, GPIO_Pin_9, NO_REMAP, GPIO_AF_5);
#endif
    RCC_MCOConfig(RCC_MCO_HSI);
}



/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
