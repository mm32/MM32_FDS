////////////////////////////////////////////////////////////////////////////////
/// @file     M_QUEUE.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE M_QUEUE EXAMPLE.
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
#define _M_QUEUE_C_
////////////////////////////////////////////////////////////////////////////////

#include "type.h"
#include <stdint.h>

#include "string.h"
#include "HAL_device.h"

#include "M_queue.h"
#include "M_uart.h"
#include "usbprop.h"


////////////////////////////////////////////////////////////////////////////////
void queueInit(MsgQueue_Def * MsgSave)
{
    memset(MsgSave->MsgQueue, 0, sizeof(MsgSave->MsgQueue));

    MsgSave->MsgQueueFront = 0;
    MsgSave->MsgQueueRear = 0;
    MsgSave->MsgQueueFlag = QUEUE_EMPTY;
}

////////////////////////////////////////////////////////////////////////////////
void QueueConfig()
{
    queueInit(&USBToUARTQueue);
    queueInit(&UARTToUSBQueue);
}

////////////////////////////////////////////////////////////////////////////////
bool PutToQueue(u8 Msg, MsgQueue_Def* MsgSave)
{
    if(MsgSave->MsgQueueFlag == QUEUE_FULL){
        return false;
    }
    MsgSave->MsgQueue[MsgSave->MsgQueueRear] = Msg;
    MsgSave->MsgQueueRear++;

    if(MsgSave->MsgQueueRear >= QUEUE_SIZE){
        MsgSave->MsgQueueRear = 0;
    }

    if(MsgSave->MsgQueueRear == MsgSave->MsgQueueFront){
        MsgSave->MsgQueueFlag = QUEUE_FULL;
    }
    else{
        MsgSave->MsgQueueFlag = QUEUE_NORMAL;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool GetFromQueue(u8 * Dat, MsgQueue_Def * MsgSave)
{
    if(MsgSave->MsgQueueFlag == QUEUE_EMPTY){
        return false;
    }
    *Dat = MsgSave -> MsgQueue[MsgSave->MsgQueueFront];
    MsgSave->MsgQueueFront++;

    if(MsgSave->MsgQueueFront >= QUEUE_SIZE){
        MsgSave->MsgQueueFront = 0;
    }

    if(MsgSave->MsgQueueFront == MsgSave->MsgQueueRear){
        MsgSave->MsgQueueFlag = QUEUE_EMPTY;
    }
    else{
        MsgSave->MsgQueueFlag = QUEUE_NORMAL;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
