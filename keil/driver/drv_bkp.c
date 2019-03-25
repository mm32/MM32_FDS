////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_BKP.C
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE BKP DRIVER LAYER FUNCTIONS.
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
#define _DRV_BKP_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_rcc.h"
#include "hal_pwr.h"
#include "hal_bkp.h"

#include "drv_bkp.h"
#include "common.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BKP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BKP_Exported_Functions
/// @{

#if !defined(__MM0Q1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of BAP.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_BKP_DCB* pDcb, u8 idx)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of BKP.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_BKP_DCB* pDcb, u8 idx)
{
    exBKP_Init();
}
#endif

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Open BKP device, in the pDcb struct.
/// @note   This function can be used for all MM32 devices supported BKP periph.
/// @param  handle: pointer to a HANDLE structure that contains the instance for
///         the specified BKP.
/// @param  pDcb: pointer to a tAPP_BKP_DCB structure that contains the
///         configuration information for the specified TIM module.
/// @retval pointer to TIM instance.
////////////////////////////////////////////////////////////////////////////////
static int BKP_OpenFile(HANDLE handle, tAPP_BKP_DCB* pDcb)
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
/// @brief  Close BKP device
/// @note   This function can be used for all MM32 devices supported BKP periph.
/// @note   Deinitializes the struct instance to 0.
///         Deinitializes the struct handle to 0.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified TIM.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void BKP_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;
    PWR_BackupAccessCmd(DISABLE);
#if defined(__MM3N1) || defined(__MM3O1)
    COMMON_DisableIpClock(emCLOCK_BKP);
#endif
    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Operating BKP_ReadFile
/// @note   This function can be used for all MM32 devices supported BKP periph.
/// @param  handle: pointer to a HANDLE structure that contains the instance for
///         the specified BKP.
/// @param  hSub  : Sub function.
/// @param  ptr   : Pointer to data flow.
/// @param  len   : File length.
/// @retval int.
////////////////////////////////////////////////////////////////////////////////
static int BKP_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;

    len = MIN(BKP_NUMBER * 4 , len);

    for (u8 i = 0; i < len / 4; i++) {
        *(u32*)ptr = *(u32*)(*handle->instance + 4 * (i + 1));
        ptr += 4;
    }
    return len;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Operating BKP_WriteFile
/// @note   This function can be used for all MM32 devices supported BKP periph.
/// @param  handle: pointer to a HANDLE structure that contains the instance for
///         the specified BKP.
/// @param  hSub  : Sub function.
/// @param  ptr   : Pointer to data flow.
/// @param  len   : File length.
/// @retval int.
////////////////////////////////////////////////////////////////////////////////
static int BKP_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;

    len = MIN(BKP_NUMBER * 4 , len);

    for (u8 i = 0; i < len / 4; i++) {
        *(u32*)(*handle->instance + 4 * (i + 1)) = *(u32*)ptr;
        ptr += 4;
    }
    return len;
}

#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  BKP CreateFile
/// @note   This function can be used for all MM32 devices supported BKP periph.
/// @param  handle: pointer to a HANDLE structure that contains the instance for
///         the specified BKP.
/// @retval File HANDLE
////////////////////////////////////////////////////////////////////////////////
void BKP_CreateFile(HANDLE  handle)
{
#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1)
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&BKP_OpenFile;
    handle->closeFile   = (int*)&BKP_CloseFile;
    handle->readFile    = (int*)&BKP_ReadFile;
    handle->writeFile   = (int*)&BKP_WriteFile;
#else
    handle->openFile    = NULL;
    handle->closeFile   = NULL;
    handle->readFile    = NULL;
    handle->writeFile   = NULL;
#endif
}



/// @}

/// @}

/// @}

