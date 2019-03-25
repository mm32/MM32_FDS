////////////////////////////////////////////////////////////////////////////////
/// @file     SPI.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SPI
///           EXAMPLES.
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
#ifndef __SPI_H
#define __SPI_H

// Files includes  -------------------------------------------------------------
#include "types.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_SPI
/// @brief SPI example modules
/// @{




////////////////////////////////////////////////////////////////////////////////
#define READ 0x03       // read data
#define FAST_READ 0x0B  // fast read
#define RDID 0x9F       // JEDEC ID
#define WREN 0x06       // write enable
#define WRDI 0x04       // write disable
#define SE 0x20         // Sector erase
#define BE 0xD8         // block erase
#define CE 0xC7         // chip erase
#define PP 0x02         // page pragram
#define RDSR 0x05       // read status register
#define WRSR 0x01       // write status register
#define DP 0xB9         // drop power
#define RES 0xAB        // reset



/* Size of buffer */
#define BUFFERSIZE (COUNTOF(aTxBuffer))

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */


typedef enum { TRANSFER_WAIT, TRANSFER_COMPLETE, TRANSFER_ERROR } TRANSFER_TypeDef;




#ifdef _SPI_C_

#define GLOBAL
#else
#define GLOBAL extern

#endif

GLOBAL bool txSuccess;
GLOBAL bool rxSuccess;

//GLOBAL u8 rxbyte[16];
//GLOBAL u8 txbyte[16];
//
//GLOBAL u8 SPI_TX_count;
//GLOBAL u8 SPI_RX_count;
//GLOBAL u8 TX_finish_flag;
//GLOBAL u8 RX_finish_flag;
//
//GLOBAL u8 SPI_datalength;

#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////
//void SPI_Configure(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStructure);
//void SPI_GPIO_Init(SPI_TypeDef* SPIx);
//void SPI_NVIC_Init(SPI_TypeDef* SPIx);
//
//
//
//u8 SPIMReadWriteByte(SPI_TypeDef* SPIx, unsigned char tx_data);
//
//void SPIM_Init(SPI_TypeDef* SPIx, SPI_BaudRatePrescaler_TypeDef spi_baud_div);
//u32  SPIMReadWriteByte(SPI_TypeDef* SPIx, u32 tx_data);
//
//
//void SPIM_Init(SPI_TypeDef* SPIx, SPI_BaudRatePrescaler_TypeDef spi_baud_div);
//void SPIM_ReadID(SPI_TypeDef* SPIx, u8* Manufacturer_ID, u8* Deive_ID);
//void SPIM_PageRead(SPI_TypeDef* SPIx, u32 address, u8* p, u16 number);
//void SPIM_DMAPageRead(SPI_TypeDef* SPIx, u32 address, u8* p, u16 number);
//void SPIM_PageProgram(SPI_TypeDef* SPIx, u32 address, u8* p, u16 number);
//void SPIM_DMAPageProgram(SPI_TypeDef* SPIx, u32 address, u8* p, u16 number);
//void SPIM_SectorErase(SPI_TypeDef* SPIx, u32 address);
//void SPIM_BlockErase(SPI_TypeDef* SPIx, u32 address);
//void SPIM_ChipErase(SPI_TypeDef* SPIx);
//void SPIM_Close(SPI_TypeDef* SPIx);
//
//
//void SPIM_CSLow(SPI_TypeDef* SPIx);
//void SPIM_CSHigh(SPI_TypeDef* SPIx);
//void SPIM_TXEn(SPI_TypeDef* SPIx);
//void SPIM_TXDisable(SPI_TypeDef* SPIx);
//void SPIM_RXEn(SPI_TypeDef* SPIx);
//void SPIM_RXDisable(SPI_TypeDef* SPIx);
//void SPIM_checkStatus(SPI_TypeDef* SPIx);
//void SPIM_WriteEnable(SPI_TypeDef* SPIx);
//void SPIM_WriteDisable(SPI_TypeDef* SPIx);


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif /*__SPI_H */
////////////////////////////////////////////////////////////////////////////////
