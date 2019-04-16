////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_SQRT.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SQRT
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
#ifndef __DRV_SQRT_H
#define __DRV_SQRT_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SQRT
/// @brief SQRT driver modules
/// @{


#if defined(__MM0P1)
////////////////////////////////////////////////////////////////////////////////
/// @defgroup SQRT_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SQRT_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  tDRV_SQRT_INSTANT
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX         sPrefix;
} tDRV_SQRT_INSTANT;


/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup SQRT_Exported_Variables
/// @{

#ifdef _DRV_SQRT_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static SQRT_TypeDef* tbIpBase[]     = {SQRT};
static s8 tbSubHandleIdx[]          = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_SQRT_INSTANT     instance[INSTANCE_NUM];

#else
#define GLOBAL extern

#endif


#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SQRT_Exported_Functions
/// @{

void DRV_SQRT_Init(void);


/// @}

#endif

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_SQRT_H */
////////////////////////////////////////////////////////////////////////////////
