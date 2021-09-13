////////////////////////////////////////////////////////////////////////////////
/// @file     DRV.C
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES THE DRIVER LAYER FUNCTIONS.
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
#define _DRV_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include <stdlib.h>

#include "system_mm32.h"

#include "hal_gpio.h"
#include "drv.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DRV
/// @brief DRV driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DRV_Exported_Library_Function
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  System tick timer for counting.
/// @param  None.
/// @retval tickCnt
////////////////////////////////////////////////////////////////////////////////
s32 DRV_GetTick(void)
{
    return tickCnt;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Input 32-bit address.
/// @param  addr : Read the address.
/// @retval  Return the address content.
////////////////////////////////////////////////////////////////////////////////
u32 inp(u32* addr)
{
    return (*addr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Input 16-bit address.
/// @param  addr : Read the address.
/// @retval Return the address content.
////////////////////////////////////////////////////////////////////////////////
u16 inp16(u16* addr)
{
    return (*addr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Input 8-bit address.
/// @param  addr : Read the address.
/// @retval Return the address content.
////////////////////////////////////////////////////////////////////////////////
u8 inp8(u8* addr)
{
    return (*addr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Output 32-bit address.
/// @param  addr: The address to write in.
/// @param  dat: The data to be written.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void outp(u32* addr, u32 dat)
{
    *addr = dat;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Output 16-bit address.
/// @param  addr: The address to write in.
/// @param  dat: The data to be written.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void outp16(u16* addr, u16 dat)
{
    *addr = dat;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Output 8-bit address.
/// @param  addr: The address to write in.
/// @param  dat: The data to be written.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void outp8(u8* addr, u8 dat)
{
    *addr = dat;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  CloseFile Common Program
/// @param  pre : Struct tPREFIX to be programed.
/// @param  size : The size of struct tPREFIX.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_CloseFileCommon(tPREFIX* pre, u16 size)
{
    u8 sub = pre->subIdx;
    memset(pre, 0xFF, size);
    pre->subIdx = sub;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Sub Handle to true index, ReadFile/WriteFile Function Use
/// @param  sub : The ID of the sub handle.
/// @param  ptr : The pointer point to the instance.
/// @param  size : The size of one instance.
/// @param  num : The numbers of the instances.
/// @retval Instance Index
////////////////////////////////////////////////////////////////////////////////
s8 DRV_FindTrueIdx(s8 sub, u8* ptr, u8 size, u8 num)
{
    if (sub == -1) return -1;
    tPREFIX* p = (tPREFIX*)ptr;
    for (u8 i = 0; i < num; i++) {
        if (p->trueIdx == sub)
            return i;
        p = (tPREFIX*)((u32)p + size);
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV layer to get the instance true index.
/// @param  ptr : The pointer point to the instance.
/// @param  num : The numbers of the instances.
/// @param  size : The size of one instance.
/// @param  sub : The ID of sub index.
/// @retval Instance true index.
////////////////////////////////////////////////////////////////////////////////
s8 DRV_GetInstanceIndex(u8* ptr, u8 num, u16 size, s8 sub)
{
    if (sub == -1)  return -1;

    tPREFIX* p = (tPREFIX*)ptr;
    for (u8 i = 0; i < num; i++) {
        if (p->trueIdx == sub)
            return i;
        p = (tPREFIX*)((u32)p + size);
    }

    p = (tPREFIX*)ptr;
    for (u8 i = 0; i < num; i++) {
        if (p->trueIdx == -1)
            return i;
        p = (tPREFIX*)((u32)p + size);
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV layer to open the file entry.
/// @param  itemBase : The pointer point to IP base table.
/// @param  itemAbsIdxIP : The ID of used IP table.
/// @param  sub : Enum, The ID of sub index.
/// @param  ptr : The pointer point to the instance.
/// @param  size : instance size.
/// @param  num : The numbers of the instances.
/// @retval Instance true index (Array subscript).
////////////////////////////////////////////////////////////////////////////////
s8 DRV_OpenFileEntry(u32* itemBase, u8* itemAbsIdxIP,  s8 sub, u8* ptr, u16 size, u16 num)
{
    s8 idx = DRV_GetInstanceIndex(ptr, num , size, sub);
    if (idx == -1)  return -1;

    tPREFIX* p;
    p = (tPREFIX*)(ptr + idx * size);
    p->pBase = (u32*)itemBase[itemAbsIdxIP[sub]];
    p->trueIdx  = sub;
    p = (tPREFIX*)(ptr + (itemAbsIdxIP[sub]) * size);
    p->subIdx   = idx;
    return idx;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV layer to exit the file open function.
/// @param  handle : The pointer point to struct HANDLE.
/// @param  ptr : The pointer point to the instance.
/// @param  idx : The true instance ID.
/// @retval Instance true index.
////////////////////////////////////////////////////////////////////////////////
bool DRV_OpenFileExit(HANDLE handle, int* ptr, s8 idx)
{
    handle->instance = ptr;
    handle->idx      = idx;
    return handle->instance != NULL;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV layer to delete IP file.
/// @param  ip : The enum of the IP.
/// @retval The status of the function.
////////////////////////////////////////////////////////////////////////////////
bool DeleteFile(HANDLE ip)
{
    if (ip != NULL) {
        free(ip);
        if (ip != NULL) 
            return false;
        return true;
    }
    
    return false;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV layer to scatter open file functions.
/// @param  handle : The pointer point to struct HANDLE.
/// @param  pDcb : The pointer point to struct user dcb.
/// @retval The status of the function.
////////////////////////////////////////////////////////////////////////////////
int OpenFile(HANDLE handle, void* pDcb)
{
    if (handle->openFile == NULL) return -1;
    return ((fpOpenFile)handle->openFile)(handle, pDcb);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV layer to scatter close file functions.
/// @param  handle : The pointer point to struct HANDLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CloseFile(HANDLE handle)
{
    ((fpCloseFile)handle->closeFile)(handle);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV layer to scatter read file functions.
/// @param  handle : The pointer point to struct HANDLE.
/// @param  hSub : The number of the sub index.
/// @param  ptr : The pointer used in scatter functions.
/// @param  len : The variable used in scatter functions.
/// @retval The status of the function.
////////////////////////////////////////////////////////////////////////////////
int ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (handle->readFile == NULL) return -1;
    return ((fpReadFile)handle->readFile)(handle, hSub, ptr, len);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV layer to scatter write file functions.
/// @param  handle : The pointer point to struct HANDLE.
/// @param  hSub : The number of the sub index.
/// @param  ptr : The pointer used in scatter functions.
/// @param  len : The variable used in scatter functions.
/// @retval The status of the function.
////////////////////////////////////////////////////////////////////////////////
int WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (handle->writeFile == NULL) return -1;
    return ((fpWriteFile)handle->writeFile)(handle, hSub, ptr, len);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DRV layer to scatter create file functions.
/// @param  ip : The enum of the IP.
/// @retval  The pointer point to struct handle.
////////////////////////////////////////////////////////////////////////////////
HANDLE CreateFile(fpCreateFile ip)
{
    HANDLE handle = (HANDLE)malloc(sizeof(tHandle));
    
    if (handle != NULL) {
        memset(handle, 0xFF, sizeof(tHandle));
        ip(handle);
        return handle;
    }
    
    return NULL;
}

/// @}


/// @}

/// @}
