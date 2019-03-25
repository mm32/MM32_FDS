////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_CRC.H
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE CRC
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
#ifndef __DRV_CRC_H
#define __DRV_CRC_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRC
/// @brief CRC driver modules
/// @{

#if defined(__MM3N1)  || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1)

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRC_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief CRC DCB Structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
    u16                 para;
} tDRV_CRC_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRC_Exported_Variables
/// @{
#ifdef _DRV_CRC_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static CRC_TypeDef* tbIpBase[]  = {CRC};
static s8 tbSubHandleIdx[]      = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_CRC_INSTANT      instance[1];

#else

#define GLOBAL extern

#endif


#undef GLOBAL
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRC_Exported_Functions
/// @{

void DRV_CRC_Init(void);

/// @}

#endif

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_CRC_H */
////////////////////////////////////////////////////////////////////////////////
