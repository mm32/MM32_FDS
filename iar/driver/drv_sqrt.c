////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_SQRT.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE SQRT DRIVER LAYER FUNCTIONS.
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
#define _DRV_SQRT_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_sqrt.h"
#include "hal_rcc.h"

#include "drv.h"
#include "common.h"
#include "drv_sqrt.h"



////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SQRT
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SQRT_Exported_Functions
/// @{

#if defined(__MM0P1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize SQRT.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of SQRT.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_SQRT_DCB* pDcb, u8 idx)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of SQRT.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_SQRT_DCB* pDcb, u8 idx)
{
    COMMON_EnableIpClock(emCLOCK_SQRT);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Open SQRT according to the specified parameters in the
///  pDcb.
/// @param  pDcb: pointer to SQRT data control block which contains the
/// configuration information for SQRT.
/// @retval The operation result
/// This parameter can be 0 or a pointer to tSQRT_INSTANT structure
/// that contains part of configuration information.
////////////////////////////////////////////////////////////////////////////////
static int SQRT_OpenFile(HANDLE handle, tAPP_SQRT_DCB* pDcb)
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
/// @brief  Close SQRT
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SQRT_CloseFile(HANDLE handle)
{
    if (handle->idx == 0)       return;
    COMMON_DisableIpClock(emCLOCK_SQRT);

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Write data to SQRT and then get the processes data
/// @param  handle: pointer to HANDLE which contains the operation information
/// for the specified peripheral.
/// @param  ptr: pointer to input and output data
/// annote: input data and output data use the same memory address
/// @param  count: data length
/// @retval Process result
/// This parameter can be ERROR or SUCCESS
////////////////////////////////////////////////////////////////////////////////
static int SQRT_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;
    SET_SQRT(*(u32*)ptr);
    *(u32*)ptr = GET_SQRT();
    return 4;
}

#endif /* __MM0P1 */

////////////////////////////////////////////////////////////////////////////////
/// @brief  SQRT CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified SQRT.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SQRT_CreateFile(HANDLE handle)
{
#if defined(__MM0P1)
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&SQRT_OpenFile;
    handle->closeFile   = (int*)&SQRT_CloseFile;
    handle->readFile    = NULL;
    handle->writeFile   = (int*)&SQRT_WriteFile;
#endif
}



///@}

///@}

///@}

