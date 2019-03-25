////////////////////////////////////////////////////////////////////////////////
/// @file     BSP.C
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE BSP LAYER FUNCTIONS.
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
#define _BSP_C_

// Files includes  -------------------------------------------------------------
#include "hal_rcc.h"
#include "hal_gpio.h"

#include "bsp.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BSP_COMMON
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BSP_Exported_Functions
/// @{


GPIO_TypeDef* GPIO_BaseFromIndex(EM_GPIO_PORT idx)
{
    GPIO_TypeDef* tbGPIO_Base[] = {GPIOA, GPIOB, GPIOC, GPIOD, 0, 0};
    return tbGPIO_Base[idx];
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO multiplexing function initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port i output mode.
///         @arg    0x14: GPIO_Mode_Out_OD    (Universal open drain output)
///         @arg    0x10: GPIO_Mode_Out_PP    (Universal push-pull output)
///         @arg    0x1C: GPIO_Mode_AF_OD     (Multiplex open drain output)
///         @arg    0x18: GPIO_Mode_AF_PP     (Multiplexed push-pull output)
/// @param  speed: Configuring GPIO port output Maximum frequency selection.
///                 This parameter can be one of the following modes:
///         @arg    GPIO_Speed_10MHz            :Maximum speed is 10MHz
///         @arg    GPIO_Speed_20MHz            :Maximum speed is 20MHz
///         @arg    GPIO_Speed_50MHz            :Maximum speed is 50MHz
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @note   The pin should already been configured in Alternate Function
///         mode(AF) using pInitStruct->GPIO_Mode = GPIO_Mode_AF_OD or
///         GPIO_Mode_AF_PP.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_AF_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed, s32 remap, s8 funcAF)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = speed;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode  = mode;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
    exGPIO_PinAFConfig(GPIOx, GPIO_Pin, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO multiplex push-pull output mode initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port output mode.
///         @arg    0x18: GPIO_Mode_AF_PP     (Multiplexed push-pull output)
/// @param  speed: Configuring GPIO port output Maximum frequency selection.
///         @arg    GPIO_Speed_50MHz            :Maximum speed is 50MHz
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_AF_PP_50MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO multiplex push-pull output mode initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port output mode.
///         @arg    0x18: GPIO_Mode_AF_PP     (Multiplexed push-pull output)
/// @param  speed: Configuring GPIO port output Maximum frequency selection.
///         @arg    GPIO_Speed_20MHz            :Maximum speed is 20MHz
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_AF_PP_20MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_AF_PP, GPIO_Speed_20MHz, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO multiplex push-pull output mode initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port output mode.
///         @arg    0x18: GPIO_Mode_AF_PP     (Multiplexed push-pull output)
/// @param  speed: Configuring GPIO port output Maximum frequency selection.
///         @arg    GPIO_Speed_10MHz            :Maximum speed is 10MHz
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_AF_PP_10MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_AF_PP, GPIO_Speed_10MHz, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO multiplex open-drain output mode initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port output mode.
///         @arg    0x1C: GPIO_Mode_AF_OD     (Multiplex open drain output)
/// @param  speed: Configuring GPIO port output Maximum frequency selection.
///         @arg    GPIO_Speed_50MHz            :Maximum speed is 50MHz
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_AF_OD_50MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_AF_OD, GPIO_Speed_50MHz, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO multiplex open-drain output mode initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port output mode.
///         @arg    0x1C: GPIO_Mode_AF_OD     (Multiplex open drain output)
/// @param  speed: Configuring GPIO port output Maximum frequency selection.
///         @arg    GPIO_Speed_20MHz            :Maximum speed is 20MHz
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_AF_OD_20MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_AF_OD, GPIO_Speed_20MHz, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO multiplex open-drain output mode initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port output mode.
///         @arg    0x1C: GPIO_Mode_AF_OD     (Multiplex open drain output)
/// @param  speed: Configuring GPIO port output Maximum frequency selection.
///         @arg    GPIO_Speed_10MHz            :Maximum speed is 10MHz
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_AF_OD_10MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_AF_OD, GPIO_Speed_10MHz, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO open-drain output mode initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port output mode.
///         @arg    0x1C: GPIO_Mode_AF_OD     (Multiplex open drain output)
/// @param  speed: Configuring GPIO port output Maximum frequency selection.
///         @arg    GPIO_Speed_10MHz            :Maximum speed is 10MHz
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_OUT_OD_10MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_Out_OD, GPIO_Speed_10MHz, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO open-drain output mode initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port output mode.
///         @arg    0x1C: GPIO_Mode_AF_OD     (Multiplex open drain output)
/// @param  speed: Configuring GPIO port output Maximum frequency selection.
///         @arg    GPIO_Speed_20MHz            :Maximum speed is 20MHz
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_OUT_OD_20MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_Out_OD, GPIO_Speed_20MHz, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO open-drain output mode initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port output mode.
///         @arg    0x1C: GPIO_Mode_AF_OD     (Multiplex open drain output)
/// @param  speed: Configuring GPIO port output Maximum frequency selection.
///         @arg    GPIO_Speed_50MHz            :Maximum speed is 50MHz
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_OUT_OD_50MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, remap, funcAF);
}






void GPIO_Mode_OUT_PP_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
    if ((GPIOx == GPIOA) && (GPIO_Pin == 13) || (GPIOx == GPIOA) && (GPIO_Pin == 14))
        return;
    u32 remap = ((GPIOx == GPIOB) && (GPIO_Pin == 3)) ?
//              (//((GPIOx == GPIOA) && (GPIO_Pin == 15)) ||
//               ((GPIOx == GPIOB) && (GPIO_Pin == 3)) ||
//               ((GPIOx == GPIOB) && (GPIO_Pin == 4))) ?
    EXTI_MAPR_SWJ_JTAGDISABLE :  NO_REMAP;

    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, remap, 0);
}

void GPIO_Mode_OUT_OD_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
    if ((GPIOx == GPIOA) && (GPIO_Pin == 13) || (GPIOx == GPIOA) && (GPIO_Pin == 14))
        return;

    u32 remap = ((GPIOx == GPIOB) && (GPIO_Pin == 3)) ?
//              (((GPIOx == GPIOA) && (GPIO_Pin == 15)) ||
//               ((GPIOx == GPIOB) && (GPIO_Pin == 3)) ||
//               ((GPIOx == GPIOB) && (GPIO_Pin == 4))) ?
    EXTI_MAPR_SWJ_JTAGDISABLE :  NO_REMAP;

    GPIO_AF_Init(GPIOx, GPIO_Pin, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, remap, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO input function initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port input mode.
///         @arg    0x00: GPIO_Mode_AIN       (Analog input)
///         @arg    0x04: GPIO_Mode_FLOATING  (Floating input)
///         @arg    0x28: GPIO_Mode_IPD       (Pull down input)
///         @arg    0x48: GPIO_Mode_IPU       (Pull up input)
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @note   The pin should already been configured in Alternate Function
///         mode(AF) using pInitStruct->GPIO_Mode = GPIO_Mode_AF_OD or
///         GPIO_Mode_AF_PP.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_IN_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, GPIOMode_TypeDef mode, s32 remap, s8 funcAF)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
    exGPIO_PinAFConfig(GPIOx, GPIO_Pin, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO pull-up input function initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port input mode.
///         @arg    0x48: GPIO_Mode_IPU       (Pull up input)
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_IPU_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_Mode_IN_Init(GPIOx, GPIO_Pin, GPIO_Mode_IPU, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO pull-down input function initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port input mode.
///         @arg    0x28: GPIO_Mode_IPD       (Pull down input)
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_IPD_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_Mode_IN_Init(GPIOx, GPIO_Pin, GPIO_Mode_IPD, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO floating input input function initialization.
/// @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
/// @param  GPIO_Pin: specifies the port bit to be written.
///         This parameter can be any combination of GPIO_Pin_x where x can be
///         (0..15).
/// @param  mode: Configuring GPIO port input mode.
///         @arg    0x04: GPIO_Mode_FLOATING  (Floating input)
/// @param  remap: selects the pin to remap.
///         This parameter can be one of the following values:
///         @arg    EXTI_MAPR_PD01              : SPI1 Alternate Function mapping
///         @arg    EXTI_MAPR_I2C1              : I2C1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART1             : UART1 Alternate Function mapping
///         @arg    EXTI_MAPR_UART3             : UART3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1_PARTIAL      : TIM1 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM1              : TIM1 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL1     : TIM2 Partial1 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2_PARTIAL2     : TIM2 Partial2 Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM2              : TIM2 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3_PARTIAL      : TIM3 Partial Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_TIM3              : TIM3 Full Alternate Function
///                 mapping
///         @arg    EXTI_MAPR_CAN               : CAN1 Alternate Function mapping
///         @arg    EXTI_MAPR_PD01              : PD01 Alternate Function mapping
///         @arg    EXTI_MAPR_SWJ_NOJNTRST      : Full SWJ Enabled (JTAG-DP + SW-DP)
///                 but without JTRST
///         @arg    EXTI_MAPR_SWJ_JTAGDISABLE   : JTAG-DP Disabled and SW-DP Enabled
///         @arg    EXTI_MAPR_SWJ_DISABLE       : Full SWJ Disabled (JTAG-DP + SW-DP)
/// @param  funcAF: selects the pin to used as Alternate function.
///         This parameter can be the GPIO_AF_x where x can be (0..7).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Mode_FLOATING_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF)
{
    GPIO_Mode_IN_Init(GPIOx, GPIO_Pin, GPIO_Mode_FLOATING, remap, funcAF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIOx clock enable.
/// @param  GPIOx: where x can be (A..F) to select the GPIO peripheral.
/// @param  portNum: specifies the port bit to be written.
///         @arg    0: PORTA
///         @arg    1: PORTB
///         @arg    2: PORTC
///         @arg    3: PORTD
///         @arg    4: PORTE
///         @arg    5: PORTF
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIOx_ClockEnable(EM_GPIO_PORT port)
{
#if defined(__REGISTER)
    #if defined(__MM3N1)
        RCC->APB2ENR |= 1 << (port + 2);
    #endif
    #if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
        RCC->AHBENR |=  1 << (port + 17);
    #endif
#else
    #if defined(__MM3N1)
        RCC_APB2PeriphClockCmd(1 << (port + 2), ENABLE);
    #endif
    #if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
        RCC_AHBPeriphClockCmd(1 << (port + 17), ENABLE);
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIOA clock enable.
/// @param  portNum: specifies the port bit to be written.
/// @arg    0                           : PORTA
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIOA_ClockEnable()    {   GPIOx_ClockEnable(emGPIOA); }

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIOB clock enable.
/// @param  portNum: specifies the port bit to be written.
/// @arg    1                           : PORTB
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIOB_ClockEnable()    {   GPIOx_ClockEnable(emGPIOB); }

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIOC clock enable.
/// @param  portNum: specifies the port bit to be written.
/// @arg    2                           : PORTC
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIOC_ClockEnable()    {   GPIOx_ClockEnable(emGPIOC); }

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIOD clock enable.
/// @param  portNum: specifies the port bit to be written.
/// @arg    3                           : PORTD
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIOD_ClockEnable()    {   GPIOx_ClockEnable(emGPIOD); }

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIOE clock enable.
/// @param  portNum: specifies the port bit to be written.
/// @arg    4                           : PORTE
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIOE_ClockEnable()    {   GPIOx_ClockEnable(emGPIOE); }

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIOF clock enable.
/// @param  portNum: specifies the port bit to be written.
/// @arg    5                           : PORTF
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIOF_ClockEnable()    {   GPIOx_ClockEnable(emGPIOF); }

/// @}F
/// @}

/// @}
