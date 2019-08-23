////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_TIM.H
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE TIM
///           BSP LAYER.
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
#ifndef __BSP_TIM_H
#define __BSP_TIM_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM_BSP
/// @brief  TIM BSP modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM_Exported_Types
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM_Exported_Variables
/// @{

#ifdef _BSP_TIM_C_

#define GLOBAL
#else
#define GLOBAL extern

#endif

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM_Exported_Functions
/// @{

//common
void BSP_TIM_CH1_GPIO_Configure(TIM_TypeDef* tim, bool remapEn, u8 remapIdx, bool inoutMode);
void BSP_TIM_CH2_GPIO_Configure(TIM_TypeDef* tim, bool remapEn, u8 remapIdx, bool inoutMode);
void BSP_TIM_CH3_GPIO_Configure(TIM_TypeDef* tim, bool remapEn, u8 remapIdx, bool inoutMode);
void BSP_TIM_CH4_GPIO_Configure(TIM_TypeDef* tim, bool remapEn, u8 remapIdx, bool inoutMode);

//special
void BSP_TIM_PWM_GPIO_Configure         (TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx);
void BSP_TIM_CapturePulse_GPIO_Configure(TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx);
void BSP_TIM_CapturePWM_GPIO_Configure  (TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx);
void BSP_TIM_Encoder_GPIO_Configure     (TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx);
void BSP_TIM_OnePulse_GPIO_Configure    (TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx);
void BSP_TIM_6Step_GPIO_Configure       (TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx);


/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __BSP_TIM_H */
////////////////////////////////////////////////////////////////////////////////
