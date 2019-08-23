////////////////////////////////////////////////////////////////////////////////
/// @file     CAN.H
/// @author
/// @version
/// @date
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE CAN
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

////////////////////////////////////////////////////////////////////////////////
#ifndef __CAN_H
#define __CAN_H
////////////////////////////////////////////////////////////////////////////////
enum{
    REFRESH_ADC,
    REFRESH_TEMP,
    REFRESH_FLASH,
    REFRESH_I2C,
    REFRESH_CAN,
    REFRESH_RTC,
    REFRESH_UART1,
    REFRESH_UART2,
    REFRESH_IR,
    REFRESH_LED
};


////////////////////////////////////////////////////////////////////////////////
#ifdef _CAN_C_
#define GLOBAL

GLOBAL bool txSuccess = false;
GLOBAL bool rxSuccess = false;

#else
#define GLOBAL extern
#endif

GLOBAL u8 testStart;
GLOBAL u16 drawBlockCnt;


#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////

void initGPIO(void);



/******************************************************************************/
#endif
/******************************************************************************/



