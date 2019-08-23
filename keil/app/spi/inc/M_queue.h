////////////////////////////////////////////////////////////////////////////////
/// @file     M_QUEUE.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE M_QUEUE
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
#ifndef __CIRCULAR_QUEUE_H_
#define __CIRCULAR_QUEUE_H_
////////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>

//#define QUEUE_SIZE 200
#define QUEUE_SIZE 80

typedef enum{
    QUEUE_EMPTY,    //  0
    QUEUE_NORMAL,   //  1
    QUEUE_FULL      //  2
}QueueState_Def;

typedef struct {
    u8 MsgQueue[QUEUE_SIZE];
    u8 MsgQueueFront;
    u8 MsgQueueRear;
    QueueState_Def MsgQueueFlag;
}MsgQueue_Def ;

////////////////////////////////////////////////////////////////////////////////
#ifdef _M_QUEUE_C_
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL MsgQueue_Def USBToUARTQueue;
GLOBAL MsgQueue_Def UARTToUSBQueue;

#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////
void QueueConfig();
bool PutToQueue(u8 Msg, MsgQueue_Def * MsgSave);
bool GetFromQueue(u8 * Dat, MsgQueue_Def * MsgSave);

//bool GetQueueFullStatus(MsgQueue_Def * MsgSave);
//bool GetQueueEmptyStatus(MsgQueue_Def * MsgSave);
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
