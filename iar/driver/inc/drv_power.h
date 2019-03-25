////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_POWER.H
/// @author   MY Zhang
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE POWER
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
#ifndef __DRV_POWER_H
#define __DRV_POWER_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup PWR
/// @brief PWR driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup PWR_Exported_Constants
/// @{

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup PWR_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV POWER INSTANT structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
    EM_TYPE             sync;                                   ///< Sync or  Async mode
    u32                 cb1;                                    ///< Callback function1
    u32                 cb2;                                    ///< Callback function1
    EM_PVD_LEVEL        pvdLevel;                               ///< PVD voltage level
    u32                 falling;                                ///< Falling edge
    u32                 rising;                                 ///< Rising edge

} tPOWER_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup PWR_Exported_Variables
/// @{
#ifdef _DRV_POWER_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static u32  tbIpBase[]          = {emFILE_PVD_R, emFILE_PVD_F, emFILE_STOP,  emFILE_SLEEP, emFILE_STANDBY};
static s8   tbSubHandleIdx[]    = {0, 1, 2, 3, 4};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static   tPOWER_INSTANT      instance[INSTANCE_NUM];

#else
#define GLOBAL extern

#endif

GLOBAL bool PVD_status;
GLOBAL bool PVD_change;


#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup PWR_Exported_Functions
/// @{


void DRV_KEY_EXTI_Init(void);
void DRV_PWR_EXTI_Init(EM_EDGE edge);
void DRV_PWR_PVD_Init(u32 level);
void DRV_PWR_WakeUp_Init(void);

void DRV_PWR_Standby(emPWR_Wait_Mode_Typedef mode);
void DRV_PWR_Stop(emPWR_Wait_Mode_Typedef mode);
void DRV_PWR_Sleep(emPWR_Wait_Mode_Typedef mode);

/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_POWER_H */
////////////////////////////////////////////////////////////////////////////////
