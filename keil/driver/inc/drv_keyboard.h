////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_KEYBOARD.H
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE KEYBOARD
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
#ifndef __DRV_KEYBOARD_H
#define __DRV_KEYBOARD_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup KEYBOARD
/// @brief KEYBOARD driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup KEY_DISPLAY_Exported_Constants
/// @{


/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup KEYBOARD_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  tKEYBOARD_INSTANCE
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
} tKEYBOARD_INSTANCE;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup KEYBOARD_Exported_Variables
/// @{

#ifdef _DRV_KEYBOARD_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static u32 tbIpBase[]       = {emFILE_KEYBOARD};
static s8 tbSubHandleIdx[]  = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static  tKEYBOARD_INSTANCE   instance[INSTANCE_NUM];

#else
#define GLOBAL extern


#endif


GLOBAL s8 KEY_buffer[8];

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup KEYBOARD_Exported_Functions
/// @{

bool DRV_KEY_GetKeyValue(void);
void Init_KeyBoard(void);
int GetKeyValue(u8* ptr);

/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __DRV_KEYBOARD_H */
////////////////////////////////////////////////////////////////////////////////



