////////////////////////////////////////////////////////////////////////////////
/// @file     RTC_APP.C
/// @author   C Yuan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE RTC_APP EXAMPLE.
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
#define _RTC_APP_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include <time.h>
#include "drv.h"
#include "rtc_app.h"


#if defined(__MM3N1)

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_RTC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RTC_Exported_Functions
/// @{


/* -----------------------------------------------------------------------------
------------------   F u n c t i o n     I m p l e m e n t  --------------------
----------------------------------------------------------------------------- */
//////////////////////////////////////////////////////////////////////////////////
//static u8 GetWeek(u32* sec)
//{
//    return ((*sec / SecsPerDay) + 6) % 7;             // 2000/01/01 is Saturday
//}
//
//////////////////////////////////////////////////////////////////////////////////
//static u16 GetYear(u32* sec)
//{
//  u8 i;
//    u32 OffSec = *sec % SecsPerFourYear;
//    for (i = 0; i < 4; i++) {
//      if (OffSec < Year_Secs_Accu[i + 1]) {
//            break;
//      }
//    }
//    u16 year = 2000 + (*sec / SecsPerFourYear * 4) + i;
//    *sec     = OffSec - Year_Secs_Accu[i];
//    return year;
//}
//
//////////////////////////////////////////////////////////////////////////////////
//static u16 GetMouthItem(u32* sec, u32* item)
//{
//    for (u8 i = 0; i < 12; i++) {
//        if (*sec < *(item + i + 1)) {
//            *sec = *sec - *(item + i);
//          return i + 1;
//        }
//    }
//    return 0;
//}
//
//////////////////////////////////////////////////////////////////////////////////
//static u16 GetMouth(u16 year, u32* sec)
//{
//    return (year % 4) ? GetMouthItem(sec, &Month_Secs_Accu_C[0]) : GetMouthItem(sec, &Month_Secs_Accu_L[0]);
//}
//
//////////////////////////////////////////////////////////////////////////////////
//static u16 GetDayItem(u16 day, u16* item)
//{
//    for (u8 i = 0; i < 12; i++) {
//        if (day < *(item + i + 1)) {
//            day = day - *(item + i);
//            break;
//        }
//    }
//    return day + 1;
//}
//
//////////////////////////////////////////////////////////////////////////////////
//static u16 GetDay(u16 year, u32* sec)
//{
//    u16 day = *sec / SecsPerDay;
//    *sec    = *sec % SecsPerDay;
//    return (year % 4) ? GetDayItem(day, &Month_Days_Accu_C[0]) : GetDayItem(day, &Month_Days_Accu_L[0]);
//}
//
//////////////////////////////////////////////////////////////////////////////////
u32 RTC_APP_DateTimeToSeconds(DATE_TypeDef* tp)
{
    u32 LeapY = (tp->year == 2000) ? 0 : ((tp->year - 2000 - 1) / 4 + 1);
    u32 ComY  = (tp->year - 2000) - LeapY;

    u32 Days = (tp->year % 4) ? (LeapY * 366 + ComY * 365 + Month_Days_Accu_C[tp->month - 1] + (tp->day - 1))
                              : (LeapY * 366 + ComY * 365 + Month_Days_Accu_L[tp->month - 1] + (tp->day - 1));
    return Days * SecsPerDay + tp->hours * 3600 + tp->minute * 60 + tp->second;
}



////////////////////////////////////////////////////////////////////////////////
/// @brief  Increase or decrease one year
/// @param  tp: a pointer of time struct DT_Deftype
/// @param  plus: 1 means decrease, others mean increase
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_APP_AdjYear(DATE_TypeDef* tp, u8 plus)
{
    if (plus) {
        if (--tp->year < 2016)
            tp->year = 2025;
    }
    else {
        if (++tp->year > 2025)
            tp->year = 2016;
    }
    u8 day = Month_Days[tp->month - 1];
    if (tp->year % 4)
        day--;
    if (tp->day > day)
        tp->day = day;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Increase or decrease one month
/// @param  tp: a pointer of time struct DT_Deftype
/// @param  plus: 1 means decrease, others mean increase
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_APP_AdjMonth(DATE_TypeDef* tp, u8 plus)
{
    if (plus) {
        if (--tp->month < 1)
            tp->month = 12;
    }
    else {
        if (++tp->month > 12)
            tp->month = 1;
    }
    u8 day = Month_Days[tp->month - 1];
    if (tp->year % 4)
        day--;
    if (tp->day > day)
        tp->day = day;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Increase or decrease one day
/// @param  tp: a pointer of time struct DT_Deftype
/// @param  plus: 1 means decrease, others means increase
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_APP_AdjDay(DATE_TypeDef* tp, u8 plus)
{
    u8 day = Month_Days[tp->month - 1];
    if (tp->year % 4)
        day--;
    if (plus) {
        if (--tp->day < 1)
            tp->day = day;
    }
    else {
        if (++tp->day > day)
            tp->day = 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Increase or decrease one hour
/// @param  tp: a pointer of time struct DT_Deftype
/// @param  plus: 1 means decrease, others means increase
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_APP_AdjHours(DATE_TypeDef* tp, u8 plus)
{
    if (plus) {
        if (--tp->hours == -1)
            tp->hours = 23;
    }
    else {
        if (++tp->hours > 23)
            tp->hours = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Increase or decrease one minute
/// @param  tp: a pointer of time struct DT_Deftype
/// @param  plus: 1 means decrease, others means increase
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_APP_AdjMinute(DATE_TypeDef* tp, u8 plus)
{
    if (plus) {
        if (--tp->minute == -1)
            tp->minute = 59;
    }
    else {
        if (++tp->minute > 59)
            tp->minute = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Increase or decrease one second
/// @param  tp: a pointer of time struct DT_Deftype
/// @param  plus: 1 means decrease, others means increase
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_APP_AdjSecond(DATE_TypeDef* tp, u8 plus)
{
    if (plus) {
        if (--tp->second == -1)
            tp->second = 59;
    }
    else {
        if (++tp->second > 59)
            tp->second = 0;
    }
}



/// @}

/// @}

/// @}


#endif


