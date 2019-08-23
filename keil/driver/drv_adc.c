////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_ADC.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES THE ADC DRIVER LAYER FUNCTIONS.
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
#define _DRV_ADC_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_nvic.h"
#include "hal_dma.h"
#include "hal_adc.h"
#include "hal_tim.h"

#include "bsp.h"
#include "bsp_adc.h"

#include "drv.h"
//#include "adc.h"
#include "common.h"
#include "drv_dma.h"
#include "drv_adc.h"
#include "drv_exti.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void Set_ADC_Clock(ADC_TypeDef* ADCx)
{
    if (ADCx == ADC1)   COMMON_EnableIpClock(emCLOCK_ADC1);
#if defined(__MM3N1) || defined(__MM0P1)
    else                COMMON_EnableIpClock(emCLOCK_ADC2);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval UART DMA IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_ADC_DMA_IRQ_TxIndex(ADC_TypeDef* ADCx)
{
    if      ((u32)ADCx == (u32)ADC1)    return DMA1_Channel1_IRQn;
#if defined(__MM3N1)
    else                                return DMA1_Channel2_IRQn;
#endif
#if defined(__MM0P1)
    else                                return DMA1_Channel4_5_IRQn;
#endif
#if !defined(__MM3N1) && !defined(__MM0P1)
    return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval UART IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_ADC_IRQ_Index(ADC_TypeDef* ADCx)
{
#if defined(__MM3N1)
    return ADC1_2_IRQn;
#endif
#if defined(__MM0N1) || defined(__MM0Q1)
    return  ADC_COMP_IRQn;
#endif
#if defined(__MM0P1)
    return (ADCx == ADC1) ? ADC1_IRQn : ADC2_IRQn;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Find the position of "1" from the LSB, Utility functon.
/// @param  dat: 32bit data.
/// @param  cun: Cycle times.
/// @retval position. if ret = -1, there is no "1"
////////////////////////////////////////////////////////////////////////////////
s8 FindBitOnPosi(u32 dat, u8 cnt)
{
    for (u8 i = 0; i < cnt; i++) {
        if (dat & 1 << i)   return i;
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Sum the number of 1 in a data, Utility functon.
/// @param  dat: 16bit data.
/// @retval The number of 1.
////////////////////////////////////////////////////////////////////////////////
u8 DataInOnBitsSum(u16 dat)
{
    u8 n = 0;
    for (u8 i = 0; i < 16; i++) {
        if (dat & 1 << i)   n++;
    }
    return n;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC Interrupt or DMA Enabling Function
/// @param  dcb: Initialize the parameters in the structure, including adc
///         number, interrupt, query, dma mode, and hardware trigger switch
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_ADC_IT_DMA_Enable(ADC_TypeDef* ADCx, EM_TYPE type, EM_ADC_MODE mode)
{
    if (type == emTYPE_IT) {
        DRV_ADC_NVIC_Init(ADCx);
        (mode == emADC_WindowComp) ? ADC_ITConfig(ADCx, ADC_IT_AWD, ENABLE) : ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);
    }
    else if (type == emTYPE_DMA) {
        if (ADCx == ADC1) {
            DRV_ADC_DMA_ConfigChannel(ADCx);
            DMA_Cmd(DMA1_ch1, ENABLE);
            DMA_ITConfig(DMA1_ch1,DMA_IT_TC,ENABLE);
            DRV_ADC_NVIC_DMA_Init(ADCx);
            ADC_DMACmd(ADCx, ENABLE);
        }
#if defined(__MM3N1) || defined(__MM0P1)
        if (ADCx == ADC2) {
            DRV_ADC_DMA_ConfigChannel(ADCx);
            DMA_Cmd(DMA1_ch2, ENABLE);
            DMA_ITConfig(DMA1_ch2,DMA_IT_TC,ENABLE);
            DRV_ADC_NVIC_DMA_Init(ADCx);
            ADC_DMACmd(ADCx, ENABLE);
        }
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC Configuration Function
/// @param  ADCx: ADC1 or ADC2
/// @param  pInitStruct: pointer to an ADC_InitTypeDef structure that contains
///         the configuration information for the specified ADC peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_ADC_Configure(ADC_TypeDef* ADCx, EM_ADC_MODE mode, EM_TRIGGER trig)
{
    u32 tbMode[]    = {ADC_CR_IMM, ADC_CR_SCAN, ADC_CR_CONTINUE, ADC_CR_CONTINUE};

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Resolution         = ADC_Resolution_12b;
    ADC_InitStructure.ADC_PRESCARE           = ADC_PCLK2_PRESCARE_8;
    ADC_InitStructure.ADC_Mode               = (ADCMODE_TypeDef)tbMode[mode];

    if (trig != emTRIGGER_Software) {
    #if defined(__MM3N1) || defined(__MM0N1)
        ADC_InitStructure.ADC_ExternalTrigConv   = trig << ADC_CR_TRGSEL_Pos;
    #endif
    #if defined(__MM0P1) || defined(__MM0Q1)
        ADC_InitStructure.ADC_ExternalTrigConv   =   (trig & 0x07)      << ADC_CR_TRGSEL_L_Pos      || // bit6-4
                                                    ((trig & 0x08) ? (1 << ADC_CR_TRGSEL_H_Pos) : 0)  || // bit18-17
                                                    ((trig & 0x10) ? (2 << ADC_CR_TRGSEL_H_Pos) : 0);
    #endif
    }
    else
        ADC_InitStructure.ADC_ExternalTrigConv = 0;

    ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;

    Set_ADC_Clock(ADCx);
    ADC_Init(ADCx, &ADC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC Configuration Function
/// @param  ADCx: ADC1 or ADC2
/// @param  chs:
/// @param  trig:
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_ADC_Function(ADC_TypeDef* ADCx, u32 chs, EM_TRIGGER trig, EM_ADC_MODE mode)
{
    if (mode == emADC_WindowComp) {
        ADC_AnalogWatchdogCmd(ADCx, ENABLE);
        ADC_AnalogWatchdogThresholdsConfig(ADCx, WINDOW_HI, WINDOW_LO);

        s8 idx = FindBitOnPosi(chs, 15);
        if (idx >= 0)
            ADC_AnalogWatchdogSingleChannelConfig(ADCx, (ADCCHANNEL_TypeDef)idx);
    }
    else
        ADC_ExternalTrigConvCmd(ADCx, (FunctionalState)(trig != emTRIGGER_Software));
}

#if defined(__EX_ADC)
////////////////////////////////////////////////////////////////////////////////
/// @brief  DMA1 channel1 IRQHandler function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel1_IRQHandler()
{
    u8 idx = instance[tbSubHandleIdx[0]].sPrefix.subIdx;

    DMA_ClearITPendingBit(DMA1_IT_TC1);


    if (instance[instance[tbSubHandleIdx[0]].sPrefix.subIdx].trig == emTRIGGER_Ext7)
        EXTI_ClearFlag(LEFT_SHIFT_BIT(11));

    u8* data;
    if ((instance[instance[tbSubHandleIdx[0]].sPrefix.subIdx].trig == emTRIGGER_Ext0) | (instance[instance[tbSubHandleIdx[0]].sPrefix.subIdx].trig == emTRIGGER_Ext6)) {
        DMA_Cmd(DMA1_ch1, DISABLE);
        data = (u8*)DMA1_ch1->CMAR;
        exDMA_SetMemoryAddress(DMA1_ch1, ((DMA1_ch1->CMAR == (u32)&temp1_0) ? (u32)&temp1_1 : (u32)&temp1_0));
        DMA_Cmd(DMA1_ch1, ENABLE);
    }

    if (instance[idx].sync == emTYPE_Sync && instance[idx].cb)
        ((fpADC)instance[idx].cb)(data, instance[idx].chNo * 4);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DMA1 channel2 IRQHandler function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel2_IRQHandler()
{
#if defined(__MM3N1) || defined(__MM0P1)
    u8 idx = instance[tbSubHandleIdx[1]].sPrefix.subIdx;
    DMA_ClearITPendingBit(DMA1_IT_TC2);


    if (instance[instance[tbSubHandleIdx[1]].sPrefix.subIdx].trig == emTRIGGER_Ext7)
        EXTI_ClearFlag(LEFT_SHIFT_BIT(15));

    u8* data;
    if ((instance[instance[tbSubHandleIdx[1]].sPrefix.subIdx].trig == emTRIGGER_Ext0) | (instance[instance[tbSubHandleIdx[1]].sPrefix.subIdx].trig == emTRIGGER_Ext6)) {
        DMA_Cmd(DMA1_ch2, DISABLE);
        data = (u8*)DMA1_ch2->CMAR;
        exDMA_SetMemoryAddress(DMA1_ch2, ((DMA1_ch2->CMAR == (u32)&temp2_0) ? (u32)&temp2_1 : (u32)&temp2_0));
        DMA_Cmd(DMA1_ch2, ENABLE);
    }

    if (instance[idx].sync == emTYPE_Sync && instance[idx].cb)
        ((fpADC)instance[idx].cb)(data, instance[idx].chNo * 4);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC1 IRQHandler Common function
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1_CommonIt(void)
{
    u32 value = ADC_GetConversionValue(ADC1) & 0x1FFF;
    u8 idx = instance[tbSubHandleIdx[0]].sPrefix.subIdx;

    if (ADC_GetITStatus(ADC1,  ADC_IT_AWD)) {
        ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
        if (instance[idx].cb)
            ((fpADC)instance[idx].cb)((u8*)&value, 4);
    }
    else {
// Clear ADC Interrupt Flag
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

// Clear EXTI Flag
        if (instance[idx].trig == emTRIGGER_Ext7)
            EXTI_ClearFlag(LEFT_SHIFT_BIT(11));

// Callback
        if (instance[idx].cb)
            ((fpADC)instance[idx].cb)((u8*)&value, 4);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC2 IRQHandler Common function
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC2_CommonIt(void)
{
    u32 value = ADC_GetConversionValue(ADC1);
    u8 idx = instance[tbSubHandleIdx[0]].sPrefix.subIdx;;

// Clear ADC Interrupt Flag
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

// Clear EXTI Flag
    if (instance[idx].trig == emTRIGGER_Ext7)
        EXTI_ClearFlag(LEFT_SHIFT_BIT(15));

// Callback
    if (instance[idx].cb)
        ((fpADC)instance[idx].cb)((u8*)&value, 4);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC1 or(and )ADC2 IRQHandler Routing
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM3N1)
void ADC1_2_IRQHandler()
{
    (ADC_GetITStatus(ADC1,  ADC_IT_EOC)) ?  ADC1_CommonIt() : ADC2_CommonIt();
}
#endif

#if defined(__MM0N1) || defined(__MM0Q1)
void ADC1_COMP_IRQHandler()
{
    ADC1_CommonIt();
}
#endif

#if defined(__MM0P1)
void ADC1_IRQHandler()
{
    ADC1_CommonIt();
}
void ADC2_IRQHandler()
{
    ADC2_CommonIt();
}
#endif

#endif  /* __EX_ADC  */

////////////////////////////////////////////////////////////////////////////////
/// @brief  DMA initialization
/// @param  ADCx: ADC1 or ADC2
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_ADC_DMA_ConfigChannel(ADC_TypeDef* ADCx)
{
    tDRV_DMA_DCB dma;
    dma.base                = ADCx;

    dma.MemoryBaseAddr      = (ADCx == ADC1) ? (u32)&adc1Value : (u32)&adc1Value;
    dma.BufferSize          = 4;//DataInOnBitsSum(ADCx->CHSR);
    dma.PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;
    dma.MemoryDataSize      = DMA_MemoryDataSize_Word;

//  dma.MemoryBaseAddr      = (u32)&temp0;
//  dma.BufferSize          = 128;
//  dma.PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;
//  dma.MemoryDataSize      = DMA_MemoryDataSize_HalfWord;

    dma.Mode                = DMA_Mode_Circular;
    dma.MemoryInc           = DMA_MemoryInc_Enable;
    dma.PeripheralBaseAddr  = (u32)&ADCx->DR;
    dma.PeripheralInc       = DMA_PeripheralInc_Disable;
    dma.Priority            = DMA_Priority_High;
    dma.dmaDir              = emDMA_p2m;
    dma.channel             = ((u32)ADCx == (u32)ADC1) ?  DMA1_ch1 : DMA1_ch2;

    DRV_DMA_Init(&dma);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable the selected Multi ADC channels and configure its sample time.
/// @param  ADCx: where n can be 1, 2 to select the ADC peripheral.
/// @param  channels: the Multi ADC channel to configure.
/// @param  sampleTime: the ADC Channel n Sample time to configure.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_ADC_RegularChannelConfig(ADC_TypeDef* ADCx, u32 chs, ADCSAM_TypeDef samp)
{
    ADCx->CFGR &= ~ADC_CFGR_SAMCTL;
    ADCx->CFGR |=  samp;

    ADCx->CHSR &= ~chs;
    ADCx->CHSR |=  chs;

    exADC_TempSensorVrefintCmd(chs, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Dma interrupt initialization
/// @param  ADCx: ADC1 or ADC2
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_ADC_NVIC_DMA_Init(ADC_TypeDef* ADCx)
{
    if (ADCx == ADC1) {
        COMMON_NVIC_Configure(Get_ADC_DMA_IRQ_TxIndex(ADCx),  0,  3);
    }
#if defined(__MM3N1) || defined(__MM0P1)
    if (ADCx == ADC2) {
        COMMON_NVIC_Configure(Get_ADC_DMA_IRQ_TxIndex(ADCx),  0,  2);
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Dma interrupt initialization
/// @param  ADCx: ADC1 or ADC2
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_ADC_NVIC_Init(ADC_TypeDef* ADCx)
{
    COMMON_NVIC_Configure(Get_ADC_IRQ_Index(ADCx),  0,  3);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the adc conversion completion standard
/// @param  None.
/// @retval Conversion complete returns true
////////////////////////////////////////////////////////////////////////////////
bool DRV_ADC_PollingStatus(u8 idx)
{
    ADC_TypeDef* ADCx  = (ADC_TypeDef*)instance[idx].sPrefix.pBase;

    if ((instance[idx].sPrefix.trueIdx == 0) && (ADC_GetFlagStatus(ADCx, ADC_IT_EOC) != RESET)) {
        ADC_ClearFlag(ADCx, ADC_IT_EOC);
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the dma Transfer completion flag
/// @param  idx: instance index.
/// @retval Dma transfer complete returns true
////////////////////////////////////////////////////////////////////////////////
bool DRV_ADC_DMA_PollingStatus(u8 idx)
{
    if (instance[idx].sPrefix.trueIdx == 0) {
        if (DMA_GetFlagStatus(DMA1_IT_TC1)) {
            DMA_ClearFlag(DMA1_IT_TC1);
            return true;
        }
    }
    else if (DMA_GetFlagStatus(DMA1_IT_TC2)) {
        DMA_ClearFlag(DMA1_IT_TC2);
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of ADC.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_ADC_DCB* pDcb, u8 idx)
{
    memset(adc1Value, 0 , sizeof(adc1Value));
    memset(adc2Value, 0 , sizeof(adc2Value));

    instance[idx].mode  = pDcb->mode;
    instance[idx].cb    = pDcb->cb;
    instance[idx].type  = pDcb->type;
    instance[idx].sync  = pDcb->sync;
    instance[idx].trig  = pDcb->trig;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of ADC.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_ADC_DCB* pDcb, u8 idx)
{
    ADC_TypeDef* ADCx = (ADC_TypeDef*)instance[idx].sPrefix.pBase;

// Initia ADCx
    DRV_ADC_Configure(ADCx, pDcb->mode, pDcb->trig);

    DRV_ADC_Function(ADCx, pDcb->chs, pDcb->trig, pDcb->mode);

    pDcb->chs    |= (pDcb->temp) ? (1 << ADC_Channel_TempSensor)    : 0;
    pDcb->chs    |= (pDcb->vRef) ? (1 << ADC_Channel_VoltReference) : 0;
    DRV_ADC_RegularChannelConfig(ADCx, pDcb->chs, ADC_Samctl_13_5);

    DRV_ADC_IT_DMA_Enable(ADCx, pDcb->type, pDcb->mode);

    ADC_Cmd(ADCx, ENABLE);

    BSP_ADC_GPIO_Configure(ADCx, pDcb->chs);

    instance[idx].chNo  =  DataInOnBitsSum(ADCx->CHSR);

    if (instance[idx].mode == emADC_Continue)
        ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Turn on ADC ,ADC pin, mode, prescaler, channel, sample period,
///         trigger mode initialization
/// @param  handle: pointer to a HANDLE structure that contains
///                 the instance for the specified ADC.
/// @param  pDcb: The pointer point to Device Control Block(DCB).

/// @retval pointer to ADC instance.
////////////////////////////////////////////////////////////////////////////////
static int ADC_OpenFile(HANDLE handle, tAPP_ADC_DCB* pDcb)
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
/// @brief  This function closes the ADC instance according to the ID
///         in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified ADC.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void ADC_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;
    ADC_DeInit((ADC_TypeDef*)(*(handle->instance)));

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Start ADC conversion
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified ADC.
/// @retval Start conversion returns true
////////////////////////////////////////////////////////////////////////////////
static int ADC_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    if (instance[idx].mode == emADC_Continue)
        return 0;

    ADC_SoftwareStartConvCmd((ADC_TypeDef*)instance[idx].sPrefix.pBase, ENABLE);
    return len;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the value after ADC conversion
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified ADC.
/// @param  ptr: the address to Store the converted value
/// @retval Get converted value returns true
////////////////////////////////////////////////////////////////////////////////
static int ADC_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    u8 n = MIN(instance[idx].chNo * 4, len);

    if (instance[idx].mode == emADC_WindowComp)
        return 0;

    if ((instance[idx].mode == emADC_Continue) || (instance[idx].type == emTYPE_DMA)) {
        if (instance[idx].sync == emTYPE_ASync) {
            ADC_TypeDef* ADCx = (ADC_TypeDef*)instance[idx].sPrefix.pBase;
            if (ADCx == ADC1) {
                memcpy(ptr, adc1Value, n);
            }
            else {
                memcpy(ptr, adc2Value, n);
            }
        }
        else n = 0;
    }
    else if (instance[idx].mode == emADC_Imm) {
        if (instance[idx].type == emTYPE_Polling) {
            ADC_TypeDef* ADCx = (ADC_TypeDef*)instance[idx].sPrefix.pBase;
            ADC_SoftwareStartConvCmd((ADC_TypeDef*)instance[idx].sPrefix.pBase, ENABLE);

            while (!(ADCx->SR & ADC_SR_ADIF));  ADCx->SR |= ADC_SR_ADIF;
            *(u32*)ptr = ADC_GetConversionValue(ADC1);
        }
        else n = 0;
    }
    return n;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified ADC.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_CreateFile(HANDLE  handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&ADC_OpenFile;
    handle->closeFile   = (int*)&ADC_CloseFile;
    handle->readFile    = (int*)&ADC_ReadFile;
    handle->writeFile   = (int*)&ADC_WriteFile;
}


/// @}


/// @}

/// @}
