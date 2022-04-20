#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <basetsd.h>
#include <stdint.h>

#include <windows.h>
#include <time.h>


#include "audio/audio.h" //low-level audio input/output
#include "modem/modem.h" //modem
#include "melpe/melpe.h" //audio codec
#include "vad/vad2.h"    //voice active detector
#include "define.h"

#include "crp.h" //data processing
#include "tx.h"  //this

vadState2 vad; //Voice Active Detector state

//recording 8 KHz speach from Mike
static short spraw[540];        //buffer for raw grabbed 8 KHz speach samples
static short spbuf[748];        //buffer for accumulate resampled voice up to melpe frame
static int spcnt = 0;           //number of accumulated samples
static unsigned char txbuf[14]; //buffer for encoded melpe frame or silency descryptor

//resampling
static float _up_pos = 1.0; //resamplers values
static short _left_sample = 0;

static long p = 0;

//playing 48 KHz baseband signal into Line
static short _jit_buf[3240];         //PCM 48KHz buffer for samples ready for playing into Line
static short *p__jit_buf = _jit_buf; //pointer to unplayed samples in the buffer
static short l__jit_buf = 0;         //number of unplayed samples in the buffer

//synchronizing grabbing and playing loops
static float _fdelay = 24000; //average recording delay
static int tgrab = 0;         //difference between pointers of recording and playing streams
#define Max_sample 500000

char buff[Max_sample], buff2[Max_sample];

int k;

//*****************************************************************************
//transmition loop: grab 8KHz speech samples from Mike,
//resample, collect frame (540 in 67.5 mS), encode
//encrypt, modulate, play 48KHz baseband signal into Line

//-phat -
// 2 con tro file input va output, tong so sample dua vao
int numf = 0;
#ifdef writeBuff
	int tx(int job, FILE *ptr_in, FILE *ptr_out, FILE *ptr_buf)
#else
	int tx(int job, FILE *ptr_in, FILE *ptr_out)
#endif

{
    int i, j;
    int a;
    int cutframe = 0;
    numf++;
    j = fread(spbuf, sizeof(short), 540, ptr_in);
    
    //  printf("\n\nFRAME %d\n",numf);
    if (j < 540)
    {
        printf("Done TX!\n");
        return 0;
    }

    //check for we have enough grabbed samples for processing
    i = vad2(spbuf + 10, &vad); //check frame is speech (by VAD)
    i += vad2(spbuf + 100, &vad);
    i += vad2(spbuf + 190, &vad);
    i += vad2(spbuf + 280, &vad);
    i += vad2(spbuf + 370, &vad)*2;
    i += vad2(spbuf + 410, &vad)*2;
    i += vad2(spbuf + 460, &vad)*4;
    txbuf[11] = 0xFF; //set defaults flag for voiced frame (voice frame auto have tfbuf[11] = 0xFF)


    
    if (i > 6) //frame is voices: compress it
    {
        melpe_a(txbuf, spbuf); //encode the speech frame
    }
    // VAD FRAME
    else //unvoiced frame: sync packet will be send
    {
        //printf("frame vad: %d\n",numf);
        txbuf[11] = 0xFE; //or set silence flag for control blocks
    }
    spcnt -= 540; //samples rest

        // ENCRYPT DATA

        //MakePkt(txbuf); //encrypt voice or get actual control packet
        a = MakePkt(txbuf); //a = -3 for voice or cnt_out for unvoice
        if (a == -3)
            txbuf[11] = 0xFE;
        else
            txbuf[11] = 0xFF;

        // Print


        txbuf[12] = (char)(numf & 0xFF);
        txbuf[13] = (char)((numf >> 8) & 0xFF);

        // MODULATION BLOCK
        #ifdef writeBuff
        fwrite(txbuf, sizeof(char), 14, ptr_buf);

        #endif
        l__jit_buf = Modulate(txbuf, _jit_buf); //modulate block
        txbuf[11] = 0;                          //clear tx buffer (processed)

        // WRITE FINAL DATA TO FILE BIN    
        fwrite(_jit_buf, sizeof(short), l__jit_buf, ptr_out); //8  KHz
        job += 128;
    //preventing of freezing audio output after underrun or overrun

    //check for delay is acceptable for playing next portion of samples

    return job;
}