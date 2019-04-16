////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_UART.C
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE UART DRIVER LAYER FUNCTIONS.
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
#define _DRV_UART_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_rcc.h"
#include "hal_nvic.h"
#include "hal_uart.h"

#include "bsp.h"

#include "drv.h"
#include "common.h"
#include "drv_dma.h"
#include "drv_uart.h"

#include "bsp_uart.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void Set_UART_Clock(UART_TypeDef* UARTx)
{
    if      (UARTx == UART1)    COMMON_EnableIpClock(emCLOCK_UART1);
    else if (UARTx == UART2)    COMMON_EnableIpClock(emCLOCK_UART2);
#if defined(__MM3N1)
    else if (UARTx == UART3)    COMMON_EnableIpClock(emCLOCK_UART3);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval DMA_Channel_TypeDef
////////////////////////////////////////////////////////////////////////////////
DMA_Channel_TypeDef* Get_UART_DMA_RxChannal(UART_TypeDef* UARTx, u8* n)
{
#if defined(__MM3N1)
    if      (UARTx == UART1) {  *n = 0; return DMA1_ch5; }
    else if (UARTx == UART2) {  *n = 1; return DMA1_ch6; }
    else if (UARTx == UART3) {  *n = 2; return DMA1_ch3; }
#endif

#if defined(__MM0N1) || defined(__MM0Q1)
    if (UARTx == UART1) {
        //if (!syscfg.cfgr.9)
            *n = 0;
            return DMA1_ch3;
        // else
        //  dma.channel = DMA1_ch5;
    }
    else {
//      if (syscfg.cfgr.9) {
//          if (UARTx == UART2) dma.channel = DMA1_ch5;
//      }
        *n = 1;
//      return 0;
    }
#endif
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval DMA_Channel_TypeDef
////////////////////////////////////////////////////////////////////////////////
DMA_Channel_TypeDef* Get_UART_DMA_TxChannal(UART_TypeDef* UARTx, u8* n)
{
#if defined(__MM3N1)
    if      (UARTx == UART1) {  *n = 0; return DMA1_ch4; }
    else if (UARTx == UART2) {  *n = 1; return DMA1_ch7; }
    else if (UARTx == UART3) {  *n = 2; return DMA1_ch2; }
//  return 0;
#endif

#if defined(__MM0N1) || defined(__MM0Q1)
    if (UARTx == UART1) {
        //if (!syscfg.cfgr.9)
            *n = 0;
            return  DMA1_ch2;
        // else
        //  dma.channel = DMA1_ch4;
    }
    else {
//      if (!syscfg.cfgr.9) {
//          if (UARTx == UART2) dma.channel = DMA1_ch4;
//      }
        *n = 1;
        return 0;
    }
#endif

#if !defined(__MM0N1) && !defined(__MM0Q1)
    return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval UART DMA IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_UART_DMA_IRQ_RxIndex(UART_TypeDef* UARTx)
{
#if defined(__MM3N1)
    if      (UARTx == UART1)    return DMA1_Channel5_IRQn;
    else if (UARTx == UART2)    return DMA1_Channel6_IRQn;
    else if (UARTx == UART3)    return DMA1_Channel3_IRQn;
    return 0;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    if      (UARTx == UART1)    return DMA1_Channel2_3_IRQn;
    else if (UARTx == UART1)    return DMA1_Channel4_5_IRQn;
    return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval UART DMA IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_UART_DMA_IRQ_TxIndex(UART_TypeDef* UARTx)
{
#if defined(__MM3N1)
    if      (UARTx == UART1)    return DMA1_Channel4_IRQn;
    else if (UARTx == UART2)    return DMA1_Channel7_IRQn;
    else if (UARTx == UART3)    return DMA1_Channel2_IRQn;
    return 0;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    if      (UARTx == UART1)    return DMA1_Channel2_3_IRQn;
    else if (UARTx == UART2)    return DMA1_Channel4_5_IRQn;
    return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval UART IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_UART_IRQ_Index(UART_TypeDef* UARTx)
{
    if      (UARTx == UART1)    return UART1_IRQn;
    else if (UARTx == UART2)    return UART2_IRQn;
#if defined(__MM3N1)
    else                        return UART3_IRQn;
#else
        return 0;
#endif
}




////////////////////////////////////////////////////////////////////////////////
/// @brief  This function Sends packet in blocking mode.
/// @param  idx: Select the UART instance ID.
/// @retval  Result of the function.
////////////////////////////////////////////////////////////////////////////////
static bool UART_PollingSendPacket_Block(u8 idx)
{
    UART_TypeDef* UARTx  = (UART_TypeDef*)instance[idx].sPrefix.pBase;

    for (u16 i = 0; i < instance[idx].txCnt; i++) {
        UART_SendData(UARTx, *(u8*)(instance[idx].txPtr++));
        instance[idx].txRealCnt++;
        while (UART_GetITStatus(UARTx, UART_IT_TXIEN) == RESET) {
            if (instance[idx].bTimeOut && (nTimeOutCnt >= instance[idx].txTimeOut)) {
                if (nTimeOutCnt >= instance[idx].txTimeOut)
                    return false;
            }
        }

        UART_ClearITPendingBit(UARTx, UART_IT_TXIEN);
    }
    instance[idx].txComplete = true;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function receives packet in blocking mode.
/// @param  idx: Select the UART instance ID.
/// @retval  Result of the function.
////////////////////////////////////////////////////////////////////////////////
static bool UART_PollingRcvPacket_Block(u8 idx)
{
    UART_TypeDef* UARTx  = (UART_TypeDef*)instance[idx].sPrefix.pBase;

    for (u16 i = 0; i < instance[idx].rxCnt; i++) {
        while (UART_GetITStatus(UARTx, UART_IT_RXIEN) == RESET){
            if (instance[idx].bTimeOut && (nTimeOutCnt >= instance[idx].rxTimeOut)) {
                if (nTimeOutCnt >= instance[idx].rxTimeOut)
                    return false;
            }
        }
        *(u8*)(instance[idx].rxPtr++) = UART_ReceiveData(UARTx);
        UART_ClearITPendingBit(UARTx, UART_IT_RXIEN);
        instance[idx].rxRealCnt++;
    }
    instance[idx].rxComplete = true;
    return true;
}

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Common Routing for UART DMA Tx interrupt.
/// @param  uartIdx: UART1,UART2... Index.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 DRV_UART_DMA_IRQHandler_TxCommon(u8 uartIdx)
{
    u8 idx = instance[tbSubHandleIdx[uartIdx]].sPrefix.subIdx;

    instance[idx].txComplete = true;
    instance[idx].txRealCnt = instance[idx].txCnt;

    if (instance[idx].sync == emTYPE_Sync) {
        ((fpUART)instance[idx].cbTx)(idx);
    }
    return idx;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Common Routing for UART DMA Rx interrupt.
/// @param  uartIdx: UART1,UART2... Index.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 DRV_UART_DMA_IRQHandler_RxCommon(u8 uartIdx)
{
    u8 idx = instance[tbSubHandleIdx[uartIdx]].sPrefix.subIdx;

    instance[idx].rxComplete = true;
    instance[idx].rxRealCnt = instance[idx].rxCnt;

    if (instance[idx].sync == emTYPE_Sync) {
        ((fpUART)instance[idx].cbRx)(idx);
    }
    return idx;
}

#if defined(__EX_UART)
#if defined(__MM3N1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART DMA communication channel 4 interrupt.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel4_IRQHandler()
{
    DMA_ClearFlag(DMA_ISR_TCIF4);
    DRV_UART_DMA_IRQHandler_TxCommon(0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART DMA communication channel 5 interrupt.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel5_IRQHandler()
{
    DMA_ClearFlag(DMA_ISR_TCIF5);
    DRV_UART_DMA_IRQHandler_RxCommon(0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART DMA communication channel 6 interrupt.
/// @param  None.
/// @retval None.
///////////////////////////////////////////////////////////////////////////////
void DMA1_Channel6_IRQHandler()
{
    DMA_ClearFlag(DMA_ISR_TCIF6);
    DRV_UART_DMA_IRQHandler_RxCommon(1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART DMA communication channel 7 interrupt.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel7_IRQHandler()
{
    DMA_ClearFlag(DMA_ISR_TCIF7);
    DRV_UART_DMA_IRQHandler_TxCommon(1);
}

#if (UART_NUM > 2)
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART DMA communication channel 2 interrupt.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel2_IRQHandler()
{
    DMA_ClearFlag(DMA_ISR_TCIF2);
    DRV_UART_DMA_IRQHandler_TxCommon(2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART DMA communication channel 3 interrupt.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel3_IRQHandler()
{
    DMA_ClearFlag(DMA_ISR_TCIF3);
    DRV_UART_DMA_IRQHandler_RxCommon(2);
}
#endif
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART DMA communication channel 2 and 3 interrupt.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel2_3_IRQHandler()
{
    if (DMA_GetITStatus(DMA_ISR_TCIF2)) {
        DMA_ClearFlag(DMA_ISR_TCIF2);
        DRV_UART_DMA_IRQHandler_TxCommon(0);
    }
    if (DMA_GetITStatus(DMA_ISR_TCIF3)) {
        DMA_ClearFlag(DMA_ISR_TCIF3);
        DRV_UART_DMA_IRQHandler_RxCommon(0);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART DMA communication channel 4 and 5 interrupt.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel4_5_IRQHandler()
{
    if (DMA_GetITStatus(DMA_ISR_TCIF4)) {
        DMA_ClearFlag(DMA_ISR_TCIF4);
        DRV_UART_DMA_IRQHandler_TxCommon(1);
    }
    if (DMA_GetITStatus(DMA_ISR_TCIF5)) {
        DMA_ClearFlag(DMA_ISR_TCIF5);
        DRV_UART_DMA_IRQHandler_RxCommon(1);
    }
}
#endif  // defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
#endif  /* __EX_UART */

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the UART interrupt according to the specified UARTx.
/// @param  UARTx: Select the UART or the UART peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_UART_NVIC_Init(UART_TypeDef* UARTx)
{
    COMMON_NVIC_Configure(Get_UART_IRQ_Index(UARTx),  1,  1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the UART DMA communication interrupt according
///         to the specified UARTx.
/// @param  UARTx: Select the UART or the UART peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_UART_NVIC_DMA_Init(UART_TypeDef* UARTx)
{
    COMMON_NVIC_Configure(Get_UART_DMA_IRQ_RxIndex(UARTx), 1, 2);
    COMMON_NVIC_Configure(Get_UART_DMA_IRQ_TxIndex(UARTx), 1, 2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the UART DMA communication channel according
///         to the specified UARTx.
/// @param  UARTx: Select the UART or the UART peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_UART_DMA_ConfigChannel(UART_TypeDef* UARTx)
{
    tDRV_DMA_DCB    dma;
    u8 n;

    dma.base                = UARTx;
    dma.MemoryBaseAddr      = 0;
    dma.BufferSize          = 0;
    dma.PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    dma.MemoryDataSize      = DMA_MemoryDataSize_Byte;
    dma.Mode                = DMA_Mode_Normal;
    dma.PeripheralInc       = DMA_PeripheralInc_Disable;
    dma.MemoryInc           = DMA_MemoryInc_Enable;

// RX
    dma.channel             = Get_UART_DMA_RxChannal(UARTx, &n);
    dma.dmaDir              = emDMA_p2m;
    dma.PeripheralBaseAddr  = (u32)&UARTx->RDR;
    dma.Priority            = DMA_Priority_VeryHigh;
    DRV_DMA_Init(&dma);

// TX
    dma.channel             = Get_UART_DMA_TxChannal(UARTx, &n);
    dma.dmaDir              = emDMA_m2p;
    dma.PeripheralBaseAddr  = (u32)&UARTx->TDR;
    dma.Priority            = DMA_Priority_VeryHigh;
    DRV_DMA_Init(&dma);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the UART DMA communication interrupt according
///         to the specified UARTx.
/// @param  UARTx: Select the UART or the UART peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_UART_DMA_RcvPacket(u8 idx)
{
    u8 n;

    UART_TypeDef* UARTx  = (UART_TypeDef*)instance[idx].sPrefix.pBase;

    DRV_DMA_TransmitPacket(Get_UART_DMA_RxChannal(UARTx, &n),
                           (u32)(instance[instance[tbSubHandleIdx[n]].sPrefix.subIdx].rxPtr),
                           instance[instance[tbSubHandleIdx[n]].sPrefix.subIdx].rxCnt);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Start the UART DMA channel to send the packet.
/// @param  idx: Select the UART instance ID.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_UART_DMA_SendPacket(u8 idx)
{
    u8 n = 0;

    UART_TypeDef* UARTx  = (UART_TypeDef*)instance[idx].sPrefix.pBase;

    DRV_DMA_TransmitPacket(Get_UART_DMA_TxChannal(UARTx, &n),
                           (u32)(instance[instance[tbSubHandleIdx[n]].sPrefix.subIdx].txPtr),
                           instance[instance[tbSubHandleIdx[n]].sPrefix.subIdx].txCnt);
}

////////////////////////////////////////////////////////////////////////////////
//
//      I n t e r r u p t    R o u t i n g
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART interrupt general handler function.
/// @param  idx: Select the UART instance ID.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UARTx_IRQHandler_Common(u8 idx)
{
    UART_TypeDef* UARTx = (UART_TypeDef*)instance[idx].sPrefix.pBase;

    // Receive packet
    if (UART_GetITStatus(UARTx, UART_IT_RXIEN) != RESET) {
        UART_ClearITPendingBit(UARTx, UART_IT_RXIEN);
        *(u8*)(instance[idx].rxPtr++) = UART_ReceiveData(UARTx);
        instance[idx].rxRealCnt++;
        if (--instance[idx].rxCnt == 0) {
            instance[idx].rxComplete = true;
            UART_ITConfig(UARTx, UART_IT_RXIEN, DISABLE);
            if (instance[idx].sync == emTYPE_Sync) {
                ((fpUART)instance[idx].cbRx)(idx);
            }
        }
    }
    // Send packet
    if (UART_GetITStatus(UARTx, UART_IT_TXIEN) != RESET) {
        UART_ClearITPendingBit(UARTx, UART_IT_TXIEN);
        instance[idx].txRealCnt++;
        if (--instance[idx].txCnt == 0) {
            UART_ITConfig(UARTx, UART_IT_TXIEN, DISABLE);
            while (!UART_GetFlagStatus(UARTx, UART_CSR_TXC));
            instance[idx].txComplete = true;
            if (instance[idx].sync == emTYPE_Sync) {
                ((fpUART)instance[idx].cbTx)(idx);
            }
        }
        else {
            UART_SendData(UARTx, *(u8*)(instance[idx].txPtr++));
        }
    }
}

#if defined(__EX_UART)
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles UART 1 interrupt request.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_IRQHandler(void)
{
    UARTx_IRQHandler_Common(instance[tbSubHandleIdx[0]].sPrefix.subIdx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles UART 2 interrupt request.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART2_IRQHandler()
{
    UARTx_IRQHandler_Common(instance[tbSubHandleIdx[1]].sPrefix.subIdx);
}

////////////////////////////////////////////////////////////////////////////////
#if defined(__MM3N1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles UART 3 interrupt request.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART3_IRQHandler()
{
    UARTx_IRQHandler_Common(instance[tbSubHandleIdx[2]].sPrefix.subIdx);
}
#endif
#endif      /* __EX_UART */


////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of UART.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_UART_DCB* pDcb, u8 idx)
{
    instance[idx].txProcess = false;
    instance[idx].rxProcess = false;
    instance[idx].type      = pDcb->type;                               // polling, interrupt, dma
    instance[idx].block     = pDcb->block;                              // Blocking, NonBlocking
    instance[idx].sync      = pDcb->sync;                               // Sync, ASync
    instance[idx].cbTx      = pDcb->cbTx;                               // Callback Tx
    instance[idx].cbRx      = pDcb->cbRx;                               // Callback Rx
    instance[idx].remapEn   = pDcb->remapEn;
    instance[idx].remapIdx  = pDcb->remapIdx;
    instance[idx].rxProcess = false;

    if (pDcb->timeOut) {
        instance[idx].bTimeOut  = true;
        instance[idx].nTimeOut  = pDcb->timeOut;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of UART.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_UART_DCB* pDcb, u8 idx)
{
    UART_WordLength_TypeDef tbWordLength[] =  {UART_WordLength_5b,UART_WordLength_6b,UART_WordLength_7b,UART_WordLength_8b};

// Initia UARTx
    UART_InitTypeDef InitStructure;

    UART_TypeDef* UARTx  = (UART_TypeDef*)instance[idx].sPrefix.pBase;

    if      (pDcb->type == emTYPE_IT)       DRV_UART_NVIC_Init(UARTx);
    else if (pDcb->type == emTYPE_DMA)      DRV_UART_NVIC_DMA_Init(UARTx);


    InitStructure.BaudRate      = pDcb->baudRate;;
    InitStructure.WordLength    = tbWordLength[pDcb->width - 5];

    if      (pDcb->stop == 1)               InitStructure.StopBits  = UART_StopBits_2;
#if defined(__MM0P1) || defined(__MM0Q1)
    else if (pDcb->stop == 2)               InitStructure.StopBits  = UART_StopBits_0_5;
    else if (pDcb->stop == 3)               InitStructure.StopBits  = UART_StopBits_1_5;
#endif
    else                                    InitStructure.StopBits  = UART_StopBits_1;

    if      (pDcb->parity == 1)             InitStructure.Parity    = UART_Parity_Even;
    else if (pDcb->parity == 2)             InitStructure.Parity    = UART_Parity_Odd;
    else                                    InitStructure.Parity    = UART_Parity_No;

    InitStructure.HWFlowControl = (UART_HW_FLOWCONTROL_TypeDef)((pDcb->hardFlow) ? UART_GCR_AUTOFLOW : 0);

    if      (pDcb->modeTxRx == emTXRX_Tx)   InitStructure.Mode = (EM_UARTMODE)UART_Mode_Rx;
    else if (pDcb->modeTxRx == emTXRX_Rx)   InitStructure.Mode = (EM_UARTMODE)UART_Mode_Tx;
    else                                    InitStructure.Mode = (EM_UARTMODE)(UART_Mode_Rx | UART_Mode_Tx);

    if      (pDcb->type == emTYPE_DMA)      InitStructure.Mode |=  UART_GCR_DMA;
    else                                    InitStructure.Mode &= ~UART_GCR_DMA;

    Set_UART_Clock(UARTx);

    UART_Init(UARTx, &InitStructure);
    UART_Cmd(UARTx, ENABLE);

// Enable Interrupt
    if (pDcb->type == emTYPE_DMA) {
        DRV_UART_DMA_ConfigChannel(UARTx);
        UART_DMACmd(UARTx, UART_GCR_DMA, ENABLE);
    }

    BSP_UART_GPIO_Configure(UARTx, pDcb->remapEn, pDcb->remapIdx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function opens the UART instance according to the specified
///         parameters in the UART_DCB_TypeDef
/// @param  pDcb: pointer to a UART_HandleTypeDef structure that contains
///                the configuration information for the specified UART module.
/// @retval  pointer to UART instance.
////////////////////////////////////////////////////////////////////////////////
static int UART_OpenFile(HANDLE handle, tAPP_UART_DCB* pDcb)
{
// Entry OpenFile Function
    s8 idx = DRV_OpenFileEntry((u32*)&tbIpBase, (u8*)&tbSubHandleIdx, pDcb->hSub, (u8*)(&instance[0]), sizeof(instance[0]),  INSTANCE_NUM);
    if (idx == -1)      return 0;

// Instance configure
    InstanceConfig(pDcb, idx);

// Hardware configure
    HardwareConfig(pDcb, idx);

// Exit OpenFile Function
    return DRV_OpenFileExit(handle, (int*)&instance[idx], idx);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function closes the UART instance according to the ID
///          in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified UART.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void UART_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    UART_DeInit((UART_TypeDef*)instance[tbSubHandleIdx[handle->idx]].sPrefix.pBase);

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  the UART receives data according to the ID in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified UART.
/// @param  ptr: Pointer to data buffer.
/// @param  count: Amount of data to be received.
/// @retval  Received data number.
////////////////////////////////////////////////////////////////////////////////
static int UART_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    if (!instance[idx].rxProcess) {
         instance[idx].rxProcess = true;
         instance[idx].rxComplete = false;
         instance[idx].rxRealCnt = 0;
         instance[idx].rxCnt = len;
         instance[idx].rxPtr = ptr;
         instance[idx].rxTimeOut = instance[handle->idx].nTimeOut + nTimeOutCnt;

        switch (instance[idx].type) {
            case emTYPE_Polling:
                if (!UART_PollingRcvPacket_Block(idx)) {
                    return emRETURN_TimeOut;
                }
                break;
            case emTYPE_IT:
//              UART_ITConfig((UART_TypeDef*)DRV_GetBase_UARTx(idx), UART_IT_RXIEN, ENABLE);
                UART_ITConfig((UART_TypeDef*)instance[tbSubHandleIdx[idx]].sPrefix.pBase, UART_IT_RXIEN, ENABLE);
                break;
            case emTYPE_DMA:
                DRV_UART_DMA_RcvPacket(idx);
                break;
            default:
                break;
        }
    }

    if (instance[idx].block == emTYPE_Block){
        while (!instance[idx].rxComplete) {
            if (instance[idx].bTimeOut && (nTimeOutCnt >= instance[idx].rxTimeOut)) {
                return emRETURN_TimeOut;
            }
        }
        instance[idx].rxProcess = false;
    }
    else {
        if (instance[idx].rxComplete){
            instance[idx].rxProcess = false;
        }
        else if ((instance[idx].type == emTYPE_DMA) || (instance[idx].type == emTYPE_IT)) {
            if (instance[idx].bTimeOut && (nTimeOutCnt >= instance[idx].rxTimeOut)) {
                return emRETURN_TimeOut;
            }
        }
    }

    return instance[idx].rxRealCnt;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  the UART sends data according to the ID in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified UART.
/// @param  ptr: Pointer to data buffer.
/// @param  count: Amount of data to be sent.
/// @retval  sent data number.
////////////////////////////////////////////////////////////////////////////////
static int UART_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    if (!(instance[idx].txProcess)) {
        instance[idx].txProcess = true;
        instance[idx].txComplete = false;
        instance[idx].txRealCnt = 0;

        instance[idx].txPtr = ptr;
        instance[idx].txCnt = len;

        instance[idx].txTimeOut = instance[idx].nTimeOut + nTimeOutCnt;

        switch (instance[idx].type) {
            case emTYPE_Polling:
                if (!UART_PollingSendPacket_Block(idx))
                    return emRETURN_TimeOut;
                instance[idx].txProcess = false;
                break;
            case emTYPE_IT:
                UART_ITConfig((UART_TypeDef*)instance[tbSubHandleIdx[idx]].sPrefix.pBase, UART_IT_TXIEN, ENABLE);
                UART_SendData((UART_TypeDef*)instance[tbSubHandleIdx[idx]].sPrefix.pBase, *(u8*)(instance[idx].txPtr++));
                break;
            case emTYPE_DMA:
                DRV_UART_DMA_SendPacket(idx);
                break;
            default:
            break;
        }
    }

    if (instance[idx].block == emTYPE_Block){
        while (!instance[idx].txComplete) {
            if (instance[idx].bTimeOut && (nTimeOutCnt >= instance[idx].txTimeOut)) {
                instance[idx].txProcess = false;
                return emRETURN_TimeOut;
            }
        }
        instance[idx].txProcess = false;
    }
    else {
        if (instance[idx].txComplete)
            instance[idx].txProcess = false;
        else if ((instance[idx].type == emTYPE_DMA) || (instance[idx].type == emTYPE_IT)) {
            if (instance[idx].bTimeOut &&(nTimeOutCnt >= instance[idx].txTimeOut)) {
                instance[idx].txProcess = false;
                return emRETURN_TimeOut;
            }
        }
    }

    return instance[idx].txRealCnt;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified UART.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART_CreateFile(HANDLE handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&UART_OpenFile;
    handle->closeFile   = (int*)&UART_CloseFile;
    handle->readFile    = (int*)&UART_ReadFile;
    handle->writeFile   = (int*)&UART_WriteFile;
}


/// @}

/// @}

/// @}
