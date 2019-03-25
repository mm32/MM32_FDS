////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_CRS.H
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE CRS
///           BSP LAYER.
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
#ifndef __BSP_CRS_H
#define __BSP_CRS_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRS_BSP
/// @brief CRS BSP modules

/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRS_Exported_Constants
/// @{

#define CRS_USE_GPIO
#define CRS_GPIO_PA8
#define CRS_MCO_PA9

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRS_Exported_Variables
/// @{

#ifdef _BSP_CRS_C_

#define GLOBAL

#else
#define GLOBAL extern
#endif

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup CRS_Exported_Functions
/// @{

void BSP_CRS_GPIO_Configure(void);
void BSP_CRS_USB_Configure(void);
void BSP_CRS_GPIO_MCO(void);

/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __BSP_CRS_H */
////////////////////////////////////////////////////////////////////////////////
