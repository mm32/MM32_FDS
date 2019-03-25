////////////////////////////////////////////////////////////////////////////////
/// @file     SPI_COMMUNICATION_INIT.C
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE SPI_COMMUNICATION_INIT EXAMPLE.
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
#define _SPI_COMMUNICATION_INIT_C_
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "MM32.h"

#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_misc.h"

#include "hal_spi.h"

#include "spi.h"
////////////////////////////////////////////////////////////////////////////////
// Initial function
////////////////////////////////////////////////////////////////////////////////
void SPI_GPIO_Init(SPI_TypeDef* SPIx)
{
    GPIO_InitTypeDef GPIO_InitStructure;

#if defined(__MM0N1) || defined(__MM0Q1)
#if defined(__MM0N1)
    if (SPIx == SPI1) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

        GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);
#if defined(SPI_MASTER_MODE)
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;  // spi_nss
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6;  // spi1_sck / spi1_mosi
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  // spi1_miso
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
#elif defined(SPI_SLAVE_MODE)
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;  // spi_nss
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;  // spi1_sck
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  // spi1_mosi
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  // spi1_miso
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
    }
#endif
    if (SPIx == SPI2) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

        GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_0);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);
#if defined(SPI_MASTER_MODE)
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_15;  // spi_sck / spi1_mosi
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;  // spi_nss
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  // spi_miso
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
#elif defined(SPI_SLAVE_MODE)
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;  // spi_sck
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;  // spi1_mosi
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;  // spi_nss
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  // spi_miso
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
    }

#elif defined(__MM3N1)
    if (SPIx == SPI1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#if defined(SPI_MASTER_MODE)
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;  // spi_nss
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;  // spi_sck
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;  // spi_mosi
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;  // spi_miso
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
#elif defined(SPI_SLAVE_MODE)
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;  // spi_nss
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;  // spi_sck
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;  // spi_mosi
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;  // spi_miso
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
    }
    if (SPIx == SPI2) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
#if defined(SPI_MASTER_MODE)
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;  // spi_nss
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;  // spi_sck
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;  // spi_mosi
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;  // spi_miso
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
#elif defined(SPI_SLAVE_MODE)
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;  // spi_nss
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;  // spi_sck
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;  // spi_mosi
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;  // spi_miso
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
    }
#endif
}

void SPI_NVIC_Init(SPI_TypeDef* SPIx)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    if (SPIx == SPI2)
        NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
#if defined(__MM0N1) || defined(__MM3N1)
    if (SPIx == SPI1)
        NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
#endif

#if defined(__MM3N1)
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
#elif defined(__MM0N1) || defined(__MM0Q1)
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
#endif

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
// Interruput handle function
////////////////////////////////////////////////////////////////////////////////
#if defined(__MM3N1) || defined(MZ206)
void SPI1_IRQHandler(void)
{
#if defined(SPI_MASTER_MODE)
    if (SPI_GetITStatus(SPI1, SPI_IT_TXEPT)) {
        SPI_ClearITPendingBit(SPI1, SPI_IT_TXEPT);
        if (SPI_TX_count < SPI_datalength) {
            SPI_SendData(SPI1, txbyte[SPI_TX_count++]);
        }
        else {
            //          while(!SPI_GetITStatus(SPI1, SPI_IT_TXEPT)){;}
            for (int i = 0; i < 100; i++) {
                ;
            }
            // SPI_CSInternalSelected(SPI2,DISABLE);
            //          SPI_ITConfig(SPI2,SPI_IT_TXEPT,DISABLE);
        }
    }
#elif defined(SPI_SLAVE_MODE)
    if (SPI_GetITStatus(SPI1, SPI_IT_RX)) {
        SPI_ClearITPendingBit(SPI1, SPI_IT_RX);

        if (SPI_RX_count < SPI_datalength) {
            rxbyte[SPI_RX_count++] = SPI_ReceiveData(SPI1);
        }
        else {
            //              SPI_ITConfig(SPI2,SPI_IT_RX,DISABLE);
        }
    }
#endif
}
#endif

void SPI2_IRQHandler(void)
{
#if defined(SPI_MASTER_MODE)
    if (SPI_GetITStatus(SPI2, SPI_IT_TXEPT)) {
        SPI_ClearITPendingBit(SPI2, SPI_IT_TXEPT);
        if (SPI_TX_count < SPI_datalength) {
            SPI_SendData(SPI2, txbyte[SPI_TX_count++]);
        }
        else {
            SPI_ITConfig(SPI2, SPI_IT_TXEPT, DISABLE);
            SPI_CSInternalSelected(SPI2, DISABLE);
            TX_finish_flag = 1;
        }
    }
#elif defined(SPI_SLAVE_MODE)
    if (SPI_GetITStatus(SPI2, SPI_IT_RX)) {
        SPI_ClearITPendingBit(SPI2, SPI_IT_RX);

        if (SPI_RX_count < SPI_datalength) {
            rxbyte[SPI_RX_count++] = SPI_ReceiveData(SPI2);
        }
        else {
            SPI_ITConfig(SPI2, SPI_IT_RX, DISABLE);
            RX_finish_flag = 1;
        }
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Main proccess function
////////////////////////////////////////////////////////////////////////////////
void SPI_Configure(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStructure)
{
    //------------------------------------------------------------------------//
    if (SPIx == SPI2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    else
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    SPI_GPIO_Init(SPIx);
    SPI_NVIC_Init(SPIx);
    SPI_Init(SPIx, SPI_InitStructure);

    //------------------------------------------------------------------------//
#if defined(SPI_MASTER_MODE)
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
    SPI_TX_count   = 0;
    TX_finish_flag = 0;
    SPI_ITConfig(SPIx, SPI_IT_TXEPT, ENABLE);
#elif defined(SPI_SLAVE_MODE)
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
    SPI_RX_count   = 0;
    RX_finish_flag = 0;
    SPI_ITConfig(SPIx, SPI_IT_RX, ENABLE);
#endif
}
