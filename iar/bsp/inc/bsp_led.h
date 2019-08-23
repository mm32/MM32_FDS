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
#else
#define GLOBAL extern
#endif


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
