////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_SPI.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES THE SPI DRIVER LAYER FUNCTIONS.
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
#define _DRV_SPI_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_rcc.h"
#include "hal_nvic.h"
#include "hal_spi.h"

#include "drv.h"
#include "common.h"
#include "drv_dma.h"
#include "drv_spi.h"

#include "bsp.h"
#include "bsp_spi.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief Set the SPI clock.
/// @param SPIx: where x can be 1, 2 to select the SPI peripheral.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void Set_SPI_Clock(SPI_TypeDef* SPIx)
{
    if      (SPIx == SPI1)  COMMON_EnableIpClock(emCLOCK_SPI1);
    else                    COMMON_EnableIpClock(emCLOCK_SPI2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Get SPI DMA Rx Channel
/// @param SPIx: where x can be 1, 2 to select the SPI peripheral.
/// @retval DMA_Channel_TypeDef
////////////////////////////////////////////////////////////////////////////////
DMA_Channel_TypeDef* Get_SPI_DMA_RxChannel(SPI_TypeDef* SPIx)
{
    if (SPIx == SPI1)   return DMA1_ch2;
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    else                return DMA1_ch4;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Get SPI DMA Tx Channel
/// @param SPIx: where x can be 1, 2 to select the SPI peripheral.
/// @retval DMA_Channel_TypeDef
////////////////////////////////////////////////////////////////////////////////
DMA_Channel_TypeDef* Get_SPI_DMA_TxChannel(SPI_TypeDef* SPIx)
{
    if (SPIx == SPI1)   return DMA1_ch3;
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    else                return DMA1_ch5;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Get SPI RX DMA interrupt index
/// @param SPIx: where x can be 1, 2 to select the SPI peripheral.
/// @retval SPI DMA IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_SPI_DMA_IRQ_RxIndex(SPI_TypeDef* SPIx)
{
#if defined(__MM3N1)
    if (SPIx == SPI1)   return DMA1_Channel2_IRQn;
    else                return DMA1_Channel4_IRQn;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    if (SPIx == SPI1)   return DMA1_Channel2_3_IRQn;
    else                return DMA1_Channel4_5_IRQn;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Get SPI TX DMA interrupt index
/// @param SPIx: where x can be 1, 2 to select the SPI peripheral.
/// @retval SPI DMA IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_SPI_DMA_IRQ_TxIndex(SPI_TypeDef* SPIx)
{
#if defined(__MM3N1)
    if (SPIx == SPI1)   return DMA1_Channel3_IRQn;
    else                return DMA1_Channel5_IRQn;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    if (SPIx == SPI1)   return DMA1_Channel2_3_IRQn;
    else                return DMA1_Channel4_5_IRQn;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Get SPI interrupt index
/// @param SPIx: where x can be 1, 2 to select the SPI peripheral.
/// @retval SPI IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_SPI_IRQ_Index(SPI_TypeDef* SPIx)
{
    if (SPIx == SPI1)   return SPI1_IRQn;
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    else                return SPI2_IRQn;
#endif
}


#if defined(__EX_SPI)
////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI DMA Rx Communication Common Function
/// @param  idx: Instance index.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_SPI_DMA_IRQHandler_RxCommon(u8 idx)
{
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;
    instance[idx].rxComplete = true;
    instance[idx].rxRealCnt = instance[idx].rxCnt;
    SPI_DMACmd(SPIx, DISABLE);

    if (instance[idx].sync == emTYPE_Sync) {
        if (instance[idx].cbRx != NULL){
            ((fpSPI)instance[idx].cbRx)(idx);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI DMA Tx Communication Common Function
/// @param  idx: Instance index.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_SPI_DMA_IRQHandler_TxCommon(u8 idx)
{
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;


    while(!SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT));
    instance[idx].txComplete = true;
    instance[idx].txRealCnt = instance[idx].txCnt;
    instance[idx].txCnt = 0;

//    while(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)){
//        READ_REG(SPIx->RDR);
//    }
//
//    if (instance[idx].master) {
//        BSP_SPI_NSS_Configure(SPIx, 0, 0, DISABLE);
//    }
    if (instance[idx].sync == emTYPE_Sync) {
        if (instance[idx].cbTx != NULL){
            ((fpSPI)instance[idx].cbTx)(idx);
        }
    }
    return;
}

#if defined(__MM3N1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI DMA communication channel 2 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
//void DMA1_Channel2_IRQHandler()
//{
//  DMA_ClearFlag(DMA_ISR_TCIF2);
//    DRV_SPI_DMA_IRQHandler_RxCommon(instance[tbSubHandleIdx[0]].sPrefix.subIdx);
//}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI DMA communication channel 3 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel3_IRQHandler()
{
    DMA_ClearFlag(DMA_ISR_TCIF3);
    DRV_SPI_DMA_IRQHandler_TxCommon(instance[tbSubHandleIdx[0]].sPrefix.subIdx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI DMA communication channel 4 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel4_IRQHandler()
{
    DMA_ClearFlag(DMA_ISR_TCIF4);
    DRV_SPI_DMA_IRQHandler_RxCommon(instance[tbSubHandleIdx[1]].sPrefix.subIdx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI DMA communication channel 5 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel5_IRQHandler()
{
    DMA_ClearFlag(DMA_ISR_TCIF5);
    DRV_SPI_DMA_IRQHandler_TxCommon(instance[tbSubHandleIdx[1]].sPrefix.subIdx);
}
#endif /* defined(__MM3N1) */

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI DMA communication channel 2 and 3 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel2_3_IRQHandler()
{
    u8 idx = instance[tbSubHandleIdx[0]].sPrefix.subIdx;

    if (DMA_GetITStatus(DMA_ISR_TCIF2)) {
        DMA_ClearFlag(DMA_ISR_TCIF2);
        DRV_SPI_DMA_IRQHandler_RxCommon(idx);
    }
    if (DMA_GetITStatus(DMA_ISR_TCIF3)) {
        DMA_ClearFlag(DMA_ISR_TCIF3);
        if (instance[idx].master == true) {
            while(!SPI_GetFlagStatus(SPI1, SPI_FLAG_TXEPT));
            BSP_SPI_NSS_Configure(SPI1, instance[idx].remapEn, instance[idx].remapIdx,DISABLE);
        }
        DRV_SPI_DMA_IRQHandler_TxCommon(idx);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI DMA communication channel 4 and 5 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel4_5_IRQHandler()
{
    u8 idx = instance[tbSubHandleIdx[1]].sPrefix.subIdx;

    if (DMA_GetITStatus(DMA_ISR_TCIF4)) {
        DMA_ClearFlag(DMA_ISR_TCIF4);
        DRV_SPI_DMA_IRQHandler_RxCommon(idx);
    }
    if (DMA_GetITStatus(DMA_ISR_TCIF5)) {
        DMA_ClearFlag(DMA_ISR_TCIF5);
        if (instance[idx].master == true) {
            while(!SPI_GetFlagStatus(SPI2,SPI_FLAG_TXEPT));
            BSP_SPI_NSS_Configure(SPI2,instance[idx].remapEn,instance[idx].remapIdx,DISABLE);
        }
        DRV_SPI_DMA_IRQHandler_TxCommon(idx);
    }
}
#endif  // defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI interrupt general handler function.
/// @param  idx: Select the SPI instance ID.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void DRV_SPI_IRQ_Common(u8 idx)
{
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;

    if ((SPIx->IER & SPI_IER_TX_IEN) && SPI_GetITStatus(SPIx, SPI_IT_TX)) {
        SPI_ClearITPendingBit(SPIx, SPI_IT_TX);

        instance[idx].txCnt --;
        instance[idx].txRealCnt ++;

        if(instance[idx].txCnt > 0){
            WRITE_REG(SPIx->TDR, *++instance[idx].txPtr);
        }
        else{
            while(!SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT));
            instance[idx].txComplete = true;
            CLEAR_BIT(SPIx->IER, SPI_IT_TX);
//            if (instance[idx].master) {
//                BSP_SPI_NSS_Configure(SPIx, 0, 0, DISABLE);
//            }
            if (instance[idx].sync == emTYPE_Sync) {
                if (instance[idx].cbTx != NULL){
                    ((fpSPI)instance[idx].cbTx)(idx);
                }
            }
            return;
        }
        return;
    }

    if ((SPIx->IER & SPI_IER_RX_IEN) && SPI_GetITStatus(SPIx, SPI_IT_RX)) {
        SPI_ClearITPendingBit(SPIx, SPI_IT_RX);

        while(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)){
            instance[idx].rxCnt --;
            instance[idx].rxRealCnt ++;
            *instance[idx].rxPtr++ = (u8)READ_REG(SPIx->RDR);
        }


        if (instance[idx].rxCnt > 0) {
            if (instance[idx].master) {
                SPI_SendData(SPIx, 0x0F);
            }
        }
        else{
            instance[idx].rxComplete = true;
            CLEAR_BIT(SPIx->IER, SPI_IER_RX_IEN);
            if (instance[idx].sync == emTYPE_Sync) {
                if (instance[idx].cbRx != NULL){
                    ((fpSPI)instance[idx].cbRx)(idx);
                }
            }
            return;
        }
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI1 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI1_IRQHandler(void)
{
    DRV_SPI_IRQ_Common(instance[tbSubHandleIdx[0]].sPrefix.subIdx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI2 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI2_IRQHandler()
{
    DRV_SPI_IRQ_Common(instance[tbSubHandleIdx[1]].sPrefix.subIdx);
}
#endif  // defined(__EX_SPI)



////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the SPI DMA communication interrupt according
///         to the specified SPIx.
/// @param  SPIx: where x can be 1, 2 to select the SPI peripheral.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void DRV_SPI_NVIC_DMA_Init(SPI_TypeDef* SPIx)
{
    COMMON_NVIC_Configure(Get_SPI_DMA_IRQ_RxIndex(SPIx),  1,  1);
    COMMON_NVIC_Configure(Get_SPI_DMA_IRQ_TxIndex(SPIx),  1,  1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the SPI communication interrupt according
///         to the specified SPIx.
/// @param  SPIx: where x can be 1, 2 to select the SPI peripheral.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void DRV_SPI_NVIC_Init(SPI_TypeDef* SPIx)
{
    COMMON_NVIC_Configure(Get_SPI_IRQ_Index(SPIx),  1,  1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the SPI DMA communication channel according
///         to the specified SPIx.
/// @param  SPIx: where x can be 1, 2 to select the SPI peripheral.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void DRV_SPI_DMA_ConfigChannel(SPI_TypeDef* SPIx)
{
    tDRV_DMA_DCB    dma;

    dma.base                    = SPIx;
    dma.BufferSize          = 0;
    dma.MemoryBaseAddr      = 0;

    dma.PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    dma.MemoryDataSize      = DMA_MemoryDataSize_Byte;
    dma.Mode                = DMA_Mode_Normal;
    dma.PeripheralInc       = DMA_PeripheralInc_Disable;
    dma.MemoryInc           = DMA_MemoryInc_Enable;

//RX
    dma.channel                 = Get_SPI_DMA_RxChannel(SPIx);
    dma.dmaDir                  = emDMA_p2m;
    dma.PeripheralBaseAddr  = (u32)&SPIx->RDR;
    dma.Priority            = DMA_Priority_VeryHigh;
    DRV_DMA_Init(&dma);

//TX
    dma.channel                 = Get_SPI_DMA_TxChannel(SPIx);
    dma.dmaDir                  = emDMA_m2p;
    dma.PeripheralBaseAddr  = (u32)&SPIx->TDR;
    dma.Priority            = DMA_Priority_Low;
    DRV_DMA_Init(&dma);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function Sends packet in blocking mode.
/// @param  idx: Select the SPI instance ID.
/// @retval  Result of the function.
////////////////////////////////////////////////////////////////////////////////
static bool SPI_PollingSendPacket(u8 idx)
{
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;

#if defined(__MT309)
    SET_BIT(SPIx->CCR, 1 << 7);
#endif

//    if (instance[idx].master) {
//        BSP_SPI_NSS_Configure(SPIx, 0, 0, ENABLE);
//    }
//
//    if(instance[idx].command != 0){
//        WRITE_REG(SPIx->TDR, instance[idx].command);
//    }

    WRITE_REG(SPIx->TDR, *instance[idx].txPtr++);
    instance[idx].txCnt --;
    instance[idx].txRealCnt ++;

    while(instance[idx].txCnt > 0) {
        if (SPI_GetITStatus(SPIx, SPI_IT_TX)){
            SPI_ClearITPendingBit(SPIx, SPI_IT_TX);
            WRITE_REG(SPIx->TDR, *instance[idx].txPtr++);
            instance[idx].txCnt --;
            instance[idx].txRealCnt ++;
        }
    }
    while(SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT) == 0) {
    }

//    while(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)){
//        READ_REG(SPIx->RDR);
//    }
//
//    if (instance[idx].master) {
//        BSP_SPI_NSS_Configure(SPIx, 0, 0, DISABLE);
//    }
    instance[idx].txComplete = true;
    if (instance[idx].sync == emTYPE_Sync) {
        if (instance[idx].cbTx != NULL){
            ((fpSPI)instance[idx].cbTx)(idx);
        }
    }
    return true;
}



////////////////////////////////////////////////////////////////////////////////
/// @brief  This function receives packet in blocking mode.
/// @param  idx: Select the SPI instance ID.
/// @retval  Result of the function.
////////////////////////////////////////////////////////////////////////////////
static bool SPI_PollingRcvPacket(u8 idx)
{
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;

//    if (instance[idx].master){
//        BSP_SPI_NSS_Configure(SPIx, 0, 0, ENABLE);
//        if(instance[idx].command != 0){
//            WRITE_REG(SPIx->TDR, instance[idx].command);
//            while(!SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL));
//            READ_REG(SPIx->RDR);
//        }
//    }

    while(instance[idx].rxCnt > 0) {
        if (instance[idx].master){
            WRITE_REG(SPIx->TDR, 0xFF);
        }
        while(!SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL));
        *instance[idx].rxPtr++ = (u8)READ_REG(SPIx->RDR);
        instance[idx].rxCnt --;
        instance[idx].rxRealCnt ++;
    }

    instance[idx].rxComplete = true;
//    if (instance[idx].master) {
//        BSP_SPI_NSS_Configure(SPIx, 0, 0, DISABLE);
//    }

    if (instance[idx].sync == emTYPE_Sync) {
        if(instance[idx].cbRx != NULL){
            ((fpSPI)instance[idx].cbRx)(idx);
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function Sends SPI packet by interrupt.
/// @param  idx: Select the SPI instance ID.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
static void SPI_ItSendPacket(u8 idx)
{
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;


//    if (instance[idx].master) {
//        BSP_SPI_NSS_Configure(SPIx, 0, 0, ENABLE);
//    }

    SPI_ClearITPendingBit(SPIx, SPI_IT_TX);

    SET_BIT(SPIx->GCR, SPI_GCR_IEN);
    SET_BIT(SPIx->IER, SPI_IER_TX_IEN);

    WRITE_REG(SPIx->TDR, *instance[idx].txPtr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function receives SPI packet by interrupt.
/// @param  idx: Select the SPI instance ID.
/// @retval  : None
////////////////////////////////////////////////////////////////////////////////
static void SPI_ItRcvPacket(u8 idx)
{
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;

    if (instance[idx].master) {
//        BSP_SPI_NSS_Configure(SPIx, 0, 0, ENABLE);
//
//        if(instance[idx].command != 0){
//            WRITE_REG(SPIx->TDR, instance[idx].command);
//            while(!SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL));
//            READ_REG(SPIx->RDR);
//            SPI_ClearITPendingBit(SPIx, SPI_IT_TX);
//        }
        SPI_ClearITPendingBit(SPIx, SPI_IT_TX);
        SPI_SendData(SPIx, 0xFF);
        while(!SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT));

//        while(!SPI_GetITStatus(SPIx, SPI_IT_TX));


    }

    SET_BIT(SPIx->GCR, SPI_GCR_IEN);
    SET_BIT(SPIx->IER, SPI_IER_RX_IEN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Start the SPI DMA channel to send the packet.
/// @param  idx: Select the SPI instance ID.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void SPI_DMASendPacket(u8 idx)
{
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;

//    if (instance[idx].master) {
//        BSP_SPI_NSS_Configure(SPIx, 0, 0, ENABLE);
//    }
//    if(instance[idx].command != 0){
//        WRITE_REG(SPIx->TDR, instance[idx].command);
//        SPI_ClearITPendingBit(SPIx, SPI_IT_TX);
//    }
    SPI_DMACmd(SPIx, ENABLE);


    DRV_DMA_TransmitPacket(Get_SPI_DMA_TxChannel(SPIx), (u32)instance[idx].txPtr, instance[idx].txCnt);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Start the SPI DMA channel to receive the packet.
/// @param  idx: Select the SPI instance ID.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void SPI_DMARcvPacket(u8 idx)
{
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;

//    SPI_ClearITPendingBit(SPIx, SPI_IT_RX);
    SPI_DMACmd(SPIx, ENABLE);

    DRV_DMA_TransmitPacket(Get_SPI_DMA_RxChannel(SPIx), (u32)instance[idx].rxPtr, instance[idx].rxCnt);

    if (instance[idx].master) {
//        BSP_SPI_NSS_Configure(SPIx, 0, 0, ENABLE);
        DRV_DMA_TransmitPacket(Get_SPI_DMA_TxChannel(SPIx), (u32)instance[idx].rxPtr, instance[idx].rxCnt);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of SPI.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_SPI_DCB* pDcb, u8 idx)
{
    instance[idx].txProcess     = false;
    instance[idx].rxProcess     = false;

    instance[idx].type          = pDcb->type;
    instance[idx].block         = pDcb->block;
    instance[idx].sync          = pDcb->sync;
    instance[idx].cbTx          = pDcb->cbTx;
    instance[idx].cbRx          = pDcb->cbRx;
    instance[idx].master        = pDcb->master;

    instance[idx].remapEn       = pDcb->remapEn;
    instance[idx].remapIdx      = pDcb->remapIdx;

    instance[idx].txProcess     = false;
    instance[idx].rxProcess     = false;
    instance[idx].txComplete    = false;
    instance[idx].rxComplete    = false;
    instance[idx].txRealCnt     = 0;
    instance[idx].rxRealCnt     = 0;

    instance[idx].command           = pDcb->command;
    instance[idx].protocol          = pDcb->protocol;

    instance[idx].parameter[0]      = pDcb->parameter[0];
    instance[idx].parameter[1]      = pDcb->parameter[1];
    instance[idx].parameter[2]      = pDcb->parameter[2];
    instance[idx].parameter[3]      = pDcb->parameter[3];

//    *(u32 *)instance[idx].parameter = *(u32 *)pDcb->parameter;
    instance[idx].parameterLength   = pDcb->parameterLength;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of SPI.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_SPI_DCB* pDcb, u8 idx)
{
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;

    BSP_SPI_GPIO_Configure(SPIx, pDcb->remapEn, pDcb->remapIdx, pDcb->master, pDcb->hardNss);

    Set_SPI_Clock(SPIx);

    if (pDcb->type == emTYPE_IT)
        DRV_SPI_NVIC_Init(SPIx);
    else if (pDcb->type == emTYPE_DMA) {
        DRV_SPI_NVIC_DMA_Init(SPIx);
        DRV_SPI_DMA_ConfigChannel(SPIx);
//        SPI_DMACmd(SPIx, ENABLE);
    }

    SPI_InitTypeDef InitStructure;

    switch(pDcb->mode) {
        case 0:
        InitStructure.SPI_CPOL  = (SPI_CPOL_TypeDef)0;
        InitStructure.SPI_CPHA  = (SPI_CPHA_TypeDef)SPI_CCR_CPHA;
        break;
        case 1:
        InitStructure.SPI_CPOL  = (SPI_CPOL_TypeDef)0;
        InitStructure.SPI_CPHA  = (SPI_CPHA_TypeDef)0;
        break;
        case 2:
        InitStructure.SPI_CPOL  = (SPI_CPOL_TypeDef)SPI_CCR_CPOL;
        InitStructure.SPI_CPHA  = (SPI_CPHA_TypeDef)SPI_CCR_CPHA;
        break;
        case 3:
        InitStructure.SPI_CPOL  = (SPI_CPOL_TypeDef)SPI_CCR_CPOL;
        InitStructure.SPI_CPHA  = (SPI_CPHA_TypeDef)0;
        break;
    }

    InitStructure.SPI_DataWidth         = 8;

    InitStructure.SPI_NSS               = (SPI_NSS_TypeDef)((pDcb->hardNss) ? SPI_GCR_NSS  : 0);
    InitStructure.SPI_Mode              = (SPI_Mode_TypeDef)((pDcb->master) ? SPI_GCR_MODE : 0);
    InitStructure.SPI_FirstBit          = (SPI_FirstBit_TypeDef)((pDcb->firstLsb) ? SPI_CCR_LSBFE : 0);

    InitStructure.SPI_BaudRatePrescaler = (SPI_BaudRatePrescaler_TypeDef)pDcb->prescaler;

    SPI_Init(SPIx, &InitStructure);

    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);

    if (pDcb->fastMode == true){
        SET_BIT(SPIx->CCR, SPI_CCR_RXEDGE | SPI_CCR_TXEDGE);
    }
    else{
        CLEAR_BIT(SPIx->CCR, SPI_CCR_RXEDGE | SPI_CCR_TXEDGE);
    }


    SPI_Cmd(SPIx, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function initial the SPI and related variebles
/// @param  pDcb: a pointer to SPI_HandleTypeDef structure that contains
///                the configuration information for the specified SPI module.
/// @retval  Process result.
////////////////////////////////////////////////////////////////////////////////
static int SPI_OpenFile(HANDLE handle, tAPP_SPI_DCB* pDcb)
{
    s8 idx = DRV_OpenFileEntry((u32*)&tbIpBase, (u8*)&tbSubHandleIdx, pDcb->hSub, (u8*)(&instance[0]), sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)      return 0;

// Instance configure
    InstanceConfig(pDcb, idx);

// Hardware configure
    HardwareConfig(pDcb, idx);

// Exit OpenFile Function
    return DRV_OpenFileExit(handle, (int*)&instance[idx], idx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function closes the SPI module
/// @param  handle: a pointer to HANDLE structure that contains the related
///                 information for the specified SPI.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

//  SPI_DeInit((SPI_TypeDef*)DRV_GetBase_SPIx(handle->idx));

    SPI_DeInit((SPI_TypeDef*)instance[tbSubHandleIdx[handle->idx]].sPrefix.pBase);

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI receiving data interface.
/// @param  handle: a pointer to HANDLE structure that contains the configure
///                 information for the specified SPI.
/// @param  ptr: Pointer to data buffer.
/// @param  count: Amount of data to be received.
/// @retval  Actually received data number.
////////////////////////////////////////////////////////////////////////////////
static int SPI_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;

    if (idx == -1)  return false;

    if (!instance[idx].rxProcess) {
        instance[idx].rxProcess = true;
        instance[idx].rxComplete = false;

        instance[idx].rxPtr = ptr;
        instance[idx].rxCnt = len;

        if (instance[idx].master) {
            BSP_SPI_NSS_Configure(SPIx, 0, 0, ENABLE);

            if(instance[idx].protocol == 1){
                WRITE_REG(SPIx->TDR, instance[idx].command);

                for(u8 i = 0; i < instance[idx].parameterLength; i++){
                    WRITE_REG(SPIx->TDR, instance[idx].parameter[i]);
                }

                while(SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT) == 0) {
                }
                while(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)){
                    READ_REG(SPIx->RDR);
                }

            }
        }


        if (instance[idx].type == emTYPE_Polling)    SPI_PollingRcvPacket(idx);
        else if (instance[idx].type == emTYPE_IT)    SPI_ItRcvPacket(idx);
        else if (instance[idx].type == emTYPE_DMA)   SPI_DMARcvPacket(idx);
    }

    if (instance[idx].block == emTYPE_Block) {
        while(!instance[idx].rxComplete);
        if (instance[idx].master) {
            BSP_SPI_NSS_Configure(SPIx, 0, 0, DISABLE);
        }
        instance[idx].rxProcess = false;
    }
    else{
        if (!instance[idx].rxComplete) {
            return false;
        }
        else{
            if (instance[idx].master) {
                BSP_SPI_NSS_Configure(SPIx, 0, 0, DISABLE);
            }
        }
    }
    return instance[idx].rxRealCnt;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI send data interface.
/// @param  handle: a pointer to HANDLE structure that contains the configure
///                 information for the specified SPI.
/// @param  ptr: Pointer to data buffer.
/// @param  count: Amount of data to be send.
/// @retval  Actually send data number.
////////////////////////////////////////////////////////////////////////////////
static int SPI_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    SPI_TypeDef* SPIx = (SPI_TypeDef*)instance[idx].sPrefix.pBase;

    if (idx == -1)  return false;

    if (!instance[idx].txProcess) {
        instance[idx].txProcess = true;
        instance[idx].txComplete = false;

        instance[idx].txPtr = ptr;
        instance[idx].txCnt = len;

        if (instance[idx].master) {
            BSP_SPI_NSS_Configure(SPIx, 0, 0, ENABLE);

            if(instance[idx].protocol == 1){
                WRITE_REG(SPIx->TDR, instance[idx].command);

                for(u8 i = 0; i < instance[idx].parameterLength; i++){
                    WRITE_REG(SPIx->TDR, instance[idx].parameter[i]);
                }

                while(SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT) == 0) {
                }
            }
        }


        if (instance[idx].type == emTYPE_Polling)    SPI_PollingSendPacket(idx);
        else if (instance[idx].type == emTYPE_IT)    SPI_ItSendPacket(idx);
        else if (instance[idx].type == emTYPE_DMA)   SPI_DMASendPacket(idx);
    }

    if (instance[idx].block == emTYPE_Block) {
        while(!instance[idx].txComplete);
        while(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)){
            READ_REG(SPIx->RDR);
        }

        if (instance[idx].master) {
            BSP_SPI_NSS_Configure(SPIx, 0, 0, DISABLE);
        }
        instance[idx].txProcess = false;
    }
    else{
        if (!instance[idx].txComplete) {
            return false;
        }
        else{
            while(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)){
                READ_REG(SPIx->RDR);
            }

            if (instance[idx].master) {
                BSP_SPI_NSS_Configure(SPIx, 0, 0, DISABLE);
            }
        }
    }

   return instance[idx].txRealCnt;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Initial SPI handler
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified SPI.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_CreateFile(HANDLE  handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&SPI_OpenFile;
    handle->closeFile   = (int*)&SPI_CloseFile;
    handle->readFile    = (int*)&SPI_ReadFile;
    handle->writeFile   = (int*)&SPI_WriteFile;
}


///@}

///@}

///@}
