////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_CRS.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE CRS DRIVER LAYER FUNCTIONS.
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
#define _DRV_CRS_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "hal_rcc.h"
#include "hal_nvic.h"
#include "hal_crs.h"

#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_crs.h"

#include "drv.h"
#include "common.h"
#include "drv_crs.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CRS
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CRS_Exported_Functions
/// @{


#if defined(__MM0N1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  CRS IRQHandler function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
#if defined(__EX_CRS)
void RCC_CRS_IRQHandler()
{
    if ((CRS_GetITStatus(CRS_ISR_OKIF)) && (exCRS_GetITSource(CRS_ISR_OKIF))) {
        CRS_MonitorWindow.CRS_STATUS = CRS_STATUS_SYNCOK;
        CRS_ITConfig(CRS_ISR_OKIF, DISABLE);
        CRS_ClearITPendingBit(CRS_ISR_OKIF);
    }

    if ((CRS_GetITStatus(CRS_ISR_WARNIF)) && (exCRS_GetITSource(CRS_ISR_WARNIF))) {
        CRS_MonitorWindow.CRS_STATUS = CRS_STATUS_SYNCWARN;
        CRS_ITConfig(CRS_ISR_WARNIF, DISABLE);
        CRS_ClearITPendingBit(CRS_ISR_WARNIF);
    }

    if ((CRS_GetITStatus(CRS_ISR_ERR)) && (exCRS_GetITSource(CRS_ISR_ERR))) {
        CRS_MonitorWindow.CRS_STATUS = CRS_STATUS_SYNCERR;
        CRS_ClearITPendingBit(CRS_ISR_ERR);
    }

    if ((CRS_GetITStatus(CRS_ISR_MISS)) && (exCRS_GetITSource(CRS_ISR_MISS))) {
        CRS_MonitorWindow.CRS_STATUS = CRS_STATUS_SYNCMISS;
        CRS_ClearITPendingBit(CRS_ISR_ERR);
    }

    if ((CRS_GetITStatus(CRS_ISR_EXPTIF)) && (exCRS_GetITSource(CRS_ISR_EXPTIF))) {
        CRS_ClearITPendingBit(CRS_ISR_EXPTIF);
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO sync source clock frequency calculation and initialize CRS
/// @param  hsi_freq: HSI clock frequency (default 48MHz).
/// @param  src_freq: Synchronization source clock frequency.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_CRS_GPIO_Freq(u32 hsiFreq, u32 srcFreq)
{
    CRS_InitTypeDef CRS_InitStructure;
    CRS_InitStructure.Polarity              = CRS_SYNCPolarity_Rising;
    CRS_InitStructure.Prescaler             = CRS_SYNC_Div1;
    CRS_InitStructure.Source                = CRS_SYNCSource_GPIO;
    CRS_InitStructure.ReloadValue           = hsiFreq / srcFreq - 1;
    CRS_InitStructure.ErrorLimitValue       = (u32)((hsiFreq / srcFreq - 1) * 0.0014 / 2);
    CRS_InitStructure.HSI48CalibrationValue = 0x00;

    exCRS_Init(&CRS_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize CRS NVIC.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_CRS_NVIC_Init()
{
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel         = RCC_CRS_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  USB Connect.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_CRS_USB_Connect()
{
    // USB Connect
    *(__IO u32*)(0x40005C00) = 0x0003U;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the system clock for CRS.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_CRS_SysClockConfig()
{
    RCCInitStruct_TypeDef RCC_InitStructure;
    RCC_InitStructure.RCC_SystemClock = SYSCLK_HSI_96MHz;  // HSE_96MHz@8MHz

    RCC_InitStructure.RCC_PrescaleAHB  = 0;  // Range: 0-7:8-15; div1-div512
    RCC_InitStructure.RCC_PrescaleAPB1 = 0;  // Range: 0:15; div1:div16
    RCC_InitStructure.RCC_PrescaleAPB2 = 0;  // Range: 0:15; div1:div16
    RCC_InitStructure.RCC_PrescaleUSB  = 1;  // Range: 0:1;  div1:div2

    RCC_InitStructure.SysTickEN     = false;
    RCC_InitStructure.SysTickPeriod = 1000;

    if (exRCC_Init(&RCC_InitStructure) == ERROR) {
        while (1) {
            // error
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of CRS.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_CRS_DCB* pDcb, u8 idx)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of CRS.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_CRS_DCB* pDcb, u8 idx)
{
    BSP_CRS_GPIO_MCO();                                 // Output

    CRS_MonitorWindow.CRS_STATUS = CRS_STATUS_INIT;

    COMMON_EnableIpClock(emCLOCK_CRS);

    DRV_CRS_NVIC_Init();

    if (pDcb->src == emCRS_GPIO) {
        BSP_CRS_GPIO_Configure();
        DRV_CRS_GPIO_Freq(pDcb->hsiFreq, pDcb->srcFreq);
    }
    else {
        BSP_CRS_USB_Configure();
        DRV_CRS_USB_Connect();
        // usb protocol
    }

    CRS_SynSourceConfig((pDcb->src) ? CRS_SYNCSource_USB : CRS_SYNCSource_GPIO);

    // Enable Automatic Calibration, Frequency Error Counter and Interrupt.
    CRS_AutomaticCalibrationCmd(ENABLE);
    CRS_FrequencyErrorCounterCmd(ENABLE);
    CRS_ITConfig(CRS_ISR_OKIF | CRS_ISR_WARNIF | CRS_ISR_ERRIF | CRS_ISR_EXPTIF, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function opens the CRS instance according to the specified
///         parameters in the CRS_DCB_TypeDef
/// @param  pDcb: pointer to a CRS_HandleTypeDef structure that contains
///                the configuration information for the specified CRS module.
/// @retval  TRUE or FALSE
////////////////////////////////////////////////////////////////////////////////
static int CRS_OpenFile(HANDLE handle, tAPP_CRS_DCB* pDcb)
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
/// @brief  This function closes the CRS instance according to the ID
///          in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified CRS.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void CRS_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;
    CRS_DeInit();
    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  the CRS read data according to the ID in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified CRS.
/// @param  ptr: Pointer to data buffer.
/// @param  count: Amount of data to be read.
/// @retval  sent data number.
////////////////////////////////////////////////////////////////////////////////
static int CRS_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;

    *ptr  = (CRS_MonitorWindow.CRS_STATUS == CRS_STATUS_SYNCOK);
    return (*ptr != 0);
}

//////////////////////////////////////////////////////////////////////////////////
///// @brief  Write data to CRS and then get the processes data
///// @param  handle: pointer to HANDLE which contains the operation information
/////           for the specified peripheral.
///// @param  ptr: pointer to input and output data
/////       annote: input data and output data use the same memory address
///// @param  count: data length
///// @retval Process result
/////           This parameter can be ERROR or SUCCESS
//////////////////////////////////////////////////////////////////////////////////
//static int CRS_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
//{
//  if (hSub == -1) return 0;
//    return len;
//}

#endif      /*  __MM0N1 */

////////////////////////////////////////////////////////////////////////////////
/// @brief  CRS CreateFile
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CRS_CreateFile(HANDLE handle)
{
#if defined(__MM0N1)
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->size        = sizeof(tbIpBase);
    handle->idx         = -1;
    handle->openFile    = (int*)&CRS_OpenFile;
    handle->closeFile   = (int*)&CRS_CloseFile;
    handle->readFile    = (int*)&CRS_ReadFile;
    handle->writeFile   = NULL;         //(int*)&CRS_WriteFile;
#endif
}



/// @}

/// @}

/// @}

