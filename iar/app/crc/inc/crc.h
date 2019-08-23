////////////////////////////////////////////////////////////////////////////////
/// @file     CRC.H
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE CRC
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
#ifndef __CRC_H
#define __CRC_H

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_CRC
/// @brief CRC example modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRC_Exported_Constants
/// @{


#define EXPECTED_VALUE_64 0xEAD095D3
//#define EXPECTED_VALUE_32 0xC700585B
/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRC_Exported_Types
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRC_Exported_Variables
/// @{
#ifdef _CRC_C_
#define GLOBAL

u32 dataBuffer[] = {
        0x454acf47, 0x4581798a, 0x454648af, 0x781234cb, 0xc4b67b1a, 0x018afcb7, 0x21872ab7, 0x4654abaf, // 0
        0x654745ac, 0x01231789, 0x11111111, 0x465464ac, 0x11118fb7, 0x7911497c, 0x781031eb, 0xfe458137,
        0x547afcb7, 0x474117fb, 0x422198af, 0xacefb845, 0x412188aa, 0xa1cf2eb3, 0x4a654816, 0x13167461,
        0x42219828, 0xacefb846, 0x412188ac, 0xa1cf3eb4, 0x44814676, 0x41141316, 0x42219823, 0xacefb844,

        0x412188a4, 0xa1cf4eb1, 0x4581afe6, 0xba319841, 0x42219847, 0xacefb848, 0x412188a5, 0xa1cf68eb, // 32
        0x4881cfb6, 0x14abcf45, 0x42219889, 0xacefb849, 0x412188a9, 0xa1cf84eb, 0x4381617a, 0x1216acfe,
        0x42219821, 0xacefb847, 0x412188a7, 0xa1cf14eb, 0x46811896, 0x46542139, 0x42219845, 0xacefb843,
        0x412188ab, 0xa1cf58eb, 0x49865416, 0x46511347, 0x42219865, 0xacefb844, 0x412188a3, 0xa1cf79eb
};


#else
#define GLOBAL extern
#endif

GLOBAL u32 resultCRC;
GLOBAL bool okCRC;


#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRC_Exported_Functions
/// @{

/// @}


/// @}

/// @}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////

