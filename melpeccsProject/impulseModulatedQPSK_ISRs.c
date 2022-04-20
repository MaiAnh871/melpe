//// Welch, Wright, & Morrow,
//// Real-time Digital Signal Processing, 2017
//
/////////////////////////////////////////////////////////////////////////
//// Filename: impulseModulatedQPSK_ISRs.c
////
//// Synopsis: Interrupt service routine for codec data transmit/receive
////
/////////////////////////////////////////////////////////////////////////
//
//#include "DSP_Config.h"
//#include "coeff.h"   // load the filter coefficients, B[n] ... extern
//#include <stdlib.h>  // needed to call the rand() function
//
//// Data is received as 2 16-bit words (left/right) packed into one
//// 32-bit word.  The union allows the data to be accessed as a single
//// entity when transferring to and from the serial port, but still be
//// able to manipulate the left and right channels independently.
//
//#define LEFT  0
//#define RIGHT 1
//
//volatile union {
//	Uint32 UINT;
//	Int16 Channel[2];
//} CodecDataIn, CodecDataOut;
//
//
///* add any global variables here */
//Int32 counter = 0;
//Int32 carry = 0;
//#define QPSK_SCALE 160000
//const Int32 samplesPerSymbol = 20;
////const Int32 cosine[4] = {1, 0, -1, 0};
////const Int32 sine[4] = {0, 1, 0, -1};
//const float cosine[6] = {1,	0.5, -0.5, -1, -0.5, 0.5};
//const float sine[6] = {0,0.866025403784439,0.866025403784439,0,-0.866025403784439,-0.866025403784439};
//
//const float QPSK_LUT[4][2] = {
//// left (quadrature), right (in-phase)
//{     1 * QPSK_SCALE,  1 * QPSK_SCALE}, /* QPSK_LUT[0]  */
//{     1 * QPSK_SCALE, -1 * QPSK_SCALE}, /* QPSK_LUT[1]  */
//{    -1 * QPSK_SCALE,  1 * QPSK_SCALE}, /* QPSK_LUT[2]  */
//{    -1 * QPSK_SCALE, -1 * QPSK_SCALE}, /* QPSK_LUT[3]  */
//};
//
//float output_gain = 1.0;
//float xI[6];
//float xQ[6];
//float yI;
//float yQ;
//float output;
//
//
interrupt void Codec_ISR(){};
/////////////////////////////////////////////////////////////////////////
//// Purpose:   Codec interface interrupt service routine
////
//// Input:     None
////
//// Returns:   Nothing
////
//// Calls:     CheckForOverrun, ReadCodecData, WriteCodecData
////
//// Notes:     None
/////////////////////////////////////////////////////////////////////////
//{
////    if(CheckForOverrun())                   // overrun error occurred (i.e. halted DSP)
////        return;                             // so serial port is reset to recover
////
////    CodecDataIn.UINT = ReadCodecData();     // get input data samples
////
////    /* Using write_uart(demodulated byte) function when you have a demodulated byte */
////
////    /* Using read_uart_buffer() function for get input byte which will be modulated */
////
////    WriteCodecData(CodecDataOut.UINT);      // send output data to  port
//}
//
