////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_GPIO.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES THE GPIO DRIVER LAYER FUNCTIONS.
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
#define _DRV_GPIO_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include <time.h>

#include "hal_gpio.h"

#include "drv.h"
#include "bsp.h"
#include "drv_gpio.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup GPIO
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup GPIO_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of GPIO.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_GPIO_DCB* pDcb, u8 idx)
{
    instance[idx].dir       = pDcb->dir;
    instance[idx].mode      = pDcb->mode;
    instance[idx].inSta     = pDcb->state;
    instance[idx].mask      = pDcb->mask;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of GPIO.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_GPIO_DCB* pDcb, u8 idx)
{
    GPIOx_ClockEnable((EM_GPIO_PORT)instance[idx].sPrefix.trueIdx);

    u16 dir                 = pDcb->dir;
    u16 mode                = pDcb->mode;
    u16 inSta               = pDcb->state;
    u16 mask                = pDcb->mask;

    for (u8 i = 0; i < 16; i++) {
        if (mask & 0x0001) {
            if (dir & 0x0001) {
                /* input */
                if (inSta & 0x0001) {
                    (mode & 0x0001) ?
                        GPIO_Mode_IPU_Init((GPIO_TypeDef*)instance[idx].sPrefix.pBase, 1 << i, NO_REMAP, NO_FUNCAF) :       // pull up
                        GPIO_Mode_IPD_Init((GPIO_TypeDef*)instance[idx].sPrefix.pBase, 1 << i, NO_REMAP, NO_FUNCAF);        // pull down
                }
                else    GPIO_Mode_FLOATING_Init((GPIO_TypeDef*)instance[idx].sPrefix.pBase, 1 << i, NO_REMAP, NO_FUNCAF);   // floating input
            }
            else {
                /* output */
                (mode & 0x0001) ? GPIO_Mode_OUT_OD_Init((GPIO_TypeDef*)instance[idx].sPrefix.pBase, 1 << i) :               // Open drain output
                                  GPIO_Mode_OUT_PP_Init((GPIO_TypeDef*)instance[idx].sPrefix.pBase, 1 << i);                // Push-pull
            }
        }
        dir   >>= 1;        mode  >>= 1;        inSta >>= 1;    mask >>= 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function opens the GPIO instance according to the specified
///         parameters in the pDcb struct.
/// @note   This function can be used for all MM32 devices.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral----GPIO.
/// @param  pDcb: pointer to a @ref tAPP_GPIO_DCB structure that contains
///         the configuration information for EEPROM
/// @retval bool: handle->instance != NULL
////////////////////////////////////////////////////////////////////////////////
static int GPIO_OpenFile(HANDLE handle, tAPP_GPIO_DCB* pDcb)
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
/// @brief  This function closes the GPIO instance according to the handle of
///         peripheral----GPIO.
/// @note   This function can be used for all MM32 devices.
/// @note   Deinitializes the specified struct instance accroding to
///         handle->idx to 0xFF.
/// @note   Deinitializes the GPIOx peripheral registers to their default reset
///         values accroding to handle->idx.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the peripheral----GPIO.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void GPIO_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    GPIO_DeInit((GPIO_TypeDef*)instance[handle->idx].sPrefix.pBase);

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  read the data in GPIO IDR register
/// @note   This function can be used for all MM32 devices.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral----GPIO.
/// @param  hSub: the virtual index of instance[hSub];
/// @param  ptr: the buffer to store the data
/// @param  len: the length of data
/// @note   In fact, this one has no effect, the length is 16.
/// @retval int: the data has gotten
////////////////////////////////////////////////////////////////////////////////
static int GPIO_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    *(u16*)ptr = GPIO_ReadInputData((GPIO_TypeDef*)instance[idx].sPrefix.pBase);
    return 2;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  write the GPIO output status in ODR register.
/// @note   This function can be used for all MM32 devices.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral----GPIO.
/// @param  hSub: the virtual index of instance[hSub];
/// @param  ptr: the buffer that store the data to be written
/// @param  len: the length of ptr
/// @note   In fact, this one has no effect, the length is 16.
/// @retval 1: the operation succeed
////////////////////////////////////////////////////////////////////////////////
static int GPIO_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    u16 dat = *(u16*)ptr |  instance[idx].dir;

    GPIO_Write((GPIO_TypeDef*)instance[idx].sPrefix.pBase, dat);
    return 2;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified GPIO.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_CreateFile(HANDLE handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&GPIO_OpenFile;
    handle->closeFile   = (int*)&GPIO_CloseFile;
    handle->readFile    = (int*)&GPIO_ReadFile;
    handle->writeFile   = (int*)&GPIO_WriteFile;
}

/// @}

/// @}

/// @}




