

//This file contains receiving procedures for PairPhone:
//We records some 8KHz audio samples from File, collects in the input buffer,
//synchronously demodulates while receives full 81 bits block,
//check for data type is voice or control, decrypt/decode or put silence,
//resampled depends actual buffer state and play 8KHz audio over Speaker.
//Estimated overall voice latency

//Total latency average 270 mS + GSM latency (typically 180 mS)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include <stddef.h>
#include <stdlib.h>
#include <basetsd.h>
#include <stdint.h>

#include <windows.h>
#include <time.h>

#else
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#endif

#include "audio/audio.h" //low-level audio input/output
#include "modem/modem.h" //modem
#include "melpe/melpe.h" //audio codec
#include "crp.h"         //data processing
#include "rx.h"          //this
#include "define.h"
#ifdef demod8k
#define Demodulate  Demodulate_8k
#define numGrab     180
#endif

//Global variables

//baseband processing
static short speech[360 * 6];   //PCM 48KHz input buffer for samples received from the line
static short *samples = speech; //pointer to samples ready for processing
static int cnt = 0;             //the number of unprocessed samples
static unsigned char buf[12];   //demodulators data output
#ifdef writeBuff
  static unsigned char bufsave[14];  
#endif
short sp[544];                  //ouputted speech frame

//resampling
float qff = 1.0;              //resampling ratio
static float up_pos = 1.0;    //resampler fractional position
static short left_sample = 0; //base sample

//playing
static short jit_buf[800]; //PCM 8KHz buffer for samples ready for playing over Speaker
//static short* p_jit_buf=jit_buf; //pointer to unplayed samples in the buffer
static short l_jit_buf = 0; //number of unplayed samples in the buffer
static int q_jit_buf = 0;   //pointer to unplayed samples in the buffer
static float fdelay = 7200; //averages playing delay

////////////////////////////////////////////////////////////////////////////
//for TEST
//**********************************************************
int rxbit = 0;
int rxerr = 0;
static float verrs = 0;
//bits set lookup table
static const unsigned char BitsSetTable256[256] =
    {
#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
        B6(0), B6(1), B6(1), B6(2)};
//*****************************************************************************
//receiving loop: grab 48KHz baseband samples from Line,
//demodulate, decrypt, decode, play 8KHz voice over Speaker
#ifdef writeBuff
	int rx(int typing, FILE *fin, FILE *fout, FILE *fbuf)	
#else
	int rx(int typing, FILE *fin, FILE *fout)
#endif

{
  //input: -1 for no typing chars, 1 - exist some chars in input buffer
  //output: 0 - no any jobs doing, 1 - some jobs were doing
  int i;
  float f;
  int job = 0;       //flag of any job were doing
  char lag_flag = 0; //block lag is locked (modems synchronization complete)
  //char lock_flag=0; //phase of carrier (1333Hz, 6 samples per period) is locked
  //char sync_flag=0; //the difference of frequency transmitter-to-receiver sampling rate is locked
  //char current_lag=0;  //block lag (0-90, aligned to last bit position, the 6 samples for bit)
  //regularly play Speaker's buffer
  //check for we have enough samples for demodulation
  if (cnt < numGrab) //check we haven't enough of unprocessed samples
  {
    //move tail to start of receiving buffer
    if (samples > speech) //check for tail
    {
      for (i = 0; i < cnt; i++)
        speech[i] = samples[i]; //move tail to start of buffer
      samples = speech;         //set pointer to start of buffer
    }
    //record
    //i=_soundgrab((char*)(samples+cnt), 180*6);  //try to grab new 48KHZ samples from Line

    if (fread(samples + cnt, sizeof(short), numGrab, fin) != numGrab)
    {
      printf("Done RX\n");
      return 0;
    }
    i = numGrab;

    if ((i > 0) && (i <= (numGrab))) //some samples grabbed
    {
      cnt += i; //add grabbed  samples to account
      job += 4; //set job
    }
  }
  else //we have enough samples for processing
  {
    i = Demodulate(samples, buf); //process samples: 36*6 (35-37)*6 samples
    samples += i;                 //move pointer to next samples (with frequency adjusting)
    cnt -= i;                     //decrease the number of unprocessed samples
    if (0x80 & buf[11])           //checks flag for output data block is ready
    {
      //check for synck and averages BER
      lag_flag = !(!(buf[11] & 0x40)); //block lag is locked (synchronization compleet)
      //printf("lag flag = %d\n",lag_flag);
      //lock_flag=!(!(buf[11]&0x20)); //phaze of carrier (1333Hz, 6 samples per period) is locked
      //sync_flag=!(!(buf[11]&0x10)); //the differency of frequency transmitter-to-receiver sampling rate is locked
      //current_lag=buf[10]>>1;  //block lag (0-90, aligned to last bit position, the 6 samples for bit)
      
      #ifdef writeBuff
        for (i=0;i<12;i++)
          bufsave[i] = buf[i];
		    bufsave[11] &= 0xFE;
      #endif
      
      buf[11] = 0xFE; //set flag default as for silence descriptor
      if (lag_flag)   //check modem sync
      {
        
        #ifdef writeBuff
          i = ProcessPkt(buf,bufsave); //decode received packet
        #else
          i = ProcessPkt(buf); //decode received packet
        #endif
        if (i == -3)
        {
          buf[11] = 0xFF; //set flag for voice data received
          #ifdef writeBuff
		       bufsave[11] |= 0x01;
          #endif 
        }
      } //end of sync ok, packets processing
    }   //end of data block received
  }     //end of a portion of sampless processing

  //check we have received data and output buffer is empty for decoding
  if (0x0E & buf[11])
  {
    //decode voice data or set silency
    job += 16;       //set job
    if (1 & buf[11]) //this is a voice frame, decode it
    {

      melpe_s(sp, buf); //decode 81 bits in 11 bytes to 540 8KHz samples
    }
    else
    {
      memset(sp, 0, 1080); //or output 67.5 mS of silence
    }
    buf[11] = 0;           //clears flag: data buffer is processed
    //Write audio
    #ifdef writeBuff
      fwrite(bufsave, sizeof(char), 14, fbuf);
    #endif
    
    fwrite(sp, sizeof(short), 540, fout);

  }

  return 1;
}

