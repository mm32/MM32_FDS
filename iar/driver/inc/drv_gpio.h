////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_GPIO.H
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE GPIO
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
#ifndef __DRV_GPIO_H
#define __DRV_GPIO_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup GPIO
/// @brief GPIO driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup GPIO_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup GPIO_Exported_Types
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  tDRV_GPIO_INSTANT
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
    u16                 dir;                ///< GPIO direction, input or output
    u16                 mode;               ///< mode, can be Push-pull output or Open drain output, can be pull up or pull down accroding to dir
    u16                 inSta;              ///< in input mode, floating or pull up/down
} tDRV_GPIO_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Variables
/// @{
#ifdef _DRV_GPIO_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
static GPIO_TypeDef* tbIpBase[]     = {GPIOA, GPIOB, GPIOC, GPIOD};
static s8 tbSubHandleIdx[]          = {0,     1,     2,     3};

//e.g.
//static GPIO_TypeDef* tbIpBase[]   = {GPIOA,        GPIOC, GPIOD};
//static s8 tbSubHandleIdx[]        = {0,     -1,     1,     2};
//static s8 tbSubHandleIdx[]        = {0,     0,      1,     2};
//
//static GPIO_TypeDef* tbIpBase[]   = {               GPIOC, GPIOD};
//static s8 tbSubHandleIdx[]        = {-1,     -1,    0,     -1};
//static s8 tbSubHandleIdx[]        = {0,      -1,    0,     -1};
//static s8 tbSubHandleIdx[]        = {-1,     0,     0,     -1};
#endif

#if defined(__MM3O1)
static GPIO_TypeDef* tbIpBase[]     = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};
static s8 tbSubHandleIdx[]          = {0,     1,     2,     3,     4,     5,     6};

//e.g.
//static GPIO_TypeDef* tbIpBase[]   = {GPIOA, GPIOB, GPIOC,        GPIOE, GPIOF, GPIOG};
//static s8 tbSubHandleIdx[]        = {0,     1,     2,     -1,    3,     4,     5};
//static s8 tbSubHandleIdx[]        = {0,     1,     2,     0,     3,     4,     5};
//
//static GPIO_TypeDef* tbIpBase[]   = {       GPIOB, GPIOC,        GPIOE, GPIOF, GPIOG};
//static s8 tbSubHandleIdx[]        = {-1,     0,     1,     -1,       2,     3,     4};
//static s8 tbSubHandleIdx[]        = {0,      0,     1,     0,        2,     3,     4};
#endif
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL  tDRV_GPIO_INSTANT   instance[INSTANCE_NUM];

#define GLOBAL
#else
#define GLOBAL extern
#endif


#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Functions
/// @{


/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_RTC_H */
////////////////////////////////////////////////////////////////////////////////
