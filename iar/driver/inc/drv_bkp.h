////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_BKP.H
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE BKP
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
#ifndef __DRV_BKP_H
#define __DRV_BKP_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup BKP
/// @brief BKP driver modules
/// @{


#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1)
////////////////////////////////////////////////////////////////////////////////
/// @defgroup BKP_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup BKP_Exported_Enumeration
/// @{

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup BKP_Exported_Types
/// @{

typedef struct {
    tPREFIX             sPrefix;
} tDRV_BKP_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup BKP_Exported_Variables
/// @{
#ifdef _DRV_BKP_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static BKP_TypeDef* tbIpBase[]  = {BKP};
static s8 tbSubHandleIdx[]      = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_BKP_INSTANT  instance[INSTANCE_NUM];

#else
#define GLOBAL extern
#endif



#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup BKP_Exported_Functions
/// @{


/// @}


#endif


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __DRV_BKP_H */
////////////////////////////////////////////////////////////////////////////////
