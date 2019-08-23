////////////////////////////////////////////////////////////////////////////////
/// @file     AES.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE AES
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
#ifndef __AES_H
#define __AES_H

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_AES
/// @brief AES example modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup AES_Exported_Variables
/// @{

#ifdef _AES_C_
#define GLOBAL

//------------------------------------------------------------------------------
u8 key_data_128[16]   = {
    0x65, 0x32, 0x19, 0x8C, 0xC6, 0xE3, 0xF1, 0xF8,
    0x7C, 0x3E, 0x1F, 0x0F, 0x87, 0x43, 0x21, 0x10
};

u8 key_data_192[24]    = {
    0xAD, 0xD6, 0x6B, 0xB5, 0x5A, 0x2D, 0x96, 0xCB,
    0x65, 0x32, 0x19, 0x8C, 0xC6, 0xE3, 0xF1, 0xF8,
    0x7C, 0x3E, 0x1F, 0x0F, 0x87, 0x43, 0x21, 0x10
};

//u8 key_data_256[32]    = {
//  0x68, 0xB4, 0x5A, 0xAD, 0xD6, 0x6B, 0xB5, 0x5A,
//  0xAD, 0xD6, 0x6B, 0xB5, 0x5A, 0x2D, 0x96, 0xCB,
//  0x65, 0x32, 0x19, 0x8C, 0xC6, 0xE3, 0xF1, 0xF8,
//  0x7C, 0x3E, 0x1F, 0x0F, 0x87, 0x43, 0x21, 0x10
//};


u8 key_data_256[32]    = {
    0x04, 0xE4, 0x21, 0x4F, 0xBC, 0x70, 0x0F, 0x2B, 0xF8, 0x91, 0x5D, 0x6C, 0xCC, 0x4D, 0x5D, 0xFA,
    0x48, 0x21, 0x9A, 0x78, 0x09, 0x24, 0xE3, 0x6F, 0xF3, 0x7C, 0xDA, 0xFE, 0x41, 0x1D, 0xEC, 0xFE
};


//------------------------------------------------------------------------------
u8 iv_data_128[16] = {
    0x68, 0xB4, 0x5A, 0xAD, 0xD6, 0x6B, 0xB5, 0x5A,
    0xAD, 0xD6, 0x6B, 0xB5, 0x5A, 0x2D, 0x96, 0xCB
};
u8 iv_data_192[16] = {
    0xC1, 0x60, 0x30, 0x18, 0x8C, 0x46, 0xA3, 0xD1,
    0x68, 0xB4, 0x5A, 0xAD, 0xD6, 0x6B, 0xB5, 0x5A
};
u8 iv_data_256[16] = {
    0x6E, 0xB7, 0x5B, 0x2D, 0x16, 0x0B, 0x05, 0x82,
    0xC1, 0x60, 0x30, 0x18, 0x8C, 0x46, 0xA3, 0xD1
};

//------------------------------------------------------------------------------
u8 input_data_128[16] = {
    'M', 'i', 'n', 'd', 'M', 'o', 't', 'i',
    'o', 'n', '-', 'M', 'M', '3', '2',   0
};
u8 input_data_192[16]  = {
    0x16, 0x0B, 0x05, 0x82, 0x6E, 0xB7, 0x5B, 0x2D,
    0xE7, 0x73, 0xB9, 0xDC, 0x73, 0x39, 0x9C, 0xCE
};
//u8 input_data_256[16]  = {
//  0xE7, 0x73, 0xB9, 0xDC, 0x73, 0x39, 0x9C, 0xCE,
//  0x3A, 0x9D, 0xCE, 0xE7, 0xA2, 0xD1, 0xE8, 0x74
//};



u8 input_data_256[16]  = {
    0x21,  0x25, 0x46,  0x84,  0x15,  0x93,  0x22,  0x11,
    0x65,  0x19, 0x81,  0x16,  0x19,  0x53,  0x24,  0x20
};


//------------------------------------------------------------------------------

u8 cipher_128[16];
u8 cipher_192[16];
u8 cipher_256[16];

bool result_128 = false;
bool result_192 = false;
bool result_256 = false;


//------------------------------------------------------------------------------
u8 inOutBuffer[16];


#else
#define GLOBAL extern

//------------------------------------------------------------------------------
GLOBAL u8 key_data_128[16];
GLOBAL u8 key_data_192[24];
GLOBAL u8 key_data_256[32];
//------------------------------------------------------------------------------
GLOBAL u8 iv_data_128[16];
GLOBAL u8 iv_data_192[16];
GLOBAL u8 iv_data_256[16];
//------------------------------------------------------------------------------
GLOBAL u8 input_data_128[16];
GLOBAL u8 input_data_192[16];
GLOBAL u8 input_data_256[16];
//------------------------------------------------------------------------------
GLOBAL u8 cipher_128[16];
GLOBAL u8 cipher_192[16];
GLOBAL u8 cipher_256[16];
//------------------------------------------------------------------------------
GLOBAL bool result_128;
GLOBAL bool result_192;
GLOBAL bool result_256;
//------------------------------------------------------------------------------
GLOBAL u8 inOutBuffer[16];

#endif

GLOBAL u8 cipher[16];
GLOBAL u8 input_data[16];



#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup AES_Exported_Functions
/// @{


/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////

