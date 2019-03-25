////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_TIM.H
/// @author   D Chen
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE TIM
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
#ifndef __DRV_TIM_H
#define __DRV_TIM_H

// Files includes  -------------------------------------------------------------
#include "hal_tim.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM
/// @brief TIM driver modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM_Exported_Types
/// @{

typedef enum {
    emTIM_DMA_Up = 1,
    emTIM_DMA_CC1,
    emTIM_DMA_CC2,
    emTIM_DMA_CC3,
    emTIM_DMA_CC4,
    emTIM_DMA_CC5
} EM_TIM_DMA_SRC;

typedef struct {
    u8                  capFinishFlag;                      // Capture End Flag
    u8                  capStartFlag;                       // Capture Start Flag
    u32                 capCCRValue;                        // Capture Register Value
    u32                 capPeriod;                          // Auto-Reload Register Update Flag
} tDRV_TIM_CAP_VALUE;

typedef struct {
    u8                  encoderDir;                         ///< encoder mode direction
    u32                 encoderCnt;                         ///< encoder mode count
} tDRV_TIM_ENCODER;

////////////////////////////////////////////////////////////////////////////////
/// @brief TIM Instant Data Control Block
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    TIM_TypeDef*        tim;                                ///< registers base address
    u16                 idx;                                ///< Virtual index;
    u8                  type;                               ///< type: interrupt, dma

    u8                  mode;                               ///< mode
    u8                  ch;
    u8                  trgo;
    u32                 cntFreq;
    u16                 psc;                                ///< prescaler
    u32                 arr;                                ///< Auto-reload Value
    u32                 pulse;                              ///< pulse
    u32                 itSrc;                              ///< interrupt source
    EM_TIM_DMA_SRC      dmaSrc;                             ///< DMA source

    bool                remapEn     :1;                     ///< GPIO whether remap
    u8                  remapIdx    :2;                     ///< GPIO remap index

} tDRV_TIM_DCB;

////////////////////////////////////////////////////////////////////////////////
/// @brief TIM Instant Data Control Block
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;
    u32                 cbUp;                               ///< update  Sync callback function.
    u32                 cbCc;                               ///< com/cmp Sync callback function.
    u32                 cntFreq;
    u32                 period;                             ///< period
    u32                 pulse;                              ///< pulse
    u8                  ch;
    u8                  trgo;
    EM_TYPE             type;                               ///< type: interrupt, dma
    EM_TYPE             sync;                               ///< sync type
    EM_TIM_MODE         mode;                               ///< mode
    bool                remapEn     :1;                     ///< GPIO whether remap
    u8                  remapIdx    :2;                     ///< GPIO remap index
} tDRV_TIM_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM_Exported_Variables
/// @{
#ifdef _DRV_TIM_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
#if defined(__MM3N1)
    static TIM_TypeDef* tbIpBase[]          = {TIM1, TIM2, TIM3, TIM4};
    static s8           tbSubHandleIdx[]    = {   0,    1,    2,    3};

// e.g.
//  static TIM_TypeDef* tbIpBase[]  = {      TIM2,       TIM4};
//  static s8           tbSubHandleIdx[]  = {   0,    0,    0,    1};
//  static s8           tbSubHandleIdx[]  = {  -1,    0,   -1,    1};
//  static s8           tbSubHandleIdx[]  = {  -1,    0,    1,    1};

#endif
#if defined(__MM0N1) || defined(__MM0Q1)
    static TIM_TypeDef* tbIpBase[]    = {TIM1, TIM2, TIM3,                         TIM14,      TIM16, TIM17};
    static s8           tbSubHandleIdx[]  = { 0,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  0,  4,  5};
//  static s8           tbSubHandleIdx[]  = { 0,  1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3, -1,  4,  5};
//  static s8           tbSubHandleIdx[]  = { 0,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  4,  5};

// e.g.
//  static TIM_TypeDef* tbIpBase[]    = {    TIM2,                                                   TIM17};
//  static s8           tbSubHandleIdx[]  = {-1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1};
//  static s8           tbSubHandleIdx[]  = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1};

#endif
#if defined(__MM0P1)
    static TIM_TypeDef* tbIpBase[]    = {TIM1, TIM2, TIM3,           TIM8,         TIM14,      TIM16, TIM17};
    static s8           tbSubHandleIdx[]  = { 0,  1,  2,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  4,  0,  5,  6};
//  static s8           tbSubHandleIdx[]  = { 0,  1,  2, -1, -1, -1, -1,  3, -1, -1, -1, -1, -1,  4, -1,  5,  6};
//  static s8           tbSubHandleIdx[]  = { 0,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  4,  3,  5,  6};

// e.g.
//  static TIM_TypeDef* tbIpBase[]    = {TIM1,                       TIM8,                            TIM17};
//  static s8           tbSubHandleIdx[]  = { 0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  2};
//  static s8           tbSubHandleIdx[]  = { 0, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1,  2};
//  static s8           tbSubHandleIdx[]  = { 0, -1, -1, -1, -1, -1,  0,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2};

#endif
#if defined(__MM3O1)
    static TIM_TypeDef* tbIpBase[]    = {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8,  TIM14};
    static s8           tbSubHandleIdx[]  = { 0,  1,  2,  3,  4,  5,  6,  7,  0,  0,  0,  0,  0,  8};
//  static s8           tbSubHandleIdx[]  = { 0,  1,  2,  3,  4,  5,  6,  7, -1, -1, -1, -1, -1,  8};
//  static s8           tbSubHandleIdx[]  = { 0,  1,  2,  3,  4,  5,  6,  7,  0,  1,  2,  3,  4,  8};

// e.g.
//  static TIM_TypeDef* tbIpBase[]    = {TIM1,                       TIM8,                TIM14};
//  static s8           tbSubHandleIdx[]  = { 0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  2};
//  static s8           tbSubHandleIdx[]  = { 0, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1,  2};
//  static s8           tbSubHandleIdx[]  = { 0, -1, -1, -1, -1, -1,  0,  1,  1,  1,  1,  1,  1,  2};

#endif
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tDRV_TIM_INSTANT  instance[INSTANCE_NUM];


#else
#define GLOBAL extern
#endif



GLOBAL tDRV_TIM_ENCODER     encoderStructure;
GLOBAL tDRV_TIM_CAP_VALUE   capValueStructure;

GLOBAL u32                  dmaRead[10];
GLOBAL u32                  gGetPeriod;
GLOBAL u8                   gGetDutyCycle;  //Duty cycle percentage * 100

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup TIM_Exported_Functions
/// @{

u32 DRV_GetRCC_TIMx(u8 idx);

TIM_TypeDef*          DRV_GetBase_TIMx(u8 idx);
DMA_Channel_TypeDef*  DRV_TIM_GetDMAChannel(tDRV_TIM_DCB* dcb);

void DRV_TIM_ClockEnable(TIM_TypeDef* TIMx);
void DRV_TIM_ON(TIM_TypeDef* TIMx);
void DRV_TIM_OFF(TIM_TypeDef* TIMx);

void DRV_TIM_NVIC_Update_Init(TIM_TypeDef* TIMx);
void DRV_TIM_NVIC_CC_Init(TIM_TypeDef* TIMx);
void DRV_TIM_CapturePWM(TIM_TypeDef* TIMx, u8 idx);
void DRV_TIM_CapturePulse(TIM_TypeDef* TIMx, u8 idx);
void DRV_TIM_UpCC_Common(u8 idx);
void DRV_TIM_6Step(TIM_TypeDef* TIMx, u8 step);
void DRV_TIM_Generate_COM_Event(TIM_TypeDef* TIMx);
void DRV_TIM_MasterSelect(TIM_TypeDef* TIMx, u8 src);
void DRV_TIM_SlaveSelect(TIM_TypeDef* TIMx, TIMSMSMODE_Typedef mode, TIMTS_TypeDef src);
void DRV_TIM_MasterInit(tDRV_TIM_DCB* dcb);

void DRV_TIM_DMA_ConfigChannel(tDRV_TIM_DCB* dcb);
void DRV_TIM_PWM_CH1_Init(tDRV_TIM_DCB* dcb);
void DRV_TIM_GetEncoder(TIM_TypeDef* TIMx);
void DRV_TIM_CapturePulseInit(tDRV_TIM_DCB* dcb);
void DRV_TIM_CapturePWMInit(tDRV_TIM_DCB* dcb);
void DRV_TIM_EncoderInit(tDRV_TIM_DCB* dcb);
void DRV_TIM_OnePulseInit(tDRV_TIM_DCB* dcb);
void DRV_TIM_6StepInit(tDRV_TIM_DCB* dcb);
void DRV_TIM_Init(tDRV_TIM_DCB* dcb);


/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __DRV_TIM_H */
////////////////////////////////////////////////////////////////////////////////
