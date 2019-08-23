////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_COMP.C
/// @author   PX Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES THE COMP DRIVER LAYER FUNCTIONS.
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
#define _DRV_COMP_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_rcc.h"
#include "hal_exti.h"
#include "hal_nvic.h"
#include "hal_adc.h"
#include "hal_comp.h"

#include "drv.h"
#include "common.h"
#include "bsp_comp.h"
#include "drv_comp.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup COMP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup COMP_Exported_Functions
/// @{

#if !defined(__MM3N1)


////////////////////////////////////////////////////////////////////////////////
/// @brief  Return the output level (high or low) of the selected comparator.
/// @param  COMP_Selection_COMPx: the selected comparator.
//#if defined(__MM0N1)
///         This parameter can be COMP_Selection_COMPx where x can be 1 to 2
///         to select the COMP peripheral.
//#endif
//#if defined(__MM0P1)
///         This parameter can be COMP_Selection_COMPx where x can be 1 to 5
///         to select the COMP peripheral.
//#endif
/// @retval  Returns the selected comparator output level: low or high.
////////////////////////////////////////////////////////////////////////////////
u8 DRV_COMP_CheckStatus(u32 comp)
{
    return (COMP_GetOutputLevel(comp)) ? 1 : 0;
}

#if defined(__EX_COMP)
////////////////////////////////////////////////////////////////////////////////
/// @brief  COMP IRQHandler function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1_COMP_IRQHandler(void)
{
    u8 idx;
    if (EXTI_GetITStatus(EXTI_Line19)) {
        EXTI_ClearFlag(EXTI_Line19);
        idx = instance[tbSubHandleIdx[0]].sPrefix.subIdx;
    }
    else if (EXTI_GetITStatus(EXTI_Line20)) {
        EXTI_ClearFlag(EXTI_Line20);
        idx = instance[tbSubHandleIdx[1]].sPrefix.subIdx;
    }
    if ((instance[idx].sync == emTYPE_Sync) && instance[idx].cb) {
        ((fpCOMP)instance[idx].cb)(idx, DRV_COMP_CheckStatus((u32)instance[idx].sPrefix.pBase));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  COMP IRQHandler function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void COMP_1_2_3_4_5_IRQHandler(void)
{
    u8 idx;
    if (EXTI_GetITStatus(EXTI_Line19)) {
        EXTI_ClearFlag(EXTI_Line19);
        idx = instance[tbSubHandleIdx[0]].sPrefix.subIdx;
    }
#if defined(__MM0N1) || defined(__MM0P1)
    else if (EXTI_GetITStatus(EXTI_Line20)) {
        EXTI_ClearFlag(EXTI_Line20);
        idx = instance[tbSubHandleIdx[1]].sPrefix.subIdx;
    }
#endif
#if defined(__MM0P1)
    else if (EXTI_GetITStatus(EXTI_Line21)) {
        EXTI_ClearFlag(EXTI_Line21);
        idx = instance[tbSubHandleIdx[2]].sPrefix.subIdx;
    }
    else if (EXTI_GetITStatus(EXTI_Line22)) {
        EXTI_ClearFlag(EXTI_Line22);
        idx = instance[tbSubHandleIdx[3]].sPrefix.subIdx;
    }
    else if (EXTI_GetITStatus(EXTI_Line23)) {
        EXTI_ClearFlag(EXTI_Line23);
        idx = instance[tbSubHandleIdx[4]].sPrefix.subIdx;
    }
#endif
    if ((instance[idx].sync == emTYPE_Sync) && instance[idx].cb) {
        ((fpCOMP)instance[idx].cb)(idx, DRV_COMP_CheckStatus((u32)instance[idx].sPrefix.pBase));
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable internal reference voltage.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_COMP_ADC_VreFint(void)
{
#if defined(__MM3N1)
    COMMON_EnableIpClock(emCLOCK_ADC2);
    ADC_RegularChannelConfig(ADC2, ADC_Channel_VoltReference, 0, ADC_Samctl_1_5);
#else
    COMMON_EnableIpClock(emCLOCK_ADC1);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_VoltReference, 0, ADC_Samctl_1_5);
#endif
}

#if defined(__MM0P1) || defined(__MM0Q1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the CRV register.
/// @param  CRV_Selection: the selected external reference voltage.
/// @param  CRV_Src: the selected external reference voltage source.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_COMP_CRV_Init(u32 crv, u32 src)
{
    exCOMP_SwitchCrv(crv);
    exCOMP_CrvSrc(src);
    exCOMP_CrvCmd(ENABLE);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the comparator interrupt.
/// @param  comp: comp no.
/// @param  type: Operter type.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_COMP_NVIC_Init(u32 comp, EM_TYPE type)
{
    if (type != emTYPE_IT) return;

    NVIC_InitTypeDef NVIC_Initstruct;

#if defined(__MM0Q1) || defined(__MM0N1)
    NVIC_Initstruct.NVIC_IRQChannel = ADC_COMP_IRQn;
#endif

#if defined(__MM0P1)
    NVIC_Initstruct.NVIC_IRQChannel = COMP1_2_3_4_5_IRQn;
#endif

    NVIC_Initstruct.NVIC_IRQChannelCmd = ENABLE;
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    NVIC_Initstruct.NVIC_IRQChannelPriority = 1;
#endif
    NVIC_Init(&NVIC_Initstruct);

    u32 line;
    if      (comp == COMP1)  line = EXTI_Line19;
#if defined(__MM0N1) || defined(__MM0P1)
    else if (comp == COMP2)  line = EXTI_Line20;
#endif
#if defined(__MM0P1)
    else if (comp == COMP3)  line = EXTI_Line21;
    else if (comp == COMP4)  line = EXTI_Line22;
    else                     line = EXTI_Line23;
#endif
    COMMON_ExitItRisingFallingEnable(line);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of COMP.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_COMP_DCB* pDcb, u8 idx)
{
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    COMMON_EnableIpClock(emCLOCK_COMP);
#endif
    instance[idx].sync      = pDcb->sync;
    instance[idx].cb        = pDcb->cb;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of COMP.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_COMP_DCB* pDcb, u8 idx)
{
    u32 tbOutput[] = {  0,
                        COMP_CSR_OUT_TIM1_BRAKE,
                        COMP_CSR_OUT_TIM1_OCREFCLR,
                        COMP_CSR_OUT_TIM1_CAPTURE1,
                        COMP_CSR_OUT_TIM2_CAPTURE4,
                        COMP_CSR_OUT_TIM2_OCREFCLR,
                        COMP_CSR_OUT_TIM3_CAPTURE1,
                        COMP_CSR_OUT_TIM3_OCREFCLR
    };

    COMP_InitTypeDef InitStructure;

    u32 comp    = tbIpBase[tbSubHandleIdx[pDcb->hSub]];

    InitStructure.Invert        = pDcb->inm << COMP_CSR_INM_Pos;
    InitStructure.NonInvert = pDcb->inp << COMP_CSR_INP_Pos;

#if defined(__MM3N1) || defined(__MM0N1)
    InitStructure.Hysteresis    = COMP_CSR_HYST_9;
#endif
#if defined(__MM0P1)
    InitStructure.Hysteresis    = COMP_CSR_HYST_15;
#endif
    InitStructure.Output        = tbOutput[pDcb->out];
    InitStructure.OutputPol     = (pDcb->pol) ? emCOMP_Inverted : emCOMP_NonInverted;
    InitStructure.Mode      = pDcb->mode << COMP_CSR_MODE_Pos;
    InitStructure.BlankingSrce  = COMP_BlankingSrce_None;

#if defined(__MM0P1) || defined(__MM0Q1)
    InitStructure.OFLT      = COMP_CSR_OFLT_0;
#endif

    COMP_Init(comp, &InitStructure);

    COMP_Cmd(comp, ENABLE);

    BSP_COMP_GPIO_Configure(comp, pDcb->inp, pDcb->inm, pDcb->out);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the comparator.
/// @param  pDcb: Data control block.
/// @retval  Comparator configuration.
////////////////////////////////////////////////////////////////////////////////
static int COMP_OpenFile(HANDLE handle, tAPP_COMP_DCB* pDcb)
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
/// @brief  This function closes the COMP instance according to the ID
///          in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified COMP.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void COMP_CloseFile(HANDLE handle)
{
    if (handle->idx == 0)       return;

    COMP_DeInit((u32)instance[tbSubHandleIdx[handle->idx]].sPrefix.pBase);
    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  the result of COMP receives.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified UART.
/// @param  ptr: Pointer to data buffer.
/// @param  count: Amount of data to be received.
/// @retval  0 or 1.
////////////////////////////////////////////////////////////////////////////////
static int COMP_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    return DRV_COMP_CheckStatus((u32)instance[tbSubHandleIdx[handle->idx]].sPrefix.pBase);
}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief  COMP CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified COMP.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void COMP_CreateFile(HANDLE handle)
{
#if !defined(__MM3N1)
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&COMP_OpenFile;
    handle->closeFile   = (int*)&COMP_CloseFile;
    handle->readFile    = (int*)&COMP_ReadFile;
    handle->writeFile   = NULL;
#endif
}


/// @}


/// @}

/// @}
