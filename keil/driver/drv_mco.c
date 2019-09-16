////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_MCO.C
/// @author   S Yi
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES THE MCO DRIVER LAYER FUNCTIONS.
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
#define _DRV_RCC_C_

// Files includes  -------------------------------------------------------------
#include <string.h>

#include "hal_rcc.h"
#include "hal_pwr.h"

#include "drv.h"
#include "common.h"
#include "bsp_mco.h"
#include "drv_mco.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RCC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RCC_Exported_Functions
/// @{


#if defined(__MM3N1) || defined(__MM0N1) || defined (__MM0P1)
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the PLL clock source and multiplication factor.
///   This function must be used only when the PLL is disabled.
/// @param  pllSrc: specifies the PLL entry clock source.
///   This parameter can be one of the following values:
/// @arg RCC_HSI_Div4: HSI oscillator clock divided
///   by 4 selected as PLL clock entry
/// @arg RCC_HSE_Div1: HSE oscillator clock selected
///   as PLL clock entry
/// @arg RCC_HSE_Div2: HSE oscillator clock divided
///   by 2 selected as PLL clock entry
/// @param  pllMul: specifies the PLL multiplication factor.
///   This parameter can be RCC_PLLMul_x where x:[31:26][22:20]
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_RCC_PllConfig(RCC_PLLSource_TypeDef pllSrc, RCC_PLLMul_TypeDef pllMul)
{
    if (pllSrc) {
        RCC_HSEConfig(RCC_HSE_ON);
        while(!RCC_WaitForFlagStartUp(RCC_FLAG_HSERDY));
    }
    else {
        RCC_HSICmd(ENABLE);
    }
    RCC_PLLConfig(pllSrc, pllMul);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the system clock (SYSCLK).
/// @param  sysClkSrc: specifies the clock source used as system
///   clock. This parameter can be one of the following values:
/// @arg RCC_HSI: specifies HSI as system clock
/// @arg RCC_HSE: specifies HSE as system clock
/// @arg RCC_PLL: specifies PLL as system clock
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DRV_RCC_SysClkConfig(SYSCLK_TypeDef sysClkSrc)
{
    switch (sysClkSrc) {
    case RCC_HSI:
        RCC_HSICmd(ENABLE);
        break;
    case RCC_HSE:
#if defined (__MM3O1) || defined (__MM0P1)|| defined (__MM0Q1)
        exRCC_Set_OSC_ITRIM_Config(RCC_CONFIG_OSC_ITRIM_2mA);
#endif
        RCC_HSEConfig(RCC_HSE_ON);
        while(!RCC_WaitForFlagStartUp(RCC_FLAG_HSERDY));
        break;
#if defined(__MM3N1) || defined(__MM0N1) || defined (__MM0P1)
    case RCC_PLL:
        DRV_RCC_PllConfig(RCC_HSE_Div2, RCC_PLLMul_2);
        RCC_PLLCmd(ENABLE);
        while(!RCC_WaitForFlagStartUp(RCC_FLAG_PLLRDY));
        break;
#endif
#if defined (__MM0P1) || defined (__MM0Q1)
    case RCC_LSI:
        RCC_LSICmd(ENABLE);
        while(!RCC_WaitForFlagStartUp(RCC_FLAG_LSIRDY));
        break;
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the instance of MCO.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void InstanceConfig(tAPP_RCC_DCB* pDcb, u8 idx)
{
    instance[idx].src = pDcb->src;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Config the hardware of MCO.
/// @param  pDcb: The pointer point to Device Control Block(DCB).
/// @param  idx: The trueIdx of the instance.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void HardwareConfig(tAPP_RCC_DCB* pDcb, u8 idx)
{
    RCC_MCO_TypeDef mcotype[] = {
        RCC_MCO_NoClock,
        RCC_MCO_LSI,
#if defined(__MM3N1)
        RCC_MCO_LSE,
#else
        RCC_MCO_NoClock,
#endif
        RCC_MCO_SYSCLK,
        RCC_MCO_HSI,
        RCC_MCO_HSE,
#if defined(__MM3N1) || defined(__MM0N1)|| defined(__MM0P1)
        RCC_MCO_PLLCLK_Div2,
#else
        RCC_MCO_NoClock
#endif
    };

    BSP_RCC_GPIO_Configure();

    if (pDcb->src == emMCO_LSI) {
        RCC_LSICmd(ENABLE);
        while(!RCC_WaitForFlagStartUp(RCC_FLAG_LSIRDY));
    }
#if defined(__MM3N1)
    if (pDcb->src == emMCO_LSE) {
        COMMON_EnableIpClock(emCLOCK_PWR);
        COMMON_EnableIpClock(emCLOCK_BKP);
        PWR_BackupAccessCmd(ENABLE);
        RCC_LSEConfig(RCC_LSE_ON);
        while(!RCC_WaitForFlagStartUp(RCC_FLAG_LSERDY));
    }
#endif
    if (pDcb->src == emMCO_SYSCLK) {
        DRV_RCC_SysClkConfig(RCC_HSI);
        RCC_SYSCLKConfig(RCC_HSI);
    }

    if (pDcb->src == emMCO_HSI) {
        RCC_HSICmd(ENABLE);
    }

    if (pDcb->src == emMCO_HSE) {
        RCC_HSEConfig(RCC_HSE_ON);
        while(!RCC_WaitForFlagStartUp(RCC_FLAG_HSERDY));
    }

#if defined(__MM3N1) || defined(__MM0N1) || defined(__MM3O1) || defined(__MM0P1)
    if (pDcb->src == emMCO_PLL_DIV2) {
        RCC_PLLConfig(RCC_HSI_Div4, RCC_PLLMul_2);
        RCC_PLLCmd(ENABLE);
        while(!RCC_WaitForFlagStartUp(RCC_FLAG_PLLRDY));
    }
#endif
    RCC_MCOConfig(mcotype[instance->src]);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the basic parameters of RCC and prepare the MCO output.
/// @param  handle: HANDLE structure that contains
///   related parameters about RCC.
/// @param  pDcb: pointer to a RCC_DCB_TypeDef structure that
///   contains the configuration information for the specified
/// @retval The address of global variables instance.
////////////////////////////////////////////////////////////////////////////////
static int RCC_OpenFile(HANDLE handle, tAPP_RCC_DCB* pDcb)
{
// Entry OpenFile Function
    s8 idx = DRV_OpenFileEntry((u32*)&tbIpBase, (u8*)&tbSubHandleIdx, pDcb->hSub, (u8*)(&instance[0]), sizeof(instance[0]), INSTANCE_NUM);
    if (idx == -1)      return 0;

// Instance configure
    InstanceConfig(pDcb, idx);

// Hardware configure
    HardwareConfig(pDcb, idx);

// Exit OpenFile Function
    return DRV_OpenFileExit(handle, (int*)&instance[idx], idx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  RCC Close File
/// @param  handle: HANDLE structure that contains
///   related parameters about RCC.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void RCC_CloseFile(HANDLE handle)
{
    if (handle->idx == -1)      return;

    RCC_DeInit();

    DRV_CloseFileCommon(&instance[handle->idx].sPrefix, sizeof(instance[0]));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  RCC create file
/// @param  handle: HANDLE structure that contains
///         related parameters about RCC.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_CreateFile(HANDLE  handle)
{
    static bool first = true;
    if (first) {
        first = false;
        memset(&instance[0], 0xFF, sizeof(instance[0]) * INSTANCE_NUM);
    }

    handle->instance    = (int*)&instance[0];
    handle->idx         = -1;
    handle->size        = sizeof(tbIpBase);
    handle->openFile    = (int*)&RCC_OpenFile;
    handle->closeFile   = (int*)&RCC_CloseFile;
    handle->readFile    = NULL;
    handle->writeFile   = NULL;
}




/// @}

/// @}

/// @}


