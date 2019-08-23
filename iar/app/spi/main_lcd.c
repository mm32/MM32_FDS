////////////////////////////////////////////////////////////////////////////////
/// @file     MAIN_LCD.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE MAIN_LCD EXAMPLE.
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
#define _SPI_FLASH_C_
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "drv.h"
#include "spi_flash.h"

#include "spi_lcd.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_SPI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{


#define BUFFER_SIZE 20
u8 newTxBuffer[BUFFER_SIZE];
u8 readIDBuffer[3];


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if (tickCnt++ >= 500) {
        tickCnt  = 0;
        tickFlag = true;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles compare.
/// @param  pBuffer: Pointer points to a buffer.
/// @param  pBuffer: Pointer points to another buffer.
/// @param  BufferLength: buffer length.
/// @retval : status.
////////////////////////////////////////////////////////////////////////////////
u8 Buffercmp(const u8* pBuffer, u8* pBuffer1, u16 BufferLength)
{
    u16 i = 0;

    for(i = 0; i < BufferLength; i++){
        if(*pBuffer != *pBuffer1){
            return i;
        }
        pBuffer++;
        pBuffer1++;
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
//SPI data send example ,polling ,interruput, DMA
////////////////////////////////////////////////////////////////////////////////
void UpdateCallback(void* fPtr)
{
    if (tickCnt > 0)
        tickCnt++;
}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Timer Compare/Capture event callback
//  @param  UartHandle: TIM handle
//  @note   This example shows a simple way to use timer Compare/Capture event,
//          and you can add your own implementation.
//  @retval None.
////////////////////////////////////////////////////////////////////////////////
void CCCallback(void* fPtr)
{
    if (tickCnt > 0)
        tickCnt++;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  @brief  Tx Transfer completed callback
//  @param  fPtr:
//  @note   This example shows a simple way to report end of IT or DMA Tx transfer, and
//          you can add your own implementation.
//  @retval None
////////////////////////////////////////////////////////////////////////////////
void TxCallback()
{
    /* Set transmission flag: transfer complete */

}

////////////////////////////////////////////////////////////////////////////////
//  @brief  Rx Transfer completed callback
//  @param  fPtr:
//  @note   This example shows a simple way to report end of IT or DMA Rx transfer, and
//          you can add your own implementation.
//  @retval None
////////////////////////////////////////////////////////////////////////////////
void RxCallback()
{

    /* Set transmission flag: transfer complete */
}

u16 color = 0;
u8 flag = 0;
u16 pwmValue = 0;
u32 adc_temp = 0;

////////////////////////////////////////////////////////////////////////////////

int main(void)
{
// Step 1:  Setting System Clock     ------------------>>>>>
    /* ====  System Clock & SysTick & AppTaskTick Setting  ==== */
    /* When the parameter is NULL, AppTaskTick function used */
    MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick);
    /* When the parameter is NULL, AppTaskTick function not be used */
//  MCUID = SetSystemClock(emSYSTICK_On, NULL);
    HANDLE hTIM = CreateFile(emIP_TIM);
    if (hTIM == NULL)       while(1);

    tAPP_TIM_DCB dcb = {
// Base parameter
        .hSub       = emFILE_TIM4,          // 1:TIM1 2:TIM2 2:TIM3 ...
        .mode       = emTIM_PWM,            // emPWM, emCapturePulse, emCapturePWM

// Timer base count configure
        .cntFreq    = 1000000,              // unit: 1Hz ==> timer counter clock = 1MHz
        .period     = 1000,                 // period = 1 MHz * 1000 = 1KHz

// Timer channel configure
        .ch         = emTIM_CH4,            // select config channel
        .pulse      = 500,                  // 500 / 1000 = 50% PWM dutycycle

// Callback function
        .cbUp    = (u32)&UpdateCallback,    //NULLNULL, //
        .cbCc    = (u32)&CCCallback,        //NULLNULL, //

// Operation mode
        .type       = emTYPE_IT,            // emTYPE_IT, emTYPE_DMA
        .sync       = emTYPE_Sync,          // emTYPE_Sync, emTYPE_ASync
        .remapEn    = false,                // 0, 1
        .remapIdx   = 0,                    // u8 value
    };

// Step 3:  Open File Device     ---------------------->>>>>
    if (!OpenFile(hTIM, (void*)&dcb))       while(1);
    pwmValue = dcb.pulse;                // set 50% PWM dutyCycle


    // Step 2:  Create File Device   ---------------------->>>>>
    HANDLE hADC = CreateFile(emIP_ADC);
    if (hADC == NULL)       while(1);

// Step 3:  Assignment DCB structure    --------------->>>>>
    tAPP_ADC_DCB dcb2 = {
        // Operation mode
        .hSub   = emFILE_ADC1,                  // EM_FILE_ADC
        .type   = emTYPE_DMA,                   // polling, interrupt, dma
        .mode   = emADC_Scan,               // Conversion mode: emADC_Imm,emADC_Scan,emADC_Continue
        .sync   = emTYPE_ASync,                 // emTYPE_Sync, emTYPE_ASync
        .trig   = emTRIGGER_Software,           // Software Start & Trigger enum
        .chs    = LEFT_SHIFT_BIT(4),          // channels: ADC_CH_n
        .temp   = false,                        // Temperature measurement:0(DISABLE),1(ENABLE)
        .vRef   = false,                        // Reference voltage:0(DISABLE),1(ENABLE)
        .cb     = NULL,                         // adc callback
    };

    if (!OpenFile(hADC, (void*)&dcb2))      while(1);


//    WriteFile(hTIM, emFILE_TIM4, (u8*)&pwmValue, 1);

    lcd_init();

    lcd_clear(WHITE);

    lcd_drawLine(10, 80, 200, 100);
    lcd_drawLine(10, 80, 200, 200);
    lcd_drawRectangle(10 ,30, 40, 70);
    lcd_drawCircle(100, 40, 30);

    POINT_COLOR = RED;
    BACK_COLOR = WHITE;
    lcd_showString(0, 100, 240, 32, 32, "Mindmotion");
    lcd_showString(0, 140, 240, 24, 24, "TFTLCD TEST 240*240");

    POINT_COLOR = WHITE;
    BACK_COLOR = BLUE;
    lcd_showString(0, 170, 240, 16, 16, "Mindmotion");
    lcd_showString(0, 200, 240, 12, 12, "2019/05/08");


    for(u16 i = 10000; i > 0; i--)
        for(u16 j = 3000; j > 0; j--)
            ;

    color = 0xF800;

    while(1){
        WriteFile(hADC, emFILE_ADC1, 0, 0);
        ReadFile(hADC, emFILE_ADC1, (u8*)&adc_temp, 16);

        pwmValue = (u16)((double)adc_temp / 4096 * 1000);

        WriteFile(hTIM, emFILE_TIM4, (u8*)&pwmValue, 1);

        if (color == 0xF800)
            flag = 0;
        else if (color == 0xFFE0)
            flag = 1;
        else if (color == 0x07E0)
            flag = 2;
        else if (color == 0x07FF)
            flag = 3;
        else if (color == 0x001F)
            flag = 4;
        else if (color == 0xF81F)
            flag = 5;


        if (flag == 0)
            color += 0x20;
        else if (flag == 1)
            color -= 0x800;
        else if (flag == 2)
            color += 0x0001;
        else if (flag == 3)
            color -= 0x20;
        else if (flag == 4)
            color += 0x800;
        else if (flag == 5)
            color -= 0x0001;

        lcd_clear(color);

    }
}
