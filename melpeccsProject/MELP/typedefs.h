/*

2.4 kbps MELP Proposed Federal Standard speech coder

Fixed-point C code, version 1.0

Copyright (c) 1998, Texas Instruments, Inc.  

Texas Instruments has intellectual property rights on the MELP
algorithm.  The Texas Instruments contact for licensing issues for
commercial and non-government use is William Gordon, Director,
Government Contracts, Texas Instruments Incorporated, Semiconductor
Group (phone 972 480 7442).

The fixed-point version of the voice codec Mixed Excitation Linear
Prediction (MELP) is based on specifications on the C-language software
simulation contained in GSM 06.06 which is protected by copyright and
is the property of the European Telecommunications Standards Institute
(ETSI). This standard is available from the ETSI publication office
tel. +33 (0)4 92 94 42 58. ETSI has granted a license to United States
Department of Defense to use the C-language software simulation contained
in GSM 06.06 for the purposes of the development of a fixed-point
version of the voice codec Mixed Excitation Linear Prediction (MELP).
Requests for authorization to make other use of the GSM 06.06 or
otherwise distribute or modify them need to be addressed to the ETSI
Secretariat fax: +33 493 65 47 16.

*/
/* typedef statements of types used in all routines */

#include <stdio.h>
#include <stdlib.h>

#ifndef __TYPEDEFS
#define __TYPEDEFS

#define LW_SIGN (long)0x80000000       /* sign bit */
#define LW_MIN (long)0x80000000
#define LW_MAX (long)0x7fffffff

#define SW_SIGN (short)0x8000          /* sign bit for Shortword type */
#define SW_MIN (short)0x8000           /* smallest Ram */
#define SW_MAX (short)0x7fff           /* largest Ram */

/* Definition of Types *
 ***********************/

typedef long int Longword;             /* 32 bit "accumulator" (L_*) */
typedef short int Shortword;           /* 16 bit "register"  (sw*) */
typedef short int ShortwordRom;        /* 16 bit ROM data    (sr*) */
typedef long int LongwordRom;          /* 32 bit ROM data    (L_r*)  */

typedef unsigned short UShortword;     /* 16 bit unsigned data */
typedef unsigned long ULongword;       /* 32 bit unsigned data */

struct NormSw {                        /* normalized Shortword fractional
                                        * number snr.man precedes snr.sh
                                        * (the shift count)i */
  Shortword man;                       /* "mantissa" stored in 16 bit
                                        * location */
  Shortword sh;                        /* the shift count, stored in 16 bit
                                        * location */
};


/* Global constants *
 ********************/


#define NP 10                          /* order of the lpc filter */
#define N_SUB 4                        /* number of subframes */
#define F_LEN 160                      /* number of samples in a frame */
#define S_LEN 40                       /* number of samples in a subframe */
#define A_LEN 170                      /* LPC analysis length */
#define OS_FCTR 6                      /* maximum LTP lag oversampling
                                        * factor */
#define OVERHANG 8                      /* comfort noise parameter:
					   number of frames averaged to 
					   CN generate reference */
#define OH_SHIFT 3                      /* shift corresponding to 
					   OVERHANG */
#define PN_INIT_SEED (Longword)0x1091988L /* initial seed for Comfort
					     noise pn-generator */

#define strStr strStr16

/* global variables */
/********************/

extern int giFrmCnt;                   /* 0,1,2,3,4..... */
extern int giSfrmCnt;                  /* 0,1,2,3 */
extern int giOverflow;
extern int giCarry;
extern int giRandomRF;


/* Primary comfort noise/DTX state variable */
extern Shortword gswDTXFrmCnt; /* encoders DTX frame counter */
extern Shortword gswLastDTXFrmCnt; /* gswDTXFrmCnt delayed by 1 frame */


/* !! upd !! PHILIPS #1*/
extern int giDTXon;   /* DTX Mode on/off */
/* !! end of update !! PHILIPS #1 */

extern int  giWriteFile;
extern FILE *gfpUtil;

#endif

