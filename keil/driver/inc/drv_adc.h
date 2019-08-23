////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_ADC.H
/// @author   Y Shi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE ADC
///           DRIVER LAYER.
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
#ifndef __DRV_ADC_H
#define __DRV_ADC_H


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup ADC
/// @brief ADC driver modules
/// @{


///////////////////////////////////////////////////////////////////////////////
/// @defgroup ADC_Exported_Constants
/// @{

#define WINDOW_HI   0x400
#define WINDOW_LO   0x100

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup ADC_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief ADC DCB Structure definition
////////////////////////////////////////////////////////////////////////////////
//typedef struct {
//  ADC_TypeDef*            ADCx;
//  EM_TYPE                 type;                       // polling, interrupt, dma
//  ADCPRE_TypeDef          pres;                       // Clock Prescaler: 2,4,6,8,10,12,14,16
//  ADCMODE_TypeDef         mode;                       // Conversion mode: ADC_Mode_Imm,ADC_Mode_Scan, ADC_Mode_Continue,
//  EM_TRIGGER              trig;                       // External trigger source selection
//  ADCRSL_TypeDef          rslt;                       // Resolution: 8~12
//  ADCDATAALI_TypeDef      align;                      // align: 0(right),1(left)
//  u32                     chs;                        // channel: 0~15
//  ADCSAM_TypeDef          samp;                       // Channel sampling period:1.5,7.5,13.5,28.5,41.5,55.5,71.5,239.5
//} tDRV_ADC_DCB;

////////////////////////////////////////////////////////////////////////////////
/// @brief ADC instance Structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX                 sPrefix;
    EM_TYPE                 type;                       // polling, interrupt, dma
    EM_ADC_MODE             mode;                       // Conversion mode: emSingle,emSingleScan,emContinueScan
    EM_TYPE                 sync;                       // synchronous mode or asynchronous mode
    u32                     cb;                         // adc callback
    EM_TRIGGER              trig;                       // External trigger source selection
    u8                      chNo;                       // total number of adc channel
} tDRV_ADC_INSTANCE;

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup ADC_Exported_Variables
/// @{

#ifdef _DRV_ADC_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
#if defined(__MM3N1) || defined(__MM0P1)
    static ADC_TypeDef* tbIpBase[]  = {ADC1, ADC2};
    static s8 tbSubHandleIdx[]          = {0,     1};

// e.g.
//  static ADC_TypeDef* tbIpBase[]  = {     ADC2};
//  static s8 tbSubHandleIdx[]          = {0,     0};
//  static s8 tbSubHandleIdx[]          = {-1,    0);
#endif

#if defined(__MM0N1) || defined(__MM3O1) || defined(__MM0Q1)
    static ADC_TypeDef* tbIpBase[]  = {ADC1};
    static s8 tbSubHandleIdx[]          = {0};
#endif
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_ADC_INSTANCE     instance[INSTANCE_NUM];


u32 adc1Value[8];
u32 adc2Value[8];
u16 temp1_0[128];
u16 temp1_1[128];

u16 temp2_0[128];
u16 temp2_1[128];



#else
#define GLOBAL extern
extern u32 adcValue[];

#endif

GLOBAL u32 ADC1_value, ADC2_value;



#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup ADC_Exported_Functions
/// @{



void DRV_ADC_NVIC_DMA_Init(ADC_TypeDef* ADCn);
void DRV_ADC_NVIC_Init(ADC_TypeDef* ADCn);

ADC_TypeDef* DRV_GetBase_ADCx(u8 idx);
void DRV_ADC_DMA_ConfigChannel(ADC_TypeDef* ADCx);

void DRV_ADC_TIM_EvTrigInit(TIM_TypeDef* tim, bool trig);
void DRV_ADC_Start_Soft(ADC_TypeDef* ADCn);
void DRV_ADC_Start_HW(TIM_TypeDef* tim);

void DRV_ADC_Getdata(ADC_TypeDef* ADCn, u32* ptr);

bool DRV_ADC_PollingStatus(u8 idx);
bool DRV_ADC_DMA_PollingStatus(u8 idx);

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__DRV_ADC_H */
////////////////////////////////////////////////////////////////////////////////
