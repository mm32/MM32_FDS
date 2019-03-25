////////////////////////////////////////////////////////////////////////////////
/// @file     ADC_COMMON.C
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-02-18
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
void ADC_Filter(u32* ptr, u16 len)
{
    item.curAdcValue[0] += *(ptr + 0);
    item.curAdcValue[1] += *(ptr + 1);
    item.curAdcValue[2] += *(ptr + 2);
    item.curAdcValue[3] += *(ptr + 3);

    midAdcValue[0] += (*(ptr + 0) & 0xFFFF);
    midAdcValue[1] += (*(ptr + 1) & 0xFFFF);
    midAdcValue[2] += (*(ptr + 2) & 0xFFFF);
    midAdcValue[3] += (*(ptr + 3) & 0xFFFF);

    if (samCnt++ == 19) {
        samCnt = 0;

        item.AdcResult[0] = (item.AdcResult[0] * 70 +  midAdcValue[0] * 30 / 20) / 100;
        item.AdcResult[1] = (item.AdcResult[1] * 70 +  midAdcValue[1] * 30 / 20) / 100;
        item.AdcResult[2] = (item.AdcResult[2] * 70 +  midAdcValue[2] * 30 / 20) / 100;
        item.AdcResult[3] = (item.AdcResult[3] * 70 +  midAdcValue[3] * 30 / 20) / 100;

        item.curAdcValue[0] = 0;
        item.curAdcValue[1] = 0;
        item.curAdcValue[2] = 0;
        item.curAdcValue[3] = 0;

        midAdcValue[0] = 0;
        midAdcValue[1] = 0;
        midAdcValue[2] = 0;
        midAdcValue[3] = 0;

        transFlag = true;
    }
}



/// @}


/// @}

/// @}
