////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_WWDG.H
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE WWDG
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
#ifndef __DRV_WWDG_H
#define __DRV_WWDG_H

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup WWDG
/// @brief WWDG driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup WWDG_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup WWDG_Exported_Types
/// @{

//typedef struct {
//  WWDG_TypeDef*       WWDGx;                              ///< Registers base address
//  bool                itEn;                               ///< Interrupt enable
//  u32                 pres;                               ///< Prescaler value
//  u8                  cnt;                                ///< Counter value: 0x40 ~ 0x7F
//} tDRV_WWDG_DCB;

typedef struct {
    tPREFIX             sPrefix;
    EM_TYPE             type;                               ///< transform type
    EM_TYPE             sync;                               ///< sync type
    u32                 cb;                                 ///< callback
} tDRV_WWDG_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup WWDG_Exported_Variables
/// @{
#ifdef _DRV_WWDG_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static WWDG_TypeDef* tbIpBase[] = {WWDG};
static s8 tbSubHandleIdx[]      = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_WWDG_INSTANT instance[INSTANCE_NUM];

#else
#define GLOBAL extern
#endif


#undef GLOBAL
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup WWDG_Exported_Functions
/// @{

void DRV_WWDG_Task(void);

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_WWDG_H */
////////////////////////////////////////////////////////////////////////////////
