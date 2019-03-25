////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_EXTI.H
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE EXTI
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
#ifndef __DRV_EXTI_H
#define __DRV_EXTI_H



////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXTI
/// @brief EXTI driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXTI_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXTI_Exported_Types
/// @{
typedef struct {
    EXTI_TypeDef*                   exti;               ///< EXTI Base address
    EM_GPIO_PORT                    port;               ///< Port : A.. E
    u32                             line;               ///< Line : 0..15
    EM_EXTI_MODE                    mode;               ///< Event or  Interrupt
    EM_EDGE                         edge;               ///< Rising of Falling
} tDRV_EXTI_DCB;


////////////////////////////////////////////////////////////////////////////////
/// @brief  tDRV_EXTI_INSTANT
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX                         sPrefix;
    EM_TYPE                         sync;               ///< Sync, ASync
    u32                             cb;                 ///< Callback function
} tDRV_EXTI_INSTANT;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXTI_Exported_Constants
/// @{


/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXTI_Exported_Variables
/// @{
#ifdef _DRV_EXTI_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static u32 tbIpBase[]       = {emFILE_EXTI0,       emFILE_EXTI10, emFILE_EXTI11,     emFILE_EXTI13};
static s8 tbSubHandleIdx[]  = {0,1,1,1,1,1,1,1,0,0,1,             2,              2, 3 };
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_EXTI_INSTANT     instance[INSTANCE_NUM];

#else
#define GLOBAL extern
#endif


#undef GLOBAL
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXTI_Exported_Functions
/// @{

void DRV_EXTI_Common(void);
void DVR_EXTI_Init(tDRV_EXTI_DCB* dcb);

void EXTI_CreateFile(HANDLE handle);

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif /* __DRV_EXTI_H */
////////////////////////////////////////////////////////////////////////////////



