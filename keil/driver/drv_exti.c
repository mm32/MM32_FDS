////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_EXTI.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE EXTI DRIVER LAYER FUNCTIONS.
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
#define _DRV_EXTI_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "hal_exti.h"
#include "hal_gpio.h"
#include "hal_nvic.h"
#include "hal_rcc.h"

#include "bsp.h"
#include "drv.h"
#include "common.h"
#include "drv_exti.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXTI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXTI_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void Set_GPIO_Clock(GPIO_TypeDef* GPIOx)
{
    if      (GPIOx == GPIOA)    COMMON_EnableIpClock(emCLOCK_GPIOA);
    else if (GPIOx == GPIOB)    COMMON_EnableIpClock(emCLOCK_GPIOB);
    else if (GPIOx == GPIOC)    COMMON_EnableIpClock(emCLOCK_GPIOC);
    else if (GPIOx == GPIOD)    COMMON_EnableIpClock(emCLOCK_GPIOD);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void DRV_EXTI_GPIO_Configure(GPIO_TypeDef*GPIOx, u32 line)
{
    GPIO_Mode_IN_Init(GPIOx, 1 << line,  GPIO_Mode_IPU, NO_REMAP, -1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTI interrupt general handler function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_EXTI_Common()
{
    u32 ev = EXTI->PR & (1 << instance[0].sPrefix.trueIdx |
                         1 << instance[1].sPrefix.trueIdx |
                         1 << instance[2].sPrefix.trueIdx |
                         1 << instance[3].sPrefix.trueIdx);
    EXTI_ClearFlag(ev);

    ((fpEXTI)instance[0].cb)(ev & 0xFFFF);
}

#if defined(__EX_EXTI)
////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTI IRQHandler function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM3N1)
void EXTI0_IRQHandler()     {   DRV_EXTI_Common();  }
void EXTI1_IRQHandler()     {   DRV_EXTI_Common();  }
void EXTI2_IRQHandler()     {   DRV_EXTI_Common();  }
void EXTI3_IRQHandler()     {   DRV_EXTI_Common();  }
void EXTI4_IRQHandler()     {   DRV_EXTI_Common();  }
void EXTI9_5_IRQHandler()   {   DRV_EXTI_Common();  }
void EXTI15_10_IRQHandler() {   DRV_EXTI_Common();  }
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
void EXTI0_1_IRQHandler()   {   DRV_EXTI_Common();  }
void EXTI2_3_IRQHandler()   {   DRV_EXTI_Common();  }
void EXTI4_15_IRQHandler()  {   DRV_EXTI_Common();  }
#endif

#endif      /* __EX_EXTI */


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is used to initialize the NVIC of EXTI.
/// @param  line: EXTI's external line.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DVR_EXTI_NVIC_Init(u32 line)
{
    u8 ch;
#if defined(__MM3N1) || defined(__MM3O1)
    if      (line > 9)  ch = EXTI15_10_IRQn;
    else if (line > 4)  ch = EXTI9_5_IRQn;
    else                ch = line + EXTI0_IRQn;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    if      (line > 3)  ch = EXTI4_15_IRQn;
    else if (line > 1)  ch = EXTI2_3_IRQn;
    else                ch = EXTI0_1_IRQn;
#endif
    COMMON_NVIC_Configure(ch,  2,  2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of EXTI.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_EXTI_DCB* pDcb, u8 idx)
{
    instance[idx].sync  = pDcb->sync;
    instance[idx].cb    = pDcb->cb;
    instance[0].cb      = pDcb->cb;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of EXTI.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_EXTI_DCB* pDcb, u8 idx)
{
    GPIO_TypeDef* GPIOx = GPIO_BaseFromIndex(pDcb->port);

    Set_GPIO_Clock(GPIOx);

    DRV_EXTI_GPIO_Configure(GPIOx, pDcb->hSub);

    EXTI_LineConfig(pDcb->port, pDcb->hSub);

    COMMON_EXTI_Enable(1 << pDcb->hSub, (EXTIMode_TypeDef)pDcb->mode, (EXTITrigger_TypeDef)pDcb->edge);

    DVR_EXTI_NVIC_Init(pDcb->hSub);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function opens the EXTI instance according to the specified
///         parameters in the EXTI_DCB_TypeDef
/// @param  pDcb: pointer to a EXTI_HandleTypeDef structure that contains
///                the configuration information for the specified EXTI module.
/// @retval  TRUE or FALSE
////////////////////////////////////////////////////////////////////////////////
static int EXTI_OpenFile(HANDLE handle, tAPP_EXTI_DCB* pDcb)
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
/// @brief  This function closes the EXTI instance according to the ID
///          in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified EXTI.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EXTI_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTI CreateFile
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI_CreateFile(HANDLE handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }
    COMMON_EnableIpClock(emCLOCK_EXTI);

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&EXTI_OpenFile;
    handle->closeFile   = (int*)&EXTI_CloseFile;
    handle->readFile    = NULL;
    handle->writeFile   = NULL;
}


/// @}


/// @}

/// @}
