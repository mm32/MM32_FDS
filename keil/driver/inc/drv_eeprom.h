////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_EEPROM.H
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE EEPROM
///           DRIVER LAYER.
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
#ifndef __DRV_EEPROM_H
#define __DRV_EEPROM_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EEPROM
/// @brief EEPROM driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EEPROM_Exported_Constants
/// @{

#define EEPROM1     ((u32*)0x0800E000)
#define EEPROM2     ((u32*)0x0800E800)
#define EEPROM3     ((u32*)0x0800F000)
#define EEPROM4     ((u32*)0x0800F800)

/// @}



////////////////////////////////////////////////////////////////////////////////
/// @defgroup EEPROM_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  tEEPROM_INSTANCE
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
    u16                 size;               // block size
    bool                erase;              // erase or no erase
} tEEPROM_INSTANCE;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup EEPROM_Exported_Variables
/// @{

#ifdef _DRV_EEPROM_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static u32* tbIpBase[]      = {EEPROM1, EEPROM2, EEPROM3, EEPROM4};
static s8 tbSubHandleIdx[]  = {0,       1,       2,       3};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tEEPROM_INSTANCE  instance[INSTANCE_NUM];

#else
#define GLOBAL extern
#endif


#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EEPROM_Exported_Functions
/// @{

void DRV_EEPROM_Init(u8 idx);


/// @}

/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif /* __DRV_EEPROM_H */
////////////////////////////////////////////////////////////////////////////////
