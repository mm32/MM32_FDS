////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_WWDG.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES THE WWDG DRIVER LAYER FUNCTIONS.
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
#define _DRV_WWDG_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_nvic.h"
#include "hal_rcc.h"
#include "hal_wwdg.h"

#include "drv.h"
#include "common.h"
#include "drv_wwdg.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup WWDG
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup WWDG_Exported_Functions
/// @{

#if defined(__EX_WWDG)
////////////////////////////////////////////////////////////////////////////////
/// @brief  WWDG IRQHandler function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM3N1) || defined(__MM0N1)
void WWDG_IRQHandler()
{
    WWDG_ClearFlag();
    if (instance[0].sync == emTYPE_Sync)
        ((fpWWDG)instance[0].sync)();
}
#endif

#if defined(__MM0P1) || defined(__MM0Q1)
void WWDG_WWDG_IRQHandler()
{
    WWDG_ClearFlag();
    if (instance[tbSubHandleIdx[0]].sync == emTYPE_Sync)
        ((fpWWDG)instance[tbSubHandleIdx[0]].sync)();
}
#endif
#endif  //  defined(__EX_WWDG)




////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the WWDG interrupt
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_WWDG_NVIC_Init()
{
    u8 ch;
#if defined(__MM3N1) || defined(__MM0N1)
    ch = WWDG_IRQn;
#endif

#if defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1)
    ch = WWDG_IWDG_IRQn;
#endif
    COMMON_NVIC_Configure(ch,  2,  2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of WWDG.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_WWDG_DCB* pDcb, u8 idx)
{
    instance[idx].sync      = pDcb->sync;                   ///< sync, async
    instance[idx].cb        = pDcb->cb;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of WWDG.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_WWDG_DCB* pDcb, u8 idx)
{
    COMMON_EnableIpClock(emCLOCK_WWDG);

    WWDG_DeInit();

    WWDG_SetPrescaler(pDcb->pres);

    DRV_WWDG_NVIC_Init();

    // Enable EWI(Early Weakup Interrupt: When counter is 0x40, before
    // decrementing to 0x3F)
    WWDG_EnableIT();

    WWDG_Enable(pDcb->cnt);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function opens the WWDG instance according to the specified
///         parameters in the WWDG_DCB_TypeDef
/// @param  pDcb: pointer to a WWDG_HandleTypeDef structure that contains
///                the configuration information for the specified WWDG module.
/// @retval  TRUE or FALSE
////////////////////////////////////////////////////////////////////////////////
static int WWDG_OpenFile(HANDLE handle, tAPP_WWDG_DCB* pDcb)
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
/// @brief  This function closes the WWDG instance according to the ID
///          in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified WWDG.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void WWDG_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;
    WWDG_DeInit();

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  the WWDG read data according to the ID in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified WWDG.
/// @param  ptr: Pointer to data buffer.
/// @param  count: Amount of data to be read.
/// @retval  sent data number.
////////////////////////////////////////////////////////////////////////////////
static int WWDG_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;
    *ptr = (u8)WWDG_GetCounter();
    return WWDG_GetCounter();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write data to WWDG and then get the processes data
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral.
/// @param  ptr: pointer to input and output data
///         annote: input data and output data use the same memory address
/// @param  count: data length
/// @retval Process result
///         This parameter can be ERROR or SUCCESS
////////////////////////////////////////////////////////////////////////////////
static int WWDG_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;
    WWDG_SetCounter(*ptr);
    return WWDG_GetCounter();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  WWDG CreateFile
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void WWDG_CreateFile(HANDLE handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&WWDG_OpenFile;
    handle->closeFile   = (int*)&WWDG_CloseFile;
    handle->readFile    = (int*)&WWDG_ReadFile;
    handle->writeFile   = (int*)&WWDG_WriteFile;
}


/// @}

/// @}

/// @}
