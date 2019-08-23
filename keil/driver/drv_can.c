////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_CAN.C
/// @author
/// @version
/// @date
/// @brief    THIS FILE PROVIDES THE CAN DRIVER LAYER FUNCTIONS.
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
#define _DRV_CAN_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_can.h"
#include "hal_nvic.h"

#include "drv.h"
#include "common.h"
#include "drv_can.h"
#include "bsp_can.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1)
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CAN
/// @{
# define BUFFER_SIZE 199
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CAN_Exported_Functions
/// @{

u8 tempBuffer[BUFFER_SIZE];
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the CAN interrupt according to the specified UARTx.
/// @param  CANx: Select the 1 to the CAN peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

void DRV_CAN_NVIC_Init(CAN_Peli_TypeDef* CANx, u8 idx)
{
#if defined(__MM3N1) || defined(__MM3O1)
    COMMON_NVIC_Configure(CAN1_RX1_IRQn,  1,  1);
#endif
#if defined(__MM0N1)
        COMMON_NVIC_Configure(CAN_IRQn,  1,  1);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function receives CAN packet by interrupt.
/// @param  idx: Select the CAN instance ID.
/// @retval  : None
////////////////////////////////////////////////////////////////////////////////
static void CAN_ItRcvPacket(u8 idx)
{
//      CAN_Peli_TypeDef* CANx = (CAN_Peli_TypeDef*)instance[idx].sPrefix.pBase;

        CAN_Peli_Receive(&gPeliRxMessage);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  CAN interrupt general handler function.
/// @param  idx: Select the CAN instance ID.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void CAN1_IRQHandler_Common(u8 idx)
{
    CAN_Peli_TypeDef* CANx = (CAN_Peli_TypeDef*)instance[idx].sPrefix.pBase;

    // Receive packet
    if(CAN_Peli_GetITStatus(CAN_IR_RI)) {
        CAN_ItRcvPacket(idx);
        exCAN_ClearITPendingBit(CANx);
        if (--instance[idx].rxCnt == 0) {
            instance[idx].rxComplete = true;
            CAN_Peli_ITConfig(CAN_IR_RI, DISABLE);
            if (instance[idx].sync == emTYPE_Sync) {
                ((fpCAN)instance[idx].cbRx)(idx);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles CAN interrupt request.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

void CAN1_RX1_IRQHandler()
{
    CAN1_IRQHandler_Common(instance[tbSubHandleIdx[0]].sPrefix.subIdx);
}

//////////////////////////////////////////////////////////////////////////////////
///// @brief  Start the CAN to send the packet.
///// @param  idx: Select the CAN instance ID.
///// @retval : None.
//////////////////////////////////////////////////////////////////////////////////
static void DRV_CAN_ITSendPacket(u8 idx)
{
//  CAN_Peli_TypeDef* CANx = (CAN_Peli_TypeDef*)instance[idx].sPrefix.pBase;

    gPeliTxMessage.DLC = instance[idx].txCnt;
    gPeliTxMessage.FF = 1;
    gPeliTxMessage.IDHH =(instance[idx].id >> 3) & 0xff;
    gPeliTxMessage.IDHL =(instance[idx].id) & 0x7;

    memcpy(gPeliTxMessage.Data, instance[idx].txPtr, gPeliTxMessage.DLC);

    CAN_Peli_Transmit(&gPeliTxMessage);
    instance[idx].txComplete = true;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function receives packet in blocking mode.
/// @param  idx: Select the CAN instance ID.
/// @retval  Result of the function.
////////////////////////////////////////////////////////////////////////////////
static bool CAN_PollingRcvPacket_Block(u8 idx)
{
//  CAN_Peli_TypeDef* CANx = (CAN_Peli_TypeDef*)instance[idx].sPrefix.pBase;

    CAN_Peli_Receive(&gPeliRxMessage);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Start the CAN to send the packet.
/// @param  idx: Select the CAN instance ID.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static bool CAN_PollingSendPacket_Block(u8 idx)
{
//  CAN_Peli_TypeDef* CANx = (CAN_Peli_TypeDef*)instance[idx].sPrefix.pBase;

    gPeliTxMessage.DLC = instance[idx].txCnt;
    gPeliTxMessage.FF = 1;
    gPeliTxMessage.IDHH =(instance[idx].id >> 3) & 0xff;
    gPeliTxMessage.IDHL =(instance[idx].id) & 0x7;

    memcpy(gPeliTxMessage.Data, instance[idx].txPtr, gPeliTxMessage.DLC);
    CAN_Peli_Transmit(&gPeliTxMessage);
    instance[idx].txComplete = true;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of CAN.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_CAN_DCB* pDcb, u8 idx)
{
    instance[idx].canType   = pDcb->canType;                                    // polling, interrupt

    instance[idx].type      = pDcb->type;                                       // polling, interrupt
    instance[idx].testMode  = pDcb->testMode;                                   // polling, interrupt
    instance[idx].block     = pDcb->block;                                      // Blocking, NonBlocking
    instance[idx].sync      = pDcb->sync;                                       // Sync, ASync
    instance[idx].cbTx      = pDcb->cbTx;                                       // Callback Tx
    instance[idx].cbRx      = pDcb->cbRx;                                       // Callback Rx
    instance[idx].sample    = pDcb->sample;
    instance[idx].baudRate  = pDcb->baudRate;
    instance[idx].id        = pDcb->id;
    instance[idx].remapEn   = pDcb->remapEn;
    instance[idx].remapIdx  = pDcb->remapIdx;

    instance[idx].txProcess     = false;
    instance[idx].rxProcess     = false;
    instance[idx].txComplete    = false;
    instance[idx].rxComplete    = false;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of CAN.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_CAN_DCB* pDcb, u8 idx)
{

    // Initia CAN

    CAN_Peli_InitTypeDef                    CAN_Peli_InitStruct;

    CAN_Peli_TypeDef* CANx = (CAN_Peli_TypeDef*)instance[idx].sPrefix.pBase;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_CAN, ENABLE);

    if (pDcb->canType == emCAN_Peli)                CAN_Mode_Cmd(CAN1, CAN_PELIMode);
    CAN_ResetMode_Cmd(CAN1,ENABLE);


    CAN_AutoCfg_BaudParam(&CAN_Peli_InitStruct,  RCC_GetPCLK1Freq(), pDcb->baudRate );  //100K baud
    /* Initialize the SAM member(where can be set (SET or RESET)) */
    if (pDcb->sample == emTriple_Sample)            CAN_Peli_InitStruct.SAM = SET;
    else if (pDcb->sample == emSingle_Sample)       CAN_Peli_InitStruct.SAM = RESET;

    if (pDcb->testMode == emCAN_SeftTestMode) {
        CAN_Peli_InitStruct.LOM = DISABLE;
        CAN_Peli_InitStruct.STM = ENABLE;
    }
    else
        CAN_Peli_InitStruct.STM = DISABLE;

    CAN_Peli_InitStruct.EWLR = 0x96;
    CAN_Peli_Init  (&CAN_Peli_InitStruct);
    if      (pDcb->type == emTYPE_IT)           DRV_CAN_NVIC_Init(CANx,idx);

    CAN_Peli_FilterInitTypeDef CAN_Peli_FilterInitStruct;

    CAN_Peli_FilterInitStruct.AFM = CAN_FilterMode_Singal;
    CAN_Peli_FilterInitStruct.CAN_FilterId0 = 0x00;
    CAN_Peli_FilterInitStruct.CAN_FilterId1 = 0x00;
    CAN_Peli_FilterInitStruct.CAN_FilterId2 = 0x00;
    CAN_Peli_FilterInitStruct.CAN_FilterId3 = 0x00;

    CAN_Peli_FilterInitStruct.CAN_FilterMaskId0 = 0xff;
    CAN_Peli_FilterInitStruct.CAN_FilterMaskId1 = 0xff;
    CAN_Peli_FilterInitStruct.CAN_FilterMaskId2 = 0xff;
    CAN_Peli_FilterInitStruct.CAN_FilterMaskId3 = 0xff;

    CAN_Peli_FilterInit( &CAN_Peli_FilterInitStruct );
    CAN_ResetMode_Cmd(CAN1,DISABLE);

    BSP_CAN_GPIO_Configure(CAN1_PELI, pDcb->remapEn, pDcb->remapIdx);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function opens the CAN instance according to the specified
///         parameters in the CAN_DCB_TypeDef
/// @param  pDcb: pointer to a CAN_HandleTypeDef structure that contains
///                the configuration information for the specified CAN module.
/// @retval  pointer to CAN instance.
////////////////////////////////////////////////////////////////////////////////
static int CAN_OpenFile(HANDLE handle, tAPP_CAN_DCB* pDcb)
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
/// @brief Write the value of the output to be written to the dac register
/// @param handle: pointer to a HANDLE structure that contains
///               the instance for the specified CAN.
/// @param ptr: Memory address
/// @param count: Number of conversions
/// @retval: None
////////////////////////////////////////////////////////////////////////////////
static int CAN_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    if (!instance[idx].txProcess) {
        instance[idx].txProcess = true;
        instance[idx].txComplete = false;

        instance[idx].txPtr = ptr;
        instance[idx].txCnt = len;

    if (instance[idx].type == emTYPE_Polling)    CAN_PollingSendPacket_Block(idx);
    else if (instance[idx].type == emTYPE_IT)    DRV_CAN_ITSendPacket(idx);
    }
    if (instance[idx].block == emTYPE_Block) {
        while(!instance[idx].txComplete);
        instance[idx].txProcess = false;
    }
    else {
        if (!instance[idx].txComplete) {
            return false;
        }
    }

   return instance[idx].txComplete;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  the result of CAN receives.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified CAN.
/// @param  ptr: Pointer to data buffer.
/// @param  count: Amount of data to be received.
/// @retval  0 or 1.
////////////////////////////////////////////////////////////////////////////////
static int CAN_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    if (!instance[idx].rxProcess) {
        instance[idx].rxProcess = true;
        instance[idx].rxComplete = false;

        instance[idx].rxCnt = len;
        instance[idx].rxPtr = ptr;


        switch (instance[idx].type) {
        case emTYPE_Polling:
            CAN_PollingRcvPacket_Block(idx);
            instance[idx].rxComplete = true;
            break;
        case emTYPE_IT:
            CAN_Peli_ITConfig(CAN_IR_RI,ENABLE);
            break;
        default:
            break;
        }
    }

    if (instance[idx].block == emTYPE_Block){
        instance[idx].rxProcess = false;
    }
    else {
        if (instance[idx].rxComplete){
            instance[idx].rxProcess = false;
        }
        else if ((instance[idx].type == emTYPE_IT)) {
            return false;
        }
    }
    return instance[idx].txComplete;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Close CAN
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void CAN_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    COMMON_DisableIpClock(emCLOCK_CAN);
    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief  CAN CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified CAN.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CAN_CreateFile(HANDLE  handle)
{
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1)
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&CAN_OpenFile;
    handle->closeFile   = (int*)&CAN_CloseFile;
    handle->readFile    = (int*)&CAN_ReadFile;
    handle->writeFile   = (int*)&CAN_WriteFile;
#endif
}

/// @}

/// @}

/// @}
