////////////////////////////////////////////////////////////////////////////////
/// @file     DRV_AES.H
/// @author   Z Yan
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE AES
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
#ifndef __DRV_AES_H
#define __DRV_AES_H

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Driver_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup AES
/// @brief AES driver modules
/// @{

#if defined(__MM3N1) || defined(__MM0N1)
////////////////////////////////////////////////////////////////////////////////
/// @defgroup AES_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup AES_Exported_Enumeration
/// @{

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup AES_Exported_Types
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief AES Instant Structure definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    tPREFIX             sPrefix;

    u16                 bits;
    u8                  itemKey[32];
    u8                  itemIv[16];
} tAES_INSTANT;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup AES_Exported_Variables
/// @{

#ifdef _DRV_AES_C_
#define GLOBAL

/*  --------------------------------------------------------------------
    The Sub Handle Index table
--------------------------------------------------------------------  */
static AES_TypeDef* tbIpBase[]  = {AES};
static s8 tbSubHandleIdx[]      = {0};
/*  --------------------------------------------------------------------
    The End of Sub Handle Index table
--------------------------------------------------------------------  */

GLOBAL static tAES_INSTANT  instance[INSTANCE_NUM];


// Array of ECB encrypt and decrypt function address
bool (*tbFunc1[]) (u8* key, u8* buffer) = {
    AES_ECB_Encrypt,
    AES_ECB_Decrypt
};

// Array of CBC,CTR,CFB,OFB encrypt and decrypt function address
bool (*tbFunc2[]) (u8* key, u8* initVectors, u8* buffer) = {
    AES_CBC_Encrypt,
    AES_CBC_Decrypt,
    AES_CTR_Encrypt,
    AES_CTR_Decrypt,
    AES_CFB_Encrypt,
    AES_CFB_Decrypt,
    AES_OFB_Encrypt,
    AES_OFB_Decrypt
};

#else
#define GLOBAL extern


#endif



#undef GLOBAL
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup AES_Exported_Functions
/// @{

void DRV_AES_Init(AES_KeySize_TypeDef keySize);
void DRV_AES_DeInit(void);

/// @}

#endif


/// @}

/// @}



////////////////////////////////////////////////////////////////////////////////
#endif /* __DRV_AES_H */
////////////////////////////////////////////////////////////////////////////////
