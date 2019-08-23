////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_DIV.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE DIV
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
#ifndef __DRV_DIV_H
#define __DRV_DIV_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DIV
/// @brief DIV driver modules
/// @{

#if defined(__MM0P1) || defined(__MM0Q1)
////////////////////////////////////////////////////////////////////////////////
/// @defgroup DIV_Exported_Constants
/// @{

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup DIV_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief DIV Instant Structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
    u16                 para;
    u32                 syncTx;
    u32                 syncRx;
} tDRV_DIV_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DIV_Exported_Variables
/// @{

#ifdef _DRV_DIV_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static DIV_TypeDef* tbIpBase[]  = {DIV};
static s8 tbSubHandleIdx[]      = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_DIV_INSTANT  instance[INSTANCE_NUM];

#else
#define GLOBAL extern

#endif


#undef GLOBAL
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DIV_Exported_Functions
/// @{


/// @}

#endif

/// @}

/// @}



////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_DIV_H */
////////////////////////////////////////////////////////////////////////////////
