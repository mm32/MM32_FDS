////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_I2C.H
/// @author   S Yi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE I2C
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
#ifndef __DRV_I2C_H
#define __DRV_I2C_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup I2C
/// @brief  I2C driver modules
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @defgroup I2C_Exported_Constants
/// @{

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup I2C_Exported_Enumeration
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief I2C mode enumerate definition
////////////////////////////////////////////////////////////////////////////////


/// @}



////////////////////////////////////////////////////////////////////////////////
/// @defgroup I2C_Exported_Types
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief I2C Instant Data Control Block
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;                            ///< Instance Prefix
    u32                 cbTx;                               //< Tx callback
    u32                 cbRx;                               //< Rx callback
    EM_TYPE             type;                               ///< transform type: polling, interrupt, dma
    EM_TYPE             sync;                               ///< Sync of ASync
    EM_TYPE             block;                              ///< Blocking of NonBlocking
    s8                  flag;                               ///< return flag

    u8                  slave;                              ///< Slave Device Address
    u16                 subAddr;                            ///< SubAddress
    u8                  subSize;                            ///< SubAddress size

    u16                 tx_cnt;                             ///< transform data count
    u16                 tx_len;                             ///< transform data length
    u8*                 tx_ptr;                             ///< transform buffer address
    u16                 rx_cnt;                             ///< receive data count
    u16                 rx_len;                             ///< receive data length
    u8*                 rx_ptr;                             ///< receive buffer address
    u32                 timeOut;                            ///< timeOut count

    bool                master      :1;                     ///< mode: master or slave
    bool                sadd        :1;                     ///< Send address
    u8                  optRD       :1;                     ///< option: write or read data, Read :1
    u8                  revRD       :1;                     ///< write or read


} tDRV_I2C_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup I2C_Exported_Variables
/// @{

#ifdef _DRV_I2C_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
#if defined(__MM3N1) || defined(__MM3O1)
    static I2C_TypeDef* tbIpBase[]  = {I2C1, I2C2};
    static s8 tbSubHandleIdx[]      = {0,    1};

// e.g.
//  static I2C_TypeDef* tbIpBase[]  = {       I2C2};
//  static s8 tbSubHandleIdx[]      = {-1,    0};
//  static s8 tbSubHandleIdx[]      = {0,     0};

// e.g.
//  static I2C_TypeDef* tbIpBase[]  = {I2C1,      };
//  static s8 tbSubHandleIdx[]      = {0,    -1};
//  static s8 tbSubHandleIdx[]      = {0,    0};

#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    static I2C_TypeDef* tbIpBase[]  = {I2C1};
    static s8 tbSubHandleIdx[]      = {0};
#endif
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_I2C_INSTANT      instance[INSTANCE_NUM];


#else
#define GLOBAL extern
#endif



#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup I2C_Exported_Functions
/// @{
void DRV_I2C_NVIC_Init(I2C_TypeDef* I2Cx);
void DRV_I2C_NVIC_DMA_Init(I2C_TypeDef* I2Cx);
void DRV_I2C_DMA_ConfigChannel(I2C_TypeDef* I2Cx);

void I2CComplete(u8 idx);
void I2CTxByte(I2C_TypeDef* I2Cx, u8 dat);
void I2CSendAddr(u8 idx);

void DRV_I2C_DMA_SendPacket(u8 idx);
void DRV_I2C_DMA_RcvPacket(u8 idx);


/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __DRV_I2C_H */
////////////////////////////////////////////////////////////////////////////////
