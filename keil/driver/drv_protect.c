////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_PROTECT.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES THE PROTECT DRIVER LAYER FUNCTIONS.
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
#define _DRV_PROTECT_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_flash.h"

#include "drv.h"
#include "drv_protect.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PROTECT
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PROTECT_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of PROTECT.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_PROTECT_DCB* pDcb, u8 idx)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of PROTECT.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_PROTECT_DCB* pDcb, u8 idx)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function opens the EEPROM instance according to the specified
///         parameters in the pDcb struct.
/// @note   This function can be used for all MM32 devices.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the "specified peripheral"----EEPROM.
/// @param  pDcb: pointer to a @ref tAPP_PROTECT_DCB structure that contains
///         the configuration information for PROTECT
/// @retval bool: handle->instance != NULL
////////////////////////////////////////////////////////////////////////////////
static int PROTECT_OpenFile(HANDLE handle, tAPP_PROTECT_DCB* pDcb)
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
/// @brief  This function closes the PROTECT instance according to the handle of
///         "peripheral"----PROTECT.
/// @note   This function can be used for all MM32 devices.
/// @note   Deinitializes the specified struct instance accroding to
///         handle->idx to 0xFF.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the "specified peripheral"----PROTECT.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void PROTECT_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}



////////////////////////////////////////////////////////////////////////////////
/// @brief  read the data in protect area
/// @note   This function can be used for all MM32 devices.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the "specified peripheral"----PROTECT.
/// @param  hSub: the virtual index of instance[hSub];
/// @param  ptr: the buffer to store the data in protect area
/// @param  len: the length of data that the user wants to get
///// note   The actual length is the minimum of len and the EEPROM blockSize
/// @retval int: false: the instance does not exist
///         true: Read data operation succeeded
////////////////////////////////////////////////////////////////////////////////
static int PROTECT_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    memcpy(ptr, (u8*)(instance[idx].sPrefix.pBase), 40);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  make flash to be protected
/// @note   This function can be used for all MM32 devices.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the "specified peripheral"----PROTECT.
/// @param  hSub: the virtual index of instance[hSub];
/// @param  ptr: file buffer point
/// @note   In practical applications, it does not work and can be NULL
/// @param  len: file length
/// @note   In practical applications, it does not work
/// @retval int: *ptr
//retval = 0: the operation of flash protect failed
//retval = 1: the operation of flash protect succeeded
////////////////////////////////////////////////////////////////////////////////
static int PROTECT_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    FLASH_Unlock();

//  FLASH->OPTKEYR = 0x45670123;
//  FLASH->OPTKEYR = 0xCDEF89AB;

    u32 addr = (u32)(instance[0].sPrefix.pBase) + 0;
    u16 dat  = 0x7F80;
    FLASH_ProgramProtect(addr, dat);

    addr = (u32)(instance[0].sPrefix.pBase) + 2;
    dat  = 0xFF00;
    FLASH_ProgramProtect(addr, dat);

//  *ptr =  (FLASH_ProgramProtect((u32)(instance[0].tbIpBase) + 0, 0x7F80) == FLASH_COMPLETE) &&
//          (FLASH_ProgramProtect((u32)(instance[0].tbIpBase) + 2, 0xFF00) == FLASH_COMPLETE);
//  FLASH->CR |= FLASH_CR_LOCK;

    FLASH_Lock();

    return *ptr;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  FLASH CreateFile
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the "specified peripheral"----PROTECT.
/// @retval none
////////////////////////////////////////////////////////////////////////////////
void PROTECT_CreateFile(HANDLE handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&PROTECT_OpenFile;
    handle->closeFile   = (int*)&PROTECT_CloseFile;
    handle->readFile    = (int*)&PROTECT_ReadFile;
    handle->writeFile   = (int*)&PROTECT_WriteFile;
}



/// @}

/// @}

/// @}
