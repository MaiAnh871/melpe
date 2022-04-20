// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017

///////////////////////////////////////////////////////////////////////
// Filename: StartUp.c
//
// Synopsis: Placeholder for code run after DSP_Init()
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"
#include "LCDK_Support_DSP.h"

void StartUp()
{
	Init_UART2(BAUD_RATE_UART);
	init_uart_buffer();
	while(!is_full_uart_buffer())
	{
	    /* Read and write data from uart */
	    uart_poolling();
	}
}
