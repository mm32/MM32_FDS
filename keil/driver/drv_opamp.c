////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_OPAMP.C
/// @author   PX Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE OPAMP DRIVER LAYER FUNCTIONS.
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
#define _DRV_OPAMP_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_rcc.h"
#include "hal_opamp.h"

#include "bsp_opamp.h"
#include "drv_opamp.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
///@addtogroup OPAMP
///@brief OPAMP driver modules
///@{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AES_Exported_Functions
/// @{

#if defined(__MM0P1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the selected OPAMP.
/// @param  idx: The number of the selected OPAMP.
/// @retval  The selected OPAMP
////////////////////////////////////////////////////////////////////////////////
//u32 DRV_GetBase_OPAMPx(u8 idx)
//{
//#if defined(__MM0P1) || defined(__MM0Q1)
//
//  u32 opamp[] = {OPAMP1, OPAMP2, OPAMP3, OPAMP4};
//#endif
//  return  (u32)(opamp[idx]);
//}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of OPAMP.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_OPAMP_DCB* pDcb, u8 idx)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of OPAMP.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_OPAMP_DCB* pDcb, u8 idx)
{
//  COMMON_EnableIpClock(emCLOCK_COMP);
    BSP_OPAMP_GPIO_Configure((u32)instance[idx].sPrefix.pBase);
    OPAMP_Configure((emOPAMP_OutEn)instance[idx].sPrefix.pBase, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the basic parameters of OPAMP to prepare transmission.
/// @param  pDcb: Data control block.
/// @retval  Operational Amplifier configuration.
////////////////////////////////////////////////////////////////////////////////
static int OPAMP_OpenFile(HANDLE handle, tAPP_OPAMP_DCB* pDcb)
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
/// @brief  This function closes the OPAMP instance according to the ID
///          in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified OPAMP.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void OPAMP_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;
    OPAMP_Configure((emOPAMP_OutEn)instance[tbSubHandleIdx[handle->idx]].sPrefix.pBase, DISABLE);
    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  the result of OPAMP receives.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified UART.
/// @param  ptr: Pointer to data buffer.
/// @param  count: Amount of data to be received.
/// @retval  0 or 1.
////////////////////////////////////////////////////////////////////////////////
static int OPAMP_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  the OPAMP sends data according to the ID in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified OPAMP.
/// @param  ptr: Pointer to data buffer.
/// @param  count: Amount of data to be sent.
/// @retval  sent data number.
////////////////////////////////////////////////////////////////////////////////
static int OPAMP_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;

    return true;
}
#endif      /* __MM0P1 */

////////////////////////////////////////////////////////////////////////////////
/// @brief  OPAMP CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified OPAMP.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void OPAMP_CreateFile(HANDLE    handle)
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
    handle->openFile    = (int*)&OPAMP_OpenFile;
    handle->closeFile   = (int*)&OPAMP_CloseFile;
    handle->readFile    = (int*)&OPAMP_ReadFile;
    handle->writeFile   = (int*)&OPAMP_WriteFile;
#endif
}



/// @}

/// @}

/// @}
