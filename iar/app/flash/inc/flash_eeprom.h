////////////////////////////////////////////////////////////////////////////////
/// @file     FLASH_EEPROM.H
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE FLASH_EEPROM
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
#ifndef __FLASH_EEPROM_H
#define __FLASH_EEPROM_H

// Includes  -------------------------------------------------------------------
#include "system_mm32.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_FLASH
/// @brief FLASH example modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup FLASH_Exported_Types
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup FLASH_Exported_Constants
/// @{

#define EEPROM_BASEADDRESS_0        0x0800E000
#define EEPROM_BASEADDRESS_1        0x0800E800
#define EEPROM_BASEADDRESS_2        0x0800F000
#define EEPROM_BASEADDRESS_3        0x0800F800

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup FLASH_Exported_Variables
/// @{
#ifdef _FLASH_EEPROM_C_
#define GLOBAL


#else
#define GLOBAL extern


#endif

GLOBAL u8 bufferWR[64];
GLOBAL u8 bufferRD[64];

#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup FLASH_Exported_Functions
/// @{

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
