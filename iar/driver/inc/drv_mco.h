////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_MCO.H
/// @author   S Yi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE MCO
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
#ifndef __DRV_RCC_H
#define __DRV_RCC_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RCC
/// @brief RCC driver modules
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @defgroup RCC_Exported_Constants
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @brief MCO output enumerate definition
/// @anchor EM_MCO_OUTPUT
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    emMCO_NoClock,                                          ///< NoClock
    emMCO_LSI,                                              ///< LSI
    emMCO_LSE,                                              ///< LSE
    emMCO_SYSCLK,                                           ///< SYSCLK
    emMCO_HSI,                                              ///< HSI
    emMCO_HSE,                                              ///< HSE
    emMCO_PLL_DIV2                                          ///< PLL_DIV2
} EM_MCO_OUTPUT;

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RCC_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief RCC Instant Data Control Block
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX                 sPrefix;                        ///< Instance Prefix
    u8                      src;                            ///< Selected MCO clock source
} tDRV_RCC_INSTANT;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup RCC_Exported_Variables
/// @{
#ifdef _DRV_RCC_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static RCC_TypeDef* tbIpBase[]  = {RCC};
static s8 tbSubHandleIdx[]      = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_RCC_INSTANT      instance[INSTANCE_NUM];


#else
#define GLOBAL extern
#endif


#undef GLOBAL
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RCC_Exported_Functions
/// @{

#if defined(__MM3N1) || defined(__MM0N1) || defined (__MM0P1)
void DRV_RCC_PllConfig(RCC_PLLSource_TypeDef pllSrc, RCC_PLLMul_TypeDef pllMul);
#endif

void DRV_RCC_SysClkConfig(SYSCLK_TypeDef sysClkSrc);

/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_RCC_H */
////////////////////////////////////////////////////////////////////////////////









