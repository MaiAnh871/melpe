/* ================================================================== */
/*                                                                    */ 
/*    Microsoft Speech coder     ANSI-C Source Code                   */
/*    SC1200 1200 bps speech coder                                    */
/*    Fixed Point Implementation      Version 7.0                     */
/*    Copyright (C) 2000, Microsoft Corp.                             */
/*    All rights reserved.                                            */
/*                                                                    */ 
/* ================================================================== */

/* ========================================= */
/* melp.c: Mixed Excitation LPC speech coder */
/* ========================================= */

#include <stdio.h>
#include <string.h>

#include "sc1200.h"
#include "mat_lib.h"
#include "global.h"
#include "macro.h"
#include "mathhalf.h"
#include "dsp_sub.h"
#include "melp_sub.h"
#include "constant.h"
#include "math_lib.h"

//#include "gpio.h"
//#include "soc_OMAPL138.h"
//#include "hw_psc_OMAPL138.h"
//#include "lcdkOMAPL138.h"
//#include "demoCfg.h"

#if NPP
#include "npp.h"
#endif

#define X05_Q7				64         /* 0.5 * (1 << 7) */
#define THREE_Q7			384        /* 3 * (1 << 7) */

//Shortword	mode = ANALYSIS;
//Shortword	mode = SYNTHESIS;
//Shortword	mode = ANA_SYN;
//Shortword   chwordsize = 8;

/* ========== Global Variables ========== */
char *in_name;
char *out_name;

/* ========== BLINK LED ========== */
#define BLINK 0

#include "melpe.h"   //low-level alsa/wave audio
static int numfr;
static short spin[748];
static short spout[748];
int main(int argc, char *argv[])
{
   //int i = 0;
   int j;
  FILE *fin, *fout;

  printf("-----------------------------------------------------\r\n");
  printf("          EDABK PairPhone VAD \r\n\n");
  printf("-----------------------------------------------------\r\n");
    printf("Running MELPe with: \nFile in \t:%s \nFile out\t:%s\n\n\n", argv[1], argv[2]);

  fin = fopen("D:\\voice-compress\\Melpe1200-2400\\1.bin", "rb");
  fout = fopen("D:\\voice-compress\\Melpe1200-2400\\data\\1.melpe", "wb");
  //short a = 0;
  printf("-----------------------------------------------------\r\n");
  printf("          EDABK PairPhone MELPe \r\n\n");
  printf("-----------------------------------------------------\r\n");
    melpe_i();

    unsigned char buf[12];

    while(1)
    {
      j = fread(spin, sizeof(short), 540, fin);

      if (j < 540)
      {
          printf("Done MELPe!\n");
          return 0;
      }
      numfr++;
      printf("\n\nFRAME %d\n",numfr);
      melpe_a(buf,spin);
      //buf[9]&=0xFE;
      //printf("buf0 = %x     buf1 = %x\n",buf[0],buf[1]);
      melpe_s(spout,buf);

      fwrite(spout, sizeof(short), 540, fout);
    }
}
//---------------------------------------------------------------------------
