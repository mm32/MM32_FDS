////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_OPAMP.C
/// @author   PX Liu
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE OPAMP BSP LAYER FUNCTIONS.
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
#define _BSP_OPAMP_C_

// Files includes  -------------------------------------------------------------
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_opamp.h"

#include "bsp.h"
#include "bsp_opamp.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
///@addtogroup OPAMP
///@brief OPAMP driver modules
///@{

#if defined(__MM0P1)

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup OPAMP_Exported_Functions
/// @{

/* ---------------------------------------------------------------------
------  P e r i p h e r a l s    i n i t i a l i z a t i o n   ---------
--------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the  channel of operational amplifier.
/// @param  OPAMPx: the selected operational amplifier.
///         This parameter can be OPAMPx where x can be 1 to 4
///         to select the OPAPM peripheral.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_OPAMP_GPIO_Configure(u32 OPAMPx)
{
#if defined(__REGISTER)  /* ----------- Register Access ------------- */
    switch((u32)OPAMPx) {
//------------------------------------------------------------------------------
    case OPAMP1:
        GPIOA_ClockEnable();
        GPIOA->CRL = (GPIOA->CRL & 0xF0FFFFFF)
                    | (u32)PORT_AIN                 << PORT_BIT6;
        break;

//------------------------------------------------------------------------------
    case OPAMP2:
        GPIOB_ClockEnable();
        GPIOB->CRL = (GPIOB->CRL & 0xFFFFF0FF)
                    | (u32)PORT_AIN                 << PORT_BIT2;
        break;

//------------------------------------------------------------------------------
    case OPAMP3:
        GPIOB_ClockEnable();
        GPIOB->CRH = (GPIOB->CRH & 0xFFFFF0FF)
                    | (u32)PORT_AIN                 << PORT_BIT10;
        break;

//------------------------------------------------------------------------------
    case OPAMP4:
        GPIOA_ClockEnable();
        GPIOA->CRH = (GPIOA->CRH & 0xFFFFFF0F)
                    | (u32)PORT_AIN                 << PORT_BIT9;
        break;

    default: break;
    }



#else       /* ----------- Hardware Abstraction Layer Access ------------- */


    switch((u32)OPAMPx) {
//------------------------------------------------------------------------------
    case OPAMP1:
        GPIOA_ClockEnable();
        GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_IPD, NO_REMAP, GPIO_AF_0);
        break;

//------------------------------------------------------------------------------
    case OPAMP2:
        GPIOB_ClockEnable();
        GPIO_Mode_IN_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_IPD, NO_REMAP, GPIO_AF_0);
        break;

//------------------------------------------------------------------------------
    case OPAMP3:
        GPIOB_ClockEnable();
        GPIO_Mode_IN_Init(GPIOB, GPIO_Pin_10, GPIO_Mode_IPD, NO_REMAP, GPIO_AF_0);
        break;

//------------------------------------------------------------------------------
    case OPAMP4:
        GPIOA_ClockEnable();
        GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_9, GPIO_Mode_IPD, NO_REMAP, GPIO_AF_0);
        break;

    default: break;
    }
/* -------------------------------------------------------------------------- */
#endif           /* ----------------- End Access  ------------------- */
/* -------------------------------------------------------------------------- */
}

/// @}

#endif
/// @}

/// @}
