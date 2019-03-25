////////////////////////////////////////////////////////////////////////////////
/// @file     DIV.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE DIV
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
#ifndef __DIV_EXAMPLE_H
#define __DIV_EXAMPLE_H

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_DIV
/// @brief DIV example modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DIV_Exported_Variables
/// @{

typedef struct {
    u32 dvdr;
    u32 dvsr;
} DIV_SRC_TypeDef;

typedef struct {
    u32 quotr;
    u32 rmdr;
} DIV_DESTI_TypeDef;

typedef union{
    DIV_SRC_TypeDef     source;
    DIV_DESTI_TypeDef   result;
}unDIV;


#ifdef _DIV_C_
#define GLOBAL

u32 dataBuffer_1[32] = {
    0x12345678, 0x09876554, 0x09822346, 0x98734662, 0x00000001, 0xFFFFFFFF, 0x11111111, 0x22222222,
    0x33333333, 0x78645323, 0x04399039, 0x04367464, 0x42345678, 0x09873554, 0x09822346, 0x98734662,
    0x00100001, 0x1FFFFFFF, 0x11511111, 0x22422222, 0x53333333, 0x78645323, 0x04399039, 0x04367464,
    0x00140001, 0x11FFFFFF, 0x12111111, 0x29422222, 0x53333333, 0x78644323, 0x04369039, 0x04767464,
};

u32 dataBufer_2[32] = {
    9, 7, 8, 2, 33, 44, 66, 77, 11, 22, 1, 4, 44, 44, 11, 8, 45, 6, 3, 9, 2332, 989, 23, 77, 79, 345, 12, 43, 67, 89, 78, 23,
};

GLOBAL int errCnt = 0;

#else
#define GLOBAL extern

GLOBAL int errCnt;

#endif

GLOBAL unDIV div;
GLOBAL u32 quotient;
GLOBAL u32 residue;
GLOBAL u32 result[2];

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DIV_Exported_Functions
/// @{

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
