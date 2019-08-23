////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_FLASH.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SPI_FLASH
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
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H



////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_SPI
/// @brief SPI example modules
/// @{

#define      FLASH_SPIx                        SPI2



////////////////////////////////////////////////////////////////////////////////
#if (0)
#define sFLASH_ID                  0X144051
#else
#define sFLASH_ID                  0X154051
#endif

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

#define W25X_WriteEnable            0x06
#define W25X_WriteDisable           0x04
#define W25X_ReadStatusReg          0x05
#define W25X_WriteStatusReg         0x01
#define W25X_ReadData               0x03
#define W25X_FastReadData           0x0B
#define W25X_FastReadDual           0x3B
#define W25X_PageProgram            0x02
#define W25X_BlockErase             0xD8
#define W25X_SectorErase            0x20
#define W25X_ChipErase              0xC7
#define W25X_PowerDown              0xB9
#define W25X_ReleasePowerDown       0xAB
#define W25X_DeviceID               0xAB
#define W25X_ManufactDeviceID       0x90
#define W25X_JedecDeviceID          0x9F




#ifdef _SPI_FLASH_C_
#define GLOBAL
#else
#define GLOBAL extern

#endif
#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////



/// @}

/// @}
u8 spiFlashInit();
void spiFlashWriteEnable();
void spiFlashWaitForWriteEnd();
void spiFlashSectorErase(u32 addr);
void SpiFlashBulkErase();
void spiFlashPageWrite(u32 addr, u8 *pDat, u16 datLen);
void spiFlashBufferWrite(u32 writeAddr, u8 *pDat, u16 datLen);
void spiFlashBufferRead(u32 addr, u8 *pDat, u16 datLen);


////////////////////////////////////////////////////////////////////////////////
#endif /*__SPI_H */
////////////////////////////////////////////////////////////////////////////////
