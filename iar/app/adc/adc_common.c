////////////////////////////////////////////////////////////////////////////////
/// @file     ADC_COMMON.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE ADC_COMMON EXAMPLE.
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
#define _ADC_COMMON_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "adc.h"
#include "adc_common.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_ADC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC 20mS Filter
/// @param  ptr: Memory address
/// @param  percent:Instantaneous specific gravity(0-100)
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1_Filter(u32* ptr, u16 len)
{
    item1.curAdcValue[0] += *(ptr + 0);
    item1.curAdcValue[1] += *(ptr + 1);
    item1.curAdcValue[2] += *(ptr + 2);
    item1.curAdcValue[3] += *(ptr + 3);

    midAdc1Value[0] += (*(ptr + 0) & 0xFFFF);
    midAdc1Value[1] += (*(ptr + 1) & 0xFFFF);
    midAdc1Value[2] += (*(ptr + 2) & 0xFFFF);
    midAdc1Value[3] += (*(ptr + 3) & 0xFFFF);

    if (samCnt1++ == 19) {
        samCnt1 = 0;

        item1.AdcResult[0] = (item1.AdcResult[0] * 70 +  midAdc1Value[0] * 30 / 20) / 100;
        item1.AdcResult[1] = (item1.AdcResult[1] * 70 +  midAdc1Value[1] * 30 / 20) / 100;
        item1.AdcResult[2] = (item1.AdcResult[2] * 70 +  midAdc1Value[2] * 30 / 20) / 100;
        item1.AdcResult[3] = (item1.AdcResult[3] * 70 +  midAdc1Value[3] * 30 / 20) / 100;

        item1.curAdcValue[0] = 0;
        item1.curAdcValue[1] = 0;
        item1.curAdcValue[2] = 0;
        item1.curAdcValue[3] = 0;

        midAdc1Value[0] = 0;
        midAdc1Value[1] = 0;
        midAdc1Value[2] = 0;
        midAdc1Value[3] = 0;

        transFlag1 = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ADC2_Filter(u32* ptr, u16 len)
{
    item2.curAdcValue[0] += *(ptr + 0);
    item2.curAdcValue[1] += *(ptr + 1);
    item2.curAdcValue[2] += *(ptr + 2);
    item2.curAdcValue[3] += *(ptr + 3);

    midAdc2Value[0] += (*(ptr + 0) & 0xFFFF);
    midAdc2Value[1] += (*(ptr + 1) & 0xFFFF);
    midAdc2Value[2] += (*(ptr + 2) & 0xFFFF);
    midAdc2Value[3] += (*(ptr + 3) & 0xFFFF);

    if (samCnt2++ == 19) {
        samCnt2 = 0;

        item2.AdcResult[0] = (item2.AdcResult[0] * 70 +  midAdc2Value[0] * 30 / 20) / 100;
        item2.AdcResult[1] = (item2.AdcResult[1] * 70 +  midAdc2Value[1] * 30 / 20) / 100;
        item2.AdcResult[2] = (item2.AdcResult[2] * 70 +  midAdc2Value[2] * 30 / 20) / 100;
        item2.AdcResult[3] = (item2.AdcResult[3] * 70 +  midAdc2Value[3] * 30 / 20) / 100;

        item2.curAdcValue[0] = 0;
        item2.curAdcValue[1] = 0;
        item2.curAdcValue[2] = 0;
        item2.curAdcValue[3] = 0;

        midAdc2Value[0] = 0;
        midAdc2Value[1] = 0;
        midAdc2Value[2] = 0;
        midAdc2Value[3] = 0;

        transFlag2 = true;
    }
}



/// @}


/// @}

/// @}
