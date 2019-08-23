////////////////////////////////////////////////////////////////////////////////
/// @file     BSP.H
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE
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
#ifndef __BSP_H
#define __BSP_H

// Files includes  -------------------------------------------------------------
#include "hal_GPIO.h"
#include "drv.h"

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Board_Support_Package
/// @brief MM32 BSP Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup BSP_COMMON
/// @brief MM32 BSP Common modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup BSP_Exported_Constants
/// @{

#define NO_REMAP            0
#define NO_FUNCAF           -1


#define PORT_INWUD          0x08
#define PORT_AIN            0x00
#define PORT_FIN            0x04

#define PORT_OUTOD          0x04
#define PORT_AFPP           0x08
#define PORT_AFOD           0x0C

#if defined(__MM3N1) || defined(__MM0Q1)
    #define PORT_10MHz      0x01
    #define PORT_20MHz      0x02
    #define PORT_50MHz      0x03
#endif
#if defined(__MM0N1) || defined(__MM0P1)
    #define PORT_10MHz      0x03
    #define PORT_20MHz      0x02
    #define PORT_50MHz      0x01
#endif

#define PORT_BIT0           0
#define PORT_BIT1           4
#define PORT_BIT2           8
#define PORT_BIT3           12
#define PORT_BIT4           16
#define PORT_BIT5           20
#define PORT_BIT6           24
#define PORT_BIT7           28

#define PORT_BIT8           0
#define PORT_BIT9           4
#define PORT_BIT10          8
#define PORT_BIT11          12
#define PORT_BIT12          16
#define PORT_BIT13          20
#define PORT_BIT14          24
#define PORT_BIT15          28

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup BSP_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  KEY_Typedef
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    bool event[4];
    bool press[4];
    u8   dlyCnt[4];
} KEY_Typedef;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup GPIO_Exported_Variables
/// @{

#ifdef _BSP_C_

#define GLOBAL
#else
#define GLOBAL extern
#endif


GLOBAL KEY_Typedef KEY;


#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup BSP_Exported_Functions
/// @{

GPIO_TypeDef* GPIO_BaseFromIndex(EM_GPIO_PORT idx);

void GPIO_AF_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed, s32 remap, s8 funcAF);

void GPIO_Mode_AF_PP_10MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);
void GPIO_Mode_AF_PP_20MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);
void GPIO_Mode_AF_PP_50MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);

void GPIO_Mode_AF_OD_10MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);
void GPIO_Mode_AF_OD_20MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);
void GPIO_Mode_AF_OD_50MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);

void GPIO_Mode_OUT_OD_10MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);
void GPIO_Mode_OUT_OD_20MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);
void GPIO_Mode_OUT_OD_50MHz_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);

void GPIO_Mode_OUT_PP_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
void GPIO_Mode_OUT_OD_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);



void GPIO_Mode_IN_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, GPIOMode_TypeDef mode, s32 remap, s8 funcAF);

void GPIO_Mode_IPU_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);
void GPIO_Mode_IPD_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);

void GPIO_Mode_FLOATING_Init(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, s32 remap, s8 funcAF);



void GPIOx_ClockEnable(EM_GPIO_PORT port);
void GPIOA_ClockEnable(void);
void GPIOB_ClockEnable(void);
void GPIOC_ClockEnable(void);
void GPIOD_ClockEnable(void);
void GPIOE_ClockEnable(void);
void GPIOF_ClockEnable(void);

/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
