////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_DIV.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE DIV DRIVER LAYER FUNCTIONS.
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
#define _DRV_DIV_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_div.h"
#include "hal_rcc.h"

#include "drv.h"
#include "common.h"
#include "drv_div.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DIV
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DIV_Exported_Functions
/// @{

#if defined(__MM0P1) || defined(__MM0Q1)

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of DIV.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_DIV_DCB* pDcb, u8 idx)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of DIV.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_DIV_DCB* pDcb, u8 idx)
{
    COMMON_EnableIpClock(emCLOCK_DIV);
    HWDivider_Init(pDcb->usign, pDcb->zero);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Open DIV according to the specified parameters in the
///  pDcb.
/// @param  pDcb: pointer to DIV data control block which contains the
/// configuration information for DIV.
/// @retval The operation result
/// This parameter can be 0 or a pointer to tDIV_INSTANT structure
/// that contains part of configuration information.
////////////////////////////////////////////////////////////////////////////////
static int DIV_OpenFile(HANDLE handle, tAPP_DIV_DCB* pDcb)
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
/// @brief  Close DIV
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void DIV_CloseFile(HANDLE handle)
{
    if (handle->idx == 0)       return;
    COMMON_DisableIpClock(emCLOCK_DIV);

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write data to DIV and then get the processes data
/// @param  handle: pointer to HANDLE which contains the operation information
/// for the specified peripheral.
/// @param  ptr: pointer to input and output data
/// annote: input data and output data use the same memory address
/// @param  count: data length
/// @retval Process result
/// This parameter can be ERROR or SUCCESS
////////////////////////////////////////////////////////////////////////////////
static int DIV_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;

    DIV->DVDR = *(u32*)(ptr + 0);
    DIV->DVSR = *(u32*)(ptr + 4);

    *(u32*)(ptr + 0) = DIV->QUOTR;
    *(u32*)(ptr + 4) = DIV->RMDR;

    return !DIV->SR;
}

#endif  /* __MM0P1 || __MM0Q1 */

////////////////////////////////////////////////////////////////////////////////
/// @brief  DIV CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified DIV.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DIV_CreateFile(HANDLE  handle)
{
#if defined(__MM0P1) || defined(__MM0Q1)
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&DIV_OpenFile;
    handle->closeFile   = (int*)&DIV_CloseFile;
    handle->readFile    = NULL;
    handle->writeFile   = (int*)&DIV_WriteFile;
#endif
}



///@}

///@}

///@}
