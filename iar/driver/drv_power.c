////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_POWER.C
/// @author   MY Zhang
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE POWER DRIVER LAYER FUNCTIONS.
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
#define _DRV_POWER_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_pwr.h"
#include "hal_exti.h"
#include "hal_nvic.h"


#include "drv.h"
#include "common.h"
#include "drv_power.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWR
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWR_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Key interrupt function initialization.
/// @param  None.
/// @arg    None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_KEY_EXTI_Init(void)
{
    COMMON_EnableIpClock(emCLOCK_EXTI);

    COMMON_ExitItFallingEnable(EXTI_Line0);
//  COMMON_ExitItRisingEnable(EXTI_Line10 | EXTI_Line11 | EXTI_Line13);

    EXTI_LineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
//  EXTI_LineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);
//  EXTI_LineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);
//  EXTI_LineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

    u8 ch;
//#if defined(__MM3N1) || defined(__MM3O1)
//    ch = EXTI15_10_IRQn;
//#endif
//#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
//    ch = EXTI4_15_IRQn;
//#endif
//  COMMON_NVIC_Configure(ch,  2,  2);

#if defined(__MM3N1) || defined(__MM3O1)
    ch = EXTI0_IRQn;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    ch = EXTI0_1_IRQn;
#endif
    COMMON_NVIC_Configure(ch,  2,  1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable PVD interrupt and set interrupt edge detection edge..
/// @param  edge: EXTI edge selection.
///   This parameter can be one of the following values:
/// @arg    edge: emPVD_falling
/// @arg    edge: emPVD_rising
/// @arg    edge: emPVD_rising_falling
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_PWR_PVD_EXTI_Init(EM_EDGE edge)
{
    if (edge == emEDGE_Rising)
        COMMON_ExitEventRisingEnable(EXTI_Line16);
    else if (edge == emEDGE_Falling)
        COMMON_ExitEventFallingEnable(EXTI_Line16);
    else
        COMMON_ExitEventRisingFallingEnable(EXTI_Line16);

    COMMON_NVIC_Configure(PVD_IRQn,  0,  0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable the voltage value of the voltage monitor.
/// @param  level: PVD level selection.
///   This parameter can be one of the following values:
///         @arg    0: emPVD_1V8
///         @arg    1: emPVD_2V1
///         @arg    2: emPVD_2V4
///         @arg    3: emPVD_2V7
///         @arg    4: emPVD_3V0
///         @arg    5: emPVD_3V3
///         @arg    6: emPVD_3V6
///         @arg    7: emPVD_3V9
///         @arg    8: emPVD_4V2
///         @arg    9: emPVD_4V5
///         @arg    10: emPVD_4V8
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_PWR_PVD_Init(u32 level)
{
    COMMON_EnableIpClock(emCLOCK_PWR);
    PWR_PVDLevelConfig(level);                                  // PWR_PVDLevel_3V0
    PWR_PVDCmd(ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Power mode wakeup function initialization state.
/// @param  None.
/// @arg    None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_PWR_WakeUp_Init(void)
{
    COMMON_EnableIpClock(emCLOCK_EXTI);
    COMMON_EnableIpClock(emCLOCK_PWR);
    PWR_WakeUpPinCmd(ENABLE);
}

/* -------------------------------------------------------
-------     I n t e r r u p t   H a n d l e r    ---------
------------------------------------------------------- */


#if defined(__EX_POWER)
#if defined(SLEEP_EXTI) || defined(STOP_EXTI)
////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTI_IRQHandler.
/// @param  None.
/// @arg    None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM3N1)
void EXTI0_IRQHandler(void)
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
void EXTI0_1_IRQHandler(void)
#endif
{
    if (EXTI_GetFlagStatus(EXTI_Line0))
        EXTI_ClearITPendingBit(EXTI_Line0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTI_IRQHandler
/// @param  None.
/// @arg    None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
//#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM0Q1)
//
//#if defined(__MM3N1)
//void EXTI15_10_IRQHandler(void)
//#endif
//
//#if defined(__MM0N1) || defined(__MM0Q1)
//void EXTI4_15_IRQHandler(void)
//#endif
//{
//  if (EXTI_GetFlagStatus(EXTI_Line10))
//     EXTI_ClearITPendingBit(EXTI_Line10);
//
//  if (EXTI_GetFlagStatus(EXTI_Line11))
//     EXTI_ClearITPendingBit(EXTI_Line11);
//
//  if (EXTI_GetFlagStatus(EXTI_Line13))
//     EXTI_ClearITPendingBit(EXTI_Line13);
//}
//#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTI4_15_IRQHandler
/// @param  None.
/// @arg    None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

//#if defined(__MM0P1)
//void EXTI4_15_IRQHandler(void)
//{
//  if (EXTI_GetFlagStatus(EXTI_Line10))
//     EXTI_ClearITPendingBit(EXTI_Line10);
//
//  if (EXTI_GetFlagStatus(EXTI_Line15))
//     EXTI_ClearITPendingBit(EXTI_Line15);
//}
//#endif

#endif      // defined(SLEEP_EXTI) || defined(STOP_EXTI)
#endif      // defined(__EX_POWER)

/* -------------------------------------------------------
-------   F u n c t i o n     I m p l e m e n t  ---------
------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  The system enters standby mode
/// @param  mode: The system enters standby modes.
///         This parameter can be one of the following modes:
///         @arg    mode: LP_WFI
///         @arg    mode: LP_WFE
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_PWR_Standby(emPWR_Wait_Mode_Typedef mode)
{
    COMMON_EnableIpClock(emCLOCK_PWR);
    DRV_PWR_WakeUp_Init();
    exPWR_EnterLowPowerMode(LP_STANDBY_MODE, mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  The system enters stop mode
/// @param  mode: The system enters stop modes.
///         This parameter can be one of the following modes:
///         @arg    mode: LP_WFI
///         @arg    mode: LP_WFE
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_PWR_Stop(emPWR_Wait_Mode_Typedef mode)
{
    COMMON_EnableIpClock(emCLOCK_PWR);
    DRV_PWR_WakeUp_Init();
    exPWR_EnterLowPowerMode(LP_STOP_MODE, mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  The system enters stleep mode
/// @param  mode: The system enters sleep modes.
///         This parameter can be one of the following modes:
///         @arg    mode: LP_WFI
///         @arg    mode: LP_WFE
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_PWR_Sleep(emPWR_Wait_Mode_Typedef mode)
{
    COMMON_EnableIpClock(emCLOCK_PWR);
    exPWR_EnterLowPowerMode(LP_SLEEP_MODE, mode);
}

/* -------------------------------------------------------
-------     I n t e r r u p t   H a n d l e r    ---------
------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTI_IRQHandler
/// @param  None.
/// @arg    None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
#if defined(__EX_POWER)
void PVD_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line16) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line16);

        PVD_status = PWR_GetFlagStatus(PWR_CSR_PVDO);

        if (instance[0].sync == emTYPE_Sync) {
            (PVD_status) ?  ((fpPVD)instance[0].rising)() :
                            ((fpPVD)instance[0].falling)();
        }
        else
            PVD_change = true;
    }
}
#endif


////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of POWER.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_POWER_DCB* pDcb, u8 idx)
{
    u32 tbPvd[] = { emPVD_1V8, emPVD_2V1, emPVD_2V4, emPVD_2V7, emPVD_3V0, emPVD_3V3,
                    emPVD_3V6, emPVD_3V9, emPVD_4V2, emPVD_4V5, emPVD_4V8 };
    instance[idx].sync      = pDcb->sync;
    instance[idx].cb1       = pDcb->cb1;
    instance[idx].cb2       = pDcb->cb2;
    instance[idx].pvdLevel  = (EM_PVD_LEVEL)tbPvd[pDcb->voltage];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of POWER.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_POWER_DCB* pDcb, u8 idx)
{
    if (instance[idx].sPrefix.trueIdx == emFILE_PVD_R) {
        DRV_PWR_PVD_EXTI_Init(emEDGE_Rising);
        DRV_PWR_PVD_Init(instance->pvdLevel);
        PWR_PVDCmd((FunctionalState)(pDcb->enable));
        PVD_change = false;
    }
    else if (instance[idx].sPrefix.trueIdx == emFILE_PVD_F) {
        DRV_PWR_PVD_EXTI_Init(emEDGE_Falling);
        DRV_PWR_PVD_Init(instance->pvdLevel);
        PWR_PVDCmd((FunctionalState)(pDcb->enable));
        PVD_change = false;
    }
    else if (instance[idx].sPrefix.trueIdx == emFILE_STOP) {
        DRV_KEY_EXTI_Init();
    }
    else if (instance[idx].sPrefix.trueIdx == emFILE_SLEEP) {
        DRV_KEY_EXTI_Init();
    }
    else if (instance[idx].sPrefix.trueIdx == emFILE_STANDBY) {
        COMMON_EnableIpClock(emCLOCK_PWR);
        DRV_PWR_WakeUp_Init();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  POWER OPEN FILE function.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified POWER.
/// @param  pDcb: The structure contains parameters for power initialization
/// @retval  pointer to POWER instance.
////////////////////////////////////////////////////////////////////////////////
static int POWER_OpenFile(HANDLE handle, tAPP_POWER_DCB* pDcb)
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
/// @brief  This function closes the POWER instance according to the ID
///          in the HANDLE.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified POWER.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void POWER_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    if ((instance[handle->idx].sPrefix.subIdx == emFILE_PVD_R) || (instance[handle->idx].sPrefix.subIdx == emFILE_PVD_F)) {
        PWR_PVDCmd(DISABLE);
    }

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  POWER READ FILE function.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified POWER.
/// @retval Get value returns true
////////////////////////////////////////////////////////////////////////////////
static int POWER_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    if ((instance[idx].sPrefix.trueIdx == emFILE_PVD_R) || (instance[idx].sPrefix.trueIdx == emFILE_PVD_F)) {
        if (PVD_change) {
            PVD_change = false;
            *ptr = PVD_status;
            return 1;
        }
        return 0;
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  POWER WRITE FILE function.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified POWER.
/// @retval Get value returns true
////////////////////////////////////////////////////////////////////////////////
static int POWER_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    exRCC_SystickDisable();

    if (instance[idx].sPrefix.trueIdx == emFILE_STOP)           DRV_PWR_Stop(LP_WFI);
    else if (instance[idx].sPrefix.trueIdx == emFILE_SLEEP)     DRV_PWR_Sleep(LP_WFI);
    else if (instance[idx].sPrefix.trueIdx == emFILE_STANDBY)   DRV_PWR_Standby(LP_WFI);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  POWER CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified POWER.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void POWER_CreateFile(HANDLE    handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&POWER_OpenFile;
    handle->closeFile   = (int*)&POWER_CloseFile;
    handle->readFile    = (int*)&POWER_ReadFile;
    handle->writeFile   = (int*)&POWER_WriteFile;
}



/// @}

/// @}

/// @}
