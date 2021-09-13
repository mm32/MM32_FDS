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
#include "bsp_led.h"
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
#if defined(LED1_PORT)
    LED1_CONFIG;
#endif
#if defined(LED2_PORT)
    LED2_CONFIG;
#endif
#if defined(LED3_PORT)
    LED3_CONFIG;
#endif
#if defined(LED4_PORT)
    LED4_CONFIG;
#endif
#if defined(LED5_PORT)
    LED5_CONFIG;
#endif
#if defined(LED6_PORT)
    LED6_CONFIG;
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
#if defined(LED1_PORT)
void LD1_on(void)	{ GPIO_ResetBits(LED1_PORT, LED1_BIT); ledStatus[0] = true;  }
void LD1_off(void)	{ GPIO_SetBits  (LED1_PORT, LED1_BIT); ledStatus[0] = false; }
#endif
#if defined(LED2_PORT)
void LD2_on(void)	{ GPIO_ResetBits(LED2_PORT, LED2_BIT); ledStatus[1] = true;  }
void LD2_off(void)	{ GPIO_SetBits  (LED2_PORT, LED2_BIT); ledStatus[1] = false; }
#endif
#if defined(LED3_PORT)
void LD3_on(void)	{ GPIO_ResetBits(LED3_PORT, LED3_BIT); ledStatus[2] = true;  }
void LD3_off(void)	{ GPIO_SetBits  (LED3_PORT, LED3_BIT); ledStatus[2] = false; }
#endif
#if defined(LED4_PORT)
void LD4_on(void)	{ GPIO_ResetBits(LED4_PORT, LED4_BIT); ledStatus[3] = true;  }
void LD4_off(void)	{ GPIO_SetBits  (LED4_PORT, LED4_BIT); ledStatus[3] = false; }
#endif
#if defined(LED5_PORT)
void LD5_on(void)	{ GPIO_ResetBits(LED5_PORT, LED5_BIT); ledStatus[4] = true;  }
void LD5_off(void)	{ GPIO_SetBits  (LED5_PORT, LED5_BIT); ledStatus[4] = false; }
#endif
#if defined(LED6_PORT)
void LD6_on(void)	{ GPIO_ResetBits(LED6_PORT, LED6_BIT); ledStatus[5] = true;  }
void LD6_off(void)	{ GPIO_SetBits  (LED6_PORT, LED6_BIT); ledStatus[5] = false; }
#endif

void CloseLED(void)	{ 
#if defined(LED1_PORT)
	LD1_off();
#endif
#if defined(LED2_PORT)
	LD2_off();
#endif
#if defined(LED3_PORT)
	LD3_off();
#endif
#if defined(LED4_PORT)
	LD4_off();
#endif
#if defined(LED5_PORT)
	LD5_off();
#endif
#if defined(LED6_PORT)
	LD6_off();
#endif
}

void OpenLED(void)	{ 
#if defined(LED1_PORT)
	LD1_on();
#endif
#if defined(LED2_PORT)
	LD2_on();
#endif
#if defined(LED3_PORT)
	LD3_on();
#endif
#if defined(LED4_PORT)
	LD4_on();
#endif
#if defined(LED5_PORT)
	LD5_on();
#endif
#if defined(LED6_PORT)
	LD6_on();
#endif
}

/// @}

/// @}

/// @}

