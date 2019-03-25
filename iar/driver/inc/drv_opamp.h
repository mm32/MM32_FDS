////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_OPAMP.H
/// @author   PX Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE OPAMP
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
#ifndef __DRV_OPAMP_H
#define __DRV_OPAMP_H



#if defined(__MM0P1)
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup OPAMP
/// @{
///

////////////////////////////////////////////////////////////////////////////////
/// @defgroup OPAMP_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup OPAMP_Exported_Enumeration
/// @{


/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup OPAMP_Exported_Types
/// @{
///

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV OPAMP INSTANT structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
} tDRV_OPAMP_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup OPAMP_Exported_Variables
/// @{
#ifdef _DRV_OPAMP_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static OPAMP_TypeDef* tbIpBase[]    = {OPAMP};
static s8 tbSubHandleIdx[]          = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_OPAMP_INSTANT    instance[INSTANCE_NUM];

#else
#define GLOBAL extern

#endif



#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup OPAMP_Exported_Functions
/// @{

u32 DRV_GetBase_OPAMPx(u8 idx);
void DRV_OPAMP_Init(u32 OPAMP_Selection, tDRV_OPAMP_INSTANT* dcb);
#endif

/// @}


/// @}

/// @}
////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_OPAMP_H */
////////////////////////////////////////////////////////////////////////////////
