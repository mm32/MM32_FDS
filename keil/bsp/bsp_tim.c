////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_TIM.C
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE TIM BSP LAYER FUNCTIONS.
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
#define _BSP_TIM_C_

// Files includes  -------------------------------------------------------------
#include "hal_gpio.h"

#include "bsp.h"
#include "drv_tim.h"

#include "bsp_tim.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
// TIM1 CH1
//      remapEn false:   PA8
//      remapEn true:
//      remapIdx 0:      PA8
//------------------------------------------------------------------------------
// TIM2 CH1
//      remapEn false:   PA0
//      remapEn true:
//      remapIdx 0:      PA15
//      remapIdx 1:      PA0
//      remapIdx 2:      PA15
//------------------------------------------------------------------------------
// TIM3 CH1
//      remapEn false:   PA6
//      remapEn true:
//      remapIdx 0:      PB4
//      remapIdx 1:      PC6
//------------------------------------------------------------------------------
// TIM4 CH1
//      remapEn false/true:   PB6
//------------------------------------------------------------------------------
// TIM14 CH1
//      remapEn false:   PB1
//      remapEn true:
//      remapIdx 0:      PA4
//      remapIdx 1:      PA7
//      remapIdx 2:      PA0
//------------------------------------------------------------------------------
// TIM16 CH1
//      remapEn false:   PB8
//      remapEn true:
//      remapIdx 0:      PA6
//      remapIdx 1:      PA10
//------------------------------------------------------------------------------
// TIM17 CH1
//      remapEn false:   PB9
//      remapEn true:
//      remapIdx 0:      PA7
//      remapIdx 1:      PD7
//------------------------------------------------------------------------------
// TIM8 CH1
//      remapEn false:   PC6
//      remapEn true:
//      remapIdx 0:      PC0

////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer Channel-1 GPIO Configure
//  @param  TIMx: where x can be 1, 2, 3, 4, 8, 14, 16 or 17 to select the TIM peripheral.
//  @param  remapEn:  true or false to select remap function.
//  @param  remapIdx: select Pin remap source.
//  @param  inoutMode: 0 is config as input-pin, 1 is config as output-pin
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_TIM_CH1_GPIO_Configure(TIM_TypeDef* tim, bool remapEn, u8 remapIdx, bool inoutMode)
{
/* -------------------------------------------------------------------------- */
#if defined(__REGISTER)  /* ----------- Register Access ------------- */
/* -------------------------------------------------------------------------- */

    switch (*(u32*)&tim) {
        case (u32)TIM1:
        if (!remapEn) {
            GPIOA_ClockEnable();
            GPIOA->CRH = (inoutMode) ?  (GPIOA->CRH & 0xFFFFFFF0) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT8) :
                                        (GPIOA->CRH & 0xFFFFFFF0) | (u32)(PORT_FIN                 << PORT_BIT8);

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
            GPIOA->AFRH = (GPIOA->AFRH & 0xFFFFFFF0) | 0x00000002;
#endif
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                GPIOA->CRH = (inoutMode) ? (GPIOA->CRH & 0xFFFFFFF0) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT8) :
                                           (GPIOA->CRH & 0xFFFFFFF0) | (u32)(PORT_FIN                 << PORT_BIT8);
#if defined(__MM3N1)
                EXTI->MAPR |= 1 << 6;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOA->AFRH = (GPIOA->AFRH & 0xFFFFFFF0) | 0x00000002;
#endif
            }
        }
        break;
        case (u32)TIM2:
        if (!remapEn) {
            GPIOA_ClockEnable();
            GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0xFFFFFFF0) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT0) :
                                       (GPIOA->CRL & 0xFFFFFFF0) | (u32)(PORT_FIN                 << PORT_BIT0);
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
            GPIOA->AFRL = (GPIOA->AFRL & 0xFFFFFFF0) | 0x00000002;
#endif
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                GPIOA->CRH = (inoutMode) ? (GPIOA->CRH & 0x0FFFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT15) :
                                           (GPIOA->CRH & 0x0FFFFFFF) | (u32)(PORT_FIN                 << PORT_BIT15);
#if defined(__MM3N1)
                EXTI->MAPR |= 1 << 8;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOA->AFRH = (GPIOA->AFRH & 0x0FFFFFFF) | 0x20000000;
#endif
            }
            if (remapIdx == 1) {
                GPIOA_ClockEnable();
                GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0xFFFFFFF0) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT0) :
                                           (GPIOA->CRL & 0xFFFFFFF0) | (u32)(PORT_FIN                 << PORT_BIT0);
#if defined(__MM3N1)
                EXTI->MAPR |= 2 << 8;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
            GPIOA->AFRL = (GPIOA->AFRL & 0xFFFFFFF0) | 0x00000002;
#endif
            }
            if (remapIdx == 2) {
                GPIOA_ClockEnable();
                GPIOA->CRH = (inoutMode) ? (GPIOA->CRH & 0x0FFFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT15) :
                                           (GPIOA->CRH & 0x0FFFFFFF) | (u32)(PORT_FIN                 << PORT_BIT15);
#if defined(__MM3N1)
                EXTI->MAPR |= 3 << 8;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOA->AFRH = (GPIOA->AFRH & 0x0FFFFFFF) | 0x20000000;
#endif
            }
        }
        break;
        case (u32)TIM3:
        if (!remapEn) {
            GPIOA_ClockEnable();
            GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0xF0FFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT6) :
                                       (GPIOA->CRL & 0xF0FFFFFF) | (u32)(PORT_FIN                 << PORT_BIT6);
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
            GPIOA->AFRL = (GPIOA->AFRL & 0xF0FFFFFF) | 0x01000000;
#endif
            }
        else {
            if (remapIdx == 0) {
                GPIOB_ClockEnable();
                GPIOB->CRL = (inoutMode) ? (GPIOB->CRL & 0xFFF0FFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT4) :
                                           (GPIOB->CRL & 0xFFF0FFFF) | (u32)(PORT_FIN                 << PORT_BIT4);
#if defined(__MM3N1)
                EXTI->MAPR |= 2 << 10;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOB->AFRL = (GPIOB->AFRL & 0xFFF0FFFF) | 0x00020000;
#endif
            }
            if (remapIdx == 1) {
                GPIOC_ClockEnable();
                GPIOC->CRL = (inoutMode) ? (GPIOC->CRL & 0xF0FFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT6) :
                                           (GPIOC->CRL & 0xF0FFFFFF) | (u32)(PORT_FIN                 << PORT_BIT6);
#if defined(__MM3N1)
                EXTI->MAPR |= 3 << 10;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOC->AFRL = (GPIOC->AFRL & 0xF0FFFFFF) | 0x01000000;
#endif
            }
        }
        break;
#if defined(__MM3N1)
        case (u32)TIM4:
        GPIOB_ClockEnable();
            GPIOB->CRL = (inoutMode) ? (GPIOB->CRL & 0xF0FFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT6) :
                                       (GPIOB->CRL & 0xF0FFFFFF) | (u32)(PORT_FIN                 << PORT_BIT6);
        break;
#endif
#if defined(__MM0P1)
        case (u32)TIM8:
        if (!remapEn) {
            GPIOC_ClockEnable();
            GPIOC->CRL = (inoutMode) ? (GPIOC->CRL & 0xF0FFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT6) :
                                       (GPIOC->CRL & 0xF0FFFFFF) | (u32)(PORT_FIN                 << PORT_BIT6);
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
            GPIOC->AFRL = (GPIOC->AFRL & 0xF0FFFFFF) | 0x03000000;
#endif
        }
        else {
            if (remapIdx == 0) {
                GPIOC_ClockEnable();
                GPIOC->CRL = (inoutMode) ? (GPIOC->CRL & 0xFFFFFFF0) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT0) :
                                           (GPIOC->CRL & 0xFFFFFFF0) | (u32)(PORT_FIN                 << PORT_BIT0);
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOC->AFRL = (GPIOC->AFRL & 0xFFFFFFF0) | 0x00000006;
#endif
            }
        }
        break;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
        case (u32)TIM14:
        if (!remapEn) {
            GPIOB_ClockEnable();
            GPIOB->CRL = (inoutMode) ? (GPIOB->CRL & 0xFFFFFF0F) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT1) :
                                       (GPIOB->CRL & 0xFFFFFF0F) | (u32)(PORT_FIN                 << PORT_BIT1);
            GPIOB->AFRL = (GPIOB->AFRL & 0xFFFFFF0F) | 0x00000000;
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0xFFF0FFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT4) :
                                           (GPIOA->CRL & 0xFFF0FFFF) | (u32)(PORT_FIN                 << PORT_BIT4);
                GPIOA->AFRL = (GPIOA->AFRL & 0xFFF0FFFF) | 0x00040000;
            }
            if (remapIdx == 1) {
                GPIOA_ClockEnable();
                GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0x0FFFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT7) :
                                           (GPIOA->CRL & 0x0FFFFFFF) | (u32)(PORT_FIN                 << PORT_BIT7);
                GPIOA->AFRL = (GPIOA->AFRL & 0x0FFFFFFF) | 0x40000000;
            }
            if (remapIdx == 2) {
                GPIOA_ClockEnable();
                GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0x0FFFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT0) :
                                           (GPIOA->CRL & 0x0FFFFFFF) | (u32)(PORT_FIN                 << PORT_BIT0);
                GPIOA->AFRL = (GPIOA->AFRL & 0xFFFFFFF0) | 0x00000006;
            }
        }
        break;
        case (u32)TIM16:
        if (!remapEn) {
            GPIOB_ClockEnable();
            GPIOB->CRH = (inoutMode) ? (GPIOB->CRH & 0xFFFFFFF0) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT8) :
                                       (GPIOB->CRH & 0xFFFFFFF0) | (u32)(PORT_FIN                 << PORT_BIT8);
            GPIOB->AFRH = (GPIOB->AFRH & 0xFFFFFFF0) | 0x000000002;
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0xF0FFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT6) :
                                           (GPIOA->CRL & 0xF0FFFFFF) | (u32)(PORT_FIN                 << PORT_BIT6);
                GPIOA->AFRL = (GPIOA->AFRL & 0xF0FFFFFF) | 0x05000000;
            }
            if (remapIdx == 1) {
                GPIOA_ClockEnable();
                GPIOA->CRH = (inoutMode) ? (GPIOA->CRH & 0xFFFFF0FF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT10) :
                                           (GPIOA->CRH & 0xFFFFF0FF) | (u32)(PORT_FIN                 << PORT_BIT10);
                GPIOA->AFRH = (GPIOA->AFRH & 0xFFFFF0FF) | 0x000000600;
            }
        }
        break;
        case (u32)TIM17:
        if (!remapEn) {
            GPIOB_ClockEnable();
            GPIOB->CRH = (inoutMode) ? (GPIOB->CRH & 0xFFFFFF0F) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT9) :
                                       (GPIOB->CRH & 0xFFFFFF0F) | (u32)(PORT_FIN                 << PORT_BIT9);
            GPIOB->AFRH = (GPIOB->AFRH & 0xFFFFFF0F) | 0x000000020;
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0x0FFFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT7) :
                                           (GPIOA->CRL & 0x0FFFFFFF) | (u32)(PORT_FIN                 << PORT_BIT7);
                GPIOA->AFRL = (GPIOA->AFRL & 0x0FFFFFFF) | 0x50000000;
            }
            if (remapIdx == 1) {
                GPIOD_ClockEnable();
                GPIOD->CRL = (inoutMode) ? (GPIOD->CRL & 0x0FFFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT7) :
                                           (GPIOD->CRL & 0x0FFFFFFF) | (u32)(PORT_FIN                 << PORT_BIT7);
                GPIOD->AFRL = (GPIOD->AFRL & 0x0FFFFFFF) | 0x60000000;
            }
        }
        break;
#endif
        default: break;
    }

/* -------------------------------------------------------------------------- */
#else     /* ----------- Hardware Abstraction Layer Access ------------- */
/* -------------------------------------------------------------------------- */

    switch (*(u32*)&tim) {
        case (u32)TIM1:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_8, NO_REMAP, GPIO_AF_2) :
                          GPIO_Mode_FLOATING_Init(GPIOA, GPIO_Pin_8,    NO_REMAP, GPIO_AF_2);
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_8, EXTI_MAPR_TIM1_PARTIAL, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(GPIOA, GPIO_Pin_8,    EXTI_MAPR_TIM1_PARTIAL, GPIO_AF_2);
            }
        }
        break;
        case (u32)TIM2:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_0, NO_REMAP, GPIO_AF_2) :
                          GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_0, NO_REMAP, GPIO_AF_2);
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_15, EXTI_MAPR_TIM2_PARTIAL1, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_15, EXTI_MAPR_TIM2_PARTIAL1, GPIO_AF_2);
            }
            if (remapIdx == 1) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_0, EXTI_MAPR_TIM2_PARTIAL2, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_0, EXTI_MAPR_TIM2_PARTIAL2, GPIO_AF_2);
            }
            if (remapIdx == 2) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_15, EXTI_MAPR_TIM2, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_15, EXTI_MAPR_TIM2, GPIO_AF_2);
            }
        }
        break;
        case (u32)TIM3:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_6, NO_REMAP, GPIO_AF_1) :
                          GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_6, NO_REMAP, GPIO_AF_1);
        }
        else {
            if (remapIdx == 0) {
                GPIOB_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_4, EXTI_MAPR_TIM3_PARTIAL, GPIO_AF_1) :
                              GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_4, EXTI_MAPR_TIM3_PARTIAL, GPIO_AF_1);
            }
            if (remapIdx == 1) {
                GPIOC_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_6, EXTI_MAPR_TIM3, GPIO_AF_1) :
                              GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_6, EXTI_MAPR_TIM3, GPIO_AF_1);
            }
        }
        break;
#if defined(__MM3N1)
        case (u32)TIM4:
        GPIOB_ClockEnable();
        (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_6, NO_REMAP, NO_FUNCAF) :
                      GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_6, NO_REMAP, NO_FUNCAF);
        break;
#endif
#if defined(__MM0P1)
        case (u32)TIM8:
        if (!remapEn) {
            GPIOC_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_6, NO_REMAP, GPIO_AF_3) :
                          GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_6, NO_REMAP, GPIO_AF_3);
        }
        else {
            if (remapIdx == 0) {
                GPIOC_ClockEnable();
                    (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_0, NO_REMAP, GPIO_AF_6) :
                                  GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_0, NO_REMAP, GPIO_AF_6);
            }
        }
        break;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
        case (u32)TIM14:
        if (!remapEn) {
            GPIOB_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_1, NO_REMAP, GPIO_AF_0) :
                          GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_1, NO_REMAP, GPIO_AF_0);
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_4, NO_REMAP, GPIO_AF_4) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_4, NO_REMAP, GPIO_AF_4);
            }
            if (remapIdx == 1) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_7, NO_REMAP, GPIO_AF_4) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_7, NO_REMAP, GPIO_AF_4);
            }
            if (remapIdx == 2) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_0, NO_REMAP, GPIO_AF_6) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_0, NO_REMAP, GPIO_AF_6);
            }
        }
        break;
        case (u32)TIM16:
        if (!remapEn) {
            GPIOB_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_8, NO_REMAP, GPIO_AF_2) :
                          GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_8, NO_REMAP, GPIO_AF_2);
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_6, NO_REMAP, GPIO_AF_5) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_6, NO_REMAP, GPIO_AF_5);
            }
            if (remapIdx == 1) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_10, NO_REMAP, GPIO_AF_6) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_10, NO_REMAP, GPIO_AF_6);
            }
        }
        break;
        case (u32)TIM17:
        if (!remapEn) {
            GPIOB_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_9, NO_REMAP, GPIO_AF_2) :
                          GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_9, NO_REMAP, GPIO_AF_2);
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_7, NO_REMAP, GPIO_AF_5) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_7, NO_REMAP, GPIO_AF_5);
            }
            if (remapIdx == 1) {
                GPIOD_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOD, GPIO_Pin_7, NO_REMAP, GPIO_AF_6) :
                              GPIO_Mode_FLOATING_Init(   GPIOD, GPIO_Pin_7, NO_REMAP, GPIO_AF_6);
            }
        }
        break;
#endif
        default: break;
    }

/* -------------------------------------------------------------------------- */
#endif   /* ----------- End Access  ------------- */
/* -------------------------------------------------------------------------- */
}

////////////////////////////////////////////////////////////////////////////////
// TIM1 CH2
//      remapEn false:   PA9
//      remapEn true:
//      remapIdx 0:      PA9
//------------------------------------------------------------------------------
// TIM2 CH2
//      remapEn false:   PA1
//      remapEn true:
//      remapIdx 0:      PB3
//      remapIdx 1:      PA1
//      remapIdx 2:      PB3
//------------------------------------------------------------------------------
// TIM3 CH2
//      remapEn false:   PA7
//      remapEn true:
//      remapIdx 0:      PB5
//      remapIdx 1:      PC7
//------------------------------------------------------------------------------
// TIM4 CH2
//      remapEn false/true:   PB7
//------------------------------------------------------------------------------
// TIM8 CH2
//      remapEn false:   PC7
//      remapEn true:
//      remapIdx 0:      PC1
////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer Channel-2 GPIO Configure
//  @param  tim: where x can be 1, 2, 3, 4 or 8 to select the TIM peripheral.
//  @param  remapEn:  true or false to select remap function.
//  @param  remapIdx: select Pin remap source.
//  @param  inoutMode: 0 is config as input-pin, 1 is config as output-pin
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_TIM_CH2_GPIO_Configure(TIM_TypeDef* tim, bool remapEn, u8 remapIdx, bool inoutMode)
{
/* -------------------------------------------------------------------------- */
#if defined(__REGISTER)  /* ----------- Register Access ------------- */
/* -------------------------------------------------------------------------- */

    switch (*(u32*)&tim) {
        case (u32)TIM1:
        if (!remapEn) {
            GPIOA_ClockEnable();
            GPIOA->CRH = (inoutMode) ?  (GPIOA->CRH & 0xFFFFFF0F) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT9) :
                                        (GPIOA->CRH & 0xFFFFFF0F) | (u32)(PORT_FIN                << PORT_BIT9);

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
            GPIOA->AFRH = (GPIOA->AFRH & 0xFFFFFF0F) | 0x00000020;
#endif
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                GPIOA->CRH = (inoutMode) ? (GPIOA->CRH & 0xFFFFFF0F) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT9) :
                                           (GPIOA->CRH & 0xFFFFFF0F) | (u32)(PORT_FIN                << PORT_BIT9);
#if defined(__MM3N1)
                EXTI->MAPR |= 1 << 6;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOA->AFRH = (GPIOA->AFRH & 0xFFFFFF0F) | 0x00000020;
#endif
            }
        }
        break;
        case (u32)TIM2:
        if (!remapEn) {
            GPIOA_ClockEnable();
            GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0xFFFFFF0F) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT1) :
                                       (GPIOA->CRL & 0xFFFFFF0F) | (u32)(PORT_FIN                << PORT_BIT1);
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
            GPIOA->AFRL = (GPIOA->AFRL & 0xFFFFFF0F) | 0x00000020;
#endif
        }
        else {
            if (remapIdx == 0) {
                GPIOB_ClockEnable();
                GPIOB->CRL = (inoutMode) ? (GPIOB->CRL & 0xFFFF0FFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT3) :
                                           (GPIOB->CRL & 0xFFFF0FFF) | (u32)(PORT_FIN                 << PORT_BIT3);
#if defined(__MM3N1)
                EXTI->MAPR |= 1 << 8;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOB->AFRL = (GPIOB->AFRL & 0xFFFF0FFF) | 0x00002000;
#endif
            }
            if (remapIdx == 1) {
                GPIOA_ClockEnable();
                GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0xFFFFFF0F) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT1) :
                                           (GPIOA->CRL & 0xFFFFFF0F) | (u32)(PORT_FIN                << PORT_BIT1);
#if defined(__MM3N1)
                EXTI->MAPR |= 2 << 8;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
            GPIOA->AFRL = (GPIOA->AFRL & 0xFFFFFF0F) | 0x00000020;
#endif
            }
            if (remapIdx == 2) {
                GPIOB_ClockEnable();
                GPIOB->CRL = (inoutMode) ? (GPIOB->CRL & 0xFFFF0FFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT3) :
                                           (GPIOB->CRL & 0xFFFF0FFF) | (u32)(PORT_FIN                 << PORT_BIT3);
#if defined(__MM3N1)
                EXTI->MAPR |= 3 << 8;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOB->AFRL = (GPIOB->AFRL & 0xFFFF0FFF) | 0x00002000;
#endif
            }
        }
        break;
        case (u32)TIM3:
        if (!remapEn) {
            GPIOA_ClockEnable();
            GPIOA->CRL = (inoutMode) ? (GPIOA->CRL & 0x0FFFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT7) :
                                       (GPIOA->CRL & 0x0FFFFFFF) | (u32)(PORT_FIN                 << PORT_BIT7);
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
            GPIOA->AFRL = (GPIOA->AFRL & 0x0FFFFFFF) | 0x10000000;
#endif
            }
        else {
            if (remapIdx == 0) {
                GPIOB_ClockEnable();
                GPIOB->CRL = (inoutMode) ? (GPIOB->CRL & 0xFF0FFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT5) :
                                           (GPIOB->CRL & 0xFF0FFFFF) | (u32)(PORT_FIN                 << PORT_BIT5);
#if defined(__MM3N1)
                EXTI->MAPR |= 2 << 10;
#endif

#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOB->AFRL = (GPIOB->AFRL & 0xFF0FFFFF) | 0x00200000;
#endif
            }
            if (remapIdx == 1) {
                GPIOC_ClockEnable();
                GPIOC->CRL = (inoutMode) ? (GPIOC->CRL & 0x0FFFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT7) :
                                           (GPIOC->CRL & 0x0FFFFFFF) | (u32)(PORT_FIN                 << PORT_BIT7);
#if defined(__MM3N1)
                EXTI->MAPR |= 3 << 10;
#endif
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOC->AFRL = (GPIOC->AFRL & 0x0FFFFFFF) | 0x10000000;
#endif
            }
        }
        break;
#if defined(__MM3N1)
        case (u32)TIM4:
        GPIOB_ClockEnable();
            GPIOB->CRL = (inoutMode) ? (GPIOB->CRL & 0x0FFFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT7) :
                                       (GPIOB->CRL & 0x0FFFFFFF) | (u32)(PORT_FIN                 << PORT_BIT7);
        break;
#endif
#if defined(__MM0P1)
        case (u32)TIM8:
        if (!remapEn) {
            GPIOC_ClockEnable();
            GPIOC->CRL = (inoutMode) ? (GPIOC->CRL & 0x0FFFFFFF) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT7) :
                                       (GPIOC->CRL & 0x0FFFFFFF) | (u32)(PORT_FIN                 << PORT_BIT7);
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
            GPIOC->AFRL = (GPIOC->AFRL & 0x0FFFFFFF) | 0x30000000;
#endif
        }
        else {
            if (remapIdx == 0) {
                GPIOC_ClockEnable();
                GPIOC->CRL = (inoutMode) ? (GPIOC->CRL & 0xFFFFFF0F) | (u32)((PORT_AFPP | PORT_20MHz) << PORT_BIT1) :
                                           (GPIOC->CRL & 0xFFFFFF0F) | (u32)(PORT_FIN                 << PORT_BIT1);
#if defined(__MM0N1) || defined(__MM0P1) || defined(__MM0Q1)
                GPIOC->AFRL = (GPIOC->AFRL & 0xFFFFFF0F) | 0x00000060;
#endif
            }
        }
        break;
#endif
        default: break;
    }

/* -------------------------------------------------------------------------- */
#else     /* ----------- Hardware Abstraction Layer Access ------------- */
/* -------------------------------------------------------------------------- */

    switch (*(u32*)&tim) {
        case (u32)TIM1:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_9, NO_REMAP, GPIO_AF_2) :
                          GPIO_Mode_FLOATING_Init(GPIOA, GPIO_Pin_9,    NO_REMAP, GPIO_AF_2);
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_9, EXTI_MAPR_TIM1_PARTIAL, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(GPIOA, GPIO_Pin_9,    EXTI_MAPR_TIM1_PARTIAL, GPIO_AF_2);
            }
        }
        break;
        case (u32)TIM2:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_1, NO_REMAP, GPIO_AF_2) :
                          GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_1, NO_REMAP, GPIO_AF_2);
        }
        else {
            if (remapIdx == 0) {
                GPIOB_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_3, EXTI_MAPR_TIM2_PARTIAL1, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_3, EXTI_MAPR_TIM2_PARTIAL1, GPIO_AF_2);
            }
            if (remapIdx == 1) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_1, EXTI_MAPR_TIM2_PARTIAL2, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_1, EXTI_MAPR_TIM2_PARTIAL2, GPIO_AF_2);
            }
            if (remapIdx == 2) {
                GPIOB_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_3, EXTI_MAPR_TIM2, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_3, EXTI_MAPR_TIM2, GPIO_AF_2);
            }
        }
        break;
        case (u32)TIM3:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_7, NO_REMAP, GPIO_AF_1) :
                          GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_7, NO_REMAP, GPIO_AF_1);
        }
        else {
            if (remapIdx == 0) {
                GPIOB_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_5, EXTI_MAPR_TIM3_PARTIAL, GPIO_AF_1) :
                              GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_5, EXTI_MAPR_TIM3_PARTIAL, GPIO_AF_1);
            }
            if (remapIdx == 1) {
                GPIOC_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_7, EXTI_MAPR_TIM3, GPIO_AF_1) :
                              GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_7, EXTI_MAPR_TIM3, GPIO_AF_1);
            }
        }
        break;
#if defined(__MM3N1)
        case (u32)TIM4:
        GPIOB_ClockEnable();
        (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_7, NO_REMAP, NO_FUNCAF) :
                      GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_7, NO_REMAP, NO_FUNCAF);
        break;
#endif
#if defined(__MM0P1)
        case (u32)TIM8:
        if (!remapEn) {
            GPIOC_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_7, NO_REMAP, GPIO_AF_3) :
                          GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_7, NO_REMAP, GPIO_AF_3);
        }
        else {
            if (remapIdx == 0) {
                GPIOC_ClockEnable();
                    (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_1, NO_REMAP, GPIO_AF_6) :
                                  GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_1, NO_REMAP, GPIO_AF_6);
            }
        }
        break;
#endif
        default: break;
    }

/* -------------------------------------------------------------------------- */
#endif   /* ----------- End Access  ------------- */
/* -------------------------------------------------------------------------- */
}

void BSP_TIM_CH3_GPIO_Configure(TIM_TypeDef* tim, bool remapEn, u8 remapIdx, bool inoutMode)
{
    switch (*(u32*)&tim) {
        case (u32)TIM1:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_10, NO_REMAP, GPIO_AF_2) :
                          GPIO_Mode_FLOATING_Init(GPIOA, GPIO_Pin_10,   NO_REMAP, GPIO_AF_2);
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_10, EXTI_MAPR_TIM1_PARTIAL, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(GPIOA, GPIO_Pin_10,   EXTI_MAPR_TIM1_PARTIAL, GPIO_AF_2);
            }
        }
        break;
        case (u32)TIM2:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_2, NO_REMAP, GPIO_AF_2) :
                          GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_2, NO_REMAP, GPIO_AF_2);
        }
        else {
            if (remapIdx == 0) {
                GPIOB_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_2, EXTI_MAPR_TIM2_PARTIAL1, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_2, EXTI_MAPR_TIM2_PARTIAL1, GPIO_AF_2);
            }
            if (remapIdx == 1) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_10, EXTI_MAPR_TIM2_PARTIAL2, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_10, EXTI_MAPR_TIM2_PARTIAL2, GPIO_AF_2);
            }
            if (remapIdx == 2) {
                GPIOB_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_10, EXTI_MAPR_TIM2, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_10, EXTI_MAPR_TIM2, GPIO_AF_2);
            }
        }
        break;
        case (u32)TIM3:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_0, NO_REMAP, GPIO_AF_1) :
                          GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_0, NO_REMAP, GPIO_AF_1);
        }
        else {
            if (remapIdx == 0) {
                GPIOB_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_0, EXTI_MAPR_TIM3_PARTIAL, GPIO_AF_1) :
                              GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_0, EXTI_MAPR_TIM3_PARTIAL, GPIO_AF_1);
            }
            if (remapIdx == 1) {
                GPIOC_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_8, EXTI_MAPR_TIM3, GPIO_AF_1) :
                              GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_8, EXTI_MAPR_TIM3, GPIO_AF_1);
            }
        }
        break;
#if defined(__MM3N1)
        case (u32)TIM4:
        GPIOB_ClockEnable();
        (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_8, NO_REMAP, NO_FUNCAF) :
                      GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_8, NO_REMAP, NO_FUNCAF);
        break;
#endif
#if defined(__MM0P1)
        case (u32)TIM8:
        if (!remapEn) {
            GPIOC_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_8, NO_REMAP, GPIO_AF_3) :
                          GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_8, NO_REMAP, GPIO_AF_3);
        }
        else {
            if (remapIdx == 0) {
                GPIOC_ClockEnable();
                    (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_2, NO_REMAP, GPIO_AF_6) :
                                  GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_2, NO_REMAP, GPIO_AF_6);
            }
        }
        break;
#endif
        default: break;
    }
}

void BSP_TIM_CH4_GPIO_Configure(TIM_TypeDef* tim, bool remapEn, u8 remapIdx, bool inoutMode)
{
    switch (*(u32*)&tim) {
        case (u32)TIM1:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_11, NO_REMAP, GPIO_AF_2) :
                          GPIO_Mode_FLOATING_Init(GPIOA, GPIO_Pin_11,   NO_REMAP, GPIO_AF_2);
        }
        else {
            if (remapIdx == 0) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_11, EXTI_MAPR_TIM1_PARTIAL, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(GPIOA, GPIO_Pin_11,   EXTI_MAPR_TIM1_PARTIAL, GPIO_AF_2);
            }
        }
        break;
        case (u32)TIM2:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_3, NO_REMAP, GPIO_AF_2) :
                          GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_3, NO_REMAP, GPIO_AF_2);
        }
        else {
            if (remapIdx == 0) {
                GPIOB_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_3, EXTI_MAPR_TIM2_PARTIAL1, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOA, GPIO_Pin_3, EXTI_MAPR_TIM2_PARTIAL1, GPIO_AF_2);
            }
            if (remapIdx == 1) {
                GPIOA_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_11, EXTI_MAPR_TIM2_PARTIAL2, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_11, EXTI_MAPR_TIM2_PARTIAL2, GPIO_AF_2);
            }
            if (remapIdx == 2) {
                GPIOB_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_11, EXTI_MAPR_TIM2, GPIO_AF_2) :
                              GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_11, EXTI_MAPR_TIM2, GPIO_AF_2);
            }
        }
        break;
        case (u32)TIM3:
        if (!remapEn) {
            GPIOA_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_1, NO_REMAP, GPIO_AF_1) :
                          GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_1, NO_REMAP, GPIO_AF_1);
        }
        else {
            if (remapIdx == 0) {
                GPIOB_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_1, EXTI_MAPR_TIM3_PARTIAL, GPIO_AF_1) :
                              GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_1, EXTI_MAPR_TIM3_PARTIAL, GPIO_AF_1);
            }
            if (remapIdx == 1) {
                GPIOC_ClockEnable();
                (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_9, EXTI_MAPR_TIM3, GPIO_AF_1) :
                              GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_9, EXTI_MAPR_TIM3, GPIO_AF_1);
            }
        }
        break;
#if defined(__MM3N1)
        case (u32)TIM4:
        GPIOB_ClockEnable();
        (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_9, NO_REMAP, NO_FUNCAF) :
                      GPIO_Mode_FLOATING_Init(   GPIOB, GPIO_Pin_9, NO_REMAP, NO_FUNCAF);
        break;
#endif
#if defined(__MM0P1)
        case (u32)TIM8:
        if (!remapEn) {
            GPIOC_ClockEnable();
            (inoutMode) ? GPIO_Mode_AF_PP_20MHz_Init(GPIOC, GPIO_Pin_9, NO_REMAP, GPIO_AF_3) :
                          GPIO_Mode_FLOATING_Init(   GPIOC, GPIO_Pin_9, NO_REMAP, GPIO_AF_3);
        }
        break;
#endif
        default: break;
    }
}


////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer PWM-Mode GPIO Configure
//  @param  TIMx: where x can be 1, 2, 3, 4, 5, 8, 14, 16 or 17 to select the TIM peripheral.
//  @param  remapEn:  true or false to select remap function.
//  @param  remapIdx: select Pin remap source.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_TIM_PWM_GPIO_Configure(TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx)
{
    if (ch & (u8)emTIM_CH1)
        BSP_TIM_CH1_GPIO_Configure(tim, remapEn, remapIdx, 1);                         //As output
    if (ch & (u8)emTIM_CH2)
        BSP_TIM_CH2_GPIO_Configure(tim, remapEn, remapIdx, 1);                         //As output
    if (ch & (u8)emTIM_CH3)
        BSP_TIM_CH3_GPIO_Configure(tim, remapEn, remapIdx, 1);                         //As output
    if (ch & (u8)emTIM_CH4)
        BSP_TIM_CH4_GPIO_Configure(tim, remapEn, remapIdx, 1);                         //As output
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer CapturePulse-Mode GPIO Configure
//  @param  TIMx: where x can be 1, 2, 3, 4 or 8 to select the TIM peripheral.
//  @param  remapEn:  true or false to select remap function.
//  @param  remapIdx: select Pin remap source.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_TIM_CapturePulse_GPIO_Configure(TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx)
{
    BSP_TIM_CH2_GPIO_Configure(tim, remapEn, remapIdx, 0);
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer CapturePWM-Mode GPIO Configure
//  @param  TIMx: where x can be 1, 2, 3, 4 or 8 to select the TIM peripheral.
//  @param  remapEn:  true or false to select remap function.
//  @param  remapIdx: select Pin remap source.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_TIM_CapturePWM_GPIO_Configure(TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx)
{
    BSP_TIM_CH1_GPIO_Configure(tim, remapEn, remapIdx, 0);
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer Encoder-Mode GPIO Configure
//  @param  TIMx: where x can be 1, 2, 3, 4 or 8 to select the TIM peripheral.
//  @param  remapEn:  true or false to select remap function.
//  @param  remapIdx: select Pin remap source.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_TIM_Encoder_GPIO_Configure(TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx)
{
    BSP_TIM_CH1_GPIO_Configure(tim, remapEn, remapIdx, 0);
    BSP_TIM_CH2_GPIO_Configure(tim, remapEn, remapIdx, 0);
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer OnePulse-Mode GPIO Configure
//  @param  TIMx: where x can be 1, 2, 3, 4 or 8 to select the TIM peripheral.
//  @param  remapEn:  true or false to select remap function.
//  @param  remapIdx: select Pin remap source.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_TIM_OnePulse_GPIO_Configure(TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx)
{

    BSP_TIM_CH1_GPIO_Configure(tim, remapEn, remapIdx, 1);
    //BSP_TIM_CH2_GPIO_Configure(tim, remapEn, remapIdx, 0);
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer 6-step Mode GPIO Configure
//  @param  TIMx: where x can be 1 or 8 to select the TIM peripheral.
//  @param  remapEn:  true or false to select remap function.
//  @param  remapIdx: select Pin remap source.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_TIM_6Step_GPIO_Configure(TIM_TypeDef* tim, u8 ch, bool remapEn, u8 remapIdx)
{
    //Todo: just config TIM1
    GPIOA_ClockEnable();
    GPIOB_ClockEnable();

    // TIM1 CH1 / CH2 / CH3
    GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_8,  NO_REMAP, GPIO_AF_2);
    GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_9,  NO_REMAP, GPIO_AF_2);
    GPIO_Mode_AF_PP_20MHz_Init(GPIOA, GPIO_Pin_10, NO_REMAP, GPIO_AF_2);
    // TIM1 CH1N / CH2N / CH3N
    GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_13, NO_REMAP, GPIO_AF_2);
    GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_14, NO_REMAP, GPIO_AF_2);
    GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_15, NO_REMAP, GPIO_AF_2);
    // TIM1 BKIN
    //GPIO_Mode_AF_PP_20MHz_Init(GPIOB, GPIO_Pin_12, NO_REMAP, GPIO_AF_2);
}

/// @}

/// @}

/// @}
