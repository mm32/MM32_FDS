////////////////////////////////////////////////////////////////////////////////
/// @file     DMA_M2M.C
/// @author   QR Wang
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE DMA_M2M EXAMPLE.
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
#define _DMA_M2M_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "system_mm32.h"

#include "drv.h"
#include "dma_m2m.h"
#include "resource.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_DMA
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DMA_Exported_Functions
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
/// @brief  DMA call back function 1.
/// @param  fptr: callback formal parameter.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA_TCCallback1(void* fptr)
{
    if (memcmp(sourceBuffer, destiBuffer2, BUFFER_SIZE) == 0) {
        u8 i = 0;
        i++;
        // ... ... ...
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DMA call back function 2.
/// @param  fptr: callback formal parameter.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA_TCCallback2(void* fptr)
{
    if (memcmp(sourceBuffer, destiBuffer1, BUFFER_SIZE) == 0) {
        u8 i = 0;
        i++;
        // ... ... ...
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Main program
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
// Step 1:  Setting System Clock     ------------------>>>>>
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
    //  MCUID = SetSystemClock(emSYSTICK_On, NULL);

// Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hDMA1 = CreateFile(emIP_DMA);        // 10
    if (hDMA1 == NULL)      while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_DMA_DCB dcb = {
        .hSub        = emFILE_DMA_CH5,

        .dir        = emDMA_DIR_m2m,
        .block      = emTYPE_Non_Block,
        .cb         = (u32)&DMA_TCCallback1,

        .periphInc  = emDMA_INC_Enable,
        .memInc     = emDMA_INC_Enable,
        .periphBits = emDMA_Bits_32,
        .memBits    = emDMA_Bits_32,

        .mode       = emDMA_MODE_Normal,
        .priority   = emDMA_PRI_High,
    };

    if (!OpenFile(hDMA1, (void*)&dcb))      while(1);
    int channnel5Idx = hDMA1->idx;

    dcb.hSub = emFILE_DMA_CH1;
    dcb.cb = (u32)&DMA_TCCallback2;
    if (!OpenFile(hDMA1, (void*)&dcb))      while(1);
    int channnel1Idx = hDMA1->idx;

    tAPP_DMA_WRITE_DCB writeDcb1 = {
        .src   = (u32)sourceBuffer,
        .desti = (u32)destiBuffer1,
    };

    tAPP_DMA_WRITE_DCB writeDcb2 = {
        .src   = (u32)sourceBuffer,
        .desti = (u32)destiBuffer2,
    };


    while(1){
        if(!success1 && WriteFile(hDMA1, emFILE_DMA_CH1, (u8*)&writeDcb1, BUFFER_SIZE)){
            success1 = true;
            hDMA1->idx = channnel1Idx;
            CloseFile(hDMA1);
        }
        if(!success2 && WriteFile(hDMA1, emFILE_DMA_CH5, (u8*)&writeDcb2, BUFFER_SIZE)){
            success2 = true;
            hDMA1->idx = channnel5Idx;
            CloseFile(hDMA1);
        }

//      if (SysKeyboard(&vkKey)) {
//          switch (vkKey) {
//              case VK_K0:
//              case VK_K1:
//              case VK_K2:
//              case VK_K3:
//              default: break;
//          }
//      }

#if defined(__SYS_RESOURCE_POLLING)
        if (tickFlag) {
            tickFlag = false;
            SysDisplay((u8*)&vdLED);
            vdLED <<= 1;
            if (vdLED >= 0x10) vdLED = 0x0001;
        }
#endif
    }
}

/// @}


/// @}

/// @}
