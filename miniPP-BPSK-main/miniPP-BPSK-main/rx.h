#include "define.h"
//control
int audio_init(void); //initialize audio devices
void audio_fin(void); //close audio devices
//runtime
#ifdef writeBuff
	int rx(int typing, FILE *ptr_in, FILE *ptr_out, FILE *prt_buf); //receiving	
#else
	int rx(int typing, FILE *ptr_in, FILE *ptr_out); //receiving
#endif

