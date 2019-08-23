////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_KEYBOARD.C
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES THE KEYBOARD DRIVER LAYER FUNCTIONS.
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
#define _DRV_KEYBOARD_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "drv.h"
#include "system_mm32.h"

#include "bsp.h"
#include "bsp_led.h"
#include "bsp_key.h"

#include "drv.h"
#include "drv_keyboard.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEYBOARD
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEYBOARD_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
s8 DRV_KEY_PutKeyBuffer(s8 key)
{
    for (u8 i = 0; i < 8; i++) {
        if (KEY_buffer[i] == -1) {
            KEY_buffer[i] = key;
            return key;
        }
    }

    memmove(KEY_buffer, KEY_buffer + 1, sizeof(KEY_buffer) - 1);

    KEY_buffer[7] = key;
    return key;
}

////////////////////////////////////////////////////////////////////////////////
s8 DRV_KEY_GetKeyBuffer()
{
    if (KEY_buffer[0] == -1)
        return -1;
    u8 ret = KEY_buffer[0];
    memmove(KEY_buffer, KEY_buffer + 1, sizeof(KEY_buffer) - 1);
    KEY_buffer[7] = -1;
    return ret;
}


////////////////////////////////////////////////////////////////////////////////
bool DRV_KEY_GetKeyValue()
{
    if (!KEY.event[0] && Key1()) {
        if ((!KEY.press[0]) && (KEY.dlyCnt[0]++ >= 20)) {
            KEY.event[0] = true;
            KEY.press[0] = true;
        }
    }
    else if (!Key1()) {
        KEY.dlyCnt[0] = 0;
        KEY.press[0]  = false;
    }

    if (!KEY.event[1] && Key2()) {
        if ((!KEY.press[1]) && (KEY.dlyCnt[1]++ >= 20)) {
            KEY.event[1] = true;
            KEY.press[1] = true;
        }
    }
    else if (!Key2()) {
        KEY.dlyCnt[1] = 0;
        KEY.press[1]  = false;
    }

    if (!KEY.event[2] && Key3()) {
        if ((!KEY.press[2]) && (KEY.dlyCnt[2]++ >= 20)) {
            KEY.event[2] = true;
            KEY.press[2] = true;
        }
    }
    else if (!Key3()) {
        KEY.dlyCnt[2] = 0;
        KEY.press[2]  = false;
    }

    if (!KEY.event[3] && Key4()) {
        if ((!KEY.press[3]) && (KEY.dlyCnt[3]++ >= 20)) {
            KEY.event[3] = true;
            KEY.press[3] = true;
        }
    }
    else if (!Key4()) {
        KEY.dlyCnt[3] = 0;
        KEY.press[3]  = false;
    }

    for (u8 i = 0; i < 4; i++) {
        if (KEY.event[i]) {
            KEY.event[i] = false;
            return DRV_KEY_PutKeyBuffer(i);

        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
void Init_KeyBoard(void)
{
    BSP_KEY_Configure();
    memset(KEY_buffer, 0xFF, sizeof(KEY_buffer));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of KEYBOARD.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_KEYBOARD_DCB* pDcb, u8 idx)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of KEYBOARD.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_KEYBOARD_DCB* pDcb, u8 idx)
{
    Init_KeyBoard();
}

////////////////////////////////////////////////////////////////////////////////
static int KEYBOARD_OpenFile(HANDLE handle, tAPP_KEYBOARD_DCB* pDcb)
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
static void KEYBOARD_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
static int KEYBOARD_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;

    if (*ptr == 0xff) {
        return DRV_KEY_GetKeyValue();
    }
    else {
        s8 ret = DRV_KEY_GetKeyBuffer();
        *ptr = ret;
        return (ret != -1);
    }
}

////////////////////////////////////////////////////////////////////////////////
static int KEYBOARD_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;

    DRV_KEY_PutKeyBuffer(*ptr);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  KEYBOARD CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified KEYBOARD.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void KEYBOARD_CreateFile(HANDLE handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&KEYBOARD_OpenFile;
    handle->closeFile   = (int*)&KEYBOARD_CloseFile;
    handle->readFile    = (int*)&KEYBOARD_ReadFile;
    handle->writeFile   = (int*)&KEYBOARD_WriteFile;
}

/// @}


/// @}

/// @}
