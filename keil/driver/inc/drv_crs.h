////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_CRS.H
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE CRS
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
#ifndef __DRV_CRS_H
#define __DRV_CRS_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRS
/// @brief CRS driver modules
/// @{


#if defined(__MM0N1)

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRS_Exported_Constants
/// @{

#define TARGET_FREQUENCY                (u32)48000          // unit: KHz
#define TRIMMING_STEP                   (u32)67             // 0.14% @KHz


#ifdef CRS_USE_USB_SOF
    #define CRS_SOURCE_FREQUENCY        (u32)1000           // unit: KHz
#endif

#if defined CRS_USE_GPIO
    #define CRS_SOURCE_FREQUENCY        (u32)1000           // unit: KHz
#endif

#if !defined(CRS_SOURCE_FREQUENCY)
    #define CRS_SOURCE_FREQUENCY        (u32)1000           // unit: KHz
#endif

#if !defined(HSI48_VALUE)
    #define HSI48_VALUE                 (u32)48000000       // unit: KHz
#endif

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRS_Exported_Enumeration
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  CRS Status Enumerate
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    CRS_STATUS_INIT      = 0xFFFF,                          ///< CRS is in initialization
    CRS_STATUS_SYNCOK    = 0x0001,                          ///< CRS sync completed
    CRS_STATUS_SYNCWARN  = 0x0002,                          ///< CRS sync warning
    CRS_STATUS_SYNCERR   = 0x0004,                          ///< CRS sync error
    CRS_STATUS_SYNCESYNC = 0x0008,                          ///< CRS Sync expectation
    CRS_STATUS_SYNCMISS  = 0x0100,                          ///< CRS sync miss
    CRS_STATUS_TRIMOVF   = 0x0200                           ///< CRS trim overflow
} EM_CRS_Status;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRS_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Monitor CRS Structure
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    u8                  CRS_STAGE_FLAG;                         ///< CRS stage flag
    u8                  CRS_ERROR_FLAG;                         ///< CRS error flag
    u32                 CRS_FREQ_NOW;                           ///< CRS now
    u32                 CRS_FREQ_ERROR;                         ///< CRS error
    u32                 CRS_TRIM_TIME;                          ///< CRS trim time
    u32                 CRS_RELOAD;                             ///< CRS reload value
    EM_CRS_Status       CRS_STATUS;                             ///< CRS status
} tCRS_Monitor;


////////////////////////////////////////////////////////////////////////////////
/// @brief  tDRV_CRS_INSTANT
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
} tDRV_CRS_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRS_Exported_Variables
/// @{

#ifdef _DRV_CRS_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static CRS_TypeDef* tbIpBase[]  = {CRS};
static s8 tbSubHandleIdx[]      = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_CRS_INSTANT  instance[INSTANCE_NUM];

#else
#define GLOBAL extern
#endif


GLOBAL tCRS_Monitor         CRS_MonitorWindow;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRS_Exported_Functions
/// @{

void DRV_CRS_SysClockConfig(void);
void DRV_CRS_GPIO_Freq(u32 hsiFreq, u32 srcFreq);
void DRV_CRS_NVIC_Init(void);
void DRV_CRS_USB_Connect(void);

/// @}

#endif

/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_CRS_H */
////////////////////////////////////////////////////////////////////////////////
