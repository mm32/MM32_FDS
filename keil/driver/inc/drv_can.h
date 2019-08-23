////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_CAN.H
/// @author
/// @version
/// @date
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE CAN
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
////////////////////////////////////////////////////////////////////////////////
#ifndef __DRV_CAN_H_
#define __DRV_CAN_H_
////////////////////////////////////////////////////////////////////////////////

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV UART INSTANT structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX                     sPrefix;
    u32                         cbTx;                           ///< transform sync
    u32                         cbRx;                           ///< receive sync
    EM_CAN_Mode                 canType;            ///< emBasic,emPeli

    EM_TYPE                     type;                           ///< transform type. polling, interrupt, dma
    EM_TYPE                     sync;                           ///< sync type
    EM_TYPE                     block;                          ///< block type
    EM_CAN_TEST_MODE            testMode;           ///< CAN_SeftTestMode,CAN_ListenOnlyMode.

    u16                         txCnt;                          ///< transform data count
    u8*                         txPtr;                          ///< transform buffer address
    u16                         rxCnt;                          ///< receive data count
    u8*                         rxPtr;                          ///< receive buffer address
    u32                         baudRate;           ///< baudRate
    u8                          sample;             ///<u8 sample
    u32                         id;                             ///< timeOut count
    bool                        remapEn ;                 ///< GPIO whether remap
    u8                          remapIdx;                   ///< GPIO remap index
    bool                        txProcess;                 ///< transform process status
    bool                        rxProcess;                 ///< receive process status
    bool                        txComplete;                 ///< transform complete status
    bool                        rxComplete;                      ///< receive complete status

} tDRV_CAN_INSTANT;

#include "hal_can.h"


typedef struct {
    u16 adc;
    u16 temp;
    u16 flash;
    u16 i2c;
    u16 can;
    u16 rtc;
    u16 uart1;
    u16 uart2;
    u16 ir;
    u16 led;
    u16 lcd;
}Peripheraldef;

////////////////////////////////////////////////////////////////////////////////
#ifdef _DRV_CAN_C_
#define GLOBAL

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1)

    static CAN_TypeDef* tbIpBase[]          = {CAN1 };
//  static CAN_Peli_TypeDef* tbIpBase[]     = {CAN1_PELI };
    static s8 tbSubHandleIdx[]              = {0    };
#endif

GLOBAL static tDRV_CAN_INSTANT      instance[INSTANCE_NUM];

#else
#define GLOBAL extern
#endif

GLOBAL CanPeliTxMsg gPeliTxMessage;
GLOBAL CanPeliRxMsg gPeliRxMessage;

GLOBAL u8 canTx;
GLOBAL u8 canSel;

GLOBAL Peripheraldef error;
GLOBAL Peripheraldef rf;
GLOBAL Peripheraldef rfCnt;
GLOBAL Peripheraldef rfOverCnt;

#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////

void DRV_CAN_GPIO_Init();
void DRV_CAN_NVIC_Init(CAN_Peli_TypeDef* CANx, u8 idx);
void DRV_CAN_Peli_Init();
static void CAN_SendPacket(CAN_Peli_TypeDef* CANx,  u8 value);
static void DRV_CAN_ITSendPacket(u8 idx);
static bool CAN_PollingSendPacket_Block(u8 idx);

/// @}
////////////////////////////////////////////////////////////////////////////////
/// @defgroup CAN_Exported_Functions
/// @{

/// @}
#endif


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_DRV_H */
////////////////////////////////////////////////////////////////////////////////
