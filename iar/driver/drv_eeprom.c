////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_EEPROM.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE EEPROM DRIVER LAYER FUNCTIONS.
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
#define _DRV_EEPROM_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "types.h"
#include "hal_flash.h"
#include "drv.h"
#include "drv_eeprom.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EEPROM
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EEPROM_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize eeprom.
/// @note   This function can be used for all MM32 devices.
/// @note   Erases the pages for EEPROM or continue to operate EEPROM
/// @param  idx: Decide which instance that express EEPROM is initialized
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_EEPROM_Init(u8 idx)
{
    if (instance[idx].erase) {
        exFLASH_EraseEE((u32)instance[idx].sPrefix.pBase + 0x0000);
        exFLASH_EraseEE((u32)instance[idx].sPrefix.pBase + 0x0400);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of EEPROM.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_EEPROM_DCB* pDcb, u8 idx)
{
    instance[idx].size  = pDcb->size;
    instance[idx].erase = pDcb->erase;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of EEPROM.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_EEPROM_DCB* pDcb, u8 idx)
{
    DRV_EEPROM_Init(idx);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function opens the EEPROM instance according to the specified
///         parameters in the pDcb struct.
/// @note   This function can be used for all MM32 devices.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the "specified peripheral"----EEPROM.
/// @param  pDcb: pointer to a @ref tAPP_EEPROM_DCB structure that contains
///         the configuration information for EEPROM
/// @retval bool: handle->instance != NULL
////////////////////////////////////////////////////////////////////////////////
static int EEPROM_OpenFile(HANDLE handle, tAPP_EEPROM_DCB* pDcb)
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
/// @brief  This function closes the EEPROM instance according to the handle of
///         peripheral----EEPROM.
/// @note   This function can be used for all MM32 devices.
/// @note   Deinitializes the specified struct instance accroding to
///         handle->idx to 0xFF.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral----EEPROM.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EEPROM_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  read the last set of data that was written in the specified EEPROM.
/// @note   This function can be used for all MM32 devices.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral----EEPROM.
/// @param  hSub: the virtual index of instance[hSub];
/// @param  ptr: the buffer to store the last set of data in EEPROM
/// @param  len: the length of data that the user wants to get
/// @note   The actual length is the minimum of len and the EEPROM blockSize
/// @retval int: -1: the EEPROM is empty
///         0: the instance does not exist
///         1: Read data operation succeeded
////////////////////////////////////////////////////////////////////////////////
static int EEPROM_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    len = MIN(len, instance[idx].size);

    u8* pAddr   = exFLASH_Locate((u32)(instance[idx].sPrefix.pBase), len);

    if (pAddr == 0) return  -1;

    pAddr -= instance[idx].size;

    memcpy(ptr, pAddr, instance[idx].size);

    return instance[idx].size;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  write the set of data in the specified EEPROM.
/// @note   This function can be used for all MM32 devices.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the "specified peripheral"----EEPROM.
/// @param  hSub: the virtual index of instance[hSub];
/// @param  ptr: the buffer that store the data to be written in EEPROM
/// @note   the data cannot be all 0xFF
/// @param  len: the length of ptr
/// @note   The actual length is the minimum of len and the EEPROM blockSize
/// @retval int: false: the EEPROM_Instance does not exist
///         true: Write data operation succeeded
////////////////////////////////////////////////////////////////////////////////
static int EEPROM_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    len = MIN(len, instance[idx].size);
    exFLASH_WriteEE((u16*)ptr, (u32)(instance[idx].sPrefix.pBase), len);
    return len;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  FLASH CreateFile
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral----EEPROM.
/// @retval none
////////////////////////////////////////////////////////////////////////////////
void EEPROM_CreateFile(HANDLE handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&EEPROM_OpenFile;
    handle->closeFile   = (int*)&EEPROM_CloseFile;
    handle->readFile    = (int*)&EEPROM_ReadFile;
    handle->writeFile   = (int*)&EEPROM_WriteFile;
}



/// @}

/// @}

/// @}
