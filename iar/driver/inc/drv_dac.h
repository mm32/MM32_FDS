////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_DAC.H
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE DAC
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
#ifndef __DRV_DAC_H
#define __DRV_DAC_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DAC
/// @brief DAC driver modules
/// @{

#if defined(__MM3N1)
////////////////////////////////////////////////////////////////////////////////
/// @defgroup DAC_Exported_Constants
/// @{

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup DAC_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief DAC DCB Structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX                     sPrefix;
    EM_TYPE                     type;                       // emTYPE_DMA or NULL
    bool                        ext;                        // External hardware trigger enable: false(Software trigger),true(Hardware trigger)
    emDACWAVE_TypeDef           wave;                       // Does not generate or generate triangular waves, noise
} tDRV_DAC_INSTANT;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup DAC_Exported_Variables
/// @{

#ifdef _DRV_DAC_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
#if defined(__MM3N1)
    static DAC_TypeDef* tbIpBase[]  = {DAC, DAC};
    static s8 tbSubHandleIdx[]      = {0,     1};

// e.g.
//  static DAC_TypeDef* tbIpBase[]  = {     DAC };
//  static s8 tbSubHandleIdx[]      = {0,      0};
//  static s8 tbSubHandleIdx[]      = {-1,     0);
#endif
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_DAC_INSTANT      instance[INSTANCE_NUM];

#else
#define GLOBAL extern
#endif



#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup DAC_Exported_Functions
/// @{

void DRV_DAC_GPIO_Configure(u8 idx);
void DRV_DAC_TIMInit(u16 psc, u16 period);
void DRV_DAC_Init(tDRV_DAC_INSTANT* dcb);
void DRV_DAC_StartHW(void);
void DRV_DAC_StartSW(u8 idx);
void DRV_DAC_DMA_Init(u8 idx, u16* add, u16 buffsize);

/// @}

#endif

/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_DAC_H */
////////////////////////////////////////////////////////////////////////////////
