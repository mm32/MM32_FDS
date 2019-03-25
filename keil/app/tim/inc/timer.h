////////////////////////////////////////////////////////////////////////////////
/// @file     TIMER.H
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE TIMER
///           EXAMPLES.
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
#ifndef __TIMER_H
#define __TIMER_H

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_TIM
/// @brief TIM example modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM_Exported_Variables
/// @{

#ifdef _TIMER_C_

#define GLOBAL
#else
#define GLOBAL extern

#endif

GLOBAL u32  pulseWidth;      // Unit: 1 us
GLOBAL bool pulseFlag;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM_Exported_Functions
/// @{

/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __TIMER_H */
////////////////////////////////////////////////////////////////////////////////
