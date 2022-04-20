///* ================================================================== */
///*                                                                    */
///*    Microsoft Speech coder     ANSI-C Source Code                   */
///*    SC1200 1200 bps speech coder                                    */
///*    Fixed Point Implementation      Version 7.0                     */
///*    Copyright (C) 2000, Microsoft Corp.                             */
///*    All rights reserved.                                            */
///*                                                                    */
///* ================================================================== */
//
///* ========================================= */
///* melp.c: Mixed Excitation LPC speech coder */
///* ========================================= */
//
//#include <stdio.h>
//#include <string.h>
//
//#include "sc1200.h"
//#include "mat_lib.h"
//#include "global.h"
//#include "macro.h"
//#include "mathhalf.h"
//#include "dsp_sub.h"
//#include "melp_sub.h"
//#include "constant.h"
//#include "math_lib.h"
//
//#include "gpio.h"
//#include "soc_OMAPL138.h"
//#include "hw_psc_OMAPL138.h"
//#include "lcdkOMAPL138.h"
////#include "demoCfg.h"
//
//#if NPP
//#include "npp.h"
//#endif
//
//#define X05_Q7				64         /* 0.5 * (1 << 7) */
//#define THREE_Q7			384        /* 3 * (1 << 7) */
//
////Shortword	mode = ANALYSIS;
////Shortword	mode = SYNTHESIS;
////Shortword	mode = ANA_SYN;
////Shortword   chwordsize = 8;
//
///* ========== Global Variables ========== */
//char *in_name;
//char *out_name;
//
///* ========== BLINK LED ========== */
//#define BLINK 0
//
///****************************** MELP 1200 Microsoft modified by Tung *****************************/
//#include "melpe.h"   //low-level alsa/wave audio
//static unsigned char numfr;
//static short spin[748];
//static short spout[748];
//int main(int argc, char *argv[])
//{
//	//====================================Testing====================================
//#if BLINK
//	GPIOBank6Pin12PinMuxSetup();
//	GPIODirModeSet(SOC_GPIO_0_REGS, 109, GPIO_DIR_OUTPUT);
//	int state = 0;
//	int i;
//#endif
//
//#if BLINK
//		GPIOPinWrite(SOC_GPIO_0_REGS, 109, state);
//		for (i = 10000000; i >= 0; i--);
//		state = !state;
//#endif
//
//#if NPP
//			npp(speech_in, speech_in);
//			npp(&(speech_in[FRAME]), &(speech_in[FRAME]));
//			npp(&(speech_in[2 * FRAME]), &(speech_in[2 * FRAME]));
//#endif
//
//   int j;
//  FILE *fin, *fout;
//
//  printf("-----------------------------------------------------\r\n");
//  printf("          EDABK PairPhone VAD \r\n\n");
//  printf("-----------------------------------------------------\r\n");
//    printf("Running MELPe with: \nFile in \t:%s \nFile out\t:%s\n\n\n", argv[1], argv[2]);
//
//  fin = fopen("C:\\Project\\voice-compress\\Melpe1200-2400\\1.bin", "rb");
//  fout = fopen("C:\\Project\\voice-compress\\Melpe1200-2400\\1.meple", "wb");
//  printf("-----------------------------------------------------\r\n");
//  printf("          EDABK PairPhone MELPe \r\n\n");
//  printf("-----------------------------------------------------\r\n");
//    melpe_i();
//
//    unsigned char buf[12];
//
//    while(1)
//    {
//      j = fread(spin, sizeof(short), 540, fin);
//
//      if (j < 540)
//      {
//          printf("Done MELPe!\nFRAME %d\n",numfr);
//          return 0;
//      }
//
//      if (++numfr%10==0)
//          printf("FRAME %d\n",numfr);
//      melpe_a(buf,spin);
//      melpe_s(spout,buf);
//
//      fwrite(spout, sizeof(short), 540, fout);
//    }
//}
