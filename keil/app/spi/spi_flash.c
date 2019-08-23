////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_FLASH.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE SPI_FLASH EXAMPLE.
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

////////////////////////////////////////////////////////////////////////////////
#define _SPI_FLASH_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "hal_spi.h"
#include "bsp_spi.h"

#include "drv.h"
#include "spi_flash.h"


u8 spiFlashInit()
{
    u8 readIDBuffer[3];

    // Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hSPI = CreateFile(emIP_SPI);
    if (hSPI == NULL)       while(1);


// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_SPI_DCB dcb = {
// Base parameter
        .hSub               = emFILE_SPI2,
        .type               = emTYPE_Polling,           // emTYPE_Polling,emTYPE_IT,emTYPE_DMA,
        .block              = emTYPE_Block,         // emTYPE_Blocking, emTYPE_NonBlocking

// Callback function
        .sync               = emTYPE_Sync,          // Sync, ASync
        .cbTx               = NULL,    //tx callback function access address
        .cbRx               = NULL,    //rx callback function access address

// operation mode
        .remapEn            = false,                // Disable : 0 ,Enable : 1
        .remapIdx           = 0,                    // u8 value

// SPI parameter
        .prescaler          = 4,
        .fastMode           = true,
        .mode               = emSPI_MODE_3,
        .hardNss            = false,
        .firstLsb           = false,
        .master             = true,
        .protocol           = 1,
        .command            = W25X_JedecDeviceID,
        .parameterLength    = 0

    };

    if (!OpenFile(hSPI, (void*)&dcb))       while(1);

    ReadFile(hSPI, emFILE_SPI2, readIDBuffer, 3);

    if(*(u32 *)readIDBuffer != sFLASH_ID){
        return 0;
    }
    else {
        return 1;
    }
}


u8 spiFlashWrite(SPI_TypeDef* SPIx, u8 command, u8 *pParameter, u8 paraLen, u8 *pDat, u16 datLen)
{
    BSP_SPI_NSS_Configure(SPIx, 0, 0, ENABLE);

    WRITE_REG(SPIx->TDR, command);

    for(u8 i = 0; i < paraLen; i++){
        WRITE_REG(SPIx->TDR, *pParameter++);
    }

    while(datLen > 0) {
        if (SPI_GetITStatus(SPIx, SPI_IT_TX)){
            SPI_ClearITPendingBit(SPIx, SPI_IT_TX);
            WRITE_REG(SPIx->TDR, *pDat++);
            datLen --;
        }
    }
    while(SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT) == 0) {
    }


    while(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)){
        READ_REG(SPIx->RDR);
    }

    BSP_SPI_NSS_Configure(SPIx, 0, 0, DISABLE);

    return 1;

}

u8 spiFlashRead(SPI_TypeDef* SPIx, u8 command, u8 *pParameter, u8 paraLen, u8 *pDat, u16 datLen)
{
    BSP_SPI_NSS_Configure(SPIx, 0, 0, ENABLE);

    WRITE_REG(SPIx->TDR, command);

    for(u8 i = 0; i < paraLen; i++){
        WRITE_REG(SPIx->TDR, *pParameter++);
    }

    while(SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT) == 0) {
    }
    while(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)){
        READ_REG(SPIx->RDR);
    }

    while(datLen > 0) {
        WRITE_REG(SPIx->TDR, 0xFF);
        while(!SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL));
        *pDat++ = (u8)READ_REG(SPIx->RDR);
        datLen --;
    }

    BSP_SPI_NSS_Configure(SPIx, 0, 0, DISABLE);

    return 1;
}

void spiFlashWriteEnable()
{
    spiFlashWrite(FLASH_SPIx, W25X_WriteEnable, NULL, 0, NULL, 0);
}

void spiFlashWaitForWriteEnd()
{
    u8 status = 0;
    do{
        spiFlashRead(FLASH_SPIx, W25X_ReadStatusReg, NULL, 0, &status, 1);
    }while(status & 0x01);
}

void spiFlashSectorErase(u32 addr)
{
    u8 temp[3] = {(addr & 0xFF0000) >> 16, (addr & 0xFF00) >> 8, (addr & 0xFF)};
    spiFlashWriteEnable();
    spiFlashWaitForWriteEnd();
    spiFlashWrite(FLASH_SPIx, W25X_SectorErase, temp, 3, NULL, 0);
    spiFlashWaitForWriteEnd();
}

void SpiFlashBulkErase()
{
    spiFlashWriteEnable();
    spiFlashWrite(FLASH_SPIx, W25X_ChipErase, NULL, 0, NULL, 0);
    spiFlashWaitForWriteEnd();
}



void spiFlashPageWrite(u32 addr, u8 *pDat, u16 datLen)
{
    u8 temp[3] = {(addr & 0xFF0000) >> 16, (addr & 0xFF00) >> 8, (addr & 0xFF)};

    spiFlashWriteEnable();
    spiFlashWrite(FLASH_SPIx, W25X_PageProgram, temp, 3, pDat, datLen);
    spiFlashWaitForWriteEnd();
}

void spiFlashBufferWrite(u32 writeAddr, u8 *pDat, u16 datLen)
{
    u8 numOfPage = 0, numOfSingle = 0, addr = 0, count = 0, temp = 0;

    addr = writeAddr % SPI_FLASH_PageSize;
    count = SPI_FLASH_PageSize - addr;
    numOfPage = datLen / SPI_FLASH_PageSize;
    numOfSingle = datLen % SPI_FLASH_PageSize;

    if(addr == 0){
        if(numOfPage == 0){
            spiFlashPageWrite(writeAddr, pDat, datLen);
        }
        else{
            while(numOfPage--){
                spiFlashPageWrite(writeAddr, pDat, SPI_FLASH_PageSize);
                writeAddr += SPI_FLASH_PageSize;
                pDat += SPI_FLASH_PageSize;
            }
            spiFlashPageWrite(writeAddr, pDat, numOfSingle);
        }
    }
    else{
        if(numOfPage == 0){
            if(numOfSingle > count){
                temp = numOfSingle - count;
                spiFlashPageWrite(writeAddr, pDat, count);
                writeAddr += count;
                pDat += count;
                spiFlashPageWrite(writeAddr, pDat, temp);
            }
            else{
                spiFlashPageWrite(writeAddr, pDat, datLen);
            }
        }
        else{
            datLen -= count;
            numOfPage = datLen / SPI_FLASH_PageSize;
            numOfSingle = datLen % SPI_FLASH_PageSize;

            spiFlashPageWrite(writeAddr, pDat, count);
            writeAddr += count;
            pDat += count;
            while(numOfPage--){
                spiFlashPageWrite(writeAddr, pDat, SPI_FLASH_PageSize);
                writeAddr += SPI_FLASH_PageSize;
                pDat += SPI_FLASH_PageSize;
            }

            if(numOfSingle != 0){
                spiFlashPageWrite(writeAddr, pDat, numOfSingle);
            }
        }
    }
}

void spiFlashBufferRead(u32 addr, u8 *pDat, u16 datLen)
{
    u8 temp[3] = {(addr & 0xFF0000) >> 16, (addr & 0xFF00) >> 8, (addr & 0xFF)};
    spiFlashRead(FLASH_SPIx, W25X_ReadData, temp, 3, pDat, datLen);
}
