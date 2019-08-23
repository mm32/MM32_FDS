////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_IWDG.H
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE IWDG
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
#ifndef __DRV_IWDG_H
#define __DRV_IWDG_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup IWDG
/// @brief IWDG driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup IWDG_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup IWDG_Exported_Types
/// @{



////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV IWDG INSTANT structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
    EM_TYPE             sync;                                   ///< sync type
} tDRV_IWDG_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup IWDG_Exported_Variables
/// @{

#ifdef _DRV_IWDG_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static IWDG_TypeDef* tbIpBase[] = {IWDG};
static s8 tbSubHandleIdx[]      = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_IWDG_INSTANT     instance[1];

#else
#define GLOBAL extern
#endif

GLOBAL bool IWDG_irqStatus;


#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup IWDG_Exported_Functions
/// @{

void DRV_IWDG_Task(void);

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif /* __DRV_IWDG_H */
////////////////////////////////////////////////////////////////////////////////
