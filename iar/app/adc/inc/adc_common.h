////////////////////////////////////////////////////////////////////////////////
/// @file     ADC_COMMON.H
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE ADC_COMMON
///           EXAMPLES.
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
#ifndef __ADC_COMMON_H
#define __ADC_COMMON_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_ADC
/// @brief ADC example modules
/// @{

///////////////////////////////////////////////////////////////////////////////
/// @defgroup ADC_Exported_Constants
/// @{



/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup ADC_Exported_Types
/// @{

typedef struct {
    u32 curAdcValue[4];
    u16 AdcResult[4];
} ADC_DAT;



/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup ADC_Exported_Variables
/// @{
#ifdef _ADC_COMMON_C_

#define GLOBAL

u32 midAdc1Value[4] = {0};
u32 midAdc2Value[4] = {0};


#else

#define GLOBAL extern


#endif

GLOBAL ADC_DAT item1;
GLOBAL bool samFlag1;
GLOBAL bool transFlag1;
GLOBAL u8 samCnt1;

GLOBAL ADC_DAT item2;
GLOBAL bool samFlag2;
GLOBAL bool transFlag2;
GLOBAL u8 samCnt2;



#undef GLOBAL
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup ADC_Exported_Functions
/// @{

void ADC1_Filter(u32* ptr, u16 len);
void ADC2_Filter(u32* ptr, u16 len);

/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
