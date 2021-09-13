////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_LED.H
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE LED
///           BSP LAYER.
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
#ifndef __BSP_LED_H
#define __BSP_LED_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_BSP
/// @brief LED BSP modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Types
/// @{

#if defined(__MM32_EVB)

    #define LED1_PORT GPIOC
    #define LED2_PORT GPIOA

    #define LED1_BIT GPIO_Pin_13
    #define LED2_BIT GPIO_Pin_8

    #define LED1_CONFIG {COMMON_EnableIpClock(emCLOCK_GPIOC);GPIO_SetBits(LED1_PORT, LED1_BIT);GPIO_Mode_OUT_OD_20MHz_Init(LED1_PORT, LED1_BIT, NO_REMAP, GPIO_AF_0);}
    #define LED2_CONFIG {COMMON_EnableIpClock(emCLOCK_GPIOA);GPIO_SetBits(LED2_PORT, LED2_BIT);GPIO_Mode_OUT_OD_20MHz_Init(LED2_PORT, LED2_BIT, NO_REMAP, GPIO_AF_0);}

    #define RED_LED_ON          LD1_on
    #define RED_LED_OFF         LD1_off
    #define GREEN_LED_ON        LD2_on
    #define GREEN_LED_OFF       LD2_off
    #define BLUE_LED_ON         LD1_on
    #define BLUE_LED_OFF        LD1_off
    #define YELLOW_LED_ON       LD2_on
    #define YELLOW_LED_OFF      LD2_off

#endif

#if defined(__MM32_MINIBOARD)

    #define LED3_PORT GPIOB
    #define LED4_PORT GPIOB
    #define LED5_PORT GPIOB
    #define LED6_PORT GPIOA

    #define LED3_BIT GPIO_Pin_5
    #define LED4_BIT GPIO_Pin_4
    #define LED5_BIT GPIO_Pin_3
    #define LED6_BIT GPIO_Pin_15

    #define LED3_CONFIG {COMMON_EnableIpClock(emCLOCK_GPIOB);GPIO_SetBits(LED3_PORT, LED3_BIT);GPIO_Mode_OUT_OD_20MHz_Init(LED3_PORT, LED3_BIT, NO_REMAP, GPIO_AF_0);}
    #define LED4_CONFIG {                                    GPIO_SetBits(LED4_PORT, LED4_BIT);GPIO_Mode_OUT_OD_20MHz_Init(LED4_PORT, LED4_BIT, NO_REMAP, GPIO_AF_0);}
    #define LED5_CONFIG {                                    GPIO_SetBits(LED5_PORT, LED5_BIT);GPIO_Mode_OUT_OD_20MHz_Init(LED5_PORT, LED5_BIT, EXTI_MAPR_SWJ_JTAGDISABLE, GPIO_AF_0);}
    #define LED6_CONFIG {COMMON_EnableIpClock(emCLOCK_GPIOA);GPIO_SetBits(LED6_PORT, LED6_BIT);GPIO_Mode_OUT_OD_20MHz_Init(LED6_PORT, LED6_BIT, NO_REMAP, GPIO_AF_0);}

    #define RED_LED_ON          LD6_on
    #define RED_LED_OFF         LD6_off
    #define GREEN_LED_ON        LD3_on
    #define GREEN_LED_OFF       LD3_off
    #define BLUE_LED_ON         LD4_on
    #define BLUE_LED_OFF        LD4_off
    #define YELLOW_LED_ON       LD5_on
    #define YELLOW_LED_OFF      LD5_off

#endif

#if defined(__MM32_MB022) || defined(__MM32_MB023) || defined(__MM32_MB024) || defined(__MM32_MB025)

    #define LED1_PORT GPIOA
    #define LED2_PORT GPIOB
    #define LED3_PORT GPIOB
    #define LED4_PORT GPIOB

    #define LED1_BIT GPIO_Pin_15
    #define LED2_BIT GPIO_Pin_3
    #define LED3_BIT GPIO_Pin_4
    #define LED4_BIT GPIO_Pin_5

    #define LED1_CONFIG {COMMON_EnableIpClock(emCLOCK_GPIOA);GPIO_SetBits(LED1_PORT, LED1_BIT); GPIO_Mode_OUT_OD_20MHz_Init(LED1_PORT, LED1_BIT, NO_REMAP, GPIO_AF_0);}
    #define LED2_CONFIG {COMMON_EnableIpClock(emCLOCK_GPIOB);GPIO_SetBits(LED2_PORT, LED2_BIT); GPIO_Mode_OUT_OD_20MHz_Init(LED2_PORT, LED2_BIT, EXTI_MAPR_SWJ_JTAGDISABLE, GPIO_AF_0);}
    #define LED3_CONFIG {                                    GPIO_SetBits(LED3_PORT, LED3_BIT); GPIO_Mode_OUT_OD_20MHz_Init(LED3_PORT, LED3_BIT, NO_REMAP, GPIO_AF_0);}
    #define LED4_CONFIG {                                    GPIO_SetBits(LED4_PORT, LED4_BIT); GPIO_Mode_OUT_OD_20MHz_Init(LED4_PORT, LED4_BIT, NO_REMAP, GPIO_AF_0);}

    #define RED_LED_ON          LD1_on
    #define RED_LED_OFF         LD1_off
    #define GREEN_LED_ON        LD2_on
    #define GREEN_LED_OFF       LD2_off
    #define BLUE_LED_ON         LD3_on
    #define BLUE_LED_OFF        LD3_off
    #define YELLOW_LED_ON       LD4_on
    #define YELLOW_LED_OFF      LD4_off

#endif

#if defined(__MM32_MB032) || defined(__MM32_MB033)

    #define LED1_PORT GPIOA
    #define LED2_PORT GPIOA
    #define LED3_PORT GPIOA
    #define LED4_PORT GPIOA

    #define LED1_BIT GPIO_Pin_1
    #define LED2_BIT GPIO_Pin_6
    #define LED3_BIT GPIO_Pin_9
    #define LED4_BIT GPIO_Pin_12

    #define LED1_CONFIG {COMMON_EnableIpClock(emCLOCK_GPIOA);GPIO_SetBits(LED1_PORT, LED1_BIT); GPIO_Mode_OUT_OD_20MHz_Init(LED1_PORT, LED1_BIT, NO_REMAP, GPIO_AF_0);}
    #define LED2_CONFIG {                                    GPIO_SetBits(LED2_PORT, LED2_BIT); GPIO_Mode_OUT_OD_20MHz_Init(LED2_PORT, LED2_BIT, NO_REMAP, GPIO_AF_0);}
    #define LED3_CONFIG {                                    GPIO_SetBits(LED3_PORT, LED3_BIT); GPIO_Mode_OUT_OD_20MHz_Init(LED3_PORT, LED3_BIT, NO_REMAP, GPIO_AF_0);}
    #define LED4_CONFIG {                                    GPIO_SetBits(LED4_PORT, LED4_BIT); GPIO_Mode_OUT_OD_20MHz_Init(LED4_PORT, LED4_BIT, NO_REMAP, GPIO_AF_0);}

    #define RED_LED_ON          LD1_on
    #define RED_LED_OFF         LD1_off
    #define GREEN_LED_ON        LD2_on
    #define GREEN_LED_OFF       LD2_off
    #define BLUE_LED_ON         LD3_on
    #define BLUE_LED_OFF        LD3_off
    #define YELLOW_LED_ON       LD4_on
    #define YELLOW_LED_OFF      LD4_off

#endif

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Variables
/// @{
#ifdef _BSP_LED_C_

#define GLOBAL

GLOBAL bool ledStatus[6] = {false, false, false, false, false, false};

#else
#define GLOBAL extern
#endif

GLOBAL bool ledStatus[6];

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Functions
/// @{

void LD1_on(void);
void LD2_on(void);
void LD3_on(void);
void LD4_on(void);
void LD5_on(void);
void LD6_on(void);
void OpenLED(void);

void LD1_off(void);
void LD2_off(void);
void LD3_off(void);
void LD4_off(void);
void LD5_off(void);
void LD6_off(void);
void CloseLED(void);


void BSP_LED_Configure(void);

/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__BSP_LED_H */
////////////////////////////////////////////////////////////////////////////////
