////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_DMA.C
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES THE DMA DRIVER LAYER FUNCTIONS.
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
#define _DRV_DMA_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "hal_rcc.h"
#include "hal_nvic.h"
#include "hal_dma.h"

#include "drv.h"
#include "common.h"
#include "drv_dma.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DMA
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DMA_Exported_Functions
/// @{


/* -----------------------------------------------------------------------------
-------------------   DMA  Interrupt  Routing   --------------------------------
----------------------------------------------------------------------------- */
#if defined(__EX_DMA)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Handles DMA interrupt request.
/// @param  idx: The true index of the DMA instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void DRV_DMA_IRQ_Common(u8 idx)
{
    u32 flagIt   = DMA1->ISR;
    u32 sourceIt = ((DMA_Channel_TypeDef*)instance[idx].sPrefix.pBase)->CCR;

    if ((flagIt & (DMA_ISR_TCIF1 << (instance[idx].sPrefix.trueIdx << 2))) && (sourceIt & DMA_CCR_TCIE)) {
        if ((sourceIt & DMA_CCR_CIRC) == 0) {
            instance[idx].Result = true;
       }

        DMA1->IFCR = DMA_ISR_TCIF1 << (instance[idx].sPrefix.trueIdx << 2);        //Clear the TC flag
        if (instance[idx].tccb != 0) {
               ((fpDMA)instance[idx].tccb)(idx);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DMA  channel 1 interrupt.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel1_IRQHandler(void)
{
    DRV_DMA_IRQ_Common(instance[tbSubHandleIdx[0]].sPrefix.subIdx);
}
#if defined(__MM3N1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  DMA  channel 5 interrupt.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel5_IRQHandler(void)
{
    DRV_DMA_IRQ_Common(instance[tbSubHandleIdx[4]].sPrefix.subIdx);
}
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  DMA  channel 5 interrupt.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel4_5_IRQHandler(void)
{
    DRV_DMA_IRQ_Common(instance[tbSubHandleIdx[4]].sPrefix.subIdx);
}

#endif
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the DMA interrupt according to the specified channel.
/// @param  preemptionPriority: Interrupt preemption priority.
/// @param  subPriority: Interrupt sub priority.
/// @param  index: DMA channel ID.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DMA_NVIC_Init(u8 preemptionPriority, u8 subPriority, u8 index)
{
    exNVIC_Init_TypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;

#if defined(__MM3N1)
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn + index;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    switch(index) {
    case 0:     NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;    break;
    case 1:
    case 2:     NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;  break;
    case 3:
    case 4:     NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;  break;
    default:                                                                break;
    }
#endif
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    exNVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Sets the DMA Transfer parameter.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified DMA.
/// @param  idx: The true index of the DMA instance.
/// @param  source: The source memory Buffer address.
/// @param  desti: The destination memory Buffer address.
/// @param  length: The length of data to be transferred from source to destination.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DMA_SetConfig(u8 idx, u32 source, u32 desti, u32 length)
{
    DMA1->IFCR = DMA_IFCR_CGIF1 << (idx << 2);  // Clear all flags

    DMA_Channel_TypeDef* ch = ((DMA_Channel_TypeDef*)instance[idx].sPrefix.pBase);

    ch->CNDTR = length;                         // Configure DMA Channel data length

    if (instance[idx].dir == emDMA_DIR_m2p) {   // Memory to Peripheral
        ch->CPAR = desti;                       // Configure DMA Channel destination address
        ch->CMAR = source;                      //  Configure DMA Channel source address
    }
    else {                                      // Peripheral to Memory
        ch->CPAR = source;                      // Configure DMA Channel source address
        ch->CMAR = desti;                       // Configure DMA Channel destination address
    }
}


/// @}

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DMA_Exported_Functions
/// @{


/* -----------------------------------------------------------------------------
----------  P e r i p h e r a l s    i n i t i a l i z a t i o n   -------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the DMA channel according to the specified
///         parameters in the tDRV_DMA_DCB.
/// @param  dma: Pointer to a tDRV_DMA_DCB structure that contains
///               the configuration information for the specified DMA Channel.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DMA_Init(tDRV_DMA_DCB* dma)
{
    DMA_InitTypeDef InitStructure;
    COMMON_EnableIpClock(emCLOCK_DMA1);

    InitStructure.BufferSize            = dma->BufferSize;

    InitStructure.DIR                   = (DMA_data_transfer_direction_TypeDef)dma->dmaDir;
    InitStructure.M2M                   = DMA_M2M_Disable;
    InitStructure.Mode                  = dma->Mode;
    InitStructure.Priority              = dma->Priority;

    InitStructure.MemoryBaseAddr        = dma->MemoryBaseAddr;
    InitStructure.MemoryDataSize        = dma->MemoryDataSize;
    InitStructure.MemoryInc             = dma->MemoryInc;           // DMA_MemoryInc_Enable;

    InitStructure.PeripheralBaseAddr    = dma->PeripheralBaseAddr;
    InitStructure.PeripheralDataSize    = dma->PeripheralDataSize;
    InitStructure.PeripheralInc         = dma->PeripheralInc;       // DMA_PeripheralInc_Disable;
#if defined(__MM0P1) || defined(__MM0Q1)
    InitStructure.AutoReload            = DMA_Auto_Reload_Disable;
#endif
    DMA_Init(dma->channel, &InitStructure);

}


////////////////////////////////////////////////////////////////////////////////
/// @brief   DMA transmits packet
/// @param  ch: Pointer to a DMA channel.
/// @param  addr: The memory Buffer address.
/// @param  len: The length of data to be transferred.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DMA_TransmitPacket(DMA_Channel_TypeDef* ch, u32 addr, u32 len)
{
    DMA_ITConfig(ch, DMA_IT_TC, ENABLE);
    DMA_Cmd(ch, DISABLE);
    exDMA_SetMemoryAddress(ch, addr);
    exDMA_SetTransmitLen(ch, len);
    DMA_Cmd(ch, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   DMA transmits packet to peripheral.
/// @param  ch: Pointer to a DMA channel.
/// @param  addr: The memory Buffer address.
/// @param  len: The length of data to be transferred.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DMA_PeripheralPacket(DMA_Channel_TypeDef* ch, u32 addr, u32 len)
{
    DMA_ITConfig(ch, DMA_IT_TC, ENABLE);
    DMA_Cmd(ch, DISABLE);
    exDMA_SetMemoryAddress(ch, addr);
    exDMA_SetTransmitLen(ch, len);
    DMA_Cmd(ch, ENABLE);
}

/* -----------------------------------------------------------------------------
------------------   F u n c t i o n     I m p l e m e n t  --------------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function opens the DMA instance according to the specified
///         parameters in the DMA_DCB_TypeDef.
/// @param  handle: Pointer to a HANDLE struct
/// @param  pDcb: Pointer to a tAPP_DMA_DCB structure that contains
///                the configuration information for the specified DMA module.
/// @retval  The status of the function.
////////////////////////////////////////////////////////////////////////////////
static int DMA_OpenFile(HANDLE handle, tAPP_DMA_DCB* pDcb)
{
    s8 idx = DRV_OpenFileEntry((u32*)&tbIpBase, (u8*)&tbSubHandleIdx, pDcb->hSub, (u8*)(&instance[0]), sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)      return 0;

    COMMON_EnableIpClock(emCLOCK_DMA1);

    instance[idx].dir   = pDcb->dir;
    instance[idx].tccb  = pDcb->cb;
    instance[idx].block = pDcb->block;
    instance[idx].Process = 0;
    instance[idx].Result = 0;


    DMA_Channel_TypeDef* ch = ((DMA_Channel_TypeDef*)instance[idx].sPrefix.pBase);

    if (pDcb->dir == emDMA_DIR_m2p)                 SET_BIT(ch->CCR,   DMA_CCR_DIR);
    else                                            CLEAR_BIT(ch->CCR, DMA_CCR_DIR);

    if (pDcb->dir == emDMA_DIR_m2m)                 SET_BIT(ch->CCR,   DMA_CCR_M2M);
    else                                            CLEAR_BIT(ch->CCR, DMA_CCR_M2M);

    if (pDcb->periphInc == emDMA_INC_Enable)        SET_BIT(ch->CCR,   DMA_CCR_PINC);
    else                                            CLEAR_BIT(ch->CCR, DMA_CCR_PINC);

    if (pDcb->memInc == emDMA_INC_Enable)           SET_BIT(ch->CCR,   DMA_CCR_MINC);
    else                                            CLEAR_BIT(ch->CCR, DMA_CCR_MINC);

    if (pDcb->periphBits == emDMA_Bits_16)          MODIFY_REG(ch->CCR, DMA_CCR_PSIZE, DMA_CCR_PSIZE_HALFWORD);
    else if (pDcb->periphBits == emDMA_Bits_32)     MODIFY_REG(ch->CCR, DMA_CCR_PSIZE, DMA_CCR_PSIZE_WORD);
    else                                            CLEAR_BIT(ch->CCR,  DMA_CCR_PSIZE);

    if (pDcb->memBits == emDMA_Bits_16)             MODIFY_REG(ch->CCR, DMA_CCR_MSIZE, DMA_CCR_MSIZE_HALFWORD);
    else if (pDcb->memBits == emDMA_Bits_32)        MODIFY_REG(ch->CCR, DMA_CCR_MSIZE, DMA_CCR_MSIZE_WORD);
    else                                            CLEAR_BIT(ch->CCR,  DMA_CCR_MSIZE);

    if (pDcb->mode == emDMA_MODE_Normal)             CLEAR_BIT(ch->CCR, DMA_CCR_CIRC);
    else                                            SET_BIT(ch->CCR,   DMA_CCR_CIRC);

    if (pDcb->priority == emDMA_PRI_Medium)          MODIFY_REG(ch->CCR, DMA_CCR_PL, DMA_CCR_PL_Medium);
    else if (pDcb->priority == emDMA_PRI_High)       MODIFY_REG(ch->CCR, DMA_CCR_PL, DMA_CCR_PL_High);
    else if (pDcb->priority == emDMA_PRI_VeryHigh)  MODIFY_REG(ch->CCR, DMA_CCR_PL, DMA_CCR_PL_VeryHigh);
    else                                            CLEAR_BIT(ch->CCR,  DMA_CCR_PL);

    DRV_DMA_NVIC_Init(1, 2, pDcb->hSub);
    SET_BIT(ch->CCR, DMA_CCR_TCIE);

    return DRV_OpenFileExit(handle, (int*)&instance[idx], idx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  the DMA sends data according to the ID in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified DMA.
/// @param  hSub: The DMA channel ID.
/// @param  pWriteDcb: pointer to a tAPP_DMA_WRITE_DCB structure that contains
///                the DCB for write file.
/// @param  length: The length of data to be transferred from source to destination.
/// @retval  Transfer status.
////////////////////////////////////////////////////////////////////////////////
static int DMA_WriteFile(HANDLE handle, s8 hSub, u8* pWriteDcb, u32 length)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    tAPP_DMA_WRITE_DCB* pDcb = (tAPP_DMA_WRITE_DCB*)pWriteDcb;

    if (!instance[idx].Process) {
        instance[idx].Process = true;
        instance[idx].Result  = false;

        DRV_DMA_SetConfig(idx, pDcb->src, pDcb->desti, length);
        SET_BIT(((DMA_Channel_TypeDef*)instance[idx].sPrefix.pBase)->CCR, DMA_CCR_EN);
    }

    if (instance[idx].block == emTYPE_Block) {
        while (!instance[idx].Result);
    }
    else {
        if (!instance[idx].Result) {
            return false;
        }
    }

    instance[idx].Process = false;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function closes the DMA instance according to the ID
///          in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified DMA.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void DMA_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    DMA_DeInit((DMA_Channel_TypeDef*)instance[handle->idx].sPrefix.pBase);

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DMA CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified DMA.
/// @retval :None
////////////////////////////////////////////////////////////////////////////////
void DMA_CreateFile(HANDLE  handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&DMA_OpenFile;
    handle->closeFile   = (int*)&DMA_CloseFile;
    handle->readFile    = NULL;
    handle->writeFile   = (int*)&DMA_WriteFile;
}

/// @}

/// @}

/// @}
