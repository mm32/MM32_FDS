////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_I2C.C
/// @author   S Yi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE I2C DRIVER LAYER FUNCTIONS.
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
#define _DRV_I2C_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_rcc.h"
#include "hal_nvic.h"
#include "hal_i2c.h"

#include "bsp.h"

#include "drv.h"
#include "common.h"
#include "drv_dma.h"

#include "drv_i2c.h"
#include "bsp_i2c.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup I2C
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup I2C_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void Set_I2C_Clock(I2C_TypeDef* I2Cx)
{
    if (I2Cx == I2C1)   COMMON_EnableIpClock(emCLOCK_I2C1);
#if defined(__MM3N1)
    else                COMMON_EnableIpClock(emCLOCK_I2C2);
#endif

}


////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval DMA_Channel_TypeDef
////////////////////////////////////////////////////////////////////////////////
DMA_Channel_TypeDef* Get_I2C_DMA_RxChannal(I2C_TypeDef* I2Cx, u8* n)
{
#if defined(__MM3N1)
    if (I2Cx == I2C1)   { *n = 0; return DMA1_ch7; }
    else                { *n = 1; return DMA1_ch5; }
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                         *n = 0;  return DMA1_ch3;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval DMA_Channel_TypeDef
////////////////////////////////////////////////////////////////////////////////
DMA_Channel_TypeDef* Get_I2C_DMA_TxChannal(I2C_TypeDef* I2Cx, u8* n)
{
#if defined(__MM3N1)
    if (I2Cx == I2C1)   { *n = 0; return DMA1_ch6; }
    else                { *n = 1; return DMA1_ch4; }
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                         *n = 0;  return DMA1_ch2;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval UART DMA IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_I2C_DMA_IRQ_RxIndex(I2C_TypeDef* I2Cx)
{
#if defined(__MM3N1)
    if      (I2Cx == I2C1)  return DMA1_Channel7_IRQn;
    else if (I2Cx == I2C2)  return DMA1_Channel5_IRQn;
                            return 0;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    if (I2Cx == I2C1)       return DMA1_Channel2_3_IRQn;
                            return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval UART DMA IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_I2C_DMA_IRQ_TxIndex(I2C_TypeDef* I2Cx)
{
#if defined(__MM3N1)
    if      (I2Cx == I2C1) return DMA1_Channel6_IRQn;
    else if (I2Cx == I2C2) return DMA1_Channel4_IRQn;
                            return 0;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    if      (I2Cx == I2C1) return DMA1_Channel2_3_IRQn;
                            return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval UART IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_I2C_IRQ_Index(I2C_TypeDef* I2Cx)
{
#if defined(__MM3N1)
    if      (I2Cx == I2C1)      return I2C1_IRQn;
    else                        return I2C2_IRQn;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    return I2C1_IRQn;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C interrupt initialization.
/// @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_I2C_NVIC_Init(I2C_TypeDef* I2Cx)
{
    COMMON_NVIC_Configure(Get_I2C_IRQ_Index(I2Cx),  0,  3);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C_DMA interrupt initialization.
/// @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_I2C_NVIC_DMA_Init(I2C_TypeDef* I2Cx)
{
    COMMON_NVIC_Configure(Get_I2C_DMA_IRQ_RxIndex(I2Cx), 1, 2);
    COMMON_NVIC_Configure(Get_I2C_DMA_IRQ_TxIndex(I2Cx), 1, 2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C_DMA channel configuration.
/// @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_I2C_DMA_ConfigChannel(I2C_TypeDef* I2Cx)
{
    tDRV_DMA_DCB    dma;
    u8 n;

    dma.base                = I2Cx;
    dma.MemoryBaseAddr      = 0;
    dma.BufferSize          = 0;
    dma.MemoryDataSize      = DMA_MemoryDataSize_Byte;
    dma.PeripheralInc       = DMA_PeripheralInc_Disable;
    dma.MemoryInc           = DMA_MemoryInc_Enable;
    dma.PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    dma.Priority            = DMA_Priority_VeryHigh;

// RX
    dma.channel             = Get_I2C_DMA_RxChannal(I2Cx, &n);
    dma.PeripheralBaseAddr  = (u32) &(I2Cx->DR);
    dma.dmaDir              = emDMA_p2m;
    dma.Mode                = DMA_Mode_Circular;
    DRV_DMA_Init(&dma);

// TX
    dma.channel             = Get_I2C_DMA_TxChannal(I2Cx, &n);
    dma.PeripheralBaseAddr  = (u32) &(I2Cx->DR);
    dma.PeripheralDataSize  = DMA_PeripheralDataSize_Word;
    dma.dmaDir              = emDMA_m2p;
    dma.Mode                = DMA_Mode_Normal;
    DRV_DMA_Init(&dma);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Parameters configure when I2C transmission complete.
/// @param  idx: index of I2Cx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2CComplete(u8 idx)
{
    I2C_TypeDef* I2Cx = (I2C_TypeDef*)instance[idx].sPrefix.pBase;

    I2C_GenerateSTOP(I2Cx, ENABLE);
    I2C_ClearFlag(I2Cx, I2Cx->RAWISR);
    instance[idx].sadd = false;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C send a byte.
/// @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
/// @param  dat: data to be sent.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2CTxByte(I2C_TypeDef* I2Cx, u8 dat)
{
    I2C_SendData(I2Cx, dat);
    while(!I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFE));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C send address.
/// @param  idx: index of I2Cx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2CSendAddr(u8 idx)
{
    if (instance[idx].master) {
        GPIO_InitTypeDef  GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        I2C_Cmd((I2C_TypeDef*)instance[idx].sPrefix.pBase, DISABLE);

        (instance[idx].optRD) ?
            I2C_Send7bitAddress((I2C_TypeDef*)instance[idx].sPrefix.pBase, instance[idx].slave, I2C_Direction_Receiver)
                : I2C_Send7bitAddress((I2C_TypeDef*)instance[idx].sPrefix.pBase, instance[idx].slave, I2C_Direction_Transmitter);

        I2C_Cmd((I2C_TypeDef*)instance[idx].sPrefix.pBase, ENABLE);
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    else {
        I2C_SlaveConfigure((I2C_TypeDef*)instance[idx].sPrefix.pBase, ENABLE);
        I2C_Cmd((I2C_TypeDef*)instance[idx].sPrefix.pBase, ENABLE);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Common routing for I2C interrupt handler.
/// @param  idx: index of I2Cx.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2Cx_IRQHandler_Common(u8 idx)
{
    I2C_TypeDef* I2Cx = (I2C_TypeDef*)instance[idx].sPrefix.pBase;
//---------- Master mode Interrupt -----------
    if (instance[idx].master) {
        static  u8 n = 0;
        static  u8 m = 0;
        static  u8 _cnt;

        if (instance[idx].flag != emRETURN_Busy) {
            I2C_ClearITPendingBit(I2Cx,I2C_IT_STOP_DET);
            I2C_ITConfig(I2Cx, I2C_IT_RX_FULL | I2C_IT_TX_EMPTY | I2C_IT_STOP_DET | I2C_IT_START_DET | I2C_IT_ACTIVITY, DISABLE);
        }
        else if (instance[idx].revRD) {
            if (I2C_GetITStatus(I2Cx, I2C_IT_TX_ABRT))
                I2CComplete(idx);
            else {
                if (I2C_GetITStatus(I2Cx, I2C_IT_TX_EMPTY)) {
                    (m++ < (instance[idx].rx_len - 1)) ? I2C_ReadCmd(I2Cx) : I2C_ClearITPendingBit(I2Cx, I2C_IT_TX_EMPTY);
                }
                if (I2C_GetITStatus(I2Cx, I2C_IT_RX_FULL)) {
                    *instance[idx].rx_ptr++ = I2C_ReceiveData(I2Cx);
                    if  ((n++ >= (instance[idx].rx_len - 1))) {
                        I2CComplete(idx);
                    }
                }
            }
        }
        else if (I2C_GetITStatus(I2Cx, I2C_IT_TX_EMPTY)) {
            if (instance[idx].sadd) {
                instance[idx].sadd = false;
                m = 0; n = 0;
                _cnt = (instance[idx].optRD) ?  0 : instance[idx].tx_len;
            }
            else if (n < _cnt) {
                I2CTxByte(I2Cx, *instance[idx].tx_ptr++);
                n++;
            }
            else if (!(instance[idx].optRD)) {
                I2CComplete(idx);
            }
            else {
                instance[idx].revRD = instance[idx].optRD;
                I2C_ReadCmd(I2Cx);
                I2C_ITConfig(I2Cx, I2C_IT_RX_FULL | I2C_IT_RX_OVER , ENABLE);
            }
        }
    }
//---------- Slave mode Interrupt -----------
    if (!(instance[idx].master)) {
        if (I2C_GetITStatus(I2Cx, I2C_IT_RD_REQ)) {
            I2C_ClearITPendingBit(I2Cx, I2C_IT_RD_REQ);
            I2CTxByte(I2Cx, *instance[idx].tx_ptr);
            instance[idx].tx_ptr++;
            if (I2C_GetFlagStatus((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_FLAG_RX_DONE)) {
                instance[idx].flag = emRETURN_Ready;
                I2CComplete(idx);
            }
        }
        else if (I2C_GetITStatus(I2Cx, I2C_IT_RX_FULL)) {
            I2C_ClearITPendingBit(I2Cx, I2C_IT_RX_FULL);
            *instance[idx].rx_ptr = I2C_ReceiveData(I2Cx);
            instance[idx].rx_ptr++;
        }
    }
}

#if defined(__EX_I2C)
////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

#if defined (__MM3N1)
void I2C1_EV_IRQHandler (void)
#endif

#if defined (__MM0N1) || defined (__MM0P1) || defined (__MM0Q1)
void I2C1_IRQHandler (void)
#endif
{
    I2Cx_IRQHandler_Common(instance[tbSubHandleIdx[0]].sPrefix.subIdx);
}

#if defined (__MM3N1)
void I2C2_EV_IRQHandler (void)
{
    I2Cx_IRQHandler_Common(instance[tbSubHandleIdx[1]].sPrefix.subIdx);
}
#endif

#endif  /* __EX_I2C */

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C block transmission by polling.
/// @param  idx: index of I2Cx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void I2C_PollingSendPacket_Block(u8 idx)
{
    I2C_TypeDef* I2Cx = (I2C_TypeDef*)instance[idx].sPrefix.pBase;

    while (instance[idx].tx_len--) {
        I2CTxByte(I2Cx, *instance[idx].tx_ptr);
        instance[idx].tx_cnt++;
        instance[idx].tx_ptr++;
    }
    instance[idx].flag = instance[idx].tx_cnt;
    I2CComplete(idx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C block receive by polling.
/// @param  idx: index of I2Cx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void I2C_PollingRcvPacket_Block(u8 idx)
{
    I2C_TypeDef* I2Cx = (I2C_TypeDef*)instance[idx].sPrefix.pBase;

    instance[idx].rx_cnt = instance[idx].rx_len;
    for (u8 i = 0; i < instance[idx].rx_len; i++) {
        while(1) {
            if (I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFE) && instance[idx].rx_cnt) {
                I2C_ReadCmd(I2Cx);
                instance[idx].rx_cnt--;
            }
            if (I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_RFNE)) {
                instance[idx].rx_ptr[i] = I2C_ReceiveData(I2Cx);
                break;
            }
        }
    }
    instance[idx].flag = instance[idx].rx_len - instance[idx].rx_cnt;
    I2CComplete(idx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C slave transmission by polling.
/// @param  idx: index of I2Cx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void I2C_PollingSendPacket_Slave(u8 idx)
{
    if (I2C_GetITStatus((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_IT_RD_REQ)) {
        I2C_ClearITPendingBit((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_IT_RD_REQ);
        I2CTxByte((I2C_TypeDef*)instance[idx].sPrefix.pBase, *instance[idx].tx_ptr);
        instance[idx].tx_cnt++;
        instance[idx].tx_ptr++;
    }
    if (I2C_GetFlagStatus((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_FLAG_RX_DONE))
        instance[idx].flag = instance[idx].tx_cnt;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C slave receive by polling.
/// @param  idx: index of I2Cx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void I2C_PollingRcvPacket_Slave(u8 idx)
{
    if (I2C_GetITStatus((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_IT_RX_FULL)) {
        I2C_ClearITPendingBit((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_IT_RX_FULL);
        *instance[idx].rx_ptr = I2C_ReceiveData((I2C_TypeDef*)instance[idx].sPrefix.pBase);
        instance[idx].rx_cnt++;
        instance[idx].rx_ptr++;
//        if (!I2C_GetFlagStatus(instance[idx].I2Cx, I2C_STATUS_FLAG_RFNE))
//            instance[idx].flag = emRETURN_Ready;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C send packet by using DMA.
/// @param  idx: index of I2Cx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_I2C_DMA_SendPacket(u8 idx)
{
    u8 n = 0;
    I2C_TypeDef* I2Cx = (I2C_TypeDef*)instance[idx].sPrefix.pBase;

    I2C_DMAConfigure(I2Cx, I2C_Direction_Transmitter);

    DRV_DMA_TransmitPacket(Get_I2C_DMA_TxChannal(I2Cx, &n), (u32)(instance[n].tx_ptr), instance[n].tx_len);
    while(!I2C_GetFlagStatus(I2Cx, I2C_STATUS_FLAG_TFE));
    instance[idx].flag = instance[idx].tx_len;
    I2CComplete(idx);
}

#if defined(__EX_I2C)
#if defined(__MM3N1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C_DMA Channel6 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel6_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC6);
    u8 idx = instance[tbSubHandleIdx[0]].sPrefix.subIdx;

    if (instance[idx].sync == emTYPE_Sync) {
        ((fpI2C)instance[idx].cbTx)(idx);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C_DMA Channel7 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel7_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC7);
    u8 idx = instance[tbSubHandleIdx[0]].sPrefix.subIdx;
    instance[idx].rx_ptr++;
    instance[idx].rx_cnt++;
    instance[idx].rx_len--;

    if (instance[idx].sync == emTYPE_Sync) {
        ((fpI2C)instance[idx].cbTx)(idx);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C_DMA Channel4 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel4_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC4);
    u8 idx = instance[tbSubHandleIdx[1]].sPrefix.subIdx;

    if (instance[idx].sync == emTYPE_Sync) {
        ((fpI2C)instance[idx].cbTx)(idx);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C_DMA Channel5 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel5_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC5);
    u8 idx = instance[tbSubHandleIdx[1]].sPrefix.subIdx;
    instance[idx].rx_ptr++;
    instance[idx].rx_cnt++;
    instance[idx].rx_len--;
    if (instance[idx].sync == emTYPE_Sync) {
        ((fpI2C)instance[idx].cbTx)(idx);
    }
}
#endif  // defined(__MM3N1)

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C_DMA Channel2_3 interrupt handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel2_3_IRQHandler(void)
{
    u8 idx = instance[tbSubHandleIdx[0]].sPrefix.subIdx;
    if (instance[idx].optRD) {
        DMA_ClearITPendingBit(DMA1_IT_TC3);
        //I2C_ReadCmd(I2C1);
        instance[idx].rx_ptr++;
        instance[idx].rx_cnt++;
        instance[idx].rx_len--;
        if (instance[idx].sync == emTYPE_Sync) {
            ((fpI2C)instance[idx].cbTx)(idx);
        }
    }
    else {
        DMA_ClearITPendingBit(DMA1_IT_TC2);
    }
}
#endif  // defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
#endif  // defined(__EX_I2C)

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C receive packet.
/// @param  idx: index of I2Cx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_I2C_DMA_RcvPacket(u8 idx)
{
    u8 n;
    I2C_TypeDef* I2Cx = (I2C_TypeDef*)instance[idx].sPrefix.pBase;

    I2C_DMAConfigure(I2Cx, I2C_Direction_Receiver);

    DRV_DMA_TransmitPacket(Get_I2C_DMA_RxChannal(I2Cx, &n), (u32)(instance[n].rx_ptr), 1);
    while(1) {
        I2C_ReadCmd(I2Cx);
        if (!(instance[n].rx_len - 1))
            break;
    }
    instance[idx].flag = instance[idx].rx_cnt;
    I2CComplete(idx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of I2C.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_I2C_DCB* pDcb, u8 idx)
{
    instance[idx].type      = pDcb->type;                                   // polling, interrupt, dma
    instance[idx].block     = pDcb->block;                                  // Blocking, NonBlocking
    instance[idx].sync      = pDcb->sync;                                   // Sync, ASync
    instance[idx].cbTx      = pDcb->cbTx;                                   // Callback Tx
    instance[idx].cbRx      = pDcb->cbRx;                                   // Callback Rx
    instance[idx].master    = pDcb->master;

    instance[idx].slave     = pDcb->slave;
    instance[idx].subAddr   = pDcb->subAddr;
    instance[idx].subSize   = pDcb->subSize;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of I2C.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_I2C_DCB* pDcb, u8 idx)
{
    I2C_InitTypeDef InitStructure;

    I2C_TypeDef* I2Cx = (I2C_TypeDef*)instance[idx].sPrefix.pBase;

    if      (pDcb->type == emTYPE_IT)   DRV_I2C_NVIC_Init(I2Cx);
    else if (pDcb->type == emTYPE_DMA)  DRV_I2C_NVIC_DMA_Init(I2Cx);

    BSP_I2C_GPIO_Configure((I2C_TypeDef*)instance[idx].sPrefix.pBase, pDcb->remapEn, pDcb->remapIdx);

    InitStructure.Mode  = pDcb->master;
    InitStructure.Speed = pDcb->fast;
    InitStructure.OwnAddress = pDcb->ownaddr;

    InitStructure.ClockSpeed = (pDcb->fast) ? 400000 : 100000;

    Set_I2C_Clock(I2Cx);

    I2C_Init(I2Cx, &InitStructure);

    if (pDcb->type == emTYPE_DMA)   DRV_I2C_DMA_ConfigChannel(I2Cx);
    if (pDcb->master == false)      I2C_SendSlaveAddress(I2Cx, pDcb->ownaddr);

}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the basic parameters of I2C to prepare transmission.
/// @param  handle: HANDLE structure that contains
///                 related parameters about I2C.
/// @param  pDcb: pDcb: The pointer point to Device Control Block(DCB).
/// @retval The address of global variables instance[].
////////////////////////////////////////////////////////////////////////////////
static int I2C_OpenFile(HANDLE handle, tAPP_I2C_DCB* pDcb)
{
// Entry OpenFile Function
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
/// @brief  Close I2C transmission.
/// @param  handle: HANDLE structure that contains
///         related parameters about I2C.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void I2C_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    I2C_DeInit((I2C_TypeDef*)instance[tbSubHandleIdx[handle->idx]].sPrefix.pBase);

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C write data.
/// @param  handle: HANDLE structure that contains
///         related parameters about I2C.
/// @param  ptr: array address of transmitted data.
/// @param  len: length of transmitted data
/// @retval The state of transmission (true or false).
////////////////////////////////////////////////////////////////////////////////
static int I2C_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

//---------- Master mode -----------
    if (instance[idx].master) {
        if (instance[idx].flag  != emRETURN_Busy) {
            instance[idx].flag   = emRETURN_Busy;
            instance[idx].sadd   = true;
            instance[idx].optRD  = false;
            instance[idx].revRD  = false;
            instance[idx].tx_cnt = 0;
            instance[idx].tx_len = len;
            instance[idx].tx_ptr = ptr;

            I2CSendAddr(idx);
            I2CTxByte((I2C_TypeDef*)instance[idx].sPrefix.pBase, instance[idx].subAddr);
            if (!I2C_GetFlagStatus((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_STATUS_FLAG_ACTIVITY))
                return instance[idx].flag = emRETURN_NoAck;

            switch (instance[idx].type) {
            case emTYPE_Polling:
                I2C_PollingSendPacket_Block(idx);
                break;
            case emTYPE_DMA:
                DRV_I2C_DMA_SendPacket(idx);
                break;
            case emTYPE_IT:
                I2C_ITConfig((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_IT_TX_EMPTY | I2C_IT_STOP_DET | I2C_IT_START_DET | I2C_IT_ACTIVITY, ENABLE);
                break;
            default:
                instance[idx].flag = emRETURN_Fault;
            }
        }
    }
//---------- Slave mode ------------
    else {
        I2CSendAddr(idx);
        if (instance[idx].flag  != emRETURN_Busy) {
            instance[idx].flag = emRETURN_Busy;
            instance[idx].optRD = false;
            instance[idx].tx_cnt = 0;
            instance[idx].tx_ptr = ptr;

            switch (instance[idx].type) {
            case emTYPE_Polling:
                do {
                    I2C_PollingSendPacket_Slave(idx);
                } while (instance[idx].flag <= 0);
                break;
            case emTYPE_IT:
                I2C_ITConfig((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_IT_RD_REQ, ENABLE);
                while (!I2C_GetFlagStatus((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_FLAG_RX_DONE));
                break;
            case emTYPE_DMA:
                return emRETURN_Fault;
            }
        }
    }
    return instance[idx].flag;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C read data.
/// @param  handle: HANDLE structure that contains
///         related parameters about I2C.
/// @param  ptr: array address of transmitted data.
///         This parameter is useless in this function.
/// @param  len: length of transmitted data
///         This parameter is useless in this function.
/// @retval The state of transmission (true or false).
////////////////////////////////////////////////////////////////////////////////
static int I2C_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    if (instance[idx].master) {
        if (instance[idx].flag  != emRETURN_Busy) {
            instance[idx].flag   = emRETURN_Busy;
            instance[idx].sadd   = true;
            instance[idx].optRD  = true;
            instance[idx].revRD  = false;
            instance[idx].rx_cnt = 0;
            instance[idx].rx_len = len;
            instance[idx].rx_ptr = ptr;

            I2CSendAddr(idx);
            for (u8 i = 0; i < instance[idx].subSize; i++) {
                I2CTxByte((I2C_TypeDef*)instance[idx].sPrefix.pBase, instance[idx].subAddr);
                if (!I2C_GetFlagStatus((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_STATUS_FLAG_ACTIVITY))
                    return instance[idx].flag = emRETURN_NoAck;
            }

            switch (instance[idx].type) {
            case emTYPE_Polling:
                I2C_PollingRcvPacket_Block(idx);
                break;
            case emTYPE_DMA:
                DRV_I2C_DMA_RcvPacket(idx);
                break;
            case emTYPE_IT:
                I2C_ITConfig((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_IT_TX_EMPTY | I2C_IT_STOP_DET | I2C_IT_START_DET | I2C_IT_ACTIVITY, ENABLE);
                break;
            default:
                instance[idx].flag = emRETURN_Fault;
            }
        }
    }
    else {
        I2CSendAddr(idx);
        if (instance[idx].flag  != emRETURN_Busy) {
            instance[idx].flag = emRETURN_Busy;
            instance[idx].optRD = true;
            instance[idx].rx_cnt = 0;
            instance[idx].rx_ptr = ptr;

            switch (instance[idx].type) {
            case emTYPE_Polling:
                do {
                    I2C_PollingRcvPacket_Slave(idx);
                } while (1);
                break;
            case emTYPE_IT:
                I2C_ITConfig((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_IT_RX_FULL, ENABLE);
                while (I2C_GetFlagStatus((I2C_TypeDef*)instance[idx].sPrefix.pBase, I2C_STATUS_FLAG_RFNE));
                break;
            case emTYPE_DMA:
                return emRETURN_Fault;
            }
        }
    }
    return instance[idx].flag;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C create file
/// @param  handle: HANDLE structure that contains
///         related parameters about I2C.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2C_CreateFile(HANDLE  handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&I2C_OpenFile;
    handle->closeFile   = (int*)&I2C_CloseFile;
    handle->readFile    = (int*)&I2C_ReadFile;
    handle->writeFile   = (int*)&I2C_WriteFile;
}





/// @}

/// @}

/// @}

