////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_AES.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE AES DRIVER LAYER FUNCTIONS.
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
#define _DRV_AES_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_aes.h"
#include "hal_rcc.h"

#include "drv.h"
#include "common.h"
#include "drv_aes.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AES
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AES_Exported_Functions
/// @{


#if defined(__MM3N1) || defined(__MM0N1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Set AES key size and enable AES clock.
/// @param  keySize: Key size.
///         This parameter can be any combination of the following values:
/// @arg    AES_Key_128: 128 bits key size
/// @arg    AES_Key_192: 192 bits key size
/// @arg    AES_Key_256: 256 bits key size
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_AES_Init(AES_KeySize_TypeDef keySize)
{
    COMMON_EnableIpClock(emCLOCK_AES);

    AES_KeySize(keySize);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Deinit AES.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_AES_DeInit()
{
    COMMON_DisableIpClock(emCLOCK_AES);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of AES.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_AES_DCB* pDcb, u8 idx)
{
    instance[idx].bits  = pDcb->bits;

    memcpy(instance[idx].itemKey, pDcb->key, 32);
    memcpy(instance[idx].itemIv,  pDcb->iv,  16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of AES.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_AES_DCB* pDcb, u8 idx)
{
    u32 itemKeySize[3] = {AES_CR_KSIZE128, AES_CR_KSIZE192, AES_CR_KSIZE256};
    DRV_AES_Init((AES_KeySize_TypeDef)itemKeySize[instance[idx].bits]);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Open AES according to the specified parameters in the pDcb.
/// @param  pDcb: pointer to AES data control block which contains the
///         configuration information for the specified AES peripheral.
/// @retval The operation result
/// This parameter can be 0 or a pointer to tAES_INSTANT structure
/// that contains part of configuration information.
////////////////////////////////////////////////////////////////////////////////
static int AES_OpenFile(HANDLE handle, tAPP_AES_DCB* pDcb)
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
/// @brief  Close AES
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void AES_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;
    DRV_AES_DeInit();
    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write data to AES and then get the processes data
/// @param  handle: pointer to HANDLE which contains the operation information
/// for the specified peripheral.
/// @param  ptr: pointer to input and output data
/// annote: input data and output data use the same memory address
/// @param  count: data length
/// @retval Process result
/// This parameter can be ERROR or SUCCESS
////////////////////////////////////////////////////////////////////////////////
static int AES_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    bool ret;

    if (handle->mode == emBOOL_Encrypt) {
        if ((instance[idx].sPrefix.trueIdx) == emFILE_AES_ECB)
            return tbFunc1[instance[idx].sPrefix.subIdx](instance[idx].itemKey, ptr);
        else if (handle->mode <= 4){
            return tbFunc2[(instance[idx].sPrefix.subIdx - 1) * 2](instance[idx].itemKey, instance[idx].itemIv, ptr);
        }
    }
    else if (handle->mode == emBOOL_Decrypt) {
        if ((instance[idx].sPrefix.trueIdx) == emFILE_AES_ECB)
            ret = tbFunc1[instance[idx].sPrefix.subIdx + 1](instance[idx].itemKey, ptr);
        else if (handle->mode <= 4) {
            ret = tbFunc2[(instance[idx].sPrefix.subIdx - 1) * 2 + 1](instance[idx].itemKey, instance[idx].itemIv, ptr);
        }
    }
    return ret;
}

#endif      /* __MM3N1 || __MM0N1 */

////////////////////////////////////////////////////////////////////////////////
/// @brief  AES CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified AES.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AES_CreateFile(HANDLE  handle)
{
#if defined(__MM3N1) || defined(__MM0N1)
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->size        = sizeof(tbIpBase);
    handle->idx         = -1;
    handle->openFile    = (int*)&AES_OpenFile;
    handle->closeFile   = (int*)&AES_CloseFile;
    handle->readFile    = NULL;
    handle->writeFile   = (int*)&AES_WriteFile;
#endif
}




///@}

///@}

///@}
