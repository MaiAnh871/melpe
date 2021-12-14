///////////////////////////////////////////////
//
// **************************
//
// Project/Software name: X-Phone
// Author: "Van Gegel" <gegelcopy@ukr.net>
//
// THIS IS A FREE SOFTWARE  AND FOR TEST ONLY!!!
// Please do not use it in the case of life and death
// This software is released under GNU LGPL:
//
// * LGPL 3.0 <http://www.gnu.org/licenses/lgpl.html>
//
// You’re free to copy, distribute and make commercial use
// of this software under the following conditions:
//
// * You have to cite the author (and copyright owner): Van Gegel
// * You have to provide a link to the author’s Homepage: <http://torfone.org>
//
///////////////////////////////////////////////

//This is a main procedure of PairPhone testing software
//one-thread implementation as a infinite loop contained procedures for:
//-receiving baseband, demodulating, decrypting, decompressing, playing over earphones (RX)
//-recording voice from mike, compressing, encrypting, modulating, sending baseband into line (TX)
//-scan keyboard, processing. Suspending thread if no job (CTR)
//---------------------------------------------------------------------------
#ifdef _WIN32

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <basetsd.h>
#include <stdint.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include "memory.h"
#include "math.h"

#else //linux

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "memory.h"
#include "math.h"

#include <sys/time.h>

#endif

#include "melpe/melpe.h"   //low-level alsa/wave audio
static char numfr;
static short spin[748]; 
static short spout[748]; 
/*
int main(int argc, char *argv[])
{
   int i = 0;
   int j;
  FILE *fin, *fout;

  printf("-----------------------------------------------------\r\n");
  printf("   		EDABK PairPhone VAD \r\n\n");
  printf("-----------------------------------------------------\r\n");
	printf("Running MELPe with: \nFile in \t:%s \nFile out\t:%s\n\n\n", argv[1], argv[2]);

  fin = fopen(argv[1], "rb");
  fout = fopen(argv[2], "wb");
  short a = 0;
  printf("-----------------------------------------------------\r\n");
  printf("   		EDABK PairPhone MELPe \r\n\n");
  printf("-----------------------------------------------------\r\n");
    melpe_i();
    numfr++;
    unsigned char buf[12];
    //  printf("\n\nFRAME %d\n",numf);
    while(1)
    {
      j = fread(spin, sizeof(short), 540, fin);
      
      if (j < 540)
      {
          printf("Done MELPe!\n");
          return 0;
      }
      melpe_a(spout, spin);
      //melpe_s(spout,buf);
      
      fwrite(spout, sizeof(short), 6, fout);  
    }
}
//---------------------------------------------------------------------------
*/

int main(int argc, char *argv[])
{
   int i = 0;
   int j;
  FILE *fin, *fout;

  printf("-----------------------------------------------------\r\n");
  printf("   		EDABK PairPhone VAD \r\n\n");
  printf("-----------------------------------------------------\r\n");
	printf("Running MELPe with: \nFile in \t:%s \nFile out\t:%s\n\n\n", argv[1], argv[2]);

  fin = fopen(argv[1], "rb");
  fout = fopen(argv[2], "wb");
  short a = 0;
  printf("-----------------------------------------------------\r\n");
  printf("   		EDABK PairPhone MELPe \r\n\n");
  printf("-----------------------------------------------------\r\n");
    melpe_i();
    numfr++;
    //unsigned char buf[12];
    //  printf("\n\nFRAME %d\n",numf);
    while(1)
    {
      j = fread(spin, sizeof(short), 6, fin);
      
      if (j < 6)
      {
          printf("Done MELPe!\n");
          return 0;
      }
      //melpe_a(buf,spin);
      melpe_s (spout,spin);
      
      fwrite(spout, sizeof(short), 540, fout);  
    }
}
//---------------------------------------------------------------------------


/*
int main(int argc, char *argv[])
{
   int i = 0;
   int j;
  FILE *fin, *fout;

  printf("-----------------------------------------------------\r\n");
  printf("   		EDABK PairPhone VAD \r\n\n");
  printf("-----------------------------------------------------\r\n");
	printf("Running MELPe with: \nFile in \t:%s \nFile out\t:%s\n\n\n", argv[1], argv[2]);

  fin = fopen(argv[1], "rb");
  fout = fopen(argv[2], "wb");
  short a = 0;
  printf("-----------------------------------------------------\r\n");
  printf("   		EDABK PairPhone MELPe \r\n\n");
  printf("-----------------------------------------------------\r\n");
    melpe_i();
    numfr++;
    unsigned char buf[12];
    //  printf("\n\nFRAME %d\n",numf);
    while(1)
    {
      j = fread(spin, sizeof(short), 540, fin);
      
      if (j < 540)
      {
          printf("Done MELPe!\n");
          return 0;
      }
      melpe_a(buf,spin);
      melpe_s(spout,buf);
      
      fwrite(spout, sizeof(short), 540, fout);  
    }
}
//---------------------------------------------------------------------------
*/
