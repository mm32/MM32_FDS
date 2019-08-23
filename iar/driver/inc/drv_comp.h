////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_COMP.H
/// @author   PX Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE COMP
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
#ifndef __DRV_COMP_H
#define __DRV_COMP_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup COMP
/// @brief COMP driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup COMP_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup COMP_Exported_Enumeration
/// @{


/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup COMP_Exported_Types
/// @{
///

////////////////////////////////////////////////////////////////////////////////
/// @brief COMP DCB Structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    u32                 comp;               ///< base offset
    u8                  inm;                ///< inverting input selection
    u8                  inp;                ///< normal phase input selection
    u32                 out;                ///< output selection
    u32                 pol;                ///< output polarity
    u32                 mode;               ///< mode
    u32                 crv;                ///< external reference voltage select
    u32                 src;                ///< external reference voltage source select
    u32                 oflt;               ///< output filter
    u32                 hysteresis;         ///< hysteresis
} tDRV_COMP_DCB;

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV COMP INSTANT structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;                    ///< Instance Prefix
    EM_TYPE             type;                       ///< polling, interrupt
    EM_TYPE             sync;                       ///< TIM sync type
    u32                 cb;                         ///< Sync callback function.
} tDRV_COMP_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup COMP_Exported_Variables
/// @{
#ifdef _DRV_COMP_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
#if defined(__MM0N1)
    static u32  tbIpBase[]          = {COMP1, COMP2};
    static s8 tbSubHandleIdx[]      = {0,     1};
// e.g.
//  static COMP_TypeDef* tbIpBase[] = {COMP1 };
//  static s8 tbSubHandleIdx[]      = {0};
// e.g.
//  static UART_TypeDef* tbIpBase[] = {COMP2};
//  static s8 tbSubHandleIdx[]      = {0};
#endif

#if defined(__MM0P1)
    static u32 tbIpBase[]           = {COMP1, COMP2, COMP3, COMP4, COMP5};
    static s8 tbSubHandleIdx[]      = {0,     1,     2,     3,     4};
// e.g.
//  static u32 tbIpBase[]           = {OCMP1, COMP2, COMP3,        COMP5};
//  static s8 tbSubHandleIdx[]      = {0,     1,     2,     3,     3};
//  static s8 tbSubHandleIdx[]      = {0,     1,     2,     -1,    3};
//  static s8 tbSubHandleIdx[]      = {0,     1,     2,     2,     3};

// e.g.
//  static u32 tbIpBase[]           = {COMP1,                     COMP5};
//  static s8 tbSubHandleIdx[]      = {0,      0,     0,     0,     1};
//  static s8 tbSubHandleIdx[]      = {0,      1,     1,     1,     1};
//  static s8 tbSubHandleIdx[]      = {0,      -1,    -1,    -1,    1};
#endif

#if defined(__MM0Q1)
    static u32 tbIpBase[]           = {COMP1};
    static s8 tbSubHandleIdx[]      = {0};
#endif
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

#if !defined(__MM3N1)
GLOBAL static tDRV_COMP_INSTANT  instance[INSTANCE_NUM];
#endif

#else
#define GLOBAL extern

#endif



#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup COMP_Exported_Functions
/// @{
#if defined(__MM0P1)
void DRV_COMP_CRV_Init(u32 crv, u32 src);
#endif

u32 DRV_GetBase_COMPx(u8 idx);
u8 DRV_COMP_StatusCheck(u32 COMP_Selection_COMPx);
void DRV_COMP_Init(tDRV_COMP_DCB* dcb);
void DRV_COMP_ADC_VreFint(void);
void DRV_COMP_NVIC_Init(u32 comp, EM_TYPE type);
/// @}


/// @}

/// @}
////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_COMP_H */
////////////////////////////////////////////////////////////////////////////////
