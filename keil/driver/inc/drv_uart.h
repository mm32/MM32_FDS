////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_UART.H
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE UART
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
#ifndef __DRV_UART_H
#define __DRV_UART_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UART
/// @brief UART driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UART_Exported_Constants
/// @{

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup UART_Exported_Types
/// @{
///

////////////////////////////////////////////////////////////////////////////////
/// @brief UART MODE enumerate definition
/// @anchor EM_UARTMODE
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    mode_Rx    = UART_Mode_Rx,                              ///< receive mode
    mode_Tx    = UART_Mode_Tx,                              ///< transform mode
    mode_Rx_Tx = UART_Mode_Rx | UART_Mode_Tx                ///< transform and receive mode
} EM_UARTMODE;

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV UART INSTANT structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX                     sPrefix;
    u32                         cbTx;                           ///< transform sync
    u32                         cbRx;                           ///< receive sync
    EM_TYPE                     type;                           ///< transform type. polling, interrupt, dma
    EM_TYPE                     sync;                           ///< sync type
    EM_TYPE                     block;                          ///< block type

    u16                         txCnt;                          ///< transform data count
    u8*                         txPtr;                          ///< transform buffer address
    u16                         txRealCnt;                      ///< transformed buffer address
    u16                         rxCnt;                          ///< receive data count
    u8*                         rxPtr;                          ///< receive buffer address
    u16                         rxRealCnt;                      ///< received data count

    u32                         nTimeOut;                       ///< timeOut count
    u32                         txTimeOut;                      ///< transform timeOut count
    u32                         rxTimeOut;                      ///< receive timeOut count
    EM_UARTMODE                 mode;                           ///< mode

    bool                        remapEn     :1;                 ///< GPIO whether remap
    u8                          remapIdx    :2;                 ///< GPIO remap index
    bool                        txProcess   :1;                 ///< transform process status
    bool                        rxProcess   :1;                 ///< receive process status
    bool                        txComplete  :1;                 ///< transform complete status
    bool                        rxComplete  :1;                 ///< receive complete status
    bool                        bTimeOut    :1;                 ///< timeOut status
} tDRV_UART_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UART_Exported_Variables
/// @{

#ifdef _DRV_UART_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
#if defined(__MM3N1)
    static UART_TypeDef* tbIpBase[]     = {UART1, UART2, UART3};
    static s8 tbSubHandleIdx[]          = {0,     1,     2};

// e.g.
//  static UART_TypeDef* tbIpBase[]     = {       UART2       };
//  static s8 tbSubHandleIdx[]          = {-1,    0,     -1};
//  static s8 tbSubHandleIdx[]          = {0,     0,     0};
//  static s8 tbSubHandleIdx[]          = {0,     0,     -1};

// e.g.
//  static UART_TypeDef* tbIpBase[]     = {       UART2, UART3  };
//  static s8 tbSubHandleIdx[]          = {-1,    0,     1};
//  static s8 tbSubHandleIdx[]          = {0,     0,     1};

// e.g.
//  static UART_TypeDef* tbIpBase[]     = {UART1,        UART3  };
//  static s8 tbSubHandleIdx[]          = {0,     0,     1};
//  static s8 tbSubHandleIdx[]          = {0,     1,     1};
//  static s8 tbSubHandleIdx[]          = {0,     -1,    1};

#endif
#if defined(__MM3O1)

    static UART_TypeDef* tbIpBase[]     = {UART1, UART2, UART3, UART4, UART5, UART6, UART7, UART8};
    static s8 tbSubHandleIdx[]          = {0,     1,     2,     3,     4,     5,     6,     7};

// e.g.
//  static UART_TypeDef* tbIpBase[]     = {UART1, UART2, UART3,                             UART8};
//  static s8 tbSubHandleIdx[]          = {0,     1,     2,     2,     2,     2,     3,     3};
//  static s8 tbSubHandleIdx[]          = {0,     1,     2,     3,     3,     3,     3,     3};
//  static s8 tbSubHandleIdx[]          = {0,     1,     2,     -1,    -1,    -1,    -1,    3};

// e.g.
//  static UART_TypeDef* tbIpBase[]     = {UART1,                                           UART8};
//  static s8 tbSubHandleIdx[]          = {0,     0,     0,     0,     0,     0,     0,     1};
//  static s8 tbSubHandleIdx[]          = {0,     1,     1,     1,     1,     1,     1,     1};
//  static s8 tbSubHandleIdx[]          = {0,    -1,     -1,    -1,    -1,    -1,    -1,    1};


#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    static UART_TypeDef* tbIpBase[]     = {UART1, UART2};
    static s8 tbSubHandleIdx[]          = {0,     1};
#endif
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */


GLOBAL static tDRV_UART_INSTANT     instance[INSTANCE_NUM];

#else
#define GLOBAL extern
#endif



#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UART_Exported_Functions
/// @{

UART_TypeDef* DRV_GetBase_UARTx(u8 idx);

void DRV_UART_PollingSendPacket_BLK(UART_TypeDef* UARTx, u8 idx, tDRV_UART_INSTANT* para);
void DRV_UART_PollingRcvPacket_BLK(UART_TypeDef* UARTx, u8 idx, tDRV_UART_INSTANT* para);

void DRV_UART_PollingSendPacket_NBLK(UART_TypeDef* UARTx, u8 idx, tDRV_UART_INSTANT* para);
void DRV_UART_PollingRcvPacket_NBLK(UART_TypeDef* UARTx, u8 idx, tDRV_UART_INSTANT* para);

void DRV_UART_PollingSendPacket(UART_TypeDef* UARTx);
void DRV_UART_PollingRcvPacket(UART_TypeDef* UARTx);

void DRV_UART_DMA_SendPacket(u8 idx);
void DRV_UART_DMA_RcvPacket(u8 idx);

void DRV_UART_NVIC_Init(UART_TypeDef* UARTx);
void DRV_UART_DMA_ConfigChannel(UART_TypeDef* UARTx);

void DRV_UART_NVIC_DMA_Init(UART_TypeDef* UARTx);

/// @}

/// @}

/// @}
////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_UART_H */
////////////////////////////////////////////////////////////////////////////////
