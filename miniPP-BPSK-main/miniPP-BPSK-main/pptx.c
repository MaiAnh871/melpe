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

#include "audio/audio.h"   //low-level alsa/wave audio
#include "crypto/libcrp.h" //cryptography primitives
#include "melpe/melpe.h"   //audio codec
#include "modem/modem.h"   //modem BPSK
#include "define.h"

#include "crp.h" //key agreement, authentication, encryption/decryption, frame synchronization
#include "ctr.h" //scan keyboard, processing. Suspending thread if no job (CTR)
// #include "rx.h"  //receiving baseband, demodulating, decrypting, decompressing, playing over earphones
#include "tx.h"  //recording voice from mike, compressing, encrypting, modulating, sending baseband into line

#ifdef mod8k
#define modem "SAMPLING RATE = 8KHz"
#else
#define modem "SAMPLING RATE = 48KHz"
#endif

int main(int argc, char *argv[])
{
  int i = 0;
  int a = 0;
  FILE *fin, *fout;

  printf("-----------------------------------------------------\r\n");
  printf("   		EDABK PairPhone TX \r\n\n");
  printf("   		%s \r\n", modem);
  printf("-----------------------------------------------------\r\n");

  //  audio_init();  //init audio
  melpe_i();
  //get file path

  #ifdef writeBuff
		printf("Running TX with: \nFile in \t:%s \nFile out\t:%s\nFile buff:\t%s\n\n\n", argv[1], argv[2], argv[3]);
		FILE *fbuf;
		fbuf = fopen(argv[3], "wb");
	#else
		printf("Running TX with: \nFile in \t:%s \nFile out\t:%s\n\n\n", argv[1], argv[2]);
	#endif
  fin = fopen(argv[1], "rb");
  fout = fopen(argv[2], "wb");

  //main loop
  do
  {
  #ifdef writeBuff
		i = tx(i, fin, fout, fbuf); //transmitting
	#else
		i = tx(i, fin, fout); //transmitting
	#endif
  } while (i);


  printf("-----------------------------------------------------\r\n");
  printf("   		EDABK PairPhone TX \r\n\n");
  printf("   		%s \r\n", modem);
  printf("-----------------------------------------------------\r\n");

  fclose(fin);
  fclose(fout);
  #ifdef writeBuff
		fclose(fbuf);
	#endif
  //tty_normode(); //restore console
  //audio_fin(); //close audio
  return 0;
}
//---------------------------------------------------------------------------
