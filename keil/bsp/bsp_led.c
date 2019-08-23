////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_LED.C
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE LED BSP LAYER FUNCTIONS.
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
#define _BSP_LED_C_

// Files includes  -------------------------------------------------------------
#include "hal_gpio.h"
#include "hal_nvic.h"
#include "hal_rcc.h"

#include "bsp.h"
#include "common.h"

/* -----------------------------------------------------------------------------
----------  P e r i p h e r a l s    i n i t i a l i z a t i o n   -------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup LED_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup LED_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize LED.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_LED_Configure(void)
{
#if defined(__MM32_EVB)

//  GPIOA_ClockEnable();
//  GPIOC_ClockEnable();
    COMMON_EnableIpClock(emCLOCK_GPIOA);
    COMMON_EnableIpClock(emCLOCK_GPIOC);


    #if defined(__REGISTER)  /* ----------- Register Access ------------- */
        GPIOA->BSRR |= 0x0100;
        GPIOC->BSRR |= 0x2000;

        GPIOA->CRH = (GPIOA->CRH & 0xFFFFFFF0) | (u32)(PORT_OUTOD | PORT_20MHz) << PORT_BIT8;   // 0x00000006;  // GPIOA_Pin8  OUTOD
        GPIOC->CRH = (GPIOC->CRH & 0xFF0FFFFF) | (u32)(PORT_OUTOD | PORT_20MHz) << PORT_BIT13;  // 0x00600000;  // GPIOC_Pin13 OUTOD

    #else       /* ----------- Hardware Abstraction Layer Access ------------- */
        GPIO_SetBits(GPIOA, GPIO_Pin_8);                                        // Close LD2
        GPIO_SetBits(GPIOC, GPIO_Pin_13);                                       // Close LD1

        GPIO_Mode_OUT_OD_20MHz_Init(GPIOA, GPIO_Pin_8,  NO_REMAP, GPIO_AF_0);
        GPIO_Mode_OUT_OD_20MHz_Init(GPIOC, GPIO_Pin_13, NO_REMAP, GPIO_AF_0);
    #endif
#endif
#if defined(__MM32_MINIBOARD)
    COMMON_EnableIpClock(emCLOCK_GPIOA);
    COMMON_EnableIpClock(emCLOCK_GPIOB);

    #if defined(__REGISTER)  /* ----------- Register Access ------------- */
        GPIOA->BSRR |= 0x8000;
        GPIOB->BSRR |= 0x0038;

        GPIOA->CRH = (GPIOA->CRH & 0x0FFFFFFF)
                    | (u32)(PORT_OUTOD | PORT_20MHz) << PORT_BIT15;             // 0x60000000;  // GPIOA_Pin15  OUTOD
        GPIOB->CRL = (GPIOB->CRL & 0xFF000FFF)
                    | (u32)(PORT_OUTOD | PORT_20MHz) << PORT_BIT3               // 0x00003000;  // GPIOB_Pin3  OUTOD
                    | (u32)(PORT_OUTOD | PORT_20MHz) << PORT_BIT4               // 0x00030000;  // GPIOB_Pin4  OUTOD
                    | (u32)(PORT_OUTOD | PORT_20MHz) << PORT_BIT5;              // 0x00300000;  // GPIOB_Pin5  OUTOD


    #else       /* ----------- Hardware Abstraction Layer Access ------------- */
        GPIO_SetBits(GPIOA, GPIO_Pin_15);                                       // Close LD6
        GPIO_SetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);              // Close LD5, LD4, LD3

        GPIO_Mode_OUT_OD_20MHz_Init(GPIOA, GPIO_Pin_15, NO_REMAP, GPIO_AF_0);
        GPIO_Mode_OUT_OD_20MHz_Init(GPIOB, GPIO_Pin_3,  EXTI_MAPR_SWJ_JTAGDISABLE, GPIO_AF_0);
        GPIO_Mode_OUT_OD_20MHz_Init(GPIOB, GPIO_Pin_4,  NO_REMAP, GPIO_AF_0);
        GPIO_Mode_OUT_OD_20MHz_Init(GPIOB, GPIO_Pin_5,  NO_REMAP, GPIO_AF_0);
    #endif
#endif
}


/* -----------------------------------------------------------------------------
------------------   F u n c t i o n     I m p l e m e n t  --------------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  LED on/off function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM32_EVB)
    #if defined(__REGISTER)  /* ----------- Register Access ------------- */
        void LD1_on(void)   {   GPIOC->BRR  = 0x2000;   }
        void LD1_off(void)  {   GPIOC->BSRR = 0x2000;   }
        void LD2_on(void)   {   GPIOA->BRR  = 0x0100;   }
        void LD2_off(void)  {   GPIOA->BSRR = 0x0100;   }
        void CloseLED(void) {   GPIOC->BSRR = 0x2000;   GPIOA->BSRR = 0x0100;   }
        void OpenLED(void)  {   GPIOC->BRR  = 0x2000;   GPIOA->BRR  = 0x0100;   }
    #else       /* ----------- Hardware Abstraction Layer Access ------------- */
        void LD1_on(void)   {   GPIO_ResetBits(GPIOC, GPIO_Pin_13); }
        void LD1_off(void)  {   GPIO_SetBits  (GPIOC, GPIO_Pin_13); }
        void LD2_on(void)   {   GPIO_ResetBits(GPIOA, GPIO_Pin_8);  }
        void LD2_off(void)  {   GPIO_SetBits  (GPIOA, GPIO_Pin_8);  }
        void CloseLED(void) {   LD1_off();  LD2_off();              }
        void OpenLED(void)  {   LD1_on();   LD2_on();               }
    #endif
#endif

#if defined(__MM32_MINIBOARD)
    #if defined(__REGISTER)  /* ----------- Register Access ------------- */
        void LD3_on(void)   {   GPIOB->BRR  = 0x0020;   }
        void LD3_off(void)  {   GPIOB->BSRR = 0x0020;   }
        void LD4_on(void)   {   GPIOB->BRR  = 0x0010;   }
        void LD4_off(void)  {   GPIOB->BSRR = 0x0010;   }
        void LD5_on(void)   {   GPIOB->BRR  = 0x0008;   }
        void LD5_off(void)  {   GPIOB->BSRR = 0x0008;   }
        void LD6_on(void)   {   GPIOA->BRR  = 0x8000;   }
        void LD6_off(void)  {   GPIOA->BSRR = 0x8000;   }
        void CloseLED(void) {   GPIOA->BSRR = 0x8000;   GPIOB->BSRR = 0x0038;   }
        void OpenLED(void)  {   GPIOA->BRR  = 0x8000;   GPIOB->BRR  = 0x0038;   }
    #else       /* ----------- Hardware Abstraction Layer Access ------------- */
        void LD3_on(void)   {   GPIO_ResetBits(GPIOB, GPIO_Pin_5);  }
        void LD3_off(void)  {   GPIO_SetBits  (GPIOB, GPIO_Pin_5);  }
        void LD4_on(void)   {   GPIO_ResetBits(GPIOB, GPIO_Pin_4);  }
        void LD4_off(void)  {   GPIO_SetBits  (GPIOB, GPIO_Pin_4);  }
        void LD5_on(void)   {   GPIO_ResetBits(GPIOB, GPIO_Pin_3);  }
        void LD5_off(void)  {   GPIO_SetBits  (GPIOB, GPIO_Pin_3);  }
        void LD6_on(void)   {   GPIO_ResetBits(GPIOA, GPIO_Pin_15); }
        void LD6_off(void)  {   GPIO_SetBits  (GPIOA, GPIO_Pin_15); }
        void CloseLED(void) {   LD3_off();LD4_off();LD5_off();LD6_off();    }
        void OpenLED(void)  {   LD3_on(); LD4_on(); LD5_on(); LD6_on();     }
    #endif
#endif

/// @}

/// @}

/// @}

