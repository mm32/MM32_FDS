////////////////////////////////////////////////////////////////////////////////
/// @file     RTC_APP.H
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE RTC_APP
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
#ifndef __RTC_APP_H
#define __RTC_APP_H

// Includes  -------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_RTC
/// @brief RTC example modules
/// @{

#if defined(__MM3N1)


////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Constants
/// @{

#define SecsPerComYear          31536000            //(365*3600*24)
#define SecsPerLeapYear         31622400            //(366*3600*24)
#define SecsPerFourYear         126230400           //((365*3600*24)*3+(366*3600*24))
#define SecsPerDay              86400               //(3600 * 24)

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Types
/// @brief RTC Struct Definition
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  DATE_TypeDef
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    u32 week;
    u32 year;
    u32 month;
    u32 day;
    s8 hours;
    s8 minute;
    s8 second;
} DATE_TypeDef;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Variables
/// @{
#ifdef _RTC_APP_C_
#define GLOBAL

u32 Year_Secs_Accu[5] = {
    0,                  //
    31622400,
    63158400,
    94694400,
    126230400
};

u32 Month_Secs_Accu_C[13] = {
    0,                  //
    2678400,
    5097600,
    7776000,
    10368000,
    13046400,
    15638400,
    18316800,
    20995200,
    23587200,
    26265600,
    28857600,
    31536000
};

u32 Month_Secs_Accu_L[13] = {
    0,                  //
    2678400,
    5184000,
    7862400,
    10454400,
    13132800,
    15724800,
    18403200,
    21081600,
    23673600,
    26352000,
    28944000,
    31622400
};

u16 Month_Days_Accu_C[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
u16 Month_Days_Accu_L[13] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
u16 Month_Days[12]        = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#else
#define GLOBAL extern

GLOBAL u32 Year_Secs_Accu[5];
GLOBAL u32 Month_Secs_Accu_C[13];
GLOBAL u32 Month_Secs_Accu_L[13];
GLOBAL u16 Month_Days_Accu_C[13];
GLOBAL u16 Month_Days_Accu_L[13];
GLOBAL u16 Month_Days[12];

#endif


//GLOBAL    time_t timer;
//GLOBAL  struct tm* tblock;
//GLOBAL    DRV_RTC_INSTANT_TypeDef     RTC_Instance;


#undef GLOBAL

/// @}



////////////////////////////////////////////////////////////////////////////////
/// @defgroup RTC_Exported_Functions
/// @{
u32  RTC_APP_DateTimeToSeconds(DATE_TypeDef* tp);
void RTC_APP_GetTime(DATE_TypeDef* tp);

void RTC_APP_SetTime(DATE_TypeDef* tp);
void RTC_APP_AdjYear(DATE_TypeDef* tp, u8 plus);
void RTC_APP_AdjMonth(DATE_TypeDef* tp, u8 plus);
void RTC_APP_AdjDay(DATE_TypeDef* tp, u8 plus);
void RTC_APP_AdjHours(DATE_TypeDef* tp, u8 plus);
void RTC_APP_AdjMinute(DATE_TypeDef* tp, u8 plus);
void RTC_APP_AdjSecond(DATE_TypeDef* tp, u8 plus);

/// @}

#endif


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
