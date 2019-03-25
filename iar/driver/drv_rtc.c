////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_RTC.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE RTC DRIVER LAYER FUNCTIONS.
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
#define _DRV_RTC_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include <time.h>

#include "hal_rcc.h"
#include "hal_pwr.h"
#include "hal_bkp.h"
#include "hal_nvic.h"
#include "hal_exti.h"
#include "hal_rtc.h"

#include "drv.h"
#include "common.h"
#include "drv_rtc.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RTC
/// @{



////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RTC_Exported_Functions
/// @{

#if defined(__MM3N1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Interrupt function of RTC_IRQHandler
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_IRQHandler(void)
{
    if (accessRTC) {
        accessRTC = false;
        RTC->CSR &= ~RTC_CSR_CNF;               // RTC_ExitConfigMode();
        PWR->CR  &= ~PWR_CR_DBP;
    }

    if (RTC_GetITStatus(RTC_IT_SEC)) {
        RTC_ClearITPendingBit(RTC_IT_SEC);

        if (instance->sync == emTYPE_Sync)
            ((fpRTC)instance[0].cbSecond)(0x01);
        else
            instance->fSecond = true;
    }

    if (RTC_GetITStatus(RTC_IT_ALR)) {
        RTC_ClearITPendingBit(RTC_IT_ALR);

        if (instance->sync == emTYPE_Sync)
            ((fpRTC)instance[0].cbAlarm)(0x02);
        else
            instance->fAlarm = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Interrupt function of RTCAlarm_IRQHandler
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTCAlarm_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_ALR)) {
        EXTI_ClearFlag(LEFT_SHIFT_BIT(17));
        RTC_ClearITPendingBit(RTC_IT_ALR);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize RTC.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_RTC_Init(void)
{
    u16 temp[10] = {0};
    for (u8 i = 0; i < 10; i++) {
        if( exBKP_ImmRead((BKPDR_Typedef)(4 * (i + 1))) != 0x0000) {
            temp[i] = exBKP_ImmRead((BKPDR_Typedef)(4 * (i + 1)));
        }
    }

    PWR_BackupAccessCmd(ENABLE);
    BKP_DeInit();

    RCC_LSEConfig(RCC_LSE_ON);
    while (!RCC_GetFlagStatus(RCC_FLAG_LSERDY));
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);

    for (u8 i = 0; i < 10; i++) {
        if( temp[i] != 0x0000) {
            *(u16*)(BKP_BASE + 4 * (i + 1)) = temp[i];
        }
    }

    RTC_WaitForSynchro();
    RTC_WaitForLastTask();

    RTC_SetPrescaler(32767);  ///< RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
    RTC_WaitForLastTask();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize RTC_NVIC.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_RTC_NVIC_Init(void)
{
    COMMON_NVIC_Configure(RTC_IRQn,  1,  1);

//  COMMON_ExitItRisingEnable(EXTI_Line17);
//  COMMON_NVIC_Configure(RTCAlarm_IRQn, 0,  0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  check validity of RTC
/// @param  None.
/// @retval bool    true: RTC has been initialized
///                 false: the first time to initialize RTC
////////////////////////////////////////////////////////////////////////////////
bool DRV_RTC_CheckValid()
{
    exBKP_Init();

    if ((exBKP_ImmRead(BKP_DR1) != 0x1234) || (exBKP_ImmRead(BKP_DR2) != 0xCDEF)) {
        exRCC_BackupReset();
        exBKP_ImmWrite(BKP_DR1, 0x1234);
        exBKP_ImmWrite(BKP_DR2, 0xCDEF);
        DRV_RTC_Init();
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of RTC.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_RTC_DCB* pDcb, u8 idx)
{
    instance[idx].sync          = pDcb->sync;
    instance[idx].cbSecond      = pDcb->cbSecond;
    instance[idx].cbAlarm       = pDcb->cbAlarm;
    instance[idx].fSecond       = false;
    instance[idx].fAlarm        = false;
    instance[idx].second        = pDcb->second;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of RTC.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_RTC_DCB* pDcb, u8 idx)
{
    if (!DRV_RTC_CheckValid()) {
        RTC_SetCounter(pDcb->second);
        instance[idx].fSecond = false;
    }
    DRV_RTC_NVIC_Init();

    RTC_EnterConfigMode();

    RTC_ITConfig((RTC_IT_TypeDef)((int)RTC_IT_SEC | (int)RTC_IT_ALR), ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function opens the RTC instance according to the specified
///         parameters in the pDcb struct.
/// @note   This function can be used for MM32F103 & MM32F3xx devices.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral----RTC.
/// @param  pDcb: pointer to a @ref tAPP_RTC_DCB structure that contains
///         the configuration information for RTC
/// @retval bool: handle->instance != NULL
////////////////////////////////////////////////////////////////////////////////
static int RTC_OpenFile(HANDLE handle, tAPP_RTC_DCB* pDcb)
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
/// @brief  This function closes the RTC instance according to the handle of
///         peripheral----RTC.
/// @note   This function can be used for MM32F103 & MM32F3xx devices.
/// @note   Turn off RTC clock, initialize the backup domain
/// @note   Deinitializes the specified struct instance accroding to
///         handle->idx to 0xFF.
/// @param  handle: pointer to HANDLE which contains the operation information
///         for the specified peripheral----RTC.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void RTC_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;
    RCC_RTCCLKCmd(DISABLE);
    BKP_DeInit();
    RTC_ExitConfigMode();

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get timeCounter or alarmCounter, get second flag or alarm flag
/// @note   This function can be used for MM32F103 & MM32F3xx devices.
/// @param  handle          pointer to HANDLE which contains the operation information
///         for the specified peripheral----RTC.
///         if handle->mode = emRTC_timeCounter, put the TimeCounter to ptr
///         if handle->mode = emRTC_timeCalendar, put the timeCalendar to ptr
/// @param  hSub: the virtual index of instance[hSub];
/// @param  ptr             the buffer to store the data
/// @param  len             file length
/// @note   In fact, this one has no effect
/// @retval int, 0x01: Second flag; 0x02: Alarm flag;-1: do not read
////////////////////////////////////////////////////////////////////////////////
static int RTC_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;

    struct tm*  pt;
    time_t      tim;

    switch (handle->mode) {
        case emRTC_TimeCalendar:
            tim = (time_t)RTC_GetCounter();
            pt = (struct tm*)ptr;
            *pt = *localtime(&tim);
            break;

        case emRTC_TimeCounter:
        default:
            *(u32*)ptr = RTC_GetCounter();
            break;
    }

    int ret = 0;
    if (instance->fSecond) {
        instance->fSecond  = false;
        ret |= 0x01;
    }

    if (instance->fAlarm) {
        instance->fAlarm = false;
        ret |= 0x02;
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Operating RTC_WriteFile, Set timeCounter or alarmCounter
/// @note   This function can be used for MM32F103 & MM32F3xx devices.
/// @note   can used for set time, set accurate alarm or set next alarm
/// @param  handle          file handle
///         if handle->mode = emRTC_timeCounter, set time accroding to counter
///         if handle->mode = emRTC_alarmCounter, set alarm accroding to counter
///         if handle->mode = emRTC_timeCalendar, set time accroding to calendar
///         if handle->mode = emRTC_alarmCalendar, set alarm accroding to calendar
/// @param  hSub: the virtual index of instance[hSub];
/// @param  ptr             the buffer to store the data to be written
/// @param  len             the length of ptr
/// @retval int             retval = 0: error,
///                         retval > 0: operation succeed
///                         retval = -1: do not read
////////////////////////////////////////////////////////////////////////////////
static int RTC_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    if (hSub == -1) return 0;

    struct tm t;

    switch (handle->mode) {
        case emRTC_TimeCounter:
            RTC_SetCounter(*(u32*)ptr);
            return RTC_GetCounter();

        case emRTC_TimeCalendar:
            memcpy((u8*)(&t), ptr, len);
            RTC_SetCounter(mktime(&t));
            return RTC_GetCounter();

        case emRTC_AlarmCounter:
            ((tDRV_RTC_INSTANT*)handle->instance)->second = RTC_GetCounter() + *(u32*)ptr;
            RTC_SetAlarm(((tDRV_RTC_INSTANT*)handle->instance)->second);
            return ((tDRV_RTC_INSTANT*)handle->instance)->second;

        case emRTC_AlarmCalendar:
            memcpy((u8*)(&t), ptr, len);
            ((tDRV_RTC_INSTANT*)handle->instance)->second = mktime(&t);
            RTC_SetAlarm(((tDRV_RTC_INSTANT*)handle->instance)->second);
            return ((tDRV_RTC_INSTANT*)handle->instance)->second;

        default:
            return 0;
    }
}

#endif  // defined(__MM3N1)


////////////////////////////////////////////////////////////////////////////////
/// @brief  RTC CreateFile
/// @param  handle: pointer to a HANDLE structure that contains
///         the instance for the specified RTC.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_CreateFile(HANDLE  handle)
{
#if defined(__MM3N1)
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->size        = sizeof(tbIpBase);
    handle->idx         = -1;
    handle->openFile    = (int*)&RTC_OpenFile;
    handle->closeFile   = (int*)&RTC_CloseFile;
    handle->readFile    = (int*)&RTC_ReadFile;
    handle->writeFile   = (int*)&RTC_WriteFile;
#endif
}



/// @}

/// @}

/// @}




