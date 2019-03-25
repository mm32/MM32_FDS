////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_UART_BAUDRATE.C
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE DRV_UART_BAUDRATE EXAMPLE.
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
#define _DRV_UART_BAUDRATE_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "hal_nvic.h"
#include "hal_tim.h"
#include "drv_uart_baudrate.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
///@addtogroup UART
///@{



////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Private_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the UART GPIO.
/// @param  mode: Select the GPIO in the UART mode or the TIM mode.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Baudrate_IO_Config(baudrateModeTypeDef mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;

#if defined(__MM3N1)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    if(mode == baudrateTIM){
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);//tim
    }
    else{
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//UART
    }
#endif

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the TIM EXTI.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM_EXTI_Config()
{
#if defined(__MM3N1)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
#endif

    exNVIC_Init_TypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    exNVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the TIM mode.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM_Config()
{
    u8 PrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
    u32 TIM1CLK = 0;

    RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM1, ENABLE);

    u32 PCLK2Clock = RCC_GetPCLK2Freq();

    u8 i = (RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;

    if(PrescTable[i] > 0){
        TIM1CLK = PCLK2Clock * 2;
    }
    else{
        TIM1CLK = PCLK2Clock;
    }

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Prescaler = TIM1CLK / 1000000 - 1;                       // CK_CNT = 1 MHz = TIM1CLK / (PSC + 1); PSC = TIM1CLK / CK_CNT - 1
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                                          // Period = 65.536ms
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                            // Set ClockDivision :0
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                        // Set counter mode: up
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                                   // Set repetition counter: 0
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ICInitTypeDef  TIM_ICInitStructure;

    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_ClearFlag(TIM1, TIM_FLAG_CC3);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);                                     // Enable the CC3 interrupt
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                                     // Enable the CC3 interrupt

    TIM_Cmd(TIM1, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles TIM1_CC interrupt request.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
#if defined(__EX_UART)
void TIM1_CC_IRQHandler()
{
//  if(TIM_GetITStatus(TIM1, TIM_IT_Update)){                                   //not in this interrupt
//      baudrateStructure.capturePeriod ++;
//      TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//  }

    if(TIM_GetITStatus(TIM1, TIM_IT_CC3)){
        if(baudrateStructure.captureStartFlag == 0){
            TIM_SetCounter(TIM1, 0);
            baudrateStructure.capturePeriod = 0;
            baudrateStructure.captureCcrValue = 0;
            TIM_OC3PolarityConfig(TIM1, TIM_OCPolarity_High);
            baudrateStructure.captureStartFlag = 1;
        }
        else{
            baudrateStructure.captureCcrValue = TIM_GetCapture3(TIM1);
            TIM_OC3PolarityConfig(TIM1, TIM_OCPolarity_Low);
            baudrateStructure.captureStartFlag = 0;
            baudrateStructure.captureFinishFlag = 1;
            TIM_Cmd(TIM1, DISABLE);
        }
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles UART initialization.
/// @param  baudRate: Set the UART baudRate.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Autobaudrate(u32 baudRate)
{
    UART_InitTypeDef UART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);

    UART_InitStructure.BaudRate             = baudRate;
    UART_InitStructure.WordLength           = UART_WordLength_8b;
    UART_InitStructure.StopBits             = UART_StopBits_1;
    UART_InitStructure.Parity               = UART_Parity_No;
    UART_InitStructure.HWFlowControl        = UART_HWFlowControl_None;
    UART_InitStructure.Mode                 = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(UART1, &UART_InitStructure);

    UART_Cmd(UART1, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function sends break Frame.
/// @param  UARTx: Select the UART or the UART peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART_SendBreakFrame(UART_TypeDef* UARTx)
{
    for(u16 i = 0; i < 10000; i++);
    UARTx->CCR |= UART_CCR_BRK;
    UARTx->CCR &= ~UART_CCR_BRK;
    for(u16 i = 0; i < 10000; i++);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function receives one byte.
/// @param  UARTx: Select the UART or the UART peripheral.
/// @retval  Received byte.
////////////////////////////////////////////////////////////////////////////////
u16 UART_ReceiveByte(UART_TypeDef* UARTx)
{
    u16 data = 0;
    while(UART_GetITStatus(UARTx, UART_ISR_RX_INTF) == RESET);
    data = UART_ReceiveData(UARTx);
    UART_ClearITPendingBit(UARTx, UART_ISR_RX_INTF);
    return data;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function transmits the data by uart.
/// @param  pData: Pointer to data buffer.
/// @param  cnt: Amount of data to be sent.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void baudrateTransmit(u8 *pData, u8 cnt)
{
    u8 complement = 0;

    UART_SendBreakFrame(UART1);

    UART_SendData(UART1, cnt);
    while (UART_GetITStatus(UART1, UART_IER_TXIEN) == RESET) {}
    UART_ClearITPendingBit(UART1, UART_IER_TXIEN);

    complement += ~cnt + 1;

    for(u16 i = 0; i < cnt; i++){
        complement += ~ *pData + 1;
        UART_SendData(UART1, *pData++);
        while (UART_GetITStatus(UART1, UART_IER_TXIEN) == RESET) {}
        UART_ClearITPendingBit(UART1, UART_IER_TXIEN);
    }

    UART_SendData(UART1, complement);
    while (UART_GetITStatus(UART1, UART_IER_TXIEN) == RESET) {}
    UART_ClearITPendingBit(UART1, UART_IER_TXIEN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function receives the data by uart.
/// @param  pData: Pointer to data buffer.
/// @param  cnt: Amount of data to be received.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void baudrateReceive(u8 *pData, u8 cnt, u16 baudrate)
{
    u8 receiveCnt = 0;
    u8 complement = 0;
    u8 checkComplement = 0;
    u16 currentBaudrate = 0;

    if(!baudrateStructure.uartStartFlag){
        if(baudrateStructure.captureFinishFlag){
            baudrateStructure.CaptureTime = baudrateStructure.capturePeriod * (0xFFFF + 1) + (baudrateStructure.captureCcrValue + 1);
            currentBaudrate = 10 * 1000000 / baudrateStructure.CaptureTime;

            if((currentBaudrate < baudrate * 1.1) && (currentBaudrate > baudrate * 0.9)){
                Baudrate_IO_Config(baudrateUART);
                Autobaudrate(10 * 1000000 / baudrateStructure.CaptureTime);
                baudrateStructure.uartStartFlag = 1;
            }
            else{
                TIM_Cmd(TIM1, ENABLE);
            }
            baudrateStructure.uartFinishFlag = 0;
            baudrateStructure.captureFinishFlag = 0;
        }
    }
    else{
        receiveCnt = UART_ReceiveByte(UART1);

        complement += ~cnt + 1;
        if(receiveCnt != cnt){
            UART_Cmd(UART1, DISABLE);
            TIM_Cmd(TIM1, ENABLE);
            baudrateStructure.uartStartFlag = 0;
            baudrateStructure.uartFinishFlag = 0;
            return;
        }

        while(cnt > 0){
            *pData = UART_ReceiveByte(UART1);
            complement += ~ *pData + 1;
            pData++;
            cnt--;
        }

        checkComplement = UART_ReceiveByte(UART1);

        Baudrate_IO_Config(baudrateTIM);
        TIM_Cmd(TIM1, ENABLE);
        UART_Cmd(UART1, DISABLE);
        baudrateStructure.uartStartFlag = 0;
        baudrateStructure.uartFinishFlag = complement == checkComplement ? 1 : 0;
    }
}

/// @}

/// @}

/// @}
