////////////////////////////////////////////////////////////////////////////////
/// @file     EXTI_ENCODE.H
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE EXTI_ENCODE
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
#ifndef __EXTI_DECODE_H
#define __EXTI_DECODE_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_EXTI
/// @brief EXTI example modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXTI_Exported_Enumeration
/// @{

typedef enum {
    emPHASE_A = 1,
    emPHASE_B = -1,
} EM_Decode_Phase;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXTI_Exported_Variables
/// @{

#ifdef _EXTI_DECODE_C_

#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL u8 PhaseA_Pre_Status;
GLOBAL u8 PhaseB_Pre_Status;
GLOBAL u8 PhaseA_Status;
GLOBAL u8 PhaseB_Status;

GLOBAL s32 Position;
GLOBAL u32 PulseCount;
GLOBAL s8 PulseDir;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXTI_Exported_Functions
/// @{

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __EXTI_H */
////////////////////////////////////////////////////////////////////////////////

