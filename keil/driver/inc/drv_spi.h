////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_SPI.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SPI
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
#ifndef __DRV_SPI_H
#define __DRV_SPI_H

// Files includes  -------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SPI
/// @brief SPI driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SPI_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SPI_Exported_Types
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV SPI INSTANT structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct{
    tPREFIX         sPrefix;            ///< SPI Instance Prefix
    bool            master;
    EM_TYPE         type;               ///< SPI transform type. polling, interrupt, dma
    EM_TYPE         block;              ///< SPI block type
    EM_TYPE         sync;               ///< SPI sync type
    u32             cbTx;               ///< SPI transform callback function address
    u32             cbRx;               ///< SPI receive callback function address

    u16             txCnt;              ///< SPI transform data count
    u8*             txPtr;              ///< SPI transform buffer address
    u16             txRealCnt;          ///< SPI actual transform data count
    u16             rxCnt;              ///< SPI received data count
    u8*             rxPtr;              ///< SPI receive buffer address
    u16             rxRealCnt;          ///< SPI actual receive data count

    bool            remapEn     :1;     ///< transform type. polling, interrupt, dma
    u8              remapIdx    :2;     ///< GPIO remap index
    bool            txProcess   :1;     ///< SPI transform process status
    bool            txComplete  :1;     ///< SPI transform complete status
    bool            rxProcess   :1;     ///< SPI receive process status
    bool            rxComplete  :1;     ///< SPI receive complete status

    u8              protocol;
    u8              command;
    u8              parameter[4];
    u16             parameterLength;


}tDRV_SPI_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Variables
/// @{
#ifdef _DRV_SPI_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
#if defined(__MM0Q1)
static SPI_TypeDef* tbIpBase[]  = {     SPI2};
static s8 tbSubHandleIdx[]      = {1,    0};
#else
static SPI_TypeDef* tbIpBase[]  = {    SPI2};
static s8 tbSubHandleIdx[]      = {1,    0};
#endif
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_SPI_INSTANT      instance[INSTANCE_NUM];


#else
#define GLOBAL extern
#endif


#undef GLOBAL
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup SPI_Exported_Functions
/// @{


bool DRV_SPI_PollingSendPacket(u8 idx);
void DRV_SPI_Polling_RcvPacket(u8 idx);
void DRV_SPI_DMA_SendPacket(SPI_TypeDef* SPIx, u8 idx);
void DRV_SPI_DMA_RcvPacket(SPI_TypeDef* SPIx, u8 idx);
void DRV_SPI_DMA_ConfigChannel(SPI_TypeDef* SPIx);
void DRV_SPI_NVIC_DMA_Init(SPI_TypeDef* SPIx);
void DRV_SPI_NVIC_Init(SPI_TypeDef* SPIx);

/// @}




/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_SPI_H */
////////////////////////////////////////////////////////////////////////////////
