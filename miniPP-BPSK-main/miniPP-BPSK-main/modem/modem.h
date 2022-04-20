#include "..\define.h"

int Modulate(unsigned char* data, short* frame);
#ifdef demod8k
		int Demodulate_8k( short* frame, unsigned char* data);
#else
	int Demodulate_48k( short* frame, unsigned char* data2);
#endif




