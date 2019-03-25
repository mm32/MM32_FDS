////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_FLASH_INTERFACE.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE SPI_FLASH_INTERFACE EXAMPLE.
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
// Support W25X16 W25X32 W25X64
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#define _SPI_FLASH_INTERFACE_C_
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Include
////////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "types.h"
#include "MM32.h"

#include "hal_spi.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_nvic.h"
#include "hal_dma.h"

#include "spi.h"

////////////////////////////////////////////////////////////////////////////////
// private function
////////////////////////////////////////////////////////////////////////////////
void SPIM_CSLow(SPI_TypeDef* SPIx)
{
    if (SPIx == SPI2)
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
#if defined(__MM3N1) || defined(__MM0N1)
    if (SPIx == SPI1)
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
#endif
}

void SPIM_CSHigh(SPI_TypeDef* SPIx)
{
    if (SPIx == SPI2)
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
#if defined(__MM3N1) || defined(__MM0N1)
    if (SPIx == SPI1)
        GPIO_SetBits(GPIOA, GPIO_Pin_4);
#endif
}

void SPIM_TXEn(SPI_TypeDef* SPIx)
{
    // Enable TXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
}

void SPIM_TXDisable(SPI_TypeDef* SPIx)
{
    // Disable TXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Tx);
}

void SPIM_RXEn(SPI_TypeDef* SPIx)
{
    // Enable RXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
}

void SPIM_RXDisable(SPI_TypeDef* SPIx)
{
    // Disable RXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Rx);
}

u8 SPIMReadWriteByte(SPI_TypeDef* SPIx, unsigned char tx_data)
{
    SPI_SendData(SPIx, tx_data);
#if defined(SPI_USE_NVIC)
    while (1) {
        if (spi_tx_flag == 1) {
            spi_tx_flag = 0;
            break;
        }
    }
    while (1) {
        if (spi_rx_flag == 1) {
            spi_rx_flag = 0;
            return SPI_ReceiveData(SPIx);
        }
    }

#elif defined(SPI_USE_POLL)

    while (1) {
        if (SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT))
            break;
    }
    while (1) {
        if (SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)) {
            return SPI_ReceiveData(SPIx);
        }
    }
#endif
}

void SPIM_checkStatus(SPI_TypeDef* SPIx)
{
    u8 temp;
    SPIM_CSLow(SPIx);  // Spi cs select
    SPIMReadWriteByte(SPIx, RDSR);
    while (1) {
        temp = SPIMReadWriteByte(SPIx, 0x00);
        if ((temp & 0x01) == 0x00)
            break;
    }
    SPIM_CSHigh(SPIx);  // Spi cs release
}

void SPIM_WriteEnable(SPI_TypeDef* SPIx)
{
    SPIM_CSLow(SPIx);  // Spi cs select
    SPIMReadWriteByte(SPIx, WREN);
    SPIM_CSHigh(SPIx);  // Spi cs release
}

void SPIM_WriteDisable(SPI_TypeDef* SPIx)
{
    SPIM_CSLow(SPIx);
    SPIMReadWriteByte(SPIx, WRDI);
    SPIM_CSHigh(SPIx);
}

////////////////////////////////////////////////////////////////////////////////
// public function
////////////////////////////////////////////////////////////////////////////////
void SPIM_ReadID(SPI_TypeDef* SPIx, u8* Manufacturer_ID, u8* Deive_ID)
{
    SPIM_CSLow(SPIx);  // Spi cs select

    SPIMReadWriteByte(SPIx, RDID);

    *Manufacturer_ID = SPIMReadWriteByte(SPIx, 0x00);
    *(Deive_ID + 1)  = SPIMReadWriteByte(SPIx, 0x00);
    *Deive_ID        = SPIMReadWriteByte(SPIx, 0x00);

    SPIM_CSHigh(SPIx);  // Spi cs release
}

void SPIM_PageRead(SPI_TypeDef* SPIx, u32 address, u8* p, u16 number)  // page = 256 bytes
{
    u8  addr0, addr1, addr2;
    u16 i;
    address = address & 0xffffff00;  // page address
    addr0   = (u8)(address >> 16);
    addr1   = (u8)(address >> 8);
    addr2   = (u8)address;

    SPIM_CSLow(SPIx);  // Spi cs select

    SPIMReadWriteByte(SPIx, READ);
    SPIMReadWriteByte(SPIx, addr0);
    SPIMReadWriteByte(SPIx, addr1);
    SPIMReadWriteByte(SPIx, addr2);

    for (i = 0; i < 256; i++)
        p[i] = SPIMReadWriteByte(SPIx, 0x00);

    SPIM_CSHigh(SPIx);  // Spi cs release
}

void SPIM_DMAPageRead(SPI_TypeDef* SPIx, u32 address, u8* p, u16 number)
{
    DMA_InitTypeDef DMA_InitStructure;
    u8              fake_Data = 0x23;

    u8  addr0, addr1, addr2;
    u32 i   = 0;
    address = address & 0xffffff00;  // page address
    addr0   = (u8)(address >> 16);
    addr1   = (u8)(address >> 8);
    addr2   = (u8)address;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    if (SPIx == SPI2) {
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPIx->TXREG);
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)(&fake_Data);
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_BufferSize         = number;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_Low;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_DeInit(DMA1_Channel5);
        DMA_Init(DMA1_Channel5, &DMA_InitStructure);

        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPIx->RXREG);
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)(p);
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize         = number;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_DeInit(DMA1_Channel4);
        DMA_Init(DMA1_Channel4, &DMA_InitStructure);

        SPIM_CSLow(SPIx);  // Spi cs select

        SPIMReadWriteByte(SPIx, READ);
        SPIMReadWriteByte(SPIx, addr0);
        SPIMReadWriteByte(SPIx, addr1);
        SPIMReadWriteByte(SPIx, addr2);

        SPI_DMACmd(SPIx, ENABLE);

        DMA_Cmd(DMA1_Channel4, ENABLE);
        DMA_Cmd(DMA1_Channel5, ENABLE);

        while (!DMA_GetFlagStatus(DMA1_FLAG_TC5))
            ;
        while (!DMA_GetFlagStatus(DMA1_FLAG_TC4))
            ;

        DMA_ClearFlag(DMA1_FLAG_TC5);
        DMA_Cmd(DMA1_Channel5, DISABLE);
        DMA_ClearFlag(DMA1_FLAG_TC4);
        DMA_Cmd(DMA1_Channel4, DISABLE);

        SPI_DMACmd(SPIx, DISABLE);

        for (i = 0; i < 10000; i++)
            ;

        SPIM_CSHigh(SPIx);

        SPIM_checkStatus(SPIx);
    }
#if defined(__MM3N1) || defined(MZ206)
    if (SPIx == SPI1) {
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPIx->TXREG);
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)(&fake_Data);
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_BufferSize         = number;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_Low;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_DeInit(DMA1_Channel3);
        DMA_Init(DMA1_Channel3, &DMA_InitStructure);

        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPIx->RXREG);
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)(p);
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize         = number;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_DeInit(DMA1_Channel2);
        DMA_Init(DMA1_Channel2, &DMA_InitStructure);

        SPIM_CSLow(SPIx);  // Spi cs select

        SPIMReadWriteByte(SPIx, READ);
        SPIMReadWriteByte(SPIx, addr0);
        SPIMReadWriteByte(SPIx, addr1);
        SPIMReadWriteByte(SPIx, addr2);

        SPI_DMACmd(SPIx, ENABLE);

        DMA_Cmd(DMA1_Channel2, ENABLE);
        DMA_Cmd(DMA1_Channel3, ENABLE);

        while (!DMA_GetFlagStatus(DMA1_FLAG_TC3))
            ;
        while (!DMA_GetFlagStatus(DMA1_FLAG_TC2))
            ;

        DMA_ClearFlag(DMA1_FLAG_TC3);
        DMA_Cmd(DMA1_Channel3, DISABLE);
        DMA_ClearFlag(DMA1_FLAG_TC2);
        DMA_Cmd(DMA1_Channel2, DISABLE);

        SPI_DMACmd(SPIx, DISABLE);

        for (i = 0; i < 10000; i++)
            ;

        SPIM_CSHigh(SPIx);

        SPIM_checkStatus(SPIx);
    }
#endif
}
void SPIM_DMAPageProgram(SPI_TypeDef* SPIx, u32 address, u8* p, u16 number)
{
    u32             i;
    DMA_InitTypeDef DMA_InitStructure;

    u8 addr0, addr1, addr2;
    address = address & 0xffffff00;  // page address
    addr0   = (unsigned char)(address >> 16);
    addr1   = (unsigned char)(address >> 8);
    addr2   = (unsigned char)address;

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPIx->TXREG);
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)(p);
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize         = number;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    if (SPIx == SPI2) {
        DMA_DeInit(DMA1_Channel5);
        DMA_Init(DMA1_Channel5, &DMA_InitStructure);

        SPIM_WriteEnable(SPIx);
        SPIM_CSLow(SPIx);

        SPIMReadWriteByte(SPIx, PP);
        SPIMReadWriteByte(SPIx, addr0);
        SPIMReadWriteByte(SPIx, addr1);
        SPIMReadWriteByte(SPIx, addr2);

        SPI_DMACmd(SPIx, ENABLE);

        DMA_Cmd(DMA1_Channel5, ENABLE);

        while (!DMA_GetFlagStatus(DMA1_FLAG_TC5))
            ;
        DMA_ClearFlag(DMA1_FLAG_TC5);
        DMA_Cmd(DMA1_Channel5, DISABLE);

        SPI_DMACmd(SPIx, DISABLE);
        for (i = 0; i < 10000; i++)
            ;
        SPIM_CSHigh(SPIx);

        SPIM_checkStatus(SPIx);
    }
#if defined(__MM3N1) || defined(MZ206)
    if (SPIx == SPI1) {
        DMA_DeInit(DMA1_Channel3);
        DMA_Init(DMA1_Channel3, &DMA_InitStructure);

        SPIM_WriteEnable(SPIx);
        SPIM_CSLow(SPIx);

        SPIMReadWriteByte(SPIx, PP);
        SPIMReadWriteByte(SPIx, addr0);
        SPIMReadWriteByte(SPIx, addr1);
        SPIMReadWriteByte(SPIx, addr2);

        DMA_Cmd(DMA1_Channel3, ENABLE);

        SPI_DMACmd(SPIx, ENABLE);

        while (!DMA_GetFlagStatus(DMA1_FLAG_TC3))
            ;

        DMA_ClearFlag(DMA1_FLAG_TC3);

        SPI_DMACmd(SPIx, DISABLE);
        for (i = 0; i < 1000; i++)
            ;
        DMA_Cmd(DMA1_Channel3, DISABLE);
        for (i = 0; i < 1000; i++)
            ;
        SPIM_CSHigh(SPIx);
        SPIM_checkStatus(SPIx);
    }
#endif
}

void SPIM_PageProgram(SPI_TypeDef* SPIx, u32 address, u8* p, u16 number)
{
    u16 j;
    u8  addr0, addr1, addr2;
    address = address & 0xffffff00;  // page address
    addr0   = (unsigned char)(address >> 16);
    addr1   = (unsigned char)(address >> 8);
    addr2   = (unsigned char)address;

    SPIM_WriteEnable(SPIx);

    SPIM_CSLow(SPIx);  // Spi cs select

    SPIMReadWriteByte(SPIx, PP);
    SPIMReadWriteByte(SPIx, addr0);
    SPIMReadWriteByte(SPIx, addr1);
    SPIMReadWriteByte(SPIx, addr2);
    for (j = 0; j < number; j++)
        SPIMReadWriteByte(SPIx, *(p++));

    SPIM_CSHigh(SPIx);  // Spi cs release

    SPIM_checkStatus(SPIx);
}

void SPIM_SectorErase(SPI_TypeDef* SPIx, u32 address)  // 4k
{
    u8 addr0, addr1, addr2;
    address = address & 0xfffff000;
    addr0   = ((unsigned char)(address >> 16)) & 0xff;
    addr1   = ((unsigned char)(address >> 8)) & 0xff;
    addr2   = ((unsigned char)address) & 0xff;

    SPIM_WriteEnable(SPIx);  //??

    SPIM_CSLow(SPIx);  // Spi cs select

    SPIMReadWriteByte(SPIx, SE);
    SPIMReadWriteByte(SPIx, addr0);
    SPIMReadWriteByte(SPIx, addr1);
    SPIMReadWriteByte(SPIx, addr2);

    SPIM_CSHigh(SPIx);  // Spi cs release

    SPIM_checkStatus(SPIx);
}

void SPIM_BlockErase(SPI_TypeDef* SPIx, u32 address)  // 64k
{
    u8 addr0, addr1, addr2;
    address = address & 0xffff0000;
    addr0   = ((unsigned char)(address >> 16)) & 0xff;
    addr1   = ((unsigned char)(address >> 8)) & 0xff;
    addr2   = ((unsigned char)address) & 0xff;

    SPIM_WriteEnable(SPIx);

    SPIM_CSLow(SPIx);  // Spi cs select

    SPIMReadWriteByte(SPIx, BE);
    SPIMReadWriteByte(SPIx, addr0);
    SPIMReadWriteByte(SPIx, addr1);
    SPIMReadWriteByte(SPIx, addr2);

    SPIM_CSHigh(SPIx);  // Spi cs release

    SPIM_checkStatus(SPIx);
}

void SPIM_ChipErase(SPI_TypeDef* SPIx)  // 4k
{
    SPIM_WriteEnable(SPIx);

    SPIM_CSLow(SPIx);  // Spi cs select

    SPIMReadWriteByte(SPIx, CE);

    SPIM_CSHigh(SPIx);  // Spi cs release

    SPIM_checkStatus(SPIx);
}

void SPIM_Close(SPI_TypeDef* SPIx)
{
    SPIM_CSHigh(SPIx);  // Spi cs release
    SPIMReadWriteByte(SPIx, 0x01);
}

void SPIM_Init(SPI_TypeDef* SPIx, SPI_BaudRatePrescaler_TypeDef spi_baud_div)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

#if defined(__MM0N1) || defined(__MM0Q1)
#if defined(__MM0N1)
    if (SPIx == SPI1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

        GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;  // spi_nss
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6;  // spi1_sck / spi1_mosi
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  // spi1_miso
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
#endif
    if (SPIx == SPI2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

        GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_15;  // spi_sck / spi1_mosi
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;  // spi_nss
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  // spi_miso
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }

#elif defined(__MM3N1)
    if (SPIx == SPI1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;  // spi_nss
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;  // spi_sck
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;  // spi_mosi
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;  // spi_miso
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if (SPIx == SPI2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;  // spi_nss
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;  // spi_sck
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;  // spi_mosi
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;  // spi_miso
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
#endif

    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth         = 8;    //SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;    // mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;  // mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;  // SPI Maximum frequency 40MHz
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_Init(SPIx, &SPI_InitStructure);

    SPIM_CSHigh(SPIx);

    SPI_Cmd(SPIx, ENABLE);
    SPIM_TXEn(SPIx);
    SPIM_RXEn(SPIx);

#if defined(SPI_USE_NVIC)
    NVIC_InitTypeDef NVIC_InitStructure;
    spi_rx_flag = 0;
    spi_tx_flag = 0;
    if (SPIx == SPI2)
        NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
#if defined(__MM0N1) || defined(__MM3N1)
    if (SPIx == SPI1)
        NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
#endif

#if defined(__MM3N1)
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
#elif defined(__MM0N1) || defined(__MM0Q1)
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
#endif

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    SPI_ITConfig(SPIx, SPI_IT_RX | SPI_IT_TXEPT, ENABLE);
#endif
}
////////////////////////////////////////////////////////////////////////////////
// Interruput handle function
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM3N1) || defined(__MM0N1)
void SPI1_IRQHandler(void)
{
    while (1) {
        if (SPI_GetITStatus(SPI1, SPI_IT_TXEPT)) {
            SPI_ClearITPendingBit(SPI1, SPI_IT_TXEPT);
            spi_tx_flag = 1;
            break;
        }

        if (SPI_GetITStatus(SPI1, SPI_IT_RX)) {
            SPI_ClearITPendingBit(SPI1, SPI_IT_RX);
            spi_rx_flag = 1;
            break;
        }
    }
}
#endif

void SPI2_IRQHandler(void)
{
    while (1) {
        if (SPI_GetITStatus(SPI2, SPI_IT_TXEPT)) {
            SPI_ClearITPendingBit(SPI2, SPI_IT_TXEPT);
            spi_tx_flag = 1;
            break;
        }

        if (SPI_GetITStatus(SPI2, SPI_IT_RX)) {
            SPI_ClearITPendingBit(SPI2, SPI_IT_RX);
            spi_rx_flag = 1;
            break;
        }
    }
}
