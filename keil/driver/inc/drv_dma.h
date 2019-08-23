////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_DMA.H
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE DMA
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
#ifndef __DRV_DMA_H
#define __DRV_DMA_H

// Files includes  -------------------------------------------------------------
#include "hal_dma.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DMA
/// @brief DMA driver modules
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @defgroup DMA_Exported_Constants
/// @{

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup DMA_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief DMA direction enumerate definition
/// @anchor EM_DMA
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    emDMA_m2p = DMA_CCR_DIR,                                        ///< transfer from memory to peripheral
    emDMA_p2m = 0,                                                  ///< transfer from peripheral to memory
    emDMA_m2m = DMA_CCR_M2M                                         ///< transfer from memory to memory
} EM_DMA;

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV DMA Init structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    void*                                   base;                   ///<
    DMA_Channel_TypeDef*                    channel;                ///< channel
    EM_DMA                                  dmaDir;                 ///< direction
    u32                                     MemoryBaseAddr;     ///< Memory Base Addr
    u32                                     BufferSize;         ///< Buffer Size
    DMA_peripheral_data_size_TypeDef        PeripheralDataSize; ///< Peripheral Data Size
    DMA_memory_data_size_TypeDef            MemoryDataSize;     ///< Memory Data Size
    u32                                     PeripheralBaseAddr; ///< Peripheral Base Addr
    DMA_peripheral_incremented_mode_TypeDef PeripheralInc;      ///< Peripheral Increase mode
    DMA_memory_incremented_mode_TypeDef     MemoryInc;          ///< Memory Increase mode
    DMA_priority_level_TypeDef              Priority;           ///< Priority
    DMA_circular_normal_mode_TypeDef        Mode;               ///< circular normal mode
} tDRV_DMA_DCB;

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV DMA instant structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX                 sPrefix;
    EM_DMA_DIR              dir;                                    ///< direction
    EM_TYPE                 block;                                  ///< clock style
    u32                     tccb;                                   ///< call back function
    bool                    Process;                                ///< transfer process status
    bool                    Result;                                 ///< transfer result status
} tDRV_DMA_INSTANT;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup DMA_Exported_Variables
/// @{
#ifdef _DRV_DMA_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
#if defined(__MM3N1)
    static DMA_Channel_TypeDef* tbIpBase[]  = {DMA1_ch1, DMA1_ch2, DMA1_ch3, DMA1_ch4, DMA1_ch5, DMA1_ch6, DMA1_ch7};
    static s8 tbSubHandleIdx[]              = {0,        1,        2,        3,        4,        5,        6 };
#endif

//  static DMA_Channel_TypeDef* tbIpBase[]  = {DMA1_ch1,                               DMA1_ch5};
//  static s8 tbSubHandleIdx[]              = {0,        1,        2,        3,        1,        5,        6 };


// e.g.
//  static DMA_Channel_TypeDef* tbIpBase[]  = {       DMA1_ch2                            };
//  static s8 tbSubHandleIdx[]              = {-1,    0,     -1,  -1,    -1,  -1,  -1,};
//  static s8 tbSubHandleIdx[]              = { 0,    0,      0,   0,     0,   0,   0,};
//  static s8 tbSubHandleIdx[]              = { 0,    0,     -1,  -1,    -1,  -1,  -1,};

// e.g.
//  static DMA_Channel_TypeDef* tbIpBase[]  = {       DMA1_ch2, DMA1_ch3                      };
//  static s8 tbSubHandleIdx[]              = {-1,    0,     1    -1,    -1,  -1,  -1,};
//  static s8 tbSubHandleIdx[]              = {0,     0,     1     0,     0,   0,   0,};

// e.g.
//  static DMA_Channel_TypeDef* tbIpBase[]  = {DMA1_ch1,        DMA1_ch3                      };
//  static s8 tbSubHandleIdx[]              = {0,     0,     1    -1,    -1,  -1,  -1,};};
//  static s8 tbSubHandleIdx[]              = {0,     1,     1     0,     0,   0,   0,};};
//  static s8 tbSubHandleIdx[]              = {0,     -1,    1    -1,    -1,  -1,  -1,};    };


#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    static DMA_Channel_TypeDef* tbIpBase[] = {DMA1_ch1,                   DMA1_ch5};
    static s8 tbSubHandleIdx[]             = {0,        1,    1,    1,    1 };
#endif
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_DMA_INSTANT      instance[INSTANCE_NUM];

#else
#define GLOBAL extern

#endif


#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup DMA_Exported_Functions
/// @{
DMA_Channel_TypeDef* DRV_GetBase_DMAx(u8 idx);

void DRV_DMA_IRQ_Common(u8 idx);

void DRV_DMA_SetConfig(u8 idx, u32 source, u32 desti, u32 length);
void DRV_DMA_NVIC_Init(u8 preemptionPriority, u8 subPriority, u8 index);

void DRV_DMA_TransmitPacket(DMA_Channel_TypeDef* ch, u32 addr, u32 len);
void DRV_DMA_PeripheralPacket(DMA_Channel_TypeDef* ch, u32 addr, u32 len);

void DRV_DMA_Init(tDRV_DMA_DCB* dma);


/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_DMA_H */
////////////////////////////////////////////////////////////////////////////////
