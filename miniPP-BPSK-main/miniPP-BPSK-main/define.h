#define mod8k             // IF NOT DEFINE, THE SAMPLING RATE OF MODULATOR IS 48KHz
#define demod8k           // IF NOT DEFINE, THE SAMPLING RATE OF DEMODULATOR IS 48KHz
#define noEncrypt           // IF DEFINE, ENCRYPT WILL BE REMOVED
#define writeBuff           // IF DEFINE, DATA WILL BE SAVE TO THE FILE
//#define writePower
//#define removeCuttedFrame
//#define newVAD
//#define cutHalfFrame

// #ifdef cutHalfFrame
//     #define BIT_IN_FRAME_CUT 32
// #else
//     #define BIT_IN_FRAME_CUT 0
// #endif 