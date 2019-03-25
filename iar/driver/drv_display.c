////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_DISPLAY.C
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE DISPLAY DRIVER LAYER FUNCTIONS.
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
#define _DRV_DISPLAY_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "drv.h"
#include "system_mm32.h"

#include "bsp.h"
#include "bsp_led.h"

#include "drv.h"
#include "drv_display.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY_DISPLAY
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DISPLAY_Exported_Functions
/// @{


void DRV_Display(u8 led)
{
#if defined(__MM32_MINIBOARD)
    (led & 0x01) ? LD6_on() : LD6_off();
    (led & 0x02) ? LD5_on() : LD5_off();
    (led & 0x04) ? LD4_on() : LD4_off();
    (led & 0x08) ? LD3_on() : LD3_off();
#endif
#if defined(__MM32_EVB)
    ((led & 0x01) || (led & 0x04)) ? LD1_on() : LD1_off();
    ((led & 0x02) || (led & 0x08)) ? LD2_on() : LD2_off();
#endif
}

////////////////////////////////////////////////////////////////////////////////
void Init_Display(void)
{
    BSP_LED_Configure();
    CloseLED();
    DRV_Display(0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of DISPLAY.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_DISPLAY_DCB* pDcb, u8 idx)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of DISPLAY.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_DISPLAY_DCB* pDcb, u8 idx)
{
    BSP_LED_Configure();
    CloseLED();
    DRV_Display(0);
}

////////////////////////////////////////////////////////////////////////////////
static int DISPLAY_OpenFile(HANDLE handle, tAPP_DISPLAY_DCB* pDcb)
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
static void DISPLAY_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;
    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
static int DISPLAY_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;

    if (len <= 4)   DRV_Display(*ptr);
    return len;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DISPLAY CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified DISPLAY.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DISPLAY_CreateFile(HANDLE  handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&DISPLAY_OpenFile;
    handle->closeFile   = (int*)&DISPLAY_CloseFile;
    handle->readFile    = NULL;
    handle->writeFile   = (int*)&DISPLAY_WriteFile;
}

/// @}


/// @}

/// @}
