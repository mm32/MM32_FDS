////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_PROTECT.H
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE PROTECT
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
#ifndef __DRV_PROTECT_H
#define __DRV_PROTECT_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup PROTECT
/// @brief PROTECT driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup PROTECT_Exported_Constants
/// @{


/// @}



////////////////////////////////////////////////////////////////////////////////
/// @defgroup PROTECT_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV PROTECT INSTANT structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
} tPROTECT_INSTANCE;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup PROTECT_Exported_Variables
/// @{

#ifdef _DRV_PROTECT_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static PROTECT_TypeDef* tbIpBase[]  = {PROTECT};
static s8 tbSubHandleIdx[]          = {0 };
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tPROTECT_INSTANCE  instance[INSTANCE_NUM];

#else
#define GLOBAL extern
#endif


#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup PROTECT_Exported_Functions
/// @{



/// @}

/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif /* __DRV_PROTECT_H */
////////////////////////////////////////////////////////////////////////////////
