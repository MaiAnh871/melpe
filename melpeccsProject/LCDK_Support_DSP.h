// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017
 
///////////////////////////////////////////////////////////////////////
// Filename: LCDK_Support_DSP.h
//
// Synopsis: Declarations for OMAPL138_Support_DSP.c
//
///////////////////////////////////////////////////////////////////////

#ifndef	LCDK_SUPPORT_DSP_H_INCLUDED
#define LCDK_SUPPORT_DSP_H_INCLUDED

#include "tistdtypes.h"
#include "OMAPL138_defines.h"
#include <c6x.h>

// sample rate and codec enumerations for Init_OMAPL138 and SetSampleRate_AIC3106
enum {AIC3106Fs48kHz, AIC3106Fs96kHz, AIC3106Fs32kHz, AIC3106Fs24kHz, AIC3106Fs16kHz, AIC3106Fs12kHz, AIC3106Fs8kHz};
#define LCDK_LineInput		0
#define LCDK_MicInput_0db	1
#define LCDK_MicInput_20db	2
// enumeration for WriteLEDs
enum {USER_LED1=1, USER_LED2 = 2, USER_LED3=4, USER_LED4 = 8};

// defined in LCDK_Support_DSP.c
float  GetSampleFreq();
void   DSP_Init();
void   DSP_Init_EDMA();
Uint32 WriteLEDs(Uint8);
Int32  ReadSwitches();
void   InitDigitalOutputs();
void   WriteDigitalOutputs(Uint8);
Int32  InitLEDs();
void   Init_Interrupts();
void   EnableInterrupts();
void   Init_Interrupts_EDMA();
void   EnableInterrupts_EDMA();
void   Init_I2C();
void   Reset_I2C();
Uint32 Write_I2C(Uint16, Uint8 *, Uint16);
Uint32 WriteRead_I2C(Uint16, Uint8 *, Uint16, Uint16);
Uint32 Read_I2C(Uint16, Uint8 *, Uint16);
Uint32 Init_AIC3106(Uint8);
Uint32 Reset_AIC3106();
Uint32 SetSampleRate_AIC3106(Uint8);
Uint32 AIC3106_write_reg(Uint8, Uint8);
void   Init_McASP0();
void   Init_UART2(Uint32);
void   Write_UART2(Uint8);
void   Puts_UART2(char *);
Uint8  Read_UART2();
Uint8  IsDataReady_UART2();
Uint8  IsTxReady_UART2();

Uint32 ReadCodecData();
void   WriteCodecData(Uint32);
Uint32 CheckForOverrun();

// defined in StartUp.c (in application directory)
void   StartUp();


#endif
