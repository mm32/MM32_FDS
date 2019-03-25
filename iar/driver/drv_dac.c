////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_DAC.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE DAC DRIVER LAYER FUNCTIONS.
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
#define _DRV_DAC_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "hal_rcc.h"
#include "hal_dac.h"
#include "hal_tim.h"

#include "bsp_dac.h"

#include "drv.h"
#include "common.h"
#include "drv_dac.h"
#include "drv_dma.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DAC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DAC_Exported_Functions
/// @{



#if defined(__MM3N1)
////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize the GPIO of the DAC
/// @param idx: DAC channel
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DAC_GPIO_Configure(u8 idx)
{
    BSP_DAC_GPIO_Configure(idx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize the DAC according to the specified parameters in the
///        tDAC_INSTANT.
/// @param dcb: Pointer to a tDAC_INSTANT structure that contains the
///             configuration information for the DAC.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DAC_Init(tDRV_DAC_INSTANT* dcb)
{
    COMMON_EnableIpClock(emCLOCK_DAC);

    DAC_InitTypeDef DAC_InitStructure;

    DAC_InitStructure.DAC_Trigger                       = (dcb->ext) ? DAC_Trigger_T2_TRIG : DAC_Trigger_Software;
    DAC_InitStructure.DAC_WaveGeneration                = dcb->wave;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude  = DAC_TriangleAmplitude_2047;
    DAC_InitStructure.DAC_OutputBuffer                  = DAC_OutputBuffer_Disable;

    if (dcb->sPrefix.subIdx == 0) {
        DAC_Init(DAC_Channel_1, &DAC_InitStructure);
        DAC_Cmd(DAC_Channel_1, ENABLE);
        if (dcb->type == emTYPE_DMA)
            DAC_DMACmd(DAC_Channel_1, ENABLE);
    }
    else if (dcb->sPrefix.subIdx == 1) {
        DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1023;
        DAC_Init(DAC_Channel_2, &DAC_InitStructure);
        DAC_Cmd(DAC_Channel_2, ENABLE);
        if (dcb->type == emTYPE_DMA)
            DAC_DMACmd(DAC_Channel_2, ENABLE);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize the TIM
/// @param psc: Prescaler
/// @param period: Reloading value
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DAC_TIMInit(u16 psc, u16 period)
{
    COMMON_EnableIpClock(emCLOCK_TIM2);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
    TIM_TimeBaseStruct.TIM_Period        = period;
    TIM_TimeBaseStruct.TIM_Prescaler     = psc;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
    TIM_SelectOutputTrigger(TIM2, TIM_TRIGSource_Update);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize the DMA channel of DAC
/// @param idx: DAC channel
/// @param add: Memory address
/// @param period: DMA buffer size
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DAC_DMA_Init(u8 idx, u16* add, u16 buffsize)
{
    tDRV_DMA_DCB dma;
    dma.base   = DAC;

    dma.BufferSize         = buffsize;
    dma.PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma.MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    dma.Mode               = DMA_Mode_Circular;
    dma.Priority           = DMA_Priority_High;
    dma.MemoryInc          = DMA_MemoryInc_Enable;
    dma.PeripheralInc      = DMA_PeripheralInc_Disable;
    dma.dmaDir                 = emDMA_m2p;
    dma.MemoryBaseAddr     = (u32)add;

    if (idx == 0) {
        dma.PeripheralBaseAddr = (u32)&DAC->DHR12R1;
        dma.channel = DMA1_ch6;
    }
    else if (idx == 1) {
        dma.PeripheralBaseAddr = (u32)&DAC->DHR12R2;
        dma.channel = DMA1_ch7;
    }
    DRV_DMA_Init(&dma);
    DMA_Cmd(dma.channel, ENABLE);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief Hardware triggered DAC
/// @param : None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DAC_StartHW()
{
    TIM_Cmd(TIM2, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Software triggered DAC
/// @param: None
/// @retval: None
////////////////////////////////////////////////////////////////////////////////
void DRV_DAC_StartSW(u8 idx)
{
    (idx == 0) ? DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE) :
                 DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of DAC.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_DAC_DCB* pDcb, u8 idx)
{
    instance[idx].type  = pDcb->type;
    instance[idx].ext   = pDcb->trig;

    u32 tbWave[] = {    DAC_WaveGeneration_Noise,
                        DAC_WaveGeneration_Triangle,
                        DAC_WaveGeneration_None
                    };
    instance[idx].wave = (emDACWAVE_TypeDef)tbWave[pDcb->wave];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of DAC.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_DAC_DCB* pDcb, u8 idx)
{
    DRV_DAC_GPIO_Configure(pDcb->hSub);

    if (pDcb->trig)
        DRV_DAC_TIMInit(7, 2000);

    DRV_DAC_Init(&instance[idx]);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief This function opens the DAC instance according to the specified
///        parameters in the DAC_DCB_TypeDef.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified DAC.
/// @param pDcb: pointer to a DAC_DCB_TypeDef structure that contains
///              the configuration information for the specified DAC module.
/// @retval: pointer to DAC instance.
////////////////////////////////////////////////////////////////////////////////
static int DAC_OpenFile(HANDLE handle, tAPP_DAC_DCB* pDcb)
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
/// @brief Close the DAC
/// @param handle: pointer to a HANDLE structure that contains
///                the instance for the specified DAC.
/// @retval: None
////////////////////////////////////////////////////////////////////////////////
static void DAC_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;
    DAC_DeInit();
    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Read the value of the DAC output
/// @param handle: pointer to a HANDLE structure that contains
///               the instance for the specified DAC.
/// @param ptr: Memory address
/// @retval: None
////////////////////////////////////////////////////////////////////////////////
static int DAC_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    if (instance[idx].sPrefix.trueIdx == 0)
        *(u16*)ptr = DAC_GetDataOutputValue(DAC_Channel_1);
    else
        *(u16*)ptr = DAC_GetDataOutputValue(DAC_Channel_2);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Write the value of the output to be written to the dac register
/// @param handle: pointer to a HANDLE structure that contains
///               the instance for the specified DAC.
/// @param ptr: Memory address
/// @param count: Number of conversions
/// @retval: None
////////////////////////////////////////////////////////////////////////////////
static int DAC_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    if (instance[idx].type == emTYPE_DMA) {
        DRV_DAC_DMA_Init(instance[idx].sPrefix.trueIdx, (u16*)ptr, len);
        if (instance[idx].ext == true)
            DRV_DAC_StartHW();
        else {
            for (u16 i = 0; i < len; i++)
                DRV_DAC_StartSW(instance[idx].sPrefix.trueIdx);
        }
    }
    else {
        if (instance[idx].ext == true)
            DRV_DAC_StartHW();
        else {
            for (u16 i = 0; i < len; i++) {
                if (instance[idx].sPrefix.trueIdx  == 0)
                    DAC_SetChannel1Data(DAC_Align_12b_R, *((u16*)ptr + i));
                else
                    DAC_SetChannel2Data(DAC_Align_12b_R, *((u16*)ptr + i));
                DRV_DAC_StartSW(instance[idx].sPrefix.trueIdx);
            }
        }
    }
    return len;
}

#endif /* __MM3N1 */



////////////////////////////////////////////////////////////////////////////////
/// @brief  DAC CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified DAC.
/// @retval File HANDLE
////////////////////////////////////////////////////////////////////////////////
void DAC_CreateFile(HANDLE  handle)
{
#if defined(__MM3N1)
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->size        = sizeof(tbIpBase);
    handle->idx         = -1;
    handle->openFile    = (int*)&DAC_OpenFile;
    handle->closeFile   = (int*)&DAC_CloseFile;
    handle->readFile    = (int*)&DAC_ReadFile;
    handle->writeFile   = (int*)&DAC_WriteFile;
#endif
}




/// @}


/// @}

/// @}
