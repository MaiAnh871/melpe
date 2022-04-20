#include "define.h"
//runtime
#ifdef writeBuff
	int tx(int job,FILE *ptr_in, FILE *ptr_out, FILE *ptr_buf);	
#else
	int tx(int job,FILE *ptr_in, FILE *ptr_out);
#endif

