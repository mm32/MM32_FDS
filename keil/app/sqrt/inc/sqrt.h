////////////////////////////////////////////////////////////////////////////////
/// @file     SQRT.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SQRT
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
#ifndef __SQRT_H
#define __SQRT_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_SQRT
/// @brief SQRT example modules
/// @{


typedef union{
    u32 root;
    u32 squareRoot;
}unSQRT;



////////////////////////////////////////////////////////////////////////////////
/// @defgroup SQRT_Exported_Variables
/// @{
#ifdef _SQRT_C_
#define GLOBAL

u32 sqrtDat[8] = {4 * 65536, 65536, 4096, 256, 64, 16, 4, 1};

#else
#define GLOBAL extern
#endif

GLOBAL unSQRT unSqrt;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SQRT_Exported_Functions
/// @{

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
