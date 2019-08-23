////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_RTC.H
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE RTC
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
#ifndef __DRV_RTC_H
#define __DRV_RTC_H

// Files includes  -------------------------------------------------------------
#include "types.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC
/// @brief RTC driver modules
/// @{


#if defined(__MM3N1) ||defined(__MM3O1)

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Types
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV RTC INSTANT structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
    EM_TYPE             sync;
    u32                 cbSecond;
    u32                 cbAlarm;
    u32                 second;

    bool                fSecond :1;
    bool                fAlarm  :1;
} tDRV_RTC_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Variables
/// @{
#ifdef _DRV_RTC_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static RTC_TypeDef* tbIpBase[]  = {RTC};
static s8 tbSubHandleIdx[]      = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL  static tDRV_RTC_INSTANT     instance[INSTANCE_NUM];

#else
#define GLOBAL extern
#endif



#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Functions
/// @{

void DRV_RTC_Init(void);
void DRV_RTC_NVIC_Init(void);
bool DRV_RTC_CheckValid(void);

/// @}

#endif


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_RTC_H */
////////////////////////////////////////////////////////////////////////////////
