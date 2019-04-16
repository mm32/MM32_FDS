////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_TIM.C
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES THE TIM DRIVER LAYER FUNCTIONS.
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
#define _DRV_TIM_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_tim.h"
#include "hal_rcc.h"
#include "hal_nvic.h"

#include "bsp.h"
#include "drv.h"
#include "common.h"
#include "drv_dma.h"
#include "drv_tim.h"
#include "bsp_tim.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable TIMER Peripheral Clock.
/// @param  TIMx: where x can be 1, 2, 3, 4, 8, 14, 16 or 17 to select the TIMER peripheral.
/// @param  idx: TIMER peripheral true idx.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_ClockEnable(TIM_TypeDef* TIMx)
{
//#if defined(__MM3N1)
//  u32 rccEn[] = {RCC_APB2ENR_TIM1, RCC_APB1ENR_TIM2, RCC_APB1ENR_TIM3, RCC_APB1ENR_TIM4};
//#endif
//#if defined(__MM0N1) || defined(__MM0Q1)
//  u32 rccEn[] = {RCC_APB2ENR_TIM1, RCC_APB1ENR_TIM2, RCC_APB1ENR_TIM3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,RCC_APB2ENR_TIM14, 0, RCC_APB2ENR_TIM16, RCC_APB2ENR_TIM17};
//#endif
//#if defined(__MM0P1)
//  u32 rccEn[] = {RCC_APB2ENR_TIM1, RCC_APB1ENR_TIM2, RCC_APB1ENR_TIM3, 0, 0, 0, 0, RCC_APB2ENR_TIM8, 0, 0, 0, 0, 0, RCC_APB2ENR_TIM14, 0,RCC_APB2ENR_TIM16, RCC_APB2ENR_TIM17};
//#endif
////#if defined(__MM3O1)
////    u32 rccEn[] = {RCC_APB2ENR_TIM1, RCC_APB1ENR_TIM2, RCC_APB1ENR_TIM3, RCC_APB1ENR_TIM4, RCC_APB1ENR_TIM5, RCC_APB1ENR_TIM6, RCC_APB1ENR_TIM7, RCC_APB2ENR_TIM8, 0, 0, 0, 0, 0, RCC_APB2ENR_TIM14};
////#endif
//
//    u32 timEn = rccEn[idx];
//
//    if (timEn == 0) return;
//// ???chendo190129: Lookup function is unstable, so currently only supports judge...
//    ((u32)TIMx > APB2PERIPH_BASE) ? RCC_APB2PeriphClockCmd(timEn, ENABLE) :
//        RCC_APB1PeriphClockCmd(timEn, ENABLE);

    if (TIMx == TIM1)       COMMON_EnableIpClock(emCLOCK_TIM1);

    else if (TIMx == TIM2)  COMMON_EnableIpClock(emCLOCK_TIM2);
    else if (TIMx == TIM3)  COMMON_EnableIpClock(emCLOCK_TIM3);
#if defined(__MM3N1) || defined(__MM3O1)
    else if (TIMx == TIM4)  COMMON_EnableIpClock(emCLOCK_TIM4);
#endif

#if defined(__MM3O1)
    else if (TIMx == TIM5)  COMMON_EnableIpClock(emCLOCK_TIM5);
    else if (TIMx == TIM6)  COMMON_EnableIpClock(emCLOCK_TIM6);
    else if (TIMx == TIM7)  COMMON_EnableIpClock(emCLOCK_TIM7);
#endif

#if defined(__MM0P1) || defined(__MM3O1)
    else if (TIMx == TIM8)  COMMON_EnableIpClock(emCLOCK_TIM8);
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1) || defined(__MM3O1)
    else if (TIMx == TIM14) COMMON_EnableIpClock(emCLOCK_TIM14);
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    else if (TIMx == TIM16) COMMON_EnableIpClock(emCLOCK_TIM16);
    else if (TIMx == TIM17) COMMON_EnableIpClock(emCLOCK_TIM17);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable TIMER count.
/// @param  TIMx: where x can be 1, 2, 3, 4, 8, 14, 16 or 17 to select the TIMER peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_ON(TIM_TypeDef* TIMx)
{
    TIM_Cmd(TIMx, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Disable TIMER count.
/// @param  TIMx: where x can be 1, 2, 3, 4, 8, 14, 16 or 17 to select the TIMER peripheral.
/// @retval RCC_APBxENR_TIMx
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_OFF(TIM_TypeDef* TIMx)
{
    TIM_Cmd(TIMx, DISABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the TIMER Master Mode.
/// @param  TIMx: where x can be 1, 2, 3, 4, 5 or 8 to select the TIMER peripheral.
/// @param  src: specifies the Trigger Output source.
///   This paramter can be one of the following values:
///  - For all TIMx
///     @arg emTIM_Reset:  The UG bit in the TIM_EGR register is used as the trigger output (TRIG).
///     @arg emTIM_Enable: The Counter Enable CEN is used as the trigger output (TRIG).
///     @arg emTIM_Update: The update event is selected as the trigger output (TRIG).
///     @arg emTIM_CC1: The trigger output sends a positive pulse when the CC1IF flag
///                              is to be set, as soon as a capture or compare match occurs (TRIG).
///     @arg emTIM_OC1Ref: OC1REF signal is used as the trigger output (TRIG).
///     @arg emTIM_OC2Ref: OC2REF signal is used as the trigger output (TRIG).
///     @arg emTIM_OC3Ref: OC3REF signal is used as the trigger output (TRIG).
///     @arg emTIM_OC4Ref: OC4REF signal is used as the trigger output (TRIG).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_MasterSelect(TIM_TypeDef* TIMx, u8 src)
{
    // Select the Master Slave Mode
    TIM_SelectMasterSlaveMode(TIMx, TIM_MasterSlaveMode_Enable);

    u32 trgoSrc = src << 4;

    // Master Mode selection
    TIM_SelectOutputTrigger(TIMx, (TIMMMS_Typedef)trgoSrc);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the TIMER Slave Mode.
/// @param  TIMx: where x can be 1, 2, 3, 4, 5 or 8 to select the TIMER peripheral.
/// @param  mode: specifies the Timer Slave Mode.
///   This parameter can be one of the following values:
///     @arg TIM_SlaveMode_Reset: Rising edge of the selected trigger signal (TRGI) re-initializes
///                               the counter and triggers an update of the registers.
///     @arg TIM_SlaveMode_Gated:     The counter clock is enabled when the trigger signal (TRGI) is high.
///     @arg TIM_SlaveMode_Trigger:   The counter starts at a rising edge of the trigger TRGI.
///     @arg TIM_SlaveMode_External1: Rising edges of the selected trigger (TRGI) clock the counter.
/// @param  src: The Input Trigger source.
///   This parameter can be one of the following values:
///     @arg TIM_TS_ITR0: Internal Trigger 0
///     @arg TIM_TS_ITR1: Internal Trigger 1
///     @arg TIM_TS_ITR2: Internal Trigger 2
///     @arg TIM_TS_ITR3: Internal Trigger 3
///     @arg TIM_TS_TI1F_ED: TI1 Edge Detector
///     @arg TIM_TS_TI1FP1: Filtered Timer Input 1
///     @arg TIM_TS_TI2FP2: Filtered Timer Input 2
///     @arg TIM_TS_ETRF: External Trigger input
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_SlaveSelect(TIM_TypeDef* TIMx, TIMSMSMODE_Typedef mode, TIMTS_TypeDef src)
{
    // Slave Mode selection
    TIM_SelectSlaveMode(TIMx, mode);
    TIM_SelectInputTrigger(TIMx, src);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  configure TIMER update-Event function.
/// @param  TIMx: where x can be 1, 2, 3, 4, 8, 14, 16 or 17 to select the TIMER peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_NVIC_Update_Init(TIM_TypeDef* TIMx)
{
    u8 ch;
#if defined(__MM3N1)
    if (TIMx == TIM1)           ch = TIM1_UP_IRQn;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    if (TIMx == TIM1)           ch = TIM1_BRK_UP_TRG_COM_IRQn;
#endif
    else if (TIMx == TIM2)      ch = TIM2_IRQn;
    else if (TIMx == TIM3)      ch = TIM3_IRQn;

#if defined(__MM3N1)
    else if (TIMx == TIM4)      ch = TIM4_IRQn;
#endif

#if defined(__MM0P1)
    else if (TIMx == TIM8)      ch = TIM8_BRK_UP_TRG_COM_IRQn;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    else if (TIMx == TIM14)     ch = TIM14_IRQn;
    else if (TIMx == TIM16)     ch = TIM16_IRQn;
    else if (TIMx == TIM17)     ch = TIM17_IRQn;
#endif

    COMMON_NVIC_Configure(ch,  1,  1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  configure TIMER update-Event function.
/// @param  TIMx: where x can be 1, 2, 3, 4, 8, 14, 16 or 17 to select the TIM peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_NVIC_CC_Init(TIM_TypeDef* TIMx)
{
    u8 ch;
    if      (TIMx == TIM1)      ch = TIM1_CC_IRQn;
    else if (TIMx == TIM2)      ch = TIM2_IRQn;
    else if (TIMx == TIM3)      ch = TIM3_IRQn;
#if defined(__MM3N1)
    else if (TIMx == TIM4)      ch = TIM4_IRQn;
#endif

#if defined(__MM0P1)
    else if (TIMx == TIM8)      ch = TIM8_CC_IRQn;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    else if (TIMx == TIM14)     ch = TIM14_IRQn;
    else if (TIMx == TIM16)     ch = TIM16_IRQn;
    else if (TIMx == TIM17)     ch = TIM17_IRQn;
#endif

    COMMON_NVIC_Configure(ch,  1,  1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  configure TIMER update-Event function.
/// @param  TIMx: where x can be 1 or 8 to select the TIM peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_NVIC_COM_Init(TIM_TypeDef* TIMx)
{
    u8 ch;
    if (TIMx == TIM1)
#if defined(__MM3N1)
                                ch = TIM1_TRG_COM_IRQn;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                                ch = TIM1_BRK_UP_TRG_COM_IRQn;
#endif
#if defined(__MM0P1)
    else if (TIMx == TIM8)      ch = TIM8_BRK_UP_TRG_COM_IRQn;
#endif
    COMMON_NVIC_Configure(ch,  1,  1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Generate TIMER(1 or 8) COM Event.
/// @param  TIMx: where x can be 1 or 8 to select the TIM peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_Generate_COM_Event(TIM_TypeDef* TIMx)
{
    TIM_GenerateEvent(TIMx, TIM_EventSource_COM);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Step fuction.
/// @param  TIMx: where x can be 1 or 8 to select the TIM peripheral.
/// @param  step: num from 0 to 5
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_6Step(TIM_TypeDef* TIMx, u8 step)
{
    if (step == 0) {
        /////////////////// Next step: Step 1 Configuration ////////////////////
        // Channel1 configuration: CH1 output: PWM
        TIM_SelectOCxM(TIMx, TIM_Channel_1, TIM_OCMode_PWM1);
        TIM_CCxCmd (TIMx, TIM_Channel_1, TIM_CCx_Enable);
        TIM_CCxNCmd(TIMx, TIM_Channel_1, TIM_CCxN_Disable);

        // Channel2 configuration: CH2N output: 1
        TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_Active);
        TIM_CCxCmd (TIM1, TIM_Channel_2, TIM_CCx_Disable);
        TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);

        // Channel3 configuration: CH3 None
        TIM_CCxCmd (TIMx, TIM_Channel_3, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_3, TIM_CCxN_Disable);
    }
    else if (step == 1){
        /////////////////// Next step: Step 2 Configuration ////////////////////
        //  Channel1 configuration: CH1 output: PWM
        TIM_SelectOCxM(TIMx, TIM_Channel_1, TIM_OCMode_PWM1);
        TIM_CCxCmd (TIMx, TIM_Channel_1, TIM_CCx_Enable);
        TIM_CCxNCmd(TIMx, TIM_Channel_1, TIM_CCxN_Disable);

        //  Channel2 configuration: CH2 None
        TIM_CCxCmd (TIMx, TIM_Channel_2, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_2, TIM_CCxN_Disable);

        //  Channel3 configuration: CH3N output: 1
        TIM_SelectOCxM(TIMx, TIM_Channel_3, TIM_OCMode_Active);
        TIM_CCxCmd (TIMx, TIM_Channel_3, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_3, TIM_CCxN_Enable);
    }
    else if (step == 2){
        /////////////////// Next step: Step 3 Configuration ////////////////////
        // Channel1 configuration: CH1 None
        TIM_CCxCmd (TIMx, TIM_Channel_1, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_1, TIM_CCxN_Disable);

        // Channel2 configuration: CH2 output: PWM
        TIM_SelectOCxM(TIMx, TIM_Channel_2, TIM_OCMode_PWM1);
        TIM_CCxCmd (TIMx, TIM_Channel_2, TIM_CCx_Enable);
        TIM_CCxNCmd(TIMx, TIM_Channel_2, TIM_CCxN_Disable);

        // Channel3 configuration: CH3N output: 1
        TIM_SelectOCxM(TIMx, TIM_Channel_3, TIM_OCMode_Active);
        TIM_CCxCmd (TIMx, TIM_Channel_3, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_3, TIM_CCxN_Enable);
    }
    else if (step == 3) {
        /////////////////// Next step: Step 4 Configuration ////////////////////
        // Channel1 configuration: CH1N output: 1
        TIM_SelectOCxM(TIMx, TIM_Channel_1, TIM_OCMode_Active);
        TIM_CCxCmd (TIMx, TIM_Channel_1, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_1, TIM_CCxN_Enable);

        // Channel2 configuration: CH2 output: PWM
        TIM_SelectOCxM(TIMx, TIM_Channel_2, TIM_OCMode_PWM1);
        TIM_CCxCmd (TIMx, TIM_Channel_2, TIM_CCx_Enable);
        TIM_CCxNCmd(TIMx, TIM_Channel_2, TIM_CCxN_Disable);

        // Channel3 configuration: CH3 None
        TIM_CCxCmd (TIMx, TIM_Channel_3, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_3, TIM_CCxN_Disable);
    }
    else if (step == 4) {
        /////////////////// Next step: Step 5 Configuration ////////////////////
        // Channel1 configuration: CH1N output: 1
        TIM_SelectOCxM(TIMx, TIM_Channel_1, TIM_OCMode_Active);
        TIM_CCxCmd (TIMx, TIM_Channel_1, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_1, TIM_CCxN_Enable);

        // Channel2 configuration: CH2 None
        TIM_CCxCmd (TIMx, TIM_Channel_2, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_2, TIM_CCxN_Disable);

        // Channel3 configuration: CH3 output: PWM
        TIM_SelectOCxM(TIMx, TIM_Channel_3, TIM_OCMode_PWM1);
        TIM_CCxCmd (TIMx, TIM_Channel_3, TIM_CCx_Enable);
        TIM_CCxNCmd(TIMx, TIM_Channel_3, TIM_CCxN_Disable);
    }
    else {
        /////////////////// Next step: Step 0 Configuration ////////////////////
        // Channel1 configuration: CH1 None
        TIM_CCxCmd (TIMx, TIM_Channel_1, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_1, TIM_CCxN_Disable);

        // Channel2 configuration: CH2N output: 1
        TIM_SelectOCxM(TIMx, TIM_Channel_2, TIM_OCMode_Active);
        TIM_CCxCmd (TIMx, TIM_Channel_2, TIM_CCx_Disable);
        TIM_CCxNCmd(TIMx, TIM_Channel_2, TIM_CCxN_Enable);

        // Channel3 configuration: CH3 output: PWM
        TIM_SelectOCxM(TIMx, TIM_Channel_3, TIM_OCMode_PWM1);
        TIM_CCxCmd (TIMx, TIM_Channel_3, TIM_CCx_Enable);
        TIM_CCxNCmd(TIMx, TIM_Channel_3, TIM_CCxN_Disable);
    }
}

////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_CapturePWM(TIM_TypeDef* TIMx, u8 idx)
{
    if (TIM_GetITStatus(TIMx, TIM_IT_CC1) != RESET) {
        //--- deal with pwm data ---
        gGetPeriod = (u32)TIM_GetCapture1(TIMx) + 1;
        gGetDutyCycle = ((u32)TIM_GetCapture2(TIMx) + 1) * 100 / ((u32)TIM_GetCapture1(TIMx) + 1);

        //---- synchro callback: timer CC1 ---
        if (instance[idx].sync == emTYPE_Sync) {
            ((fpTIM)instance[idx].cbCc)(idx);
        }
        TIM_ClearITPendingBit(TIMx, TIM_IT_CC1);
    }
    if (TIM_GetITStatus(TIMx, TIM_IT_Update) != RESET) {
        //---- synchro callback: timer update --- ???chendo:not support
        ///if (instance[idx].sync == emTYPE_Sync) {
        ///    ((fpTIM)instance[idx].syncUp)(idx);
        ///}
        TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
    }
}

////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_CapturePulse(TIM_TypeDef* TIMx, u8 idx)
{
    if (TIM_GetITStatus(TIMx, TIM_IT_Update) != RESET) {
        //--- deal with pulse overflow cnt -----------------------------------//
        capValueStructure.capPeriod ++;

        //---- synchro callback: timer update --------------------------------//
        if (instance[idx].sync == emTYPE_Sync) {
            ((fpTIM)instance[idx].cbUp)(idx);
        }

        TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
    }
    if (TIM_GetITStatus(TIMx, TIM_IT_CC2) != RESET) {
        //--- deal with first capture pulse: Rising edge ---------------------//
        if (!capValueStructure.capStartFlag) {
            TIM_SetCounter(TIMx, 0);
            exTIM_SetIC2Plority(TIMx, TIM_ICPolarity_Falling);                  // change capture: --|__
            capValueStructure.capPeriod     = 0;
            capValueStructure.capCCRValue   = 0;
            capValueStructure.capStartFlag  = 1;
        }
        //--- deal with second capture pulse: Falling edge -------------------//
        else {
            capValueStructure.capCCRValue   = TIM_GetCapture2(TIMx);
            capValueStructure.capFinishFlag = 1;
            capValueStructure.capStartFlag  = 0;
            exTIM_SetIC2Plority(TIMx, TIM_ICPolarity_Rising);                   // change capture: __|--
        }
        //---- synchro callback: timer CC2 ---
        if (instance[idx].sync == emTYPE_Sync) {
            ((fpTIM)instance[idx].cbCc)(idx);
        }
        TIM_ClearITPendingBit(TIMx, TIM_IT_CC2);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  TIMER Update or Compare/Capture IRQHandler function
/// @param  idx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_UpCC_Common(u8 idx)
{
    TIM_TypeDef* TIMx = (TIM_TypeDef*)instance[idx].sPrefix.pBase;

    //--- capture pwm mode IRQ handle ----------------------------------------//
    if (instance[idx].mode == emTIM_CapturePWM)
        DRV_TIM_CapturePWM(TIMx, idx);

    //--- capture pulse mode IRQ handle --------------------------------------//
    if (instance[idx].mode == emTIM_CapturePulse)
        DRV_TIM_CapturePulse(TIMx, idx);

    //--- other mode IRQ handle ----
    else {
        if (TIM_GetITStatus(TIMx, TIM_IT_Update) != RESET) {
            //---- synchro callback: timer update ----------------------------//
            if (instance[idx].sync == emTYPE_Sync) {
                ((fpTIM)instance[idx].cbUp)(idx);
            }
            TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
        }
        if (TIM_GetITStatus(TIMx, TIM_IT_CC1) != RESET) {
            //---- synchro callback: timer CC1 -------------------------------//
            if (instance[idx].sync == emTYPE_Sync) {
                ((fpTIM)instance[idx].cbCc)(idx);
            }
            TIM_ClearITPendingBit(TIMx, TIM_IT_CC1);
        }
        else if (TIM_GetITStatus(TIMx, TIM_IT_CC2) != RESET) {
            //---- synchro callback: timer CC2 -------------------------------//
            if (instance[idx].sync == emTYPE_Sync) {
                ((fpTIM)instance[idx].cbCc)(idx);
            }
            TIM_ClearITPendingBit(TIMx, TIM_IT_CC2);
        }
        else if (TIM_GetITStatus(TIMx, TIM_IT_CC3) != RESET) {
            //---- synchro callback: timer CC3 -------------------------------//
            if (instance[idx].sync == emTYPE_Sync) {
                ((fpTIM)instance[idx].cbCc)(idx);
            }
            TIM_ClearITPendingBit(TIMx, TIM_IT_CC3);
        }
        else if (TIM_GetITStatus(TIMx, TIM_IT_CC4) != RESET) {
            //---- synchro callback: timer CC4 -------------------------------//
            if (instance[idx].sync == emTYPE_Sync) {
                ((fpTIM)instance[idx].cbCc)(idx);
            }
            TIM_ClearITPendingBit(TIMx, TIM_IT_CC4);
        }
#if defined(__MM0P1) || defined(__MM0Q1)
        else if (TIM_GetITStatus(TIMx, TIM_IT_CC5) != RESET) {
            //---- synchro callback: timer CC5 -------------------------------//
            if (instance[idx].sync == emTYPE_Sync) {
                ((fpTIM)instance[idx].cbCc)(idx);
            }
            TIM_ClearITPendingBit(TIMx, TIM_IT_CC5);
        }
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////

#if defined(__EX_TIM)
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM3N1)
void TIM1_UP_IRQHandler(void)                                                   // TIM1 Update Interrupt Handler
{
    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[0]].sPrefix.subIdx);
}
#endif

//#if defined(__MM3N1)
//void TIM1_TRG_COM_IRQHandler(void)                                            // TIM1 COM Interrupt Handler
//{
//    DRV_TIM_6Step(TIM1);
//}
//#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[0]].sPrefix.subIdx);
}
#endif

////////////////////////////////////////////////////////////////////////////////
void TIM1_CC_IRQHandler(void)                                                   // TIM1 CC Interrupt Handler
{

    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[0]].sPrefix.subIdx);
}

////////////////////////////////////////////////////////////////////////////////
#if defined(__MM0P1)
void TIM8_BRK_UP_TRG_COM_IRQHandler(void)                                       // TIM8 Update Interrupt Handler
{
    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[7]].sPrefix.subIdx);
}

////////////////////////////////////////////////////////////////////////////////
void TIM8_CC_IRQHandler(void)                                                   // TIM8 CC Interrupt Handler
{
    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[7]].sPrefix.subIdx);
}
#endif

////////////////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler()                                                          // TIM2 Interrupt Handler
{
    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[1]].sPrefix.subIdx);
}

////////////////////////////////////////////////////////////////////////////////
void TIM3_IRQHandler()                                                          // TIM3 Interrupt Handler
{
    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[2]].sPrefix.subIdx);
}

////////////////////////////////////////////////////////////////////////////////
#if defined(__MM3N1)
void TIM4_IRQHandler()                                                          // TIM4 Interrupt Handler
{
    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[3]].sPrefix.subIdx);
}
#endif

////////////////////////////////////////////////////////////////////////////////
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
void TIM14_IRQHandler()                                                         // TIM14 Interrupt Handler
{
    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[13]].sPrefix.subIdx);
}

void TIM16_IRQHandler()                                                         // TIM16 Interrupt Handler
{
    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[15]].sPrefix.subIdx);
}

void TIM17_IRQHandler()                                                         // TIM17 Interrupt Handler
{
    DRV_TIM_UpCC_Common(instance[tbSubHandleIdx[16]].sPrefix.subIdx);
}

#endif  // defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)

#endif  // defined(__EX_TIM)

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the DMA channel of TIMx.
/// @param  dcb : device control block.
/// @retval dma channel
////////////////////////////////////////////////////////////////////////////////
DMA_Channel_TypeDef* DRV_TIM_GetDMAChannel(tDRV_TIM_DCB* dcb)
{
    DMA_Channel_TypeDef* ch[] = {DMA1_ch1, DMA1_ch2, DMA1_ch3, DMA1_ch4, DMA1_ch5
#if defined(__MM3N1)
        , DMA1_ch6, DMA1_ch7
#endif
    };
#if defined(__MM3N1)
    if (dcb->tim == TIM1) {
        if (dcb->dmaSrc == emTIM_DMA_Up)     return ch[4];
        if (dcb->dmaSrc == emTIM_DMA_CC1)    return ch[1];
        if (dcb->dmaSrc == emTIM_DMA_CC2)    return ch[2];
        if (dcb->dmaSrc == emTIM_DMA_CC3)    return ch[5];
        if (dcb->dmaSrc == emTIM_DMA_CC4)    return ch[3];
    }
    if (dcb->tim == TIM2) {
        if (dcb->dmaSrc == emTIM_DMA_Up)     return ch[1];
        if (dcb->dmaSrc == emTIM_DMA_CC1)    return ch[4];
        if (dcb->dmaSrc == emTIM_DMA_CC2)    return ch[6];
        if (dcb->dmaSrc == emTIM_DMA_CC3)    return ch[0];
        if (dcb->dmaSrc == emTIM_DMA_CC4)    return ch[6];
    }
    if (dcb->tim == TIM3) {
        if (dcb->dmaSrc == emTIM_DMA_Up)     return ch[2];
        if (dcb->dmaSrc == emTIM_DMA_CC1)    return ch[5];
        if (dcb->dmaSrc == emTIM_DMA_CC3)    return ch[1];
        if (dcb->dmaSrc == emTIM_DMA_CC4)    return ch[2];
    }
    if (dcb->tim == TIM4) {
        if (dcb->dmaSrc == emTIM_DMA_Up)     return ch[6];
        if (dcb->dmaSrc == emTIM_DMA_CC1)    return ch[0];
        if (dcb->dmaSrc == emTIM_DMA_CC2)    return ch[3];
        if (dcb->dmaSrc == emTIM_DMA_CC3)    return ch[4];
    }
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    if (dcb->tim == TIM1) {
        if (dcb->dmaSrc == emTIM_DMA_Up)     return ch[4];
        if (dcb->dmaSrc == emTIM_DMA_CC1)    return ch[1];
        if (dcb->dmaSrc == emTIM_DMA_CC2)    return ch[2];
        if (dcb->dmaSrc == emTIM_DMA_CC3)    return ch[4];
        if (dcb->dmaSrc == emTIM_DMA_CC4)    return ch[3];
    }
    if (dcb->tim == TIM2) {
        if (dcb->dmaSrc == emTIM_DMA_Up)     return ch[1];
        if (dcb->dmaSrc == emTIM_DMA_CC1)    return ch[4];
        if (dcb->dmaSrc == emTIM_DMA_CC2)    return ch[2];
        if (dcb->dmaSrc == emTIM_DMA_CC3)    return ch[0];
        if (dcb->dmaSrc == emTIM_DMA_CC4)    return ch[3];
    }
    if (dcb->tim == TIM3) {
        if (dcb->dmaSrc == emTIM_DMA_Up)     return ch[2];
        if (dcb->dmaSrc == emTIM_DMA_CC1)    return ch[3];
        if (dcb->dmaSrc == emTIM_DMA_CC3)    return ch[1];
        if (dcb->dmaSrc == emTIM_DMA_CC4)    return ch[2];
    }
    if (dcb->tim == TIM16) {
        if (dcb->dmaSrc == emTIM_DMA_Up)     return ch[2];
        if (dcb->dmaSrc == emTIM_DMA_CC1)    return ch[2];
    }
    if (dcb->tim == TIM17) {
        if (dcb->dmaSrc == emTIM_DMA_Up)     return ch[0];
        if (dcb->dmaSrc == emTIM_DMA_CC1)    return ch[0];
    }
#endif
#if defined(__MM0P1)
    if (dcb->tim == TIM8) {
        if (dcb->dmaSrc == emTIM_DMA_Up)     return ch[0];
        if (dcb->dmaSrc == emTIM_DMA_CC1)    return ch[2];
        if (dcb->dmaSrc == emTIM_DMA_CC2)    return ch[4];
        if (dcb->dmaSrc == emTIM_DMA_CC3)    return ch[0];
        if (dcb->dmaSrc == emTIM_DMA_CC4)    return ch[1];
    }
#endif
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the DMA channel for TIMx.
/// @param  dcb :  device control block.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_DMA_ConfigChannel(tDRV_TIM_DCB* dcb)
{
    if (DRV_TIM_GetDMAChannel(dcb) == 0) return;

    tDRV_DMA_DCB dma = {
        .MemoryBaseAddr     = (u32)&dmaRead,
        .PeripheralBaseAddr = (u32)&RCC->CR,
        .BufferSize         = 10,
        .PeripheralDataSize = DMA_PeripheralDataSize_Word,
        .MemoryDataSize     = DMA_MemoryDataSize_Word,
        .Mode               = DMA_Mode_Circular,                            // DMA_Mode_Circular, DMA_Mode_Normal
        .channel                = DRV_TIM_GetDMAChannel(dcb),
        .dmaDir                 = emDMA_p2m,
        .PeripheralInc      = DMA_PeripheralInc_Disable,
        .MemoryInc          = DMA_MemoryInc_Enable,
        .Priority           = DMA_Priority_VeryHigh
    };

    DRV_DMA_Init(&dma);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the PWM mode of TIMx Channel-1.
/// @param  dcb : device control block.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_PWM_CH1_Init(tDRV_TIM_DCB* dcb)
{
    TIM_TimeBaseInitTypeDef pBase = {
        .TIM_Prescaler         = dcb->psc,
        .TIM_Period            = dcb->arr,
        .TIM_ClockDivision     = TIM_CKD_DIV1,
        .TIM_CounterMode       = TIM_CounterMode_Up,
        .TIM_RepetitionCounter = 0
    };

    TIM_OCInitTypeDef pOC = {
        .TIM_Pulse        = dcb->pulse,
        .TIM_OCMode       = TIM_OCMode_PWM1,
        .TIM_OutputState  = TIM_OutputState_Enable,
        .TIM_OutputNState = TIM_OutputNState_Disable,
        .TIM_OCPolarity   = TIM_OCPolarity_High,
        .TIM_OCNPolarity  = TIM_OCNPolarity_High,
        .TIM_OCIdleState  = TIM_OCIdleState_Reset,
        .TIM_OCNIdleState = TIM_OCNIdleState_Reset
    };

    TIM_TimeBaseInit(dcb->tim, &pBase);

    if (dcb->ch == emTIM_CH1) {
        TIM_OC1Init(dcb->tim, &pOC);
        TIM_ITConfig(dcb->tim, TIM_IT_CC1, ENABLE);
    }
    if (dcb->ch == emTIM_CH2) {
        TIM_OC2Init(dcb->tim, &pOC);
        TIM_ITConfig(dcb->tim, TIM_IT_CC2, ENABLE);
    }
    if (dcb->ch == emTIM_CH3) {
        TIM_OC3Init(dcb->tim, &pOC);
        TIM_ITConfig(dcb->tim, TIM_IT_CC3, ENABLE);
    }
    if (dcb->ch == emTIM_CH4) {
        TIM_OC4Init(dcb->tim, &pOC);
        TIM_ITConfig(dcb->tim, TIM_IT_CC4, ENABLE);
    }
    TIM_ITConfig(dcb->tim, TIM_IT_Update, ENABLE);

    TIM_CCPreloadControl(dcb->tim, ENABLE);
    TIM_CtrlPWMOutputs(dcb->tim, ENABLE);

    if (dcb->type == emTYPE_IT) {
        DRV_TIM_NVIC_Update_Init(dcb->tim);
        DRV_TIM_NVIC_CC_Init(dcb->tim);
    }

    else if (dcb->type == emTYPE_DMA) {//???chendo0301: only support CH1
        dcb->dmaSrc = emTIM_DMA_CC1;                                            // emTIM_Up, emTIM_CC1 ....
        DRV_TIM_DMA_ConfigChannel(dcb);
        TIM_DMACmd(dcb->tim, TIM_DMA_CC1, ENABLE);                              // TIM_DMA_Update, TIM_DMA_CC1, TIM_DMA_CC2..
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the timer counter direction.
/// @param  TIMx: where x can be 1, 2, 3, 4, 8 to select the TIMER peripheral.
/// @retval encoderStructure.encoderDir : dir 1(count down): 0(count up)
///         encoderStructure.encoderCnt : count (0: TIMx.period)
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_GetEncoder(TIM_TypeDef* TIMx)
{
    encoderStructure.encoderDir = (inp((u32*)TIMx) & 0x10) ? 1 : 0;             //0: count up ... 1: count down
    encoderStructure.encoderCnt = TIM_GetCounter(TIMx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the CapturePulse mode of TIMx Channel-2.
/// @param  dcb :  device control block.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_CapturePulseInit(tDRV_TIM_DCB* dcb)
{
    TIM_TimeBaseInitTypeDef pBase = {
        .TIM_Prescaler         = dcb->psc,
        .TIM_Period            = dcb->arr,
        .TIM_ClockDivision     = TIM_CKD_DIV1,
        .TIM_CounterMode       = TIM_CounterMode_Up,
        .TIM_RepetitionCounter = 0
    };

    TIM_ICInitTypeDef pIC = {
        .TIM_Channel     = TIM_Channel_2,                                       // select: TIM_Channel_1, TIM_Channel_2, TIM_Channel_3, TIM_Channel_4
        .TIM_ICFilter    = 0,
        .TIM_ICSelection = TIM_ICSelection_DirectTI,                            // select: TIM_ICSelection_DirectTI, TIM_ICSelection_IndirectTI
        .TIM_ICPrescaler = TIM_ICPSC_DIV1,
        .TIM_ICPolarity  = TIM_ICPolarity_Rising                                // select: TIM_ICPolarity_Rising, TIM_ICPolarity_Falling
    };

    TIM_TimeBaseInit(dcb->tim, &pBase);
    TIM_ICInit(dcb->tim, &pIC);

    DRV_TIM_NVIC_Update_Init(dcb->tim);
    DRV_TIM_NVIC_CC_Init(dcb->tim);
    TIM_ITConfig(dcb->tim, (TIMIT_TypeDef)(TIM_IT_Update | TIM_IT_CC2), ENABLE);
    TIM_ClearITPendingBit(dcb->tim, (TIMIT_TypeDef)(TIM_IT_Update | TIM_IT_CC2));

    if (dcb->type == emTYPE_DMA) {
        dcb->dmaSrc = emTIM_DMA_Up;                                                 // emTIM_Up, emTIM_CC1 ....
        DRV_TIM_DMA_ConfigChannel(dcb);
        TIM_DMACmd(dcb->tim, TIM_DMA_Update, ENABLE);                           // TIM_DMA_Update, TIM_DMA_CC1, TIM_DMA_CC2..
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the CapturePWM mode of TIMx Channel-2.
/// @param  dcb :  device control block.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_CapturePWMInit(tDRV_TIM_DCB* dcb)
{
    TIM_TimeBaseInitTypeDef pBase = {
        .TIM_Prescaler         = dcb->psc,
        .TIM_Period            = dcb->arr,
        .TIM_ClockDivision     = TIM_CKD_DIV1,
        .TIM_CounterMode       = TIM_CounterMode_Up,
        .TIM_RepetitionCounter = 0
    };

    TIM_ICInitTypeDef pIC = {
        .TIM_Channel     = TIM_Channel_1,                                       // select: TIM_Channel_1, TIM_Channel_2
        .TIM_ICFilter    = 0,
        .TIM_ICSelection = TIM_ICSelection_DirectTI,                            // select: TIM_ICSelection_DirectTI, TIM_ICSelection_IndirectTI
        .TIM_ICPrescaler = TIM_ICPSC_DIV1,
        .TIM_ICPolarity  = TIM_ICPolarity_Rising                                // select: TIM_ICPolarity_Rising, TIM_ICPolarity_Falling
    };

    TIM_TimeBaseInit(dcb->tim, &pBase);
    TIM_PWMIConfig(dcb->tim, &pIC);

    DRV_TIM_SlaveSelect(dcb->tim, TIM_SlaveMode_Reset, TIM_TS_TI1FP1);
    TIM_SelectMasterSlaveMode(dcb->tim, TIM_MasterSlaveMode_Enable);

    DRV_TIM_NVIC_Update_Init(dcb->tim);
    DRV_TIM_NVIC_CC_Init(dcb->tim);
    TIM_ITConfig(dcb->tim, TIM_IT_CC1, ENABLE);
    TIM_ClearITPendingBit(dcb->tim, TIM_IT_CC1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the Encoder mode of TIMx.
/// @param  dcb : device control block.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_EncoderInit(tDRV_TIM_DCB* dcb)
{
    TIM_TimeBaseInitTypeDef pBase = {
        .TIM_Prescaler         = dcb->psc,
        .TIM_Period            = dcb->arr,
        .TIM_ClockDivision     = TIM_CKD_DIV1,
        .TIM_CounterMode       = TIM_CounterMode_Up,
        .TIM_RepetitionCounter = 0
    };

    TIM_ICInitTypeDef pIC = {
        .TIM_Channel     = TIM_Channel_1,                                       // select: TIM_Channel_1, TIM_Channel_2
        .TIM_ICFilter    = 2,
        .TIM_ICSelection = TIM_ICSelection_DirectTI,                            // select: TIM_ICSelection_DirectTI, TIM_ICSelection_IndirectTI
        .TIM_ICPrescaler = TIM_ICPSC_DIV1,
        .TIM_ICPolarity  = TIM_ICPolarity_Rising                                // select: TIM_ICPolarity_Rising, TIM_ICPolarity_Falling
    };

    TIM_TimeBaseInit(dcb->tim, &pBase);
    TIM_EncoderInterfaceConfig(dcb->tim, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);  //TIM_ICPolarity_Rising, TIM_ICPolarity_Falling, TIM_ICPolarity_BothEdge

    TIM_ICInit(dcb->tim, &pIC);
    pIC.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(dcb->tim, &pIC);

    TIM_SetCounter(dcb->tim, 0);

    DRV_TIM_NVIC_Update_Init(dcb->tim);
    TIM_ITConfig(dcb->tim, TIM_IT_Update, ENABLE);
    TIM_ClearITPendingBit(dcb->tim, TIM_IT_Update);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the Encoder mode of TIMx.
/// @param  dcb : device control block.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_OnePulseInit(tDRV_TIM_DCB* dcb)
{
    TIM_TimeBaseInitTypeDef pBase = {
        .TIM_Prescaler         = dcb->psc,
        .TIM_Period            = dcb->arr,
        .TIM_ClockDivision     = TIM_CKD_DIV1,
        .TIM_CounterMode       = TIM_CounterMode_Up,
        .TIM_RepetitionCounter = 0
    };

    TIM_OCInitTypeDef pOC = {
        .TIM_Pulse        = dcb->pulse,
        .TIM_OCMode       = TIM_OCMode_PWM1,
        .TIM_OutputState  = TIM_OutputState_Enable,
        .TIM_OCPolarity   = TIM_OCPolarity_Low,
        .TIM_OCIdleState  = TIM_OCIdleState_Reset
    };

    // if use timer channel 2 to trigger onePulse mode, please undo
    //TIM_ICInitTypeDef pIC = {
    //  .TIM_Channel     = TIM_Channel_2,                                       // select: TIM_Channel_1, TIM_Channel_2
    //  .TIM_ICFilter    = 0,
    //  .TIM_ICSelection = TIM_ICSelection_DirectTI,                            // select: TIM_ICSelection_DirectTI, TIM_ICSelection_IndirectTI
    //  .TIM_ICPrescaler = TIM_ICPSC_DIV1,
    //  .TIM_ICPolarity  = TIM_ICPolarity_Rising                                // select: TIM_ICPolarity_Rising, TIM_ICPolarity_Falling
    //};
    //TIM_ICInit(dcb->tim, &pIC);
    //DRV_TIM_SlaveInit(dcb->tim, TIM_SlaveMode_Trigger, TIM_TS_TI2FP2);//config slave mode

    TIM_TimeBaseInit(dcb->tim, &pBase);

    if (dcb->ch & (u8)emTIM_CH1) {
        TIM_OC1Init(dcb->tim, &pOC);
        TIM_ITConfig(dcb->tim, TIM_IT_CC1, ENABLE);
    }
    if (dcb->ch & (u8)emTIM_CH2) {
        TIM_OC2Init(dcb->tim, &pOC);
        TIM_ITConfig(dcb->tim, TIM_IT_CC2, ENABLE);
    }
    if (dcb->ch & (u8)emTIM_CH3) {
        TIM_OC3Init(dcb->tim, &pOC);
        TIM_ITConfig(dcb->tim, TIM_IT_CC3, ENABLE);
    }
    if (dcb->ch & (u8)emTIM_CH4) {
        TIM_OC4Init(dcb->tim, &pOC);
        TIM_ITConfig(dcb->tim, TIM_IT_CC4, ENABLE);
    }

    TIM_SelectOnePulseMode(dcb->tim, TIM_OPMode_Single); // config OnePulse mode

    DRV_TIM_NVIC_Update_Init(dcb->tim);
    DRV_TIM_NVIC_CC_Init(dcb->tim);

    TIM_ITConfig(dcb->tim, TIM_IT_Update, ENABLE);

    TIM_ClearITPendingBit(dcb->tim, (TIMIT_TypeDef)(0xFF)); // clear all flag
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the 6-step mode of TIMx.
/// @param  dcb : device control block.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_6StepInit(tDRV_TIM_DCB* dcb)
{
    TIM_TimeBaseInitTypeDef pBase = {
        .TIM_Prescaler         = dcb->psc,
        .TIM_Period            = dcb->arr,
        .TIM_ClockDivision     = TIM_CKD_DIV1,
        .TIM_CounterMode       = TIM_CounterMode_CenterAligned3,
        .TIM_RepetitionCounter = 0
    };

    TIM_OCInitTypeDef pOC = {
        .TIM_Pulse        = dcb->pulse,
        .TIM_OCMode       = TIM_OCMode_PWM1,
        .TIM_OutputState  = TIM_OutputState_Enable,
        .TIM_OutputNState = TIM_OutputNState_Enable,
        .TIM_OCPolarity   = TIM_OCPolarity_High,
        .TIM_OCNPolarity  = TIM_OCNPolarity_High,
        .TIM_OCIdleState  = TIM_OCIdleState_Set,
        .TIM_OCNIdleState = TIM_OCNIdleState_Set
    };

    TIM_BDTRInitTypeDef pBDTR = {
        .TIM_DeadTime        = 0xFF,
        .TIM_Break           = TIM_Break_Disable,
        .TIM_LOCKLevel       = TIM_LOCKLevel_OFF,
        .TIM_OSSRState       = TIM_OSSRState_Disable,
        .TIM_OSSIState       = TIM_OSSIState_Disable,
        .TIM_BreakPolarity   = TIM_BreakPolarity_Low,
        .TIM_AutomaticOutput = TIM_AutomaticOutput_Enable
    };

    TIM_TimeBaseInit(dcb->tim, &pBase);
    TIM_OC1Init(dcb->tim, &pOC);
    TIM_OC2Init(dcb->tim, &pOC);
    TIM_OC3Init(dcb->tim, &pOC);
    TIM_BDTRConfig(dcb->tim, &pBDTR);

    TIM_CCPreloadControl(dcb->tim, ENABLE);

    TIM_CtrlPWMOutputs(dcb->tim, ENABLE);

    //DRV_TIM_NVIC_COM_Init(dcb->tim);
    //TIM_ITConfig(dcb->tim, TIM_IT_COM, ENABLE);
    //TIM_ClearITPendingBit(dcb->tim, TIM_IT_COM);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the Encoder mode of TIMx.
/// @param  dcb : device control block.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_MasterInit(tDRV_TIM_DCB* dcb)
{
    TIM_TimeBaseInitTypeDef pBase = {
        .TIM_Prescaler         = dcb->psc,
        .TIM_Period            = dcb->arr,
        .TIM_ClockDivision     = TIM_CKD_DIV1,
        .TIM_CounterMode       = TIM_CounterMode_Up,
        .TIM_RepetitionCounter = 0
    };

    TIM_OCInitTypeDef pOC = {
        .TIM_Pulse        = dcb->pulse,
        .TIM_OCMode       = TIM_OCMode_PWM1,
        .TIM_OutputState  = TIM_OutputState_Enable,
        .TIM_OCPolarity   = TIM_OCPolarity_Low,
        .TIM_OCIdleState  = TIM_OCIdleState_Reset
    };

    TIM_TimeBaseInit(dcb->tim, &pBase);

    if (dcb->ch & (u8)emTIM_CH1)
        TIM_OC1Init(dcb->tim, &pOC);
    if (dcb->ch & (u8)emTIM_CH2)
        TIM_OC2Init(dcb->tim, &pOC);
    if (dcb->ch & (u8)emTIM_CH3)
        TIM_OC3Init(dcb->tim, &pOC);
    if (dcb->ch & (u8)emTIM_CH4)
        TIM_OC4Init(dcb->tim, &pOC);
#if defined(__MM0P1) || defined(__MM0Q1)
    if (dcb->ch & (u8)emTIM_CH5)
        exTIM_OC5Init(dcb->tim, &pOC);
#endif

    TIM_ClearFlag(dcb->tim, (TIMFLAG_Typedef)(0xFF));   // clear all flag
    DRV_TIM_MasterSelect(dcb->tim, dcb->trgo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the TIMER.
/// @param  dcb : device control block.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_TIM_Init(tDRV_TIM_DCB* dcb)
{
    // if APBxPsc = HCLK / 1, timerClock = APBxPsc; else timerClock = APBxPsc * 2;
    u32 timclk;
    RCC_ClocksTypeDef rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);

    if ((u32)dcb->tim >= APB2PERIPH_BASE) {
        if (rcc_clocks.PCLK2_Frequency == rcc_clocks.HCLK_Frequency)
            timclk = rcc_clocks.PCLK2_Frequency;
        else
            timclk = rcc_clocks.PCLK2_Frequency * 2 / rcc_clocks.HCLK_Frequency;
    }
    else {
        if (rcc_clocks.PCLK1_Frequency == rcc_clocks.HCLK_Frequency)
            timclk = rcc_clocks.PCLK1_Frequency;
        else
            timclk = rcc_clocks.PCLK1_Frequency * 2 / rcc_clocks.HCLK_Frequency;
    }

    dcb->psc = timclk / dcb->cntFreq - 1;

    //------------------- Enable timer clock ---------------------------------//
    DRV_TIM_ClockEnable(dcb->tim);

    //------------------- Configure timer dcb --------------------------------//
    if (dcb->mode == emTIM_PWM) {
        BSP_TIM_PWM_GPIO_Configure(dcb->tim, dcb->ch, dcb->remapEn, dcb->remapIdx);
        DRV_TIM_PWM_CH1_Init(dcb);
    }
    if (dcb->mode == emTIM_CapturePulse) {
        memset(&capValueStructure, 0 , sizeof(capValueStructure));
        BSP_TIM_CapturePulse_GPIO_Configure(dcb->tim, dcb->ch, dcb->remapEn, dcb->remapIdx);
        DRV_TIM_CapturePulseInit(dcb);
    }
    if (dcb->mode == emTIM_CapturePWM) {
        BSP_TIM_CapturePWM_GPIO_Configure(dcb->tim, dcb->ch, dcb->remapEn, dcb->remapIdx);
        DRV_TIM_CapturePWMInit(dcb);
    }
    if (dcb->mode == emTIM_Encoder) {
        BSP_TIM_Encoder_GPIO_Configure(dcb->tim, dcb->ch, dcb->remapEn, dcb->remapIdx);
        DRV_TIM_EncoderInit(dcb);
    }
    if (dcb->mode == emTIM_OnePulse) {
        dcb->pulse = 1;                             // pwm1 mode(low Active):  _------
        dcb->arr += dcb->pulse;                     // ccrValue: pulse(=1) ==> ARR = dcb->arr + pulse
        BSP_TIM_OnePulse_GPIO_Configure(dcb->tim, dcb->ch, dcb->remapEn, dcb->remapIdx);
        DRV_TIM_OnePulseInit(dcb);
    }
    if (dcb->mode == emTIM_6Step) {
        BSP_TIM_6Step_GPIO_Configure(dcb->tim, dcb->ch, dcb->remapEn, dcb->remapIdx);
        DRV_TIM_6StepInit(dcb);
    }
    if (dcb->mode == emTIM_Master) {
        DRV_TIM_MasterInit(dcb);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of TIM.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_TIM_DCB* pDcb, u8 idx)
{
    instance[idx].mode      = pDcb->mode;
    instance[idx].cntFreq   = pDcb->cntFreq;
    instance[idx].period    = pDcb->period;
    instance[idx].cbUp      = pDcb->cbUp;
    instance[idx].cbCc      = pDcb->cbCc;
    instance[idx].sync      = pDcb->sync;
    instance[idx].type      = pDcb->type;
    instance[idx].remapEn   = pDcb->remapEn;
    instance[idx].remapIdx  = pDcb->remapIdx;
    instance[idx].trgo      = pDcb->trgo;
    instance[idx].ch        = pDcb->ch;
    instance[idx].pulse     = pDcb->pulse;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of TIM.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_TIM_DCB* pDcb, u8 idx)
{
    tDRV_TIM_DCB dcb = {
        .type     = instance[idx].type,
        .tim      = (TIM_TypeDef*)instance[idx].sPrefix.pBase,
        .idx      = instance[idx].sPrefix.trueIdx,
        .remapEn  = instance[idx].remapEn,
        .remapIdx = instance[idx].remapIdx,
        .mode     = instance[idx].mode,
        .cntFreq  = instance[idx].cntFreq,
        .arr      = instance[idx].period - 1,
        .pulse    = instance[idx].pulse,
        .ch       = instance[idx].ch,
        .trgo     = instance[idx].trgo
    };

    DRV_TIM_Init(&dcb);
    DRV_TIM_ON(dcb.tim);
}



////////////////////////////////////////////////////////////////////////////////
/// @brief  TIM OPEN FILE function.
/// @param  handle: pointer to a HANDLE structure that contains the instance for
///         the specified TIM.
/// @param  pDcb: pointer to a tAPP_TIM_DCB structure that contains the
///         configuration information for the specified TIM module.
/// @retval pointer to TIM instance.
////////////////////////////////////////////////////////////////////////////////
static int TIM_OpenFile(HANDLE handle, tAPP_TIM_DCB* pDcb)
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
/// @brief  TIM CLOSE FILE function.
/// @param  handle: pointer to a HANDLE structure that contains
///                the instance for the specified TIM.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void TIM_CloseFile(HANDLE handle)
{   if (handle->idx == -1)      return;
    TIM_DeInit((TIM_TypeDef*)instance[tbSubHandleIdx[handle->idx]].sPrefix.pBase);

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  TIM READ FILE function.
/// @param  handle: pointer to a HANDLE structure that contains the instance for
///         the specified TIM.
/// @param  hSub  : Sub function.
/// @param  ptr   : Pointer to data flow.
/// @param  len   : File length.
/// @retval Waitting for support.
////////////////////////////////////////////////////////////////////////////////
static int TIM_ReadFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    //--- timer capture pulse mode: deal with pulse data ---------------------//
    if (instance[idx].mode == emTIM_CapturePulse) {
        if (capValueStructure.capFinishFlag) {
            *((u32*)ptr) = (capValueStructure.capPeriod * (0xFFFF + 1) + capValueStructure.capCCRValue + 1);
            capValueStructure.capFinishFlag = 0;
        }
    }

    //--- Encoder mode: get the direction and count --------------------------//
    if (instance[idx].mode == emTIM_Encoder) {
        //*ptr = DRV_TIM_GetDir((TIM_TypeDef*)(*handle->instance));
        DRV_TIM_GetEncoder((TIM_TypeDef*)instance[idx].sPrefix.pBase);
    }

    // Waitting for support other mode ...

    return true;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  TIM WRITE FILE function.
/// @param  handle: pointer to a HANDLE structure that contains the instance for
///         the specified TIM.
/// @param  hSub  : Sub function.
/// @param  ptr   : Pointer to data flow.
/// @param  len   : File length.
/// @retval Waitting for support.
////////////////////////////////////////////////////////////////////////////////
static int TIM_WriteFile(HANDLE handle, s8 hSub, u8* ptr, u16 len)
{
    s8 idx = DRV_FindTrueIdx(hSub, (u8*)&instance, sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)  return 0;

    if (instance[idx].mode == emTIM_OnePulse) {
        (*ptr) ? DRV_TIM_ON((TIM_TypeDef*)instance[idx].sPrefix.pBase) : DRV_TIM_OFF((TIM_TypeDef*)instance[idx].sPrefix.pBase);
    }

    if (instance[idx].mode == emTIM_PWM) {
        TIM_SetCompare1((TIM_TypeDef*)instance[idx].sPrefix.pBase, *(u16*)ptr);
    }

    if (instance[idx].mode == emTIM_6Step) {
        if (*ptr) {
            DRV_TIM_ON((TIM_TypeDef*)instance[idx].sPrefix.pBase);                   // Enable timer count
            DRV_TIM_Generate_COM_Event((TIM_TypeDef*)instance[idx].sPrefix.pBase);   // Trigger commutation signal  by software
            DRV_TIM_6Step((TIM_TypeDef*)instance[idx].sPrefix.pBase, (*(ptr + 4)));  // Handle step function
        }
        else
            DRV_TIM_OFF((TIM_TypeDef*)instance[idx].sPrefix.pBase);                  // Disable timer count
    }
    // Waitting for support other mode ...
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  TIM CreateFile
/// @param  handle: pointer to a HANDLE structure that contains the instance for
///         the specified TIM.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM_CreateFile(HANDLE  handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&TIM_OpenFile;
    handle->closeFile   = (int*)&TIM_CloseFile;
    handle->readFile    = (int*)&TIM_ReadFile;
    handle->writeFile   = (int*)&TIM_WriteFile;
}

/// @}

/// @}

/// @}
