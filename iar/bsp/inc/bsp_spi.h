////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_SPI.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SPI
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
#ifndef __BSP_SPI_H
#define __BSP_SPI_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SPI_BSP
/// @brief  SPI BSP modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SPI_Exported_Types
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SPI_Exported_Variables
/// @{
#ifdef _BSP_SPI_C_
#define GLOBAL


#else
#define GLOBAL extern

#endif



#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SPI_Exported_Functions
/// @{

void BSP_SPI_NSS_Configure(SPI_TypeDef* SPIx, bool remapEn, u16 remapIdx, FunctionalState newState);
void BSP_SPI_GPIO_Configure(SPI_TypeDef* SPIx, bool remapEn, u16 remapIdx, bool master, bool nss);

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__BSP_SPI_H */
////////////////////////////////////////////////////////////////////////////////
