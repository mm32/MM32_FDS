////////////////////////////////////////////////////////////////////////////////
/// @file     DAC.H
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE DAC
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
#ifndef __DAC_H
#define __DAC_H

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_DAC
/// @brief DAC example modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DAC_Exported_Types
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DAC_Exported_Variables
/// @{
#ifdef _DAC_C_
#define GLOBAL

u16 sineWave1[32] = {
    2448,2832,3186,3496,3751,3940,4057,4095,4057,3940,
    3751,3496,3186,2832,2448,2048,1648,1264,910,600,345,
    156,39,0,39,156,345,600,910,1264,1648,2048
};

u16 sineWave2[32] = {
    0,39,156,345,600,910,1264,1648,2448,2832,3186,
    3496,3751,3940,4057,4095,4057,3940,3751,3496,
    3186,2832,2448,2048,1648,1264,910,600,345,156,39,0
};

#else
#define GLOBAL extern
u16 sineWave1[32];
u16 sineWave2[32];

#endif



#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DAC_Exported_Functions
/// @{

/// @}


/// @}

/// @}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
