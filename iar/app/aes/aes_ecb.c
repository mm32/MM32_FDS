////////////////////////////////////////////////////////////////////////////////
/// @file     AES_ECB.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE AES_ECB EXAMPLE.
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
#define _AES_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "system_mm32.h"
#include "drv.h"
#include "aes.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_AES
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AES_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if (tickCnt++ >= 500) {
        tickCnt  = 0;
        tickFlag = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Main function, program entry.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
// Step 1:  Setting System Clock     ------------------>>>>>
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is not NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is not NULL, AppTaskTick function not be used */
//  MCUID = SetSystemClock(emSYSTICK_On, NULL);

// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hAES = CreateFile(emIP_AES);
    if (hAES == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_AES_DCB dcb = {
        .hSub         = emFILE_AES_ECB                   ///>  MODE: AES_CBC
    };

    while (1) {
// Step 4:  Open and write File Device   --------------------->>>>>
//---- 128 bit ----------
/*
        dcb.bits    = emAES_BITS_128;                                           // 128bit
        dcb.key     = key_data_128;
//      dcb.iv      = iv_data_128;

        OpenFile(hAES, (void*)&dcb);

        hAES->mode = emBOOL_Encrypt;
        memcpy(input_data, input_data_128, sizeof(input_data_128));
        memcpy(inOutBuffer, input_data_128, sizeof(input_data_128));
        WriteFile(hAES, NULL, (u8*)inOutBuffer, sizeof(inOutBuffer));
        memcpy(cipher, inOutBuffer, sizeof(inOutBuffer));

        hAES->mode = emBOOL_Decrypt;
        WriteFile(hAES, NULL, (u8*)inOutBuffer, sizeof(inOutBuffer));
        result_128 = (!memcmp(input_data_128, inOutBuffer, 16)) ?  true : false;

//---- 192 bit ----------
        dcb.bits    = emAES_BITS_192;                                           // 192bit
        dcb.key     = key_data_192;
//      dcb.iv      = iv_data_192;

        OpenFile(hAES, (void*)&dcb);

        hAES->mode = emBOOL_Encrypt;
        memcpy(input_data, input_data_192, sizeof(input_data_128));
        memcpy(inOutBuffer, input_data_192, sizeof(input_data_192));
        WriteFile(hAES, NULL, (u8*)inOutBuffer, sizeof(inOutBuffer));
        memcpy(cipher, inOutBuffer, sizeof(inOutBuffer));

        hAES->mode = emBOOL_Decrypt;
        WriteFile(hAES, NULL, (u8*)inOutBuffer, sizeof(inOutBuffer));
        result_192 = (!memcmp(input_data_192, inOutBuffer, 16)) ?  true : false;
*/
//---- 256 bit ----------
        dcb.bits    = emAES_BITS_256;                   // 256bit
        dcb.key     = key_data_256;
//      dcb.iv      = iv_data_256;

        OpenFile(hAES, (void*)&dcb);

        hAES->mode = emBOOL_Encrypt;
        memcpy(input_data, input_data_256, sizeof(input_data_128));
        memcpy(inOutBuffer, input_data_256, sizeof(input_data_256));
        WriteFile(hAES, NULL, (u8*)inOutBuffer, sizeof(inOutBuffer));
        memcpy(cipher, inOutBuffer, sizeof(inOutBuffer));

        hAES->mode = emBOOL_Decrypt;
        WriteFile(hAES, NULL, (u8*)inOutBuffer, sizeof(inOutBuffer));
        result_256 = (!memcmp(input_data_256, inOutBuffer, 16)) ?  true : false;

        result_128 = result_192 = result_256 = false;
    }
}
/// @}


/// @}

/// @}

