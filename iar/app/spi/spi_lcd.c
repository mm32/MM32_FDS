////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_LCD.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE SPI_LCD EXAMPLE.
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

#define _SPI_LCD_C_

#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "hal_exti.h"
#include "drv.h"
#include "hal_spi.h"
#include "hal_rcc.h"
#include "hal_gpio.h"

#include "bsp.h"
#include "bsp_spi.h"

#include "spi_lcd.h"
#include "font.h"



static void lcd_gpioInit()
{
    GPIOB_ClockEnable();
    GPIOC_ClockEnable();

    GPIO_SetBits(GPIOC, GPIO_Pin_11);
    GPIO_AF_Init(GPIOC, GPIO_Pin_11, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_0); //LCD_spi_CS

    GPIO_Mode_AF_PP_50MHz_Init  (GPIOB, GPIO_Pin_13, NO_REMAP, GPIO_AF_0);      // LCD_spi2_sck
    GPIO_Mode_AF_PP_50MHz_Init  (GPIOB, GPIO_Pin_15, NO_REMAP, GPIO_AF_0);      // LCD_spi2_mosi

//    GPIO_AF_Init(GPIOB, GPIO_Pin_9, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_0); //LCD_BS
    GPIO_AF_Init(GPIOC, GPIO_Pin_12, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, NO_REMAP, GPIO_AF_0); //LCD_CD

    LCD_CS(1);
    LCD_BS(0); //close backlight
}

static void lcd_spiInit()
{
    Set_SPI_Clock(SPI2);
//    SET_BIT(SPI2->GCR, SPI_GCR_DWSEL);

    SPI_InitTypeDef InitStructure;

    InitStructure.SPI_CPOL  = SPI_CPOL_Low;
    InitStructure.SPI_CPHA  = SPI_CPHA_1Edge;
    InitStructure.SPI_DataWidth         = 9;

    InitStructure.SPI_NSS               = SPI_NSS_Soft;
    InitStructure.SPI_Mode              = SPI_Mode_Master;
    InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    InitStructure.SPI_BaudRatePrescaler = 2;

    SPI_Init(SPI2, &InitStructure);

    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);

//    if (pDcb->fastMode == true){
//        SET_BIT(SPIx->CCR, SPI_CCR_RXEDGE | SPI_CCR_TXEDGE);
//    }
//    else{
//        CLEAR_BIT(SPIx->CCR, SPI_CCR_RXEDGE | SPI_CCR_TXEDGE);
//    }

//    SET_BIT(SPI2->GCR, SPI_GCR_DWSEL);

    SPI_Cmd(SPI2, ENABLE);
}

void spiWritex(u16 x)
{
    __asm volatile("cpsid i");
    WRITE_REG(SPI2->TDR, x >> 1);
    WRITE_REG(SPI2->TDR, x << 7);
    __asm volatile("cpsie i");
    while(!SPI_GetFlagStatus(SPI2, SPI_FLAG_TXEPT)) {
    }
}



static void lcd_writeCmd(u8 cmd)
{
    LCD_CS(0);
    u16 x = cmd;
    spiWritex(x);
    LCD_CS(1);
}

static void lcd_writeData(u8 data)
{
    LCD_CS(0);
    u16 x = data | 0x100;
    spiWritex(x);
    LCD_CS(1);
}


static void lcd_writeData16(u16 data)
{
    LCD_CS(0);
    spiWritex((data  >> 8)  | 0x100);
    spiWritex((data & 0xff) | 0x100);
    LCD_CS(1);
}



void lcd_addressSet(u16 x1, u16 y1, u16 x2, u16 y2)
{
    lcd_writeCmd(0x2a);
    lcd_writeData(x1 >> 8);
    lcd_writeData(x1);
    lcd_writeData(x2 >> 8);
    lcd_writeData(x2);

    lcd_writeCmd(0x2b);
    lcd_writeData(y1 >> 8);
    lcd_writeData(y1);
    lcd_writeData(y2 >> 8);
    lcd_writeData(y2);

    lcd_writeCmd(0x2C);
}

#if (1)
void lcd_clear(u16 color)
{
    u16 i, j;
//    u8 data[2] = {0};
//
//    data[0] = color >> 8;
//    data[1] = color;

    lcd_addressSet(0, 0, LCD_Width - 1, LCD_Height - 1);

//    for(j = 0; j < LCD_Buf_Size / 2; j++)
//    {
//        lcd_buf[j * 2] =  data[0];
//        lcd_buf[j * 2 + 1] =  data[1];
//    }

//    LCD_DC(1);

    for(i = 0; i < (LCD_Width * LCD_Height) / 8; i++)
    {
        lcd_writeData16(color);
        lcd_writeData16(color);
        lcd_writeData16(color);
        lcd_writeData16(color);
        lcd_writeData16(color);
        lcd_writeData16(color);
        lcd_writeData16(color);
        lcd_writeData16(color);
    }
}


#else

void lcd_clear(u16 color)
{
    u16 i, j;
    u8 data[2] = {0};

    data[0] = color >> 8;
    data[1] = color;

    lcd_addressSet(0, 0, LCD_Width - 1, LCD_Height - 1);

    for(j = 0; j < LCD_Buf_Size / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }

    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++){

        for(j = 0; j < LCD_Buf_Size; j++){
            lcd_writeData(lcd_buf[j]);
        }

    }
}


#endif


u8 lcd_init()
{
    lcd_gpioInit();
    lcd_spiInit();
        LCD_BS(1);

    for(u16 i = 10000; i > 0; i--);
    lcd_writeCmd(0x11);
    for(u16 i = 10000; i > 0; i--);


    /* Memory Data Access Control */
    lcd_writeCmd(0x36);
    lcd_writeData(0x00);

    /* RGB 5-6-5-bit  */
    lcd_writeCmd(0x3A);
    lcd_writeData(0x65);

    /* Porch Setting */
    lcd_writeCmd(0xB2);
    lcd_writeData(0x0C);
    lcd_writeData(0x0C);
    lcd_writeData(0x00);
    lcd_writeData(0x33);
    lcd_writeData(0x33);

    /*  Gate Control */
    lcd_writeCmd(0xB7);
    lcd_writeData(0x72);

    /* VCOM Setting */
    lcd_writeCmd(0xBB);
    lcd_writeData(0x3D);   //Vcom=1.625V

    /* LCM Control */
    lcd_writeCmd(0xC0);
    lcd_writeData(0x2C);

    /* VDV and VRH Command Enable */
    lcd_writeCmd(0xC2);
    lcd_writeData(0x01);

    /* VRH Set */
    lcd_writeCmd(0xC3);
    lcd_writeData(0x19);

    /* VDV Set */
    lcd_writeCmd(0xC4);
    lcd_writeData(0x20);

    /* Frame Rate Control in Normal Mode */
    lcd_writeCmd(0xC6);
    lcd_writeData(0x0F);    //60MHZ

    /* Power Control 1 */
    lcd_writeCmd(0xD0);
    lcd_writeData(0xA4);
    lcd_writeData(0xA1);

    /* Positive Voltage Gamma Control */
    lcd_writeCmd(0xE0);
    lcd_writeData(0xD0);
    lcd_writeData(0x04);
    lcd_writeData(0x0D);
    lcd_writeData(0x11);
    lcd_writeData(0x13);
    lcd_writeData(0x2B);
    lcd_writeData(0x3F);
    lcd_writeData(0x54);
    lcd_writeData(0x4C);
    lcd_writeData(0x18);
    lcd_writeData(0x0D);
    lcd_writeData(0x0B);
    lcd_writeData(0x1F);
    lcd_writeData(0x23);

    /* Negative Voltage Gamma Control */
    lcd_writeCmd(0xE1);
    lcd_writeData(0xD0);
    lcd_writeData(0x04);
    lcd_writeData(0x0C);
    lcd_writeData(0x11);
    lcd_writeData(0x13);
    lcd_writeData(0x2C);
    lcd_writeData(0x3F);
    lcd_writeData(0x44);
    lcd_writeData(0x51);
    lcd_writeData(0x2F);
    lcd_writeData(0x1F);
    lcd_writeData(0x1F);
    lcd_writeData(0x20);
    lcd_writeData(0x23);

    /* Display Inversion On */
    lcd_writeCmd(0x21);

    lcd_writeCmd(0x29);

    lcd_addressSet(0, 0, LCD_Width - 1, LCD_Height - 1);

    lcd_clear(WHITE);

//    LCD_BS(1);


    lcd_clear(WHITE);
    lcd_clear(BLACK);
    lcd_clear(BLUE);
    lcd_clear(RED);
    lcd_clear(MAGENTA);
    lcd_clear(GREEN);
    lcd_clear(CYAN);
    lcd_clear(YELLOW);
    lcd_clear(BRRED);
    lcd_clear(GRAY);
    lcd_clear(LGRAY);
    lcd_clear(BROWN);
    lcd_clear(WHITE);

    return 1;
}

void lcd_drawPoint(u16 x, u16 y)
{
    lcd_addressSet(x, y, x, y);
    lcd_writeData16(POINT_COLOR);
}

void LCD_drawColorPoint(u16 x, u16 y,u16 color)
{
    lcd_addressSet(x, y, x, y);
    lcd_writeData16(color);
}

void lcd_drawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    u32 i = 0;

    if(y1 == y2){
        lcd_addressSet(x1, y1, x2, y2);

        for(u16 x = 0; x < (x2 - x1); x++){
            lcd_writeData16(POINT_COLOR);
        }
        return;
    }

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if(delta_x > 0)incx = 1;

    else if(delta_x == 0)incx = 0;

    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if(delta_y > 0)incy = 1;

    else if(delta_y == 0)incy = 0;

    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if(delta_x > delta_y)distance = delta_x;

    else distance = delta_y;

    for(t = 0; t <= distance + 1; t++)
    {
        lcd_drawPoint(row, col);
        xerr += delta_x ;
        yerr += delta_y ;

        if(xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if(yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

void lcd_drawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    lcd_drawLine(x1, y1, x2, y1);
    lcd_drawLine(x1, y1, x1, y2);
    lcd_drawLine(x1, y2, x2, y2);
    lcd_drawLine(x2, y1, x2, y2);
}

void lcd_drawCircle(u16 x0, u16 y0, u8 r)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while(a <= b){
        lcd_drawPoint(x0 - b, y0 - a);
        lcd_drawPoint(x0 + b, y0 - a);
        lcd_drawPoint(x0 - a, y0 + b);
        lcd_drawPoint(x0 - b, y0 - a);
        lcd_drawPoint(x0 - a, y0 - b);
        lcd_drawPoint(x0 + b, y0 + a);
        lcd_drawPoint(x0 + a, y0 - b);
        lcd_drawPoint(x0 + a, y0 + b);
        lcd_drawPoint(x0 - b, y0 + a);
        a++;

        if(di < 0)di += 4 * a + 6;
        else{
            di += 10 + 4 * (a - b);
            b--;
        }
        lcd_drawPoint(x0 + a, y0 + b);
    }
}

static u32 lcd_pow(u8 m, u8 n)
{
    u32 result = 1;

    while(n--)result *= m;

    return result;
}

void lcd_showChar(u16 x, u16 y, char chr, u8 size)
{
    u8 temp, t1, t;
    u8 csize;
    u16 colortemp;
    u8 sta;

    chr = chr - ' ';

    if((x > (LCD_Width - size / 2)) || (y > (LCD_Height - size)))   return;

    lcd_addressSet(x, y, x + size / 2 - 1, y + size - 1);

    if((size == 16) || (size == 32) )
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            if(size == 16)temp = asc2_1608[chr][t];
            else if(size == 32)temp = asc2_3216[chr][t];
            else return;

            for(t1 = 0; t1 < 8; t1++)
            {
                if(temp & 0x80) colortemp = POINT_COLOR;
                else colortemp = BACK_COLOR;

                lcd_writeData16(colortemp);
                temp <<= 1;
            }
        }
    }

    else if  (size == 12)   //12o?б┴?им?
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            temp = asc2_1206[chr][t];

            for(t1 = 0; t1 < 6; t1++)
            {
                if(temp & 0x80) colortemp = POINT_COLOR;
                else colortemp = BACK_COLOR;

                lcd_writeData16(colortemp);
                temp <<= 1;
            }
        }
    }

    else if(size == 24)
    {
        csize = (size * 16) / 8;

        for(t = 0; t < csize; t++)
        {
            temp = asc2_2412[chr][t];

            if(t % 2 == 0)sta = 8;
            else sta = 4;

            for(t1 = 0; t1 < sta; t1++)
            {
                if(temp & 0x80) colortemp = POINT_COLOR;
                else colortemp = BACK_COLOR;

                lcd_writeData16(colortemp);
                temp <<= 1;
            }
        }
    }
}

void lcd_showNum(u16 x, u16 y, u32 num, u8 len, u8 size)
{
    u8 t, temp;
    u8 enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;

        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                lcd_showChar(x + (size / 2)*t, y, ' ', size);
                continue;
            }

            else enshow = 1;
        }

        lcd_showChar(x + (size / 2)*t, y, temp + '0', size);
    }
}

void lcd_showxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode)
{
    u8 t, temp;
    u8 enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;

        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                if(mode)lcd_showChar(x + (size / 2)*t, y, '0', size);
                else
                    lcd_showChar(x + (size / 2)*t, y, ' ', size);

                continue;
            }

            else enshow = 1;
        }

        lcd_showChar(x + (size / 2)*t, y, temp + '0', size);
    }
}

void lcd_showString(u16 x, u16 y, u16 width, u16 height, u8 size, char *p)
{
    u8 x0 = x;
    width += x;
    height += y;

    while((*p <= '~') && (*p >= ' '))
    {
        if(x >= width)
        {
            x = x0;
            y += size;
        }

        if(y >= height)break;

        lcd_showChar(x, y, *p, size);
        x += size / 2;
        p++;
    }
}
