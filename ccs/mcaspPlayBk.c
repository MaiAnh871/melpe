/**
 * \file  mcaspPlayBk.c
 *
 * \brief Sample application for McASP. This application loops back the input
 *        at LINE_IN of the EVM to the LINE_OUT of the EVM.
 */

/*
* Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "edma_event.h"
#include "interrupt.h"
#include "soc_OMAPL138.h"
#include "hw_syscfg0_OMAPL138.h"
#include "lcdkOMAPL138.h"
#include "codecif.h"
#include "mcasp.h"
#include "aic31.h"
#include "edma.h"
#include "psc.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#include "MELP/sc1200.h"
#include "MELP/mat_lib.h"
#include "MELP/global.h"
#include "MELP/macro.h"
#include "MELP/mathhalf.h"
#include "MELP/dsp_sub.h"
#include "MELP/melp_sub.h"
#include "MELP/constant.h"
#include "MELP/math_lib.h"

/*============================================================================
				Khu Vuc Melp
============================================================================*/
#define X05_Q7				64         /* 0.5 * (1 << 7) */
#define THREE_Q7			384        /* 3 * (1 << 7) */
#define BUG_GPIO			1
#define BUG_GPIO_ANA		1
#define BUG_GPIO_SYS		1

/* ====== External memory ====== */

//Shortword	mode = ANALYSIS;
//Shortword	mode = SYNTHESIS;
//Shortword	mode = ANA_SYN;
//Shortword   chwordsize = 8;


/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
******************************************************************************/
/*
** Values which are configurable
*/
/* Slot size to send/receive data */
#define SLOT_SIZE                             (16u)

/* Word size to send/receive data. Word size <= Slot size */
#define WORD_SIZE                             (16u)

/* Sampling Rate which will be used by both transmit and receive sections */
#define SAMPLING_RATE                         (8000u)

/* Number of channels, L & R */
#define NUM_I2S_CHANNELS                      (1u)

/* Number of samples to be used per audio buffer */
#define NUM_SAMPLES_PER_AUDIO_BUF             (540u)

/* Number of buffers used per tx/rx */
#define NUM_BUF                               (2u)

/* Number of linked parameter set used per tx/rx */
#define NUM_PAR                               (1u)

/* Specify where the parameter set starting is */
#define PAR_ID_START                          (40u)

/* Number of samples in loop buffer */
#define NUM_SAMPLES_LOOP_BUF                  (10u)

/* AIC3106 codec address */
#define I2C_SLAVE_CODEC_AIC31                 (0x18u)

/* Interrupt channels to map in AINTC */
#define INT_CHANNEL_I2C                       (2u)
#define INT_CHANNEL_MCASP                     (2u)
#define INT_CHANNEL_EDMACC                    (2u)

/* McASP Serializer for Receive */
#define MCASP_XSER_RX                         (14u)

/* McASP Serializer for Transmit */
#define MCASP_XSER_TX                         (13u)

/*
** Below Macros are calculated based on the above inputs
*/
#define NUM_TX_SERIALIZERS                    ((NUM_I2S_CHANNELS >> 1) \
                                               + (NUM_I2S_CHANNELS & 0x01))
#define NUM_RX_SERIALIZERS                    ((NUM_I2S_CHANNELS >> 1) \
                                               + (NUM_I2S_CHANNELS & 0x01))
#define I2S_SLOTS                             ((1 << NUM_I2S_CHANNELS) - 1)

#define BYTES_PER_SAMPLE                      ((WORD_SIZE >> 3) \
                                               * NUM_I2S_CHANNELS)

#define AUDIO_BUF_SIZE                        (NUM_SAMPLES_PER_AUDIO_BUF \
                                               * BYTES_PER_SAMPLE)

#define TX_DMA_INT_ENABLE                     (EDMA3CC_OPT_TCC_SET(1) | (1 \
                                               << EDMA3CC_OPT_TCINTEN_SHIFT))
#define RX_DMA_INT_ENABLE                     (EDMA3CC_OPT_TCC_SET(0) | (1 \
                                               << EDMA3CC_OPT_TCINTEN_SHIFT))

#define PAR_RX_START                          (PAR_ID_START)
#define PAR_TX_START                          (PAR_RX_START + NUM_PAR)

/*
** Definitions which are not configurable
*/
#define SIZE_PARAMSET                         (32u)
#define OPT_FIFO_WIDTH                        (0x02 << 8u)


/******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
******************************************************************************/
static void McASPErrorIsr(void);
static void McASPErrorIntSetup(void);
static void AIC31I2SConfigure(void);
static void McASPI2SConfigure(void);
static void McASPTxDMAComplHandler(void);
static void McASPRxDMAComplHandler(void);
static void EDMA3CCComplIsr(void);
static void I2SDataTxRxActivate(void);
static void I2SDMAParamInit(void);
static void ParamTxLoopJobSet(unsigned short parId);
static void BufferTxDMAActivate(unsigned int txBuf, unsigned short numSamples,
                                unsigned short parToUpdate,
                                unsigned short linkAddr);
static void BufferRxDMAActivate(unsigned int rxBuf, unsigned short parId,
                                unsigned short parLink);



/******************************************************************************
				Khu vuc melp
*******************************************************************************/
unsigned char mang_test[]=
{
 217, 206, 204, 197, 115, 155, 148, 67, 193, 136, 0,
 30, 33, 133, 2, 3, 122, 166, 122, 170, 148, 0,
 11, 97, 198, 64, 73, 156, 189, 234, 193, 58, 0,
 6, 234, 247, 137, 141, 171, 201, 248, 27, 1, 0,
 147, 123, 154, 226, 189, 42, 243, 247, 103, 144, 0,
 134, 109, 234, 217, 42, 139, 244, 131, 162, 109, 0,
 13, 98, 93, 181, 194, 220, 77, 186, 245, 67, 0,
 30, 99, 99, 13, 1, 207, 85, 248, 63, 20, 0,
 25, 231, 65, 50, 125, 72, 253, 134, 194, 74, 0,
 152, 246, 214, 148, 125, 111, 208, 234, 253, 203, 0,
 11, 219, 32, 11, 43, 168, 178, 28, 144, 3, 0,
 12, 89, 70, 72, 200, 148, 100, 186, 248, 123, 0,
 95, 115, 195, 34, 168, 113, 102, 188, 0, 69, 0,
 30, 107, 234, 77, 247, 18, 133, 26, 195, 194, 0,
 31, 114, 121, 197, 1, 222, 121, 186, 253, 106, 0,
 198, 1, 135, 68, 125, 62, 196, 252, 157, 175, 0,
 97, 167, 30, 61, 27, 175, 85, 46, 26, 123, 0,
 30, 108, 103, 6, 76, 196, 169, 186, 157, 129, 0,
 139, 102, 102, 76, 32, 196, 236, 186, 185, 40, 0,
 138, 70, 5, 182, 230, 247, 76, 186, 253, 18, 0,
 11, 96, 21, 245, 212, 156, 132, 186, 237, 41, 0,
 10, 116, 227, 232, 109, 237, 170, 188, 5, 218, 0,
 19, 249, 47, 247, 79, 129, 239, 183, 66, 113, 0,
 0, 128, 118, 143, 103, 66, 17, 95, 6, 50, 1,
 1, 192, 189, 31, 125, 136, 0, 72, 2, 55, 0,
 0, 192, 153, 51, 186, 16, 7, 76, 0, 233, 0,
 65, 85, 156, 135, 104, 242, 125, 135, 32, 22, 0,
 96, 157, 30, 185, 21, 9, 229, 168, 26, 49, 0,
 11, 192, 86, 85, 61, 233, 23, 186, 89, 79, 0,
 10, 85, 75, 249, 185, 222, 69, 186, 253, 211, 0,
 31, 162, 100, 49, 236, 254, 118, 186, 189, 33, 0,
 140, 186, 223, 242, 97, 93, 120, 190, 13, 53, 0,
 181, 107, 98, 227, 132, 235, 110, 180, 128, 173, 0,
 30, 104, 133, 176, 250, 161, 201, 186, 160, 181, 0,
 11, 106, 99, 145, 234, 252, 139, 62, 37, 65, 0,
 140, 221, 32, 127, 157, 152, 12, 176, 128, 181, 0,
 135, 86, 195, 73, 251, 127, 193, 186, 237, 37, 0,
 140, 110, 5, 112, 173, 216, 243, 186, 93, 138, 0,
 7, 26, 87, 93, 249, 120, 221, 186, 253, 164, 0,
 12, 77, 107, 14, 189, 103, 108, 248, 47, 189, 0,
 159, 53, 7, 189, 231, 153, 134, 174, 211, 39, 0,
 10, 93, 199, 187, 91, 187, 180, 248, 63, 8, 0,
 21, 103, 195, 25, 249, 17, 220, 146, 128, 51, 0,
 10, 101, 103, 6, 185, 251, 180, 248, 63, 39, 0,
 181, 176, 198, 251, 108, 74, 202, 191, 165, 35, 0,
 30, 82, 70, 84, 163, 179, 64, 186, 253, 10, 0,
 11, 195, 229, 157, 115, 181, 21, 254, 66, 167, 0,
 30, 81, 121, 229, 242, 237, 41, 186, 249, 67, 0,
 31, 227, 159, 144, 189, 115, 118, 184, 61, 62, 0,
 160, 217, 68, 165, 243, 63, 53, 18, 49, 18, 0,
 11, 85, 102, 52, 231, 252, 50, 186, 189, 35, 0,
 138, 101, 35, 157, 66, 253, 96, 186, 93, 175, 0,
 159, 84, 5, 54, 87, 116, 28, 186, 245, 87, 0,
 10, 96, 102, 152, 114, 213, 232, 186, 233, 77, 0,
 7, 162, 199, 128, 158, 101, 124, 184, 5, 20, 0,
 30, 242, 214, 80, 231, 228, 6, 186, 253, 103, 0,
 141, 181, 85, 148, 85, 96, 249, 60, 87, 83, 0,
 134, 77, 121, 212, 156, 104, 88, 138, 201, 43, 0,
 31, 90, 167, 4, 165, 150, 108, 184, 93, 27, 0,
 138, 203, 117, 193, 124, 48, 129, 168, 106, 191, 0,
 141, 174, 244, 156, 107, 207, 45, 186, 249, 231, 0,
 6, 26, 104, 41, 89, 247, 97, 186, 253, 106, 0,
 11, 43, 13, 249, 182, 111, 1, 184, 9, 233, 0,
 32, 215, 174, 1, 65, 213, 214, 70, 49, 53, 0,
 1, 192, 141, 27, 31, 34, 18, 183, 6, 168, 1,
 0, 64, 189, 27, 31, 38, 18, 95, 6, 50, 1,
 97, 153, 216, 164, 253, 149, 15, 72, 50, 232, 0,
 32, 218, 51, 169, 26, 151, 24, 157, 36, 162, 0,
 167, 192, 188, 51, 144, 136, 5, 163, 0, 238, 0,
 140, 110, 234, 91, 237, 251, 180, 186, 253, 79, 0,
 11, 228, 39, 239, 125, 239, 240, 186, 249, 105, 0,
 6, 194, 37, 253, 177, 221, 122, 62, 61, 165, 0,
 159, 197, 117, 197, 85, 225, 223, 142, 131, 22, 0,
 138, 124, 45, 242, 61, 201, 28, 186, 189, 4, 0,
 159, 252, 173, 78, 114, 86, 56, 186, 237, 245, 0,
 30, 136, 36, 183, 253, 242, 213, 186, 253, 33, 0,
 25, 196, 26, 93, 93, 243, 158, 76, 203, 103, 0,
 158, 253, 93, 235, 45, 41, 183, 58, 192, 172, 0,
 13, 98, 106, 156, 81, 246, 200, 186, 189, 115, 0,
 158, 93, 79, 245, 117, 214, 61, 152, 17, 121, 0,
 147, 253, 59, 167, 222, 63, 239, 28, 71, 131, 0,
 30, 217, 100, 193, 187, 224, 118, 66, 4, 245, 0,
 139, 70, 42, 50, 57, 251, 178, 186, 253, 70, 0,
 10, 96, 83, 85, 183, 122, 92, 186, 185, 19, 0,
 139, 70, 79, 65, 253, 87, 236, 186, 253, 3, 0,
 12, 108, 115, 213, 116, 206, 220, 62, 29, 82, 0,
 141, 47, 108, 128, 119, 212, 41, 250, 249, 231, 0,
 134, 101, 100, 80, 127, 238, 33, 186, 253, 121, 0,
 31, 202, 5, 127, 177, 233, 226, 188, 137, 236, 0,
 18, 165, 154, 167, 36, 248, 255, 90, 64, 205, 0,
 31, 56, 199, 74, 231, 247, 118, 186, 245, 79, 0,
 6, 162, 70, 5, 237, 222, 218, 186, 249, 47, 0,
 11, 229, 214, 213, 232, 203, 229, 186, 249, 87, 0,
 146, 23, 152, 40, 29, 20, 253, 124, 109, 227, 0,
 193, 154, 78, 9, 95, 158, 30, 79, 51, 198, 0,
 0, 192, 141, 30, 61, 98, 6, 95, 6, 50, 1,
 193, 109, 207, 158, 162, 60, 231, 215, 17, 59, 0,
 160, 221, 144, 170, 181, 102, 245, 51, 26, 215, 0,
 159, 254, 29, 212, 75, 114, 60, 186, 189, 177, 0,
 18, 242, 210, 180, 213, 249, 241, 188, 71, 43, 0,
 153, 19, 215, 136, 31, 180, 193, 26, 242, 5, 0,
 12, 58, 5, 189, 213, 242, 5, 186, 253, 252, 0,
 13, 28, 133, 65, 42, 41, 102, 220, 5, 247, 0,
 140, 142, 119, 88, 173, 203, 131, 186, 60, 38, 0,
 31, 248, 247, 71, 212, 249, 209, 186, 21, 124, 0,
 10, 228, 94, 233, 59, 122, 245, 186, 149, 216, 0,
 7, 229, 244, 32, 114, 221, 205, 186, 253, 57, 0,
 30, 181, 164, 113, 180, 243, 236, 186, 253, 99, 0,
 13, 156, 228, 31, 184, 220, 157, 186, 253, 228, 0,
 138, 244, 154, 213, 125, 118, 84, 62, 121, 6, 0,
 135, 218, 218, 5, 242, 248, 144, 16, 55, 36, 0,
 160, 153, 92, 224, 255, 138, 27, 151, 50, 15, 0,
 225, 108, 159, 21, 78, 116, 222, 96, 22, 5, 0,
 152, 197, 198, 92, 189, 140, 64, 171, 210, 161, 0,
 135, 68, 197, 85, 255, 134, 148, 186, 189, 213, 0,
 140, 100, 226, 69, 104, 114, 85, 186, 213, 166, 0,
 7, 248, 237, 66, 173, 223, 65, 186, 253, 148, 0,
 140, 84, 166, 67, 69, 180, 232, 186, 125, 236, 0,
 139, 134, 78, 148, 136, 233, 129, 188, 6, 170, 0,
 134, 182, 119, 201, 105, 231, 78, 250, 189, 214, 0,
 141, 110, 87, 189, 232, 124, 28, 186, 149, 229, 0,
 12, 80, 69, 54, 163, 122, 204, 186, 237, 85, 0,
 199, 1, 215, 156, 185, 240, 145, 248, 175, 43, 0,
 70, 183, 198, 223, 170, 67, 97, 196, 75, 112, 0,
 135, 101, 249, 11, 116, 221, 8, 59, 2, 169, 0,
 30, 114, 87, 161, 61, 39, 212, 186, 189, 207, 0,
 11, 109, 231, 89, 128, 242, 64, 186, 189, 2, 0,
 236, 192, 129, 95, 143, 12, 32, 54, 33, 163, 0,
 161, 167, 59, 87, 61, 101, 233, 174, 23, 135, 0,
 6, 93, 228, 140, 159, 111, 249, 186, 237, 47, 0,
 141, 237, 95, 225, 253, 155, 197, 186, 253, 1, 0,
 62, 240, 231, 136, 12, 114, 241, 248, 11, 31, 0,
 97, 30, 138, 56, 150, 60, 242, 24, 24, 23, 0,
 158, 89, 135, 108, 237, 132, 133, 186, 189, 43, 0,
 147, 167, 58, 137, 226, 35, 243, 191, 71, 235, 0,
 152, 110, 95, 73, 121, 181, 84, 122, 242, 18, 0,
 13, 67, 39, 80, 140, 215, 133, 248, 11, 60, 0,
 12, 113, 194, 169, 221, 207, 73, 186, 249, 239, 0,
 13, 85, 194, 171, 229, 254, 72, 186, 253, 231, 0,
 30, 85, 70, 100, 132, 252, 57, 186, 253, 243, 0,
 71, 100, 66, 148, 174, 63, 100, 60, 68, 169, 0,
 192, 235, 88, 22, 47, 13, 27, 151, 24, 179, 0,
 192, 235, 88, 22, 47, 13, 27, 151, 24, 179, 0
};

unsigned char cho_phep_nen=0;
unsigned char gia_tri_melp=0,gia_tri_mang=0;
Shortword *bit_buffer;
Shortword speech_in[NUM_SAMPLES_PER_AUDIO_BUF];
Shortword speech_out[NUM_SAMPLES_PER_AUDIO_BUF];



/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
******************************************************************************/
static unsigned char loopBuf[NUM_SAMPLES_LOOP_BUF * BYTES_PER_SAMPLE] = {0};

/*
** Transmit buffers. If any new buffer is to be added, define it here and
** update the NUM_BUF.
*/
static unsigned char pingtx_Buf[AUDIO_BUF_SIZE];
static unsigned char pongtx_Buf[AUDIO_BUF_SIZE];

/*
** Receive buffers. If any new buffer is to be added, define it here and
** update the NUM_BUF.
*/
static unsigned char pingrx_Buf[AUDIO_BUF_SIZE];
static unsigned char pongrx_Buf[AUDIO_BUF_SIZE];

/*
** Next buffer to receive data. The data will be received in this buffer.
*/
static volatile unsigned int nxtBufToRcv = 0;

/*
** The RX buffer which filled latest.
*/
static volatile unsigned int lastFullRxBuf = 0;

/*
** The offset of the paRAM ID, from the starting of the paRAM set.
*/
static volatile unsigned short parOffRcvd = 0;

/*
** The offset of the paRAM ID sent, from starting of the paRAM set.
*/
static volatile unsigned short parOffSent = 0;

/*
** The offset of the paRAM ID to be sent next, from starting of the paRAM set.
*/
static volatile unsigned short parOffTxToSend = 0;

/*
** The transmit buffer which was sent last.
*/
static volatile unsigned int lastSentTxBuf = NUM_BUF - 1;

/******************************************************************************
**                      INTERNAL CONSTATNT DEFINITIONS
******************************************************************************/
/* Array of receive buffer pointers */
static unsigned int const rxBufPtr[NUM_BUF] =
       {
           (unsigned int) pingrx_Buf,
           (unsigned int) pongrx_Buf
       };

/* Array of transmit buffer pointers */
static unsigned int const txBufPtr[NUM_BUF] =
       {
           (unsigned int) pingtx_Buf,
           (unsigned int) pongtx_Buf
       };

/*
** Default paRAM for Transmit section. This will be transmitting from
** a loop buffer.
*/
static struct EDMA3CCPaRAMEntry const txDefaultPar =
       {
           (unsigned int)(EDMA3CC_OPT_DAM  | (0x02 << 8u)), /* Opt field */
           (unsigned int)loopBuf, /* source address */
           (unsigned short)(BYTES_PER_SAMPLE), /* aCnt */
           (unsigned short)(NUM_SAMPLES_LOOP_BUF), /* bCnt */
           (unsigned int) SOC_MCASP_0_DATA_REGS, /* dest address */
           (short) (BYTES_PER_SAMPLE), /* source bIdx */
           (short)(0), /* dest bIdx */
           (unsigned short)(PAR_TX_START * SIZE_PARAMSET), /* link address */
           (unsigned short)(0), /* bCnt reload value */
           (short)(0), /* source cIdx */
           (short)(0), /* dest cIdx */
           (unsigned short)1 /* cCnt */
       };

/*
** Default paRAM for Receive section.
*/
static struct EDMA3CCPaRAMEntry const rxDefaultPar =
       {
           (unsigned int)(EDMA3CC_OPT_SAM  | (0x02 << 8u)), /* Opt field */
           (unsigned int)SOC_MCASP_0_DATA_REGS, /* source address */
           (unsigned short)(BYTES_PER_SAMPLE), /* aCnt */
           (unsigned short)(1), /* bCnt */
           (unsigned int)pingrx_Buf, /* dest address */
           (short) (0), /* source bIdx */
           (short)(BYTES_PER_SAMPLE), /* dest bIdx */
           (unsigned short)(PAR_RX_START * SIZE_PARAMSET), /* link address */
           (unsigned short)(0), /* bCnt reload value */
           (short)(0), /* source cIdx */
           (short)(0), /* dest cIdx */
           (unsigned short)1 /* cCnt */
       };

/******************************************************************************
**                          FUNCTION DEFINITIONS
******************************************************************************/
/*
** Assigns loop job for a parameter set
*/
static void ParamTxLoopJobSet(unsigned short parId)
{
    EDMA3CCPaRAMEntry paramSet;

    memcpy(&paramSet, &txDefaultPar, SIZE_PARAMSET - 2);

    /* link the paRAM to itself */
    paramSet.linkAddr = parId * SIZE_PARAMSET;

    EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, parId, &paramSet);
}

/*
** Initializes the DMA parameters.
** The RX basic paRAM set(channel) is 0 and TX basic paRAM set (channel) is 1.
**
** The RX paRAM set 0 will be initialized to receive data in the rx buffer 0.
** The transfer completion interrupt will not be enabled for paRAM set 0;
** paRAM set 0 will be linked to linked paRAM set starting (PAR_RX_START) of RX.
** and further reception only happens via linked paRAM set.
** For example, if the PAR_RX_START value is 40, and the number of paRAMS is 2,
** reception paRAM set linking will be initialized as 0-->40-->41-->40
**
** The TX paRAM sets will be initialized to transmit from the loop buffer.
** The size of the loop buffer can be configured.
** The transfer completion interrupt will not be enabled for paRAM set 1;
** paRAM set 1 will be linked to linked paRAM set starting (PAR_TX_START) of TX.
** All other paRAM sets will be linked to itself.
** and further transmission only happens via linked paRAM set.
** For example, if the PAR_RX_START value is 42, and the number of paRAMS is 2,
** So transmission paRAM set linking will be initialized as 1-->42-->42, 43->43.
*/
static void I2SDMAParamInit(void)
{
    EDMA3CCPaRAMEntry paramSet;
    int idx;

    /* Initialize the 0th paRAM set for receive */
    memcpy(&paramSet, &rxDefaultPar, SIZE_PARAMSET - 2);

    EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, EDMA3_CHA_MCASP0_RX, &paramSet);

    /* further paramsets, enable interrupt */
    paramSet.opt |= RX_DMA_INT_ENABLE;

    for(idx = 0 ; idx < NUM_PAR; idx++)
    {
        paramSet.destAddr = rxBufPtr[idx];

        paramSet.linkAddr = (PAR_RX_START + ((idx + 1) % NUM_PAR))
                             * (SIZE_PARAMSET);

        paramSet.bCnt =  NUM_SAMPLES_PER_AUDIO_BUF;

        /*
        ** for the first linked paRAM set, start receiving the second
        ** sample only since the first sample is already received in
        ** rx buffer 0 itself.
        */
        if( 0 == idx)
        {
            paramSet.destAddr += BYTES_PER_SAMPLE;
            paramSet.bCnt -= BYTES_PER_SAMPLE;
        }

        EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, (PAR_RX_START + idx), &paramSet);
    }

    /* Initialize the required variables for reception */
    nxtBufToRcv = idx % NUM_BUF;
    lastFullRxBuf = NUM_BUF - 1;
    parOffRcvd = 0;

    /* Initialize the 1st paRAM set for transmit */
    memcpy(&paramSet, &txDefaultPar, SIZE_PARAMSET);

    EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, EDMA3_CHA_MCASP0_TX, &paramSet);

    /* rest of the params, enable loop job */
    for(idx = 0 ; idx < NUM_PAR; idx++)
    {
        ParamTxLoopJobSet(PAR_TX_START + idx);
    }

    /* Initialize the variables for transmit */
    parOffSent = 0;
    lastSentTxBuf = NUM_BUF - 1;
}

/*
** Function to configure the codec for I2S mode
*/
static void AIC31I2SConfigure(void)
{
    volatile unsigned int delay = 0xFFF;

    AIC31Reset(SOC_I2C_0_REGS);
    while(delay--);

    /* Configure the data format and sampling rate */
    AIC31DataConfig(SOC_I2C_0_REGS, AIC31_DATATYPE_I2S, SLOT_SIZE, 0);
    AIC31SampleRateConfig(SOC_I2C_0_REGS, AIC31_MODE_BOTH, SAMPLING_RATE);

    /* Initialize both ADC and DAC */
    AIC31ADCInit(SOC_I2C_0_REGS);
    AIC31DACInit(SOC_I2C_0_REGS);
}

/*
** Configures the McASP Transmit Section in I2S mode.
*/
static void McASPI2SConfigure(void)
{
    McASPRxReset(SOC_MCASP_0_CTRL_REGS);
    McASPTxReset(SOC_MCASP_0_CTRL_REGS);

    /* Enable the FIFOs for DMA transfer */
    McASPReadFifoEnable(SOC_MCASP_0_FIFO_REGS, 1, 1);
    McASPWriteFifoEnable(SOC_MCASP_0_FIFO_REGS, 1, 1);

    /* Set I2S format in the transmitter/receiver format units */
    McASPRxFmtI2SSet(SOC_MCASP_0_CTRL_REGS, WORD_SIZE, SLOT_SIZE,
                     MCASP_RX_MODE_DMA);
    McASPTxFmtI2SSet(SOC_MCASP_0_CTRL_REGS, WORD_SIZE, SLOT_SIZE,
                     MCASP_TX_MODE_DMA);

    /* Configure the frame sync. I2S shall work in TDM format with 2 slots */
    McASPRxFrameSyncCfg(SOC_MCASP_0_CTRL_REGS, 1, MCASP_RX_FS_WIDTH_WORD,
                        MCASP_RX_FS_EXT_BEGIN_ON_FALL_EDGE);
    McASPTxFrameSyncCfg(SOC_MCASP_0_CTRL_REGS, 1, MCASP_TX_FS_WIDTH_WORD,
                        MCASP_TX_FS_EXT_BEGIN_ON_RIS_EDGE);

    /* configure the clock for receiver */
    McASPRxClkCfg(SOC_MCASP_0_CTRL_REGS, MCASP_RX_CLK_EXTERNAL, 0, 0);
    McASPRxClkPolaritySet(SOC_MCASP_0_CTRL_REGS, MCASP_RX_CLK_POL_RIS_EDGE);
    McASPRxClkCheckConfig(SOC_MCASP_0_CTRL_REGS, MCASP_RX_CLKCHCK_DIV32,
                          0x00, 0xFF);

    /* configure the clock for transmitter */
    McASPTxClkCfg(SOC_MCASP_0_CTRL_REGS, MCASP_TX_CLK_EXTERNAL, 0, 0);
    McASPTxClkPolaritySet(SOC_MCASP_0_CTRL_REGS, MCASP_TX_CLK_POL_FALL_EDGE);
    McASPTxClkCheckConfig(SOC_MCASP_0_CTRL_REGS, MCASP_TX_CLKCHCK_DIV32,
                          0x00, 0xFF);

    /* Enable synchronization of RX and TX sections  */
    McASPTxRxClkSyncEnable(SOC_MCASP_0_CTRL_REGS);

    /* Enable the transmitter/receiver slots. I2S uses 2 slots */
    McASPRxTimeSlotSet(SOC_MCASP_0_CTRL_REGS, I2S_SLOTS);
    McASPTxTimeSlotSet(SOC_MCASP_0_CTRL_REGS, I2S_SLOTS);

    /*
    ** Set the serializers, Currently only one serializer is set as
    ** transmitter and one serializer as receiver.
    */
    McASPSerializerRxSet(SOC_MCASP_0_CTRL_REGS, MCASP_XSER_RX);
    McASPSerializerTxSet(SOC_MCASP_0_CTRL_REGS, MCASP_XSER_TX);

    /*
    ** Configure the McASP pins
    ** Input - Frame Sync, Clock and Serializer Rx
    ** Output - Serializer Tx is connected to the input of the codec
    */
    McASPPinMcASPSet(SOC_MCASP_0_CTRL_REGS, 0xFFFFFFFF);
    McASPPinDirOutputSet(SOC_MCASP_0_CTRL_REGS, MCASP_PIN_AXR(MCASP_XSER_TX));
    McASPPinDirInputSet(SOC_MCASP_0_CTRL_REGS, MCASP_PIN_AFSX
                                               | MCASP_PIN_ACLKX
                                               | MCASP_PIN_AFSR
                                               | MCASP_PIN_ACLKR
                                               | MCASP_PIN_AXR(MCASP_XSER_RX));

    /* Enable error interrupts for McASP */
    McASPTxIntEnable(SOC_MCASP_0_CTRL_REGS, MCASP_TX_DMAERROR
                                            | MCASP_TX_CLKFAIL
                                            | MCASP_TX_SYNCERROR
                                            | MCASP_TX_UNDERRUN);

    McASPRxIntEnable(SOC_MCASP_0_CTRL_REGS, MCASP_RX_DMAERROR
                                            | MCASP_RX_CLKFAIL
                                            | MCASP_RX_SYNCERROR
                                            | MCASP_RX_OVERRUN);
}

/*
** Sets up the interrupts for EDMA in AINTC
*/
static void EDMA3IntSetup(void)
{
#ifdef _TMS320C6X
	IntRegister(C674X_MASK_INT5, EDMA3CCComplIsr);
	IntEventMap(C674X_MASK_INT5, SYS_INT_EDMA3_0_CC0_INT1);
	IntEnable(C674X_MASK_INT5);
#else
    IntRegister(SYS_INT_CCINT0, EDMA3CCComplIsr);
    IntChannelSet(SYS_INT_CCINT0, INT_CHANNEL_EDMACC);
    IntSystemEnable(SYS_INT_CCINT0);
#endif
}

/*
** Sets up the error interrupts for McASP in AINTC
*/
static void McASPErrorIntSetup(void)
{
#ifdef _TMS320C6X
	IntRegister(C674X_MASK_INT6, McASPErrorIsr);
	IntEventMap(C674X_MASK_INT6, SYS_INT_MCASP0_INT);
	IntEnable(C674X_MASK_INT6);
#else
    /* Register the error ISR for McASP */
    IntRegister(SYS_INT_MCASPINT, McASPErrorIsr);

    IntChannelSet(SYS_INT_MCASPINT, INT_CHANNEL_MCASP);
    IntSystemEnable(SYS_INT_MCASPINT);
#endif
}

/*
** Activates the data transmission/reception
** The DMA parameters shall be ready before calling this function.
*/
static void I2SDataTxRxActivate(void)
{
    /* Start the clocks */
    McASPRxClkStart(SOC_MCASP_0_CTRL_REGS, MCASP_RX_CLK_EXTERNAL);
    McASPTxClkStart(SOC_MCASP_0_CTRL_REGS, MCASP_TX_CLK_EXTERNAL);

    /* Enable EDMA for the transfer */
    EDMA3EnableTransfer(SOC_EDMA30CC_0_REGS, EDMA3_CHA_MCASP0_RX,
                        EDMA3_TRIG_MODE_EVENT);
    EDMA3EnableTransfer(SOC_EDMA30CC_0_REGS,
                        EDMA3_CHA_MCASP0_TX, EDMA3_TRIG_MODE_EVENT);

    /* Activate the  serializers */
    McASPRxSerActivate(SOC_MCASP_0_CTRL_REGS);
    McASPTxSerActivate(SOC_MCASP_0_CTRL_REGS);

    /* make sure that the XDATA bit is cleared to zero */
    while(McASPTxStatusGet(SOC_MCASP_0_CTRL_REGS) & MCASP_TX_STAT_DATAREADY);

    /* Activate the state machines */
    McASPRxEnable(SOC_MCASP_0_CTRL_REGS);
    McASPTxEnable(SOC_MCASP_0_CTRL_REGS);
}

/*
** Activates the DMA transfer for a parameterset from the given buffer.
*/
void BufferTxDMAActivate(unsigned int txBuf, unsigned short numSamples,
                         unsigned short parId, unsigned short linkPar)
{
    EDMA3CCPaRAMEntry paramSet;

    /* Copy the default paramset */
    memcpy(&paramSet, &txDefaultPar, SIZE_PARAMSET - 2);

    /* Enable completion interrupt */
    paramSet.opt |= TX_DMA_INT_ENABLE;
    paramSet.srcAddr =  txBufPtr[txBuf];
    paramSet.linkAddr = linkPar * SIZE_PARAMSET;
    paramSet.bCnt = numSamples;

    EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, parId, &paramSet);
}


//void writebl(Shortword output[], FILE *fp_out, Shortword size)

//{
//    fwrite(output,2,size,fp_out);
//}


/*
** The main function. Application starts here.
*/
int main(void)
{
    unsigned short parToSend;
    unsigned short parToLink;
	unsigned int i;

	//FILE *fp_out;
	//out_name = "Data\\vov1_new.pcm";
	//fp_out = fopen(out_name,"wb");


    /* Set up pin mux for I2C module 0 */
    I2CPinMuxSetup(0);
    McASPPinMuxSetup();

    /* Power up the McASP module */
    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_MCASP0, PSC_POWERDOMAIN_ALWAYS_ON,
		     PSC_MDCTL_NEXT_ENABLE);

    /* Power up EDMA3CC_0 and EDMA3TC_0 */
    PSCModuleControl(SOC_PSC_0_REGS, HW_PSC_CC0, PSC_POWERDOMAIN_ALWAYS_ON,
		     PSC_MDCTL_NEXT_ENABLE);
    PSCModuleControl(SOC_PSC_0_REGS, HW_PSC_TC0, PSC_POWERDOMAIN_ALWAYS_ON,
		     PSC_MDCTL_NEXT_ENABLE);

#ifdef _TMS320C6X
    // Initialize the DSP interrupt controller
    IntDSPINTCInit();
#else
    /* Initialize the ARM Interrupt Controller.*/
    IntAINTCInit();
#endif

    /* Initialize the I2C 0 interface for the codec AIC31 */
    I2CCodecIfInit(SOC_I2C_0_REGS, INT_CHANNEL_I2C, I2C_SLAVE_CODEC_AIC31);

    EDMA3Init(SOC_EDMA30CC_0_REGS, 0);
    EDMA3IntSetup();

    McASPErrorIntSetup();

#ifdef _TMS320C6X
    IntGlobalEnable();
#else
    /* Enable the interrupts generation at global level */
    IntMasterIRQEnable();
    IntGlobalEnable();
    IntIRQEnable();
#endif

    /*
    ** Request EDMA channels. Channel 0 is used for reception and
    ** Channel 1 is used for transmission
    */
    EDMA3RequestChannel(SOC_EDMA30CC_0_REGS, EDMA3_CHANNEL_TYPE_DMA,
                        EDMA3_CHA_MCASP0_TX, EDMA3_CHA_MCASP0_TX, 0);
    EDMA3RequestChannel(SOC_EDMA30CC_0_REGS, EDMA3_CHANNEL_TYPE_DMA,
                        EDMA3_CHA_MCASP0_RX, EDMA3_CHA_MCASP0_RX, 0);

    /* Initialize the DMA parameters */
    I2SDMAParamInit();

    /* Configure the Codec for I2S mode */
    AIC31I2SConfigure();

    /* Configure the McASP for I2S */
    McASPI2SConfigure();

    /* Activate the audio transmission and reception */
    I2SDataTxRxActivate();
	/* ====== Initialize MELP analysis and synthesis ====== */
	frameSize = (Shortword) BLOCK;
	/* Computing bit=Num = rate * frameSize / FSAMP.  Note that bitNum        */
	/* computes the number of bytes written to the channel and it has to be   */
	/* exact.  We first carry out the division and then have the multiplica-  */
	/* tion with rounding.                                                    */
	bitNum12 = 81;

	melp_ana_init();
	melp_syn_init();
#if(BUG_GPIO)
	    /* The Local PSC number for GPIO is 3. GPIO belongs to PSC1 module.*/
    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_GPIO, PSC_POWERDOMAIN_ALWAYS_ON,
		     PSC_MDCTL_NEXT_ENABLE);

    /* Pin Multiplexing of pin 12 of GPIO Bank 6.*/
    GPIOBank6Pin12PinMuxSetup();

    /* Sets the pin 109 (GP6[12]) as input.*/
    GPIODirModeSet(SOC_GPIO_0_REGS, 109, GPIO_DIR_OUTPUT);
	GPIOPinWrite(SOC_GPIO_0_REGS, 109, GPIO_PIN_HIGH);
#endif

    /*
    ** Looop forever. if a new buffer is received, the lastFullRxBuf will be
    ** updated in the rx completion ISR. if it is not the lastSentTxBuf,
    ** buffer is to be sent. This has to be mapped to proper paRAM set.
    */
    while(1)
    {
        if(lastFullRxBuf != lastSentTxBuf)
        {
            /*
            ** Start the transmission from the link paramset. The param set
            ** 1 will be linked to param set at PAR_TX_START. So do not
            ** update paRAM set1.
            */
            parToSend =  PAR_TX_START + (parOffTxToSend % NUM_PAR);
            parOffTxToSend = (parOffTxToSend + 1) % NUM_PAR;
            parToLink  = PAR_TX_START + parOffTxToSend;

            lastSentTxBuf = (lastSentTxBuf + 1) % NUM_BUF;

			switch(lastFullRxBuf)
			{
				case 0:
				for(i=0;i<AUDIO_BUF_SIZE/2;i++)
				{
					speech_in[i] = (unsigned int)((unsigned int)(pingrx_Buf[2*i])+(unsigned int)(pingrx_Buf[2*i+1]<<8));
				}
				break;
				case 1:
				for(i=0;i<AUDIO_BUF_SIZE/2;i++)
				{
					speech_in[i] = (unsigned int)((unsigned int)(pongrx_Buf[2*i])+(unsigned int)(pongrx_Buf[2*i+1]<<8));
				}
				break;
				default:
				break;
			}
			gia_tri_melp =1;
			  //for(i=0;i<AUDIO_BUF_SIZE/2;i++)
			 //{
			//	 speech_out[i] = speech_in[i];
			 //}
			switch(lastSentTxBuf)
			{
				case 0:
				for(i=0;i<AUDIO_BUF_SIZE/2;i++)
				{
					pingtx_Buf[2*i] =  (unsigned char)(speech_out[i]);
					pingtx_Buf[2*i+1] = (unsigned char)(speech_out[i]>>8);
				}
				break;
				case 1:
				for(i=0;i<AUDIO_BUF_SIZE/2;i++)
				{
					pongtx_Buf[2*i] =  (unsigned char)(speech_out[i]);
					pongtx_Buf[2*i+1] = (unsigned char)(speech_out[i]>>8);
				}
				break;
				break;
				default:
				break;
			}

            /* Copy the buffer */
            /* memcpy((void *)txBufPtr[lastSentTxBuf],
                   (void *)rxBufPtr[lastFullRxBuf],
                   AUDIO_BUF_SIZE); */

            /*
            ** Send the buffer by setting the DMA params accordingly.
            ** Here the buffer to send and number of samples are passed as
            ** parameters. This is important, if only transmit section
            ** is to be used.
            */
            BufferTxDMAActivate(lastSentTxBuf, NUM_SAMPLES_PER_AUDIO_BUF,
                                (unsigned short)parToSend,
                                (unsigned short)parToLink);
        }
		if(1==gia_tri_melp)
		{
			gia_tri_melp=0;
#if(BUG_GPIO_ANA)
			GPIOPinWrite(SOC_GPIO_0_REGS, 109, GPIO_PIN_LOW);
			analysis(speech_in, melp_par);
			//GPIOPinWrite(SOC_GPIO_0_REGS, 109, GPIO_PIN_HIGH);
#else
			analysis(speech_in, melp_par);
#endif
			/* if(gia_tri_mang<142)
			{
					for(i=0;i<11;i++)
					{
						chbuf[i] = mang_test[11*gia_tri_mang+i];
					}
					gia_tri_mang++;
			}
			else
			{
				gia_tri_mang=0;
			} */
#if(BUG_GPIO_SYS)
			//GPIOPinWrite(SOC_GPIO_0_REGS, 109, GPIO_PIN_LOW);
			synthesis(melp_par, speech_out);
			GPIOPinWrite(SOC_GPIO_0_REGS, 109, GPIO_PIN_HIGH);
#else
			synthesis(melp_par, speech_out);
#endif
			/* for(i=0;i<AUDIO_BUF_SIZE/2;i++)
			{
				 speech_out[i] = speech_in[i];
			}  */
			/*  for (i=0; i<160; i++)
			{
				fputc((speech_out[i])    & 0x0FF,fp_out);
        		fputc((speech_out[i]>>8) & 0x0FF,fp_out);
			} */
		}
    }
}

/*
** Activates the DMA transfer for a parameter set from the given buffer.
*/
static void BufferRxDMAActivate(unsigned int rxBuf, unsigned short parId,
                                unsigned short parLink)
{
    EDMA3CCPaRAMEntry paramSet;

    /* Copy the default paramset */
    memcpy(&paramSet, &rxDefaultPar, SIZE_PARAMSET - 2);

    /* Enable completion interrupt */
    paramSet.opt |= RX_DMA_INT_ENABLE;
    paramSet.destAddr =  rxBufPtr[rxBuf];
    paramSet.bCnt =  NUM_SAMPLES_PER_AUDIO_BUF;
    paramSet.linkAddr = parLink * SIZE_PARAMSET ;

    EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, parId, &paramSet);
}

/*
** This function will be called once receive DMA is completed
*/
static void McASPRxDMAComplHandler(void)
{
    unsigned short nxtParToUpdate;

    /*
    ** Update lastFullRxBuf to indicate a new buffer reception
    ** is completed.
    */
    lastFullRxBuf = (lastFullRxBuf + 1) % NUM_BUF;
    nxtParToUpdate =  PAR_RX_START + parOffRcvd;
    parOffRcvd = (parOffRcvd + 1) % NUM_PAR;

    /*
    ** Update the DMA parameters for the received buffer to receive
    ** further data in proper buffer
    */
    BufferRxDMAActivate(nxtBufToRcv, nxtParToUpdate,
                        PAR_RX_START + parOffRcvd);

    /* update the next buffer to receive data */
    nxtBufToRcv = (nxtBufToRcv + 1) % NUM_BUF;
}

/*
** This function will be called once transmit DMA is completed
*/
static void McASPTxDMAComplHandler(void)
{
    ParamTxLoopJobSet((unsigned short)(PAR_TX_START + parOffSent));

    parOffSent = (parOffSent + 1) % NUM_PAR;
}

/*
** EDMA transfer completion ISR
*/
static void EDMA3CCComplIsr(void)
{
#ifdef _TMS320C6X
	IntEventClear(SYS_INT_EDMA3_0_CC0_INT1);
#else
    IntSystemStatusClear(SYS_INT_CCINT0);
#endif

    /* Check if receive DMA completed */
    if(EDMA3GetIntrStatus(SOC_EDMA30CC_0_REGS) & (1 << EDMA3_CHA_MCASP0_RX))
    {
        /* Clear the interrupt status for the 0th channel */
        EDMA3ClrIntr(SOC_EDMA30CC_0_REGS, EDMA3_CHA_MCASP0_RX);
        McASPRxDMAComplHandler();
    }

    /* Check if transmit DMA completed */
    if(EDMA3GetIntrStatus(SOC_EDMA30CC_0_REGS) & (1 << EDMA3_CHA_MCASP0_TX))
    {
        /* Clear the interrupt status for the first channel */
        EDMA3ClrIntr(SOC_EDMA30CC_0_REGS, EDMA3_CHA_MCASP0_TX);
        McASPTxDMAComplHandler();
    }
}

/*
** Error ISR for McASP
*/
static void McASPErrorIsr(void)
{
#ifdef _TMS320C6X
	IntEventClear(SYS_INT_MCASP0_INT);
#else
    IntSystemStatusClear(SYS_INT_MCASPINT);
#endif

    ; /* Perform any error handling here.*/
}

/***************************** End Of File ***********************************/
