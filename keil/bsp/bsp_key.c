////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_KEY.C
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE KEY BSP LAYER FUNCTIONS.
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
#define _BSP_KEY_C_

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
/// @addtogroup KEY_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize KEY.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_KEY_Configure(void)
{
#if defined(__MM32_EVB)

    COMMON_EnableIpClock(emCLOCK_GPIOA);
    COMMON_EnableIpClock(emCLOCK_GPIOC);
    #if defined(__REGISTER)  /* ----------- Register Access ------------- */
        GPIOA->CRL = (GPIOA->CRL & 0xFFFFFFF0) | (u32)PORT_INWUD << PORT_BIT0;  // 0x00000008;  // GPIOA_Pin0-IPD
        GPIOA->BRR |= 1 << 0;
        GPIOC->CRH = (GPIOC->CRH & 0xFFFFF0FF) | (u32)PORT_INWUD << PORT_BIT10; // 0x00000800;  // GPIOC_Pin10-IPU
        GPIOC->BSRR |= 1 << 10;
        GPIOA->CRH = (GPIOA->CRH & 0x0FFFFFFF) | (u32)PORT_INWUD << PORT_BIT15; // 0x80000000;  // GPIOA_Pin15-IPU
        GPIOA->BSRR |= 1 << 15;
    #else       /* ----------- Hardware Abstraction Layer Access ------------- */
        //  Key1 => nRST
        //  Key2 => PA0
        GPIO_Mode_IPD_Init(GPIOA, GPIO_Pin_0, NO_REMAP, GPIO_AF_0);
        //  Key3 => PC10
        GPIO_Mode_IPU_Init(GPIOC, GPIO_Pin_10, NO_REMAP, GPIO_AF_0);
        //  Key4 => PA15
        GPIO_Mode_IPU_Init(GPIOA, GPIO_Pin_15, NO_REMAP, GPIO_AF_0);
    #endif

#else  // MM32_MiniBoard

    COMMON_EnableIpClock(emCLOCK_GPIOA);
    COMMON_EnableIpClock(emCLOCK_GPIOB);
    COMMON_EnableIpClock(emCLOCK_GPIOC);

    #if defined(__REGISTER)  /* ----------- Register Access ------------- */
        GPIOC->CRH = (GPIOC->CRH & 0xFF0FFFFF) | (u32)PORT_INWUD << PORT_BIT13; // 0x00800000;  // GPIOC_Pin13-IPU
        GPIOC->BSRR |= 1 << 13;
        GPIOA->CRL = (GPIOA->CRL & 0xFFFFFFF0) | (u32)PORT_INWUD << PORT_BIT0;  // 0x00000008;  // GPIOA_Pin0-IPD
        GPIOA->BRR |= 1 << 0;
        GPIOB->CRH = (GPIOB->CRH & 0xFFFF00FF) | (u32)PORT_INWUD << PORT_BIT10  // 0x00000800;  // GPIOB_Pin10-IPU
                                               | (u32)PORT_INWUD << PORT_BIT11; // 0x00008000;  // GPIOB_Pin11-IPU
        GPIOB->BSRR |= 1 << 10;
        GPIOB->BSRR |= 1 << 11;
    #else       /* ----------- Hardware Abstraction Layer Access ------------- */
        //  Key1 => PC13
        GPIO_Mode_IPU_Init(GPIOC, GPIO_Pin_13, NO_REMAP, GPIO_AF_0);
        //  Key2 => PA0
        GPIO_Mode_IPD_Init(GPIOA, GPIO_Pin_0,  NO_REMAP, GPIO_AF_0);
        //  Key3, Key4 => PB10, PB11
        GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_10, NO_REMAP, GPIO_AF_0);
        GPIO_Mode_IPU_Init(GPIOB, GPIO_Pin_11, NO_REMAP, GPIO_AF_0);
    #endif
#endif
}

/* -----------------------------------------------------------------------------
------------------   F u n c t i o n     I m p l e m e n t  --------------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  get key is set or reset.
/// @param  None.
/// @retval bool: 1: key is set
///         0: kei is reset
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM32_EVB)
    #if defined(__REGISTER)  /* ----------- Register Access ------------- */
        bool Key1(void) {   return  false; }
        bool Key2(void) {   return  ((GPIOA->IDR & 0x0001)) ? 1 : 0;    }
        bool Key3(void) {   return  ((GPIOC->IDR & 0x0400)) ? 0 : 1;    }
        bool Key4(void) {   return  ((GPIOA->IDR & 0x8000)) ? 0 : 1;    }
    #else       /* ----------- Hardware Abstraction Layer Access ------------- */
        bool Key1(void) {   return  false; }
        bool Key2(void) {   return  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);   }
        bool Key3(void) {   return !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);  }
        bool Key4(void) {   return !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);  }
    #endif
#else  // MM32_MiniBoard
    #if defined(__REGISTER)  /* ----------- Register Access ------------- */
        bool Key1(void) {   return  ((GPIOC->IDR & 0x2000)) ? 0 : 1;    }
        bool Key2(void) {   return  ((GPIOA->IDR & 0x0001)) ? 1 : 0;    }
        bool Key3(void) {   return  ((GPIOB->IDR & 0x0400)) ? 0 : 1;    }
        bool Key4(void) {   return  ((GPIOB->IDR & 0x0800)) ? 0 : 1;    }
    #else       /* ----------- Hardware Abstraction Layer Access ------------- */
        bool Key1(void) {   return !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);  }
        bool Key2(void) {   return  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);   }
        bool Key3(void) {   return !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);  }
        bool Key4(void) {   return !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);  }
    #endif
#endif

/// @}

/// @}

/// @}

