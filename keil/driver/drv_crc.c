////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_CRC.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE CRC DRIVER LAYER FUNCTIONS.
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
#define _DRV_CRC_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "drv.h"
#include "hal_rcc.h"
#include "hal_crc.h"

#include "drv_crc.h"
#include "common.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CRC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CRC_Exported_Functions
/// @{

#if defined(__MM3N1)  || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize CRC
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_CRC_Init()
{
    COMMON_EnableIpClock(emCLOCK_CRC);
    CRC_ResetDR();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of CRC.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_CRC_DCB* pDcb, u8 idx)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of CRC.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_CRC_DCB* pDcb, u8 idx)
{
    DRV_CRC_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Turn on CRC
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified CRC.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @retval  pointer to CRC instance.
////////////////////////////////////////////////////////////////////////////////
static int CRC_OpenFile(HANDLE handle, tAPP_CRC_DCB* pDcb)
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
/// @brief  Close crc
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified CRC.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void CRC_CloseFile(HANDLE handle)
{
    if (handle->idx == 0)       return;
    CRC_ResetDR();
    COMMON_DisableIpClock(emCLOCK_CRC);

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the value after CRC
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified CRC.
/// @retval Got the CRC value then returns true
////////////////////////////////////////////////////////////////////////////////
static int CRC_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    *(u32*)ptr = CRC_GetCRC();
    return len;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write the value to the crc register for crc
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified CRC.
/// @param  ptr: the address to Store the converted value
/// @param  count: CRC count
/// @retval CRC calculation successfully returns true
////////////////////////////////////////////////////////////////////////////////
static int CRC_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    CRC_ResetDR();

    for (u16 i = 0; i < (len / 4); i++) {
        CRC_CalcCRC(*(u32*)ptr);
        ptr += 4;
    }
    return len;
}

#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  CRC CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified CRC.
/// @retval File HANDLE
////////////////////////////////////////////////////////////////////////////////
void CRC_CreateFile(HANDLE  handle)
{
#if defined(__MM3N1)  || defined(__MM3O1) || defined(__MM0P1) || defined(__MM0Q1)
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&CRC_OpenFile;
    handle->closeFile   = (int*)&CRC_CloseFile;
    handle->readFile    = (int*)&CRC_ReadFile;
    handle->writeFile   = (int*)&CRC_WriteFile;
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

