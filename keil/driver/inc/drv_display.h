////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_DISPLAY.H
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE DISPLAY
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
#ifndef __DRV_DISPLAY_H
#define __DRV_DISPLAY_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DISPALY
/// @brief DISPALY driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DISPLAY_Exported_Constants
/// @{

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup DISPLAY_Exported_Types
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  tDISPLAY_INSTANCE
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
} tDISPLAY_INSTANCE;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup DISPLAY_Exported_Variables
/// @{

#ifdef _DRV_DISPLAY_C_
#define GLOBAL


/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static u32 tbIpBase[]       = {emFILE_DISPLAY};
static s8 tbSubHandleIdx[]  = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDISPLAY_INSTANCE     instance[INSTANCE_NUM];

#else
#define GLOBAL extern



#endif


#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DISPLAY_Exported_Functions
/// @{

void DRV_Display(u8 led);

void Init_Display(void);

//int DISPLAY_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len);

/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __DRV_DISPLAY_H */
////////////////////////////////////////////////////////////////////////////////



