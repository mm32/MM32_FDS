////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_COMP.C
/// @author   PX Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE COMP BSP LAYER FUNCTIONS.
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
#define _BSP_COMP_C_

// Files includes  -------------------------------------------------------------

#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_comp.h"

#include "bsp.h"
#include "bsp_comp.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup COMP_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup COMP_Exported_Functions
/// @{

/* ---------------------------------------------------------------------
------  P e r i p h e r a l s    i n i t i a l i z a t i o n   ---------
--------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the  channel of comparator.
/// @param  COMPx: the selected comparator.
//  @param  inp: non invert input selection
//  @param  inm: invert input selection
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BSP_COMP_GPIO_Configure(u32 COMPx, u8 inp, u8 inm, u8 out)
{
#if defined(__MM0N1) || defined(__MM0Q1)
    GPIOA_ClockEnable();
#endif
#if defined(__MM0P1)
    GPIOA_ClockEnable(); GPIOB_ClockEnable(); GPIOD_ClockEnable();
#endif

#if defined(__REGISTER)  /* ----------- Register Access ------------- */
    switch((u32)COMPx) {
    case COMP1:
#if defined(__MM0N1)
/*  __MM0N1 -------------------------------------------------------------
        compare 1:
            OUT:    PA0,  PA6,  PA11
            INP:    PA 0 : 7
            INM:    PA4, PA5, PA0, PA6
        compare 2:
            OUT:    PA2,  PA7,  PA12
            INP:    PA 0 : 7
            INM:    PA4, PA5, PA2, PA6
-----------------------------------------------------------------------*/
                                GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << (inp * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << (inp * 4)) | (0x07 << (inp * 4));

        if      (inm == 4) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((4  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((4  % 8) * 4)) | (0x07 << ((4  % 8) * 4));}
        else if (inm == 5) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((5  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((5  % 8) * 4)) | (0x07 << ((5  % 8) * 4));}
        else if (inm == 6) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((0  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((0  % 8) * 4)) | (0x07 << ((0  % 8) * 4));}
        else if (inm == 7) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((6  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((6  % 8) * 4)) | (0x07 << ((60  % 8) * 4));}


        if      (out == 1) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((0  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((0  % 8) * 4)) | (0x07 << ((0  % 8) * 4));}
        else if (out == 2) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((6  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((6  % 8) * 4)) | (0x07 << ((6  % 8) * 4));}
        else if (out == 3) {    GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((11 % 8) * 4));
                                GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((11 % 8) * 4)) | (0x07 << ((11 % 8) * 4));}
#endif
#if defined(__MM0P1)
/*  __MM0P1 -------------------------------------------------------------
        compare 1:
            OUT:    PA0, PA6, PA11
            INP:    PD3, PA6, PB2, PB10
            INM:    PB12, PA8, PD2

        compare 2:
            OUT:    PA2, PA7, PA12, PC10
            INP:    PA11, PA6, PB2, PB10
            INM:    PB12, PA8, PA12

        compare 3:
            OUT:    PA14£¬ PD4
            INP:    PB8
            INM:    PB9

        compare 4:
            OUT:    PA13, PD0, PD5
            INP:    PD7
            INM:    PC13

        compare 5:
            OUT:    PD1, PD6
            INP:    PC14
            INM:    PC15
-----------------------------------------------------------------------*/
        if      (inp == 0) {    GPIOD->CRL  = GPIOD->CRL  & ~(0x0F << ((3  % 8) * 4));
                                GPIOD->AFRL = GPIOD->AFRL & ~(0x0F << ((3  % 8) * 4)) | (0x07 << ((3  % 8) * 4));}
        else if (inp == 1) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((6  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((6  % 8) * 4)) | (0x07 << ((6  % 8) * 4));}
        else if (inp == 2) {    GPIOB->CRL  = GPIOB->CRL  & ~(0x0F << ((2  % 8) * 4));
                                GPIOB->AFRL = GPIOB->AFRL & ~(0x0F << ((2  % 8) * 4)) | (0x07 << ((2  % 8) * 4));}
        else if (inp == 3) {    GPIOB->CRH  = GPIOB->CRH  & ~(0x0F << ((10 % 8) * 4));
                                GPIOB->AFRH = GPIOB->AFRH & ~(0x0F << ((10 % 8) * 4)) | (0x07 << ((10 % 8) * 4));}
        else if (inm == 0) {    GPIOB->CRH  = GPIOB->CRH  & ~(0x0F << ((12 % 8) * 4));
                                GPIOB->AFRH = GPIOB->AFRH & ~(0x0F << ((12 % 8) * 4)) | (0x07 << ((12 % 8) * 4));}
        else if (inm == 1) {    GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((8  % 8) * 4));
                                GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((8  % 8) * 4)) | (0x07 << ((8  % 8) * 4));}
        else if (inm == 2) {    GPIOD->CRL  = GPIOD->CRL  & ~(0x0F << ((2  % 8) * 4));
                                GPIOD->AFRL = GPIOD->AFRL & ~(0x0F << ((2  % 8) * 4)) | (0x07 << ((2  % 8) * 4));}

        if      (out == 1) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((2  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((2  % 8) * 4)) | (0x07 << ((2  % 8) * 4));}
        else if (out == 2) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((7  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((7  % 8) * 4)) | (0x07 << ((7  % 8) * 4));}
        else if (out == 3) {    GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((12 % 8) * 4));
                                GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((12 % 8) * 4)) | (0x07 << ((12 % 8) * 4));}
#endif
#if defined(__MM0Q1)
/*  __MM0Q1 -------------------------------------------------------------
        compare 1:
            OUT:    PA0,  PA6,  PA11
            INP:    PA 1 : 4
            INM:    PA 5 : 7
-----------------------------------------------------------------------*/
        GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((++inp) * 4));
        GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((++inp) * 4)) | (0x07 << ((++inp) * 4));

        GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((inm + 5) * 4));
        GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((inm + 5) * 4)) | (0x07 << ((inm + 5) * 4));


        if      (out == 1) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((0  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((0  % 8) * 4)) | (0x07 << ((0  % 8) * 4));}
        else if (out == 2) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((6  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((6  % 8) * 4)) | (0x07 << ((6  % 8) * 4));}
        else if (out == 3) {    GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((11 % 8) * 4));
                                GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((11 % 8) * 4)) | (0x07 << ((11 % 8) * 4));}
#endif
        break;


#if defined(__MM0N1)
    case COMP2:
                                GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << (8  * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << (inp * 4)) | (0x07 << (inp * 4));

        if      (inm == 4) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((4  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((4  % 8) * 4)) | (0x07 << ((4  % 8) * 4));}
        else if (inm == 5) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((5  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((5  % 8) * 4)) | (0x07 << ((5  % 8) * 4));}
        else if (inm == 6) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((2  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((2  % 8) * 4)) | (0x07 << ((2  % 8) * 4));}
        else if (inm == 7) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((6  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((6  % 8) * 4)) | (0x07 << ((6  % 8) * 4));}

        if      (out == 0) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((2  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((2  % 8) * 4)) | (0x07 << ((2  % 8) * 4));}
        else if (out == 1) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((7  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((7  % 8) * 4)) | (0x07 << ((7  % 8) * 4));}
        else if (out == 2) {    GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((12 % 8) * 4));
                                GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((12 % 8) * 4)) | (0x07 << ((12 % 8) * 4));}

#endif
#if defined(__MM0P1)
    case COMP2:
        if      (inp == 0) {    GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((11 % 8) * 4));
                                GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((11 % 8) * 4)) | (0x07 << ((11 % 8) * 4));}
        else if (inp == 1) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((6  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((6  % 8) * 4)) | (0x07 << ((6  % 8) * 4));}
        else if (inp == 2) {    GPIOB->CRL  = GPIOB->CRL  & ~(0x0F << ((2  % 8) * 4));
                                GPIOB->AFRL = GPIOB->AFRL & ~(0x0F << ((2  % 8) * 4)) | (0x07 << ((2  % 8) * 4));}
        else if (inp == 3) {    GPIOB->CRH  = GPIOB->CRH  & ~(0x0F << ((10 % 8) * 4));
                                GPIOB->AFRH = GPIOB->AFRH & ~(0x0F << ((10 % 8) * 4)) | (0x07 << ((10 % 8) * 4));}
        else if (inm == 0) {    GPIOB->CRH  = GPIOB->CRH  & ~(0x0F << ((12 % 8) * 4));
                                GPIOB->AFRH = GPIOB->AFRH & ~(0x0F << ((12 % 8) * 4)) | (0x07 << ((12 % 8) * 4));}
        else if (inm == 1) {    GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((8  % 8) * 4));
                                GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((8  % 8) * 4)) | (0x07 << ((8  % 8) * 4));}
        else if (inm == 2) {    GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((12 % 8) * 4));
                                GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((12 % 8) * 4)) | (0x07 << ((12 % 8) * 4));}

        if      (out == 1) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((2  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((2  % 8) * 4)) | (0x07 << ((2  % 8) * 4));}
        else if (out == 2) {    GPIOA->CRL  = GPIOA->CRL  & ~(0x0F << ((7  % 8) * 4));
                                GPIOA->AFRL = GPIOA->AFRL & ~(0x0F << ((7  % 8) * 4)) | (0x07 << ((7  % 8) * 4));}
        else if (out == 3) {    GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((12 % 8) * 4));
                                GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((12 % 8) * 4)) | (0x07 << ((12 % 8) * 4));}
        else if (out == 4) {    GPIOC->CRH  = GPIOC->CRH  & ~(0x0F << ((10 % 8) * 4));
                                GPIOC->AFRH = GPIOC->AFRH & ~(0x0F << ((10 % 8) * 4)) | (0x07 << ((10 % 8) * 4));}
#endif
        break;

#if defined(MZ308)
    case COMP3:             GPIOB->CRH  = GPIOB->CRH  & ~(0x0F << ((8  % 8) * 4));
                            GPIOB->AFRH = GPIOB->AFRH & ~(0x0F << ((12 % 8) * 4)) | (0x07 << ((12 % 8) * 4));
                            GPIOB->CRH  = GPIOB->CRH  & ~(0x0F << ((9  % 8) * 4));
                            GPIOB->AFRH = GPIOB->AFRH & ~(0x0F << ((9  % 8) * 4)) | (0x07 << ((9  % 8) * 4));

        if (out == 1) {     GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((14 % 8) * 4));
                            GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((14 % 8) * 4)) | (0x07 << ((14 % 8) * 4));}
        if (out == 2) {     GPIOD->CRL  = GPIOD->CRL  & ~(0x0F << ((4  % 8) * 4));
                            GPIOD->AFRL = GPIOD->AFRL & ~(0x0F << ((4  % 8) * 4)) | (0x07 << ((4  % 8) * 4));}
        break;

    case COMP4:             GPIOD->CRL  = GPIOD->CRL  & ~(0x0F << ((7  % 8) * 4));
                            GPIOD->AFRL = GPIOD->AFRL & ~(0x0F << ((7  % 8) * 4)) | (0x07 << ((7  % 8) * 4));
                            GPIOC->CRH  = GPIOC->CRH  & ~(0x0F << ((13 % 8) * 4));
                            GPIOC->AFRH = GPIOC->AFRH & ~(0x0F << ((13 % 8) * 4)) | (0x07 << ((13 % 8) * 4));

        if (out == 1) {     GPIOA->CRH  = GPIOA->CRH  & ~(0x0F << ((13 % 8) * 4));
                            GPIOA->AFRH = GPIOA->AFRH & ~(0x0F << ((13 % 8) * 4)) | (0x07 << ((13 % 8) * 4));}
        if (out == 2) {     GPIOD->CRL  = GPIOD->CRL  & ~(0x0F << ((0  % 8) * 4));
                            GPIOD->AFRL = GPIOD->AFRL & ~(0x0F << ((0  % 8) * 4)) | (0x07 << ((0  % 8) * 4));}
        if (out == 3) {     GPIOD->CRL  = GPIOD->CRL  & ~(0x0F << ((5  % 8) * 4));
                            GPIOD->AFRL = GPIOD->AFRL & ~(0x0F << ((5  % 8) * 4)) | (0x07 << ((5  % 8) * 4));}
        break;

    case COMP5:             GPIOC->CRH  = GPIOC->CRH  & ~(0x0F << ((14 % 8) * 4));
                            GPIOC->AFRH = GPIOC->AFRH & ~(0x0F << ((14 % 8) * 4)) | (0x07 << ((14 % 8) * 4));
                            GPIOC->CRH  = GPIOC->CRH  & ~(0x0F << ((15 % 8) * 4));
                            GPIOC->AFRH = GPIOC->AFRH & ~(0x0F << ((15 % 8) * 4)) | (0x07 << ((15 % 8) * 4));
        if (out == 1) {     GPIOD->CRL  = GPIOD->CRL  & ~(0x0F << ((1  % 8) * 4));
                            GPIOD->AFRL = GPIOD->AFRL & ~(0x0F << ((1  % 8) * 4)) | (0x07 << ((1  % 8) * 4));}
        if (out == 2) {     GPIOD->CRL  = GPIOD->CRL  & ~(0x0F << ((6  % 8) * 4));
                            GPIOD->AFRL = GPIOD->AFRL & ~(0x0F << ((6  % 8) * 4)) | (0x07 << ((6  % 8) * 4));}
        break;
#endif
    default: break;
    }


#else       /* ----------- Hardware Abstraction Layer Access ------------- */
    switch(COMPx) {
    case COMP1:
#if defined(__MM0N1)
        if (inm == 4)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        if (inm == 5)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        if (inm == 6)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_0, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        if (inm == 7)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);


        GPIO_Mode_IN_Init(GPIOA, 1 << inp , GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        GPIO_Mode_IN_Init(GPIOA, 1 << out , GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
#endif
#if defined(__MM0P1)
        if      (inp == 0)  GPIO_Mode_IN_Init(GPIOD, GPIO_Pin_3,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        if      (inp == 1)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_6,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inp == 2)  GPIO_Mode_IN_Init(GPIOB, GPIO_Pin_2,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inp == 3)  GPIO_Mode_IN_Init(GPIOB, GPIO_Pin_10, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inm == 0)  GPIO_Mode_IN_Init(GPIOB, GPIO_Pin_12, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inm == 1)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_8,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inm == 2)  GPIO_Mode_IN_Init(GPIOD, GPIO_Pin_2,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);

        if      (out == 1)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_2,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 2)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_7,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 3)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_12, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
#endif
#if defined(__MM0Q1)
        if (inm == 0)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        if (inm == 1)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        if (inm == 2)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_7, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);

        GPIO_Mode_IN_Init(GPIOA, 1 << (inp + 1) , GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        if      (out == 1)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_0,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 2)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_6,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 3)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_11, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
#endif
        break;


#if defined(__MM0N1)
    case COMP2:
        if (inm == 4)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        if (inm == 5)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        if (inm == 6)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        if (inm == 7)   GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);

        GPIO_Mode_IN_Init(GPIOA, 1 << inp , GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        GPIO_Mode_IN_Init(GPIOA, 1 << out , GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);

#endif
#if defined(__MM0P1)
    case COMP2:
        if      (inp == 0)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_6,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inp == 1)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_11, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inp == 2)  GPIO_Mode_IN_Init(GPIOB, GPIO_Pin_2,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inp == 3)  GPIO_Mode_IN_Init(GPIOB, GPIO_Pin_10, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inm == 0)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_8,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inm == 1)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_12, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (inm == 2)  GPIO_Mode_IN_Init(GPIOB, GPIO_Pin_12, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);

        if      (out == 1)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_2,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 2)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_7,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 3)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_12, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 4)  GPIO_Mode_IN_Init(GPIOC, GPIO_Pin_10, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
#endif
        break;

#if defined(__MM0P1)
    case COMP3:             GPIO_Mode_IN_Init(GPIOB, GPIO_Pin_8,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
                            GPIO_Mode_IN_Init(GPIOB, GPIO_Pin_9,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);

        if      (out == 1)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_14, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 2)  GPIO_Mode_IN_Init(GPIOD, GPIO_Pin_4,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        break;

    case COMP4:             GPIO_Mode_IN_Init(GPIOD, GPIO_Pin_7,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
                            GPIO_Mode_IN_Init(GPIOC, GPIO_Pin_12, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);

        if      (out == 1)  GPIO_Mode_IN_Init(GPIOA, GPIO_Pin_13, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 2)  GPIO_Mode_IN_Init(GPIOD, GPIO_Pin_0,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 3)  GPIO_Mode_IN_Init(GPIOD, GPIO_Pin_5,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        break;

    case COMP5:             GPIO_Mode_IN_Init(GPIOC, GPIO_Pin_14, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
                            GPIO_Mode_IN_Init(GPIOC, GPIO_Pin_15, GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);

        if      (out == 1)  GPIO_Mode_IN_Init(GPIOD, GPIO_Pin_1,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        else if (out == 2)  GPIO_Mode_IN_Init(GPIOD, GPIO_Pin_6,  GPIO_Mode_AIN, NO_REMAP, GPIO_AF_7);
        break;
#endif

    default: break;
    }
/* -------------------------------------------------------------------------- */
#endif           /* ----------------- End Access  ------------------- */
/* -------------------------------------------------------------------------- */
}

/// @}


/// @}

/// @}
