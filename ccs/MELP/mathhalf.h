/*

2.4 kbps MELP Proposed Federal Standard speech coder

Fixed-point C code, version 1.0

Copyright (c) 1998, Texas Instruments, Inc.

Texas Instruments has intellectual property rights on the MELP
algorithm.	The Texas Instruments contact for licensing issues for
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

#ifndef _MATHHALF_H_
#define _MATHHALF_H_
#include "gsm.h"

/* addition */

//Shortword	add(Shortword var1, Shortword var2);                     /* 1 ops */

//Shortword	sub(Shortword var1, Shortword var2);                     /* 1 ops */

//Longword	L_add(Longword L_var1, Longword L_var2);                 /* 2 ops */

//Longword	L_sub(Longword L_var1, Longword L_var2);                 /* 2 ops */

/* multiplication */

//Shortword	mult(Shortword var1, Shortword var2);                    /* 1 ops */

//Longword	L_mult(Shortword var1, Shortword var2);                  /* 1 ops */

/* arithmetic shifts */

//Shortword	shr(Shortword var1, Shortword var2);                     /* 1 ops */

//Shortword	shl(Shortword var1, Shortword var2);                     /* 1 ops */

//Longword	L_shr(Longword L_var1, Shortword var2);                  /* 2 ops */

//Longword	L_shl(Longword L_var1, Shortword var2);                  /* 2 ops */

Shortword	shift_r(Shortword var, Shortword var2);                  /* 2 ops */

//Longword	L_shift_r(Longword L_var, Shortword var2);               /* 3 ops */

/* absolute value  */

//Shortword	abs_s(Shortword var1);                                   /* 1 ops */

//Longword	L_abs(Longword var1);                                    /* 3 ops */

/* multiply accumulate	*/

//Longword	L_mac(Longword L_var3, Shortword var1, Shortword var2);	  /* 1 op */

//Longword	L_msu(Longword L_var3, Shortword var1, Shortword var2);   /* 1 op */

//Shortword	msu_r(Longword L_var3, Shortword var1, Shortword var2);   /* 2 op */

/* negation  */

//Shortword	negate(Shortword var1);                                  /* 1 ops */

//Longword	L_negate(Longword L_var1);                               /* 2 ops */

/* Accumulator manipulation */

//Longword	L_deposit_l(Shortword var1);                             /* 1 ops */

//Longword	L_deposit_h(Shortword var1);                             /* 1 ops */

//Shortword	extract_l(Longword L_var1);                              /* 1 ops */

//Shortword	extract_h(Longword L_var1);                              /* 1 ops */

/* Round */

//Shortword	my_round(Longword L_var1);                                  /* 1 ops */

/* Normalization */

//Shortword	norm_l(Longword L_var1);                                /* 30 ops */

//Shortword	norm_s(Shortword var1);                                 /* 15 ops */

/* Division */

Shortword	divide_s(Shortword var1, Shortword var2);               /* 18 ops */

/* -------------------------------------------------------------------------- */
/* 40-Bit Routines....added by Andre 11/23/99 */

/* new 40 bits basic operators */

//Word40 L40_add(Word40 acc, Longword L_var1);

Word40 L40_sub(Word40 acc, Longword L_var1);
#pragma FUNC_ALWAYS_INLINE(L40_mac)
Word40 L40_mac(Word40 acc, Shortword var1, Shortword var2);

#pragma FUNC_ALWAYS_INLINE(L40_msu)
Word40 L40_msu(Word40 acc, Shortword var1, Shortword var2);

Word40 L40_shl(Word40 acc, Shortword var1);

Word40 L40_shr(Word40 acc, Shortword var1);

Word40 L40_negate(Word40 acc);

Shortword norm32(Word40 acc);

//Longword L_sat32(Word40 acc);



#endif

