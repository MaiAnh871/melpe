// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017
 
///////////////////////////////////////////////////////////////////////
// Filename: DSP_Config.h
//
// Synopsis: Declarations for configuring the codec parameters
//
///////////////////////////////////////////////////////////////////////

#ifndef	DSP_CONFIG_H_INCLUDED
#define DSP_CONFIG_H_INCLUDED

#include "LCDK_Support_DSP.h"
#include "uart.h"

#define DSPBOARDTYPE_TI_OMAPL138_LCDK

// uncomment just the line for the sample rate when using the LCDK
#define SampleRateSetting 		AIC3106Fs48kHz		// 48kHz sample rate
//#define SampleRateSetting 		AIC3106Fs96kHz		// 96kHz sample rate
//#define SampleRateSetting 		AIC3106Fs32kHz		// 32kHz sample rate
//#define SampleRateSetting 		AIC3106Fs24kHz		// 24kHz sample rate
//#define SampleRateSetting 		AIC3106Fs16kHz		// 16kHz sample rate
//#define SampleRateSetting 		AIC3106Fs12kHz		// 12kHz sample rate
//#define SampleRateSetting 		AIC3106Fs8kHz		//  8kHz sample rate

// uncomment just the line for the codec input in use
#define CodecType 			LCDK_LineInput		// LCDK using line input
//#define CodecType 				LCDK_MicInput_0db	// LCDK using microphone input, no preamp
//#define CodecType 			LCDK_MicInput_20db	// LCDK using microphone input, +20db gain


#endif
