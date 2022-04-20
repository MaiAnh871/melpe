// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017

///////////////////////////////////////////////////////////////////////
// Filename: LCDK_Support_DSP.c
//
// Synopsis: Functions to support basic initialization of the OMAP-L138
//           hardware resources.
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"
#include "AIC3106.h"


static float SampleFreq = 0.0F;

///////////////////////////////////////////////////////////////////////
// Purpose:   Returns the current sample frequency as determined by the 
//            Init_OMAPL138 and SetSampleRate_AIC3106 functions.
//
// Input:     None
//
// Returns:   Sample frequency in floating point format
//
// Calls:     None
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
float GetSampleFreq()
{
	return SampleFreq;
}

///////////////////////////////////////////////////////////////////////
// Purpose:   Initializes OMAP-L138 DSP and codec  
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Init_Interrupts, Init_I2C, Init_AIC3106, Init_McASP0, EnableInterrupts   
//
// Notes:     SampleRateSetting is define in OMAPL138_Config.h
///////////////////////////////////////////////////////////////////////
void DSP_Init()
{
	Init_Interrupts();
	Init_I2C();
	if(!Init_AIC3106(SampleRateSetting)) {
		while(1);	// stall on codec init failure
	}
	Init_McASP0();
	EnableInterrupts();
}

///////////////////////////////////////////////////////////////////////
// Purpose:   Initializes OMAP-L138 DSP and codec for EDMA use 
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Init_Interrupts_EDMA, Init_I2C, Init_AIC3106, Init_McASP0, EnableInterrupts_EDMA   
//
// Notes:     SampleRateSetting is define in OMAPL138_Config.h
///////////////////////////////////////////////////////////////////////
void DSP_Init_EDMA()
{
	Init_Interrupts_EDMA();
	Init_I2C();
	if(!Init_AIC3106(SampleRateSetting)) {
		while(1);	// stall on codec init failure
	}
	Init_McASP0();
	EnableInterrupts_EDMA();
}

Int32 InitLEDs()
///////////////////////////////////////////////////////////////////////
// Purpose:   Initializes GPIO to drive user LEDs
//
// Input:     None
//
// Returns:   1 always
//
// Calls:     Nothing
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	GPIO_CLR_DATA01 = 0x00000200;	// clear GP0[9]
	GPIO_DIR01 &= ~0x00000200;		// GP0[9] to outputs
	GPIO_CLR_DATA23 = 0x00001000;	// clear GP2[12]
	GPIO_DIR23 &= ~0x00001000;		// GP2[12] to outputs
	GPIO_CLR_DATA67 = 0x00003000;	// clear GP6[13:12]
	GPIO_DIR67 &= ~0x00003000;		// GP6[13:12] to outputs

	return 1;
}

Uint32 WriteLEDs(Uint8 led_bits)
///////////////////////////////////////////////////////////////////////
// Purpose:   Writes to user LED1/2 on board  
//
// Input:     None
//
// Returns:   1 always
//
// Calls:     Nothing
//
// Notes:     Must call InitLEDs before calling this function
///////////////////////////////////////////////////////////////////////
{
	if(led_bits & USER_LED1)
		GPIO_SET_DATA67 = 0x00002000;	// set GP6[13]
	else
		GPIO_CLR_DATA67 = 0x00002000;	// clear GP6[13]
	if(led_bits & USER_LED2)
		GPIO_SET_DATA67 = 0x00001000;	// set GP6[12]
	else
		GPIO_CLR_DATA67 = 0x00001000;	// clear GP6[12]
	if(led_bits & USER_LED3)
		GPIO_SET_DATA23 = 0x00001000;	// set GP2[12]
	else
		GPIO_CLR_DATA23 = 0x00001000;	// clear GP2[12]
	if(led_bits & USER_LED4)
		GPIO_SET_DATA01 = 0x00000200;	// set GP0[9]
	else
		GPIO_CLR_DATA01 = 0x00000200;	// clear GP0[9]

    return 1;
}

Int32  ReadSwitches()
///////////////////////////////////////////////////////////////////////
// Purpose:   Reads user DIP switches  
//
// Input:     None
//
// Returns:   Switch state
//
// Calls:     Nnthing
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	return (Int32)((GPIO_IN_DATA01 >> 1) & 0xF);
}

void InitDigitalOutputs()
///////////////////////////////////////////////////////////////////////
// Purpose:   Initializes digital output pins on board  
//            On the LCDK, the user LEDs are used for this
//            purpose since they are directly written to
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	WriteLEDs(0);
}

void WriteDigitalOutputs(Uint8 data)
///////////////////////////////////////////////////////////////////////
// Purpose:   Writes to digital output pins on board  
//            On the LCDK, the user LEDs are used for this
//            purpose since they are directly written to
//
// Input:     data - value written to outputs
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	WriteLEDs(data);
}

void Init_Interrupts()
///////////////////////////////////////////////////////////////////////
// Purpose:   Initializes interrupts  
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	INTCx *intc = INTC0_Base;

//	ISTP = 0x80000000;					// set vector table location shared RAM
	ISTP = 0x11800000;					// set vector table location DSP RAM
	intc->intmux3 = 0x0F0E0D00 | 61; 	// assign McASP0 irq to INT12
}

void EnableInterrupts()
///////////////////////////////////////////////////////////////////////
// Purpose:   Enables McASP interrupt  
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
  	IER |= 0x1002;  					// enable McASP interrupt (12)
  	ICR = 0xffff;       				// clear all pending interrupts
 	CSR |= 1;           				// set GIE
}
void Init_Interrupts_EDMA()
///////////////////////////////////////////////////////////////////////
// Purpose:   Initializes interrupts for EDMA use 
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
//	ISTP = 0x80000000;					// set vector table location shared RAM
	ISTP = 0x11800000;					// set vector table location DSP RAM
	// EDMA3_CC0_INT1 irq sent to INT8 by default
}

void EnableInterrupts_EDMA()
///////////////////////////////////////////////////////////////////////
// Purpose:   Enables EDMA3_CC0_INT1 interrupt  
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
  	IER |= 0x0102;  					// enable EDMA3_CC0_INT1 interrupt (8)
  	ICR = 0xffff;       				// clear all pending interrupts
 	CSR |= 1;           				// set GIE
}

void wait(Uint32 delay)
///////////////////////////////////////////////////////////////////////
// Purpose:   Simple software delay loop  
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
    volatile Uint32 i;

    for (i = 0;i < delay;i++)
    	;
}


#define I2C_timeout 	100000		// iteration limit for I2C 

void Init_I2C()
///////////////////////////////////////////////////////////////////////
// Purpose:   Initializes I2C controller 
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	I2C *i2c = I2C0_Base;

    i2c->cmdr  = 0;				// into reset   
    i2c->psc   = 7;				// clock in is 300/4=75MHz, prescaled to 75/8=9.375MHz
    i2c->clkl  = 37;			// config for 100kHz I2C clock
    i2c->clkh  = 37;
    i2c->cmdr  |= ICCMDR_IRS;	// out of reset
}

void Reset_I2C()
///////////////////////////////////////////////////////////////////////
// Purpose:   Resets I2C controller 
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     None
{
	I2C *i2c = I2C0_Base;

 	i2c->cmdr = 0;
}


Uint32 Write_I2C(Uint16 addr, Uint8* pdata, Uint16 num_bytes)
///////////////////////////////////////////////////////////////////////
// Purpose:   Write to device on I2C link 
//
// Input:     addr - I2C address to write to
//            pdata - pointer to data to write
//            num_bytes - number of bytes of data to write
//
// Returns:   Nothing
//
// Calls:     wait, Reset_I2C 
//
// Notes:     None
{
    volatile Int32 timeout, i;
   	I2C *i2c = I2C0_Base;

 	i2c->cnt = num_bytes;           
    i2c->sar = addr; 
	i2c->cmdr = ICCMDR_STT | ICCMDR_TRX | ICCMDR_MST | ICCMDR_IRS | ICCMDR_FREE;
	wait(100);
	for (i = 0;i < num_bytes;i++) {
		i2c->dxr = pdata[i];			// write data
		timeout = I2C_timeout;
        while((i2c->str & ICSTR_ICXRDY) == 0) {
			if(timeout-- < 0) {
				Reset_I2C();
				return 0;
			}
		}
	}
	i2c->cmdr |= ICCMDR_STP;			// force STOP
	return i;
}
Uint32 ReadCodecData()
///////////////////////////////////////////////////////////////////////
// Purpose:   Read McASP receive data
//
// Input:     None
//
// Returns:   Data from McASP
//
// Calls:     Nothing 
//
// Notes:     Assumes data is ready
///////////////////////////////////////////////////////////////////////
{
	McASP *port = McASP0_Base;
	
	return port->rbuf[14];
}

void WriteCodecData(Uint32 data)
///////////////////////////////////////////////////////////////////////
// Purpose:   Write McASP transmit data
//
// Input:     data - data to write to McASP
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     Assumes transmitter is ready
///////////////////////////////////////////////////////////////////////
{
	McASP *port = McASP0_Base;
	
	port->xbuf[13] = data;
}

Uint32 CheckForOverrun()
///////////////////////////////////////////////////////////////////////
// Purpose:   Check if a McASP overrun occurred and re-init McASP if so
//
// Input:     None
//
// Returns:   1 if a McASP re-initt occurred, 0 otherwise
//
// Calls:     Init_McASP0 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	McASP *port = McASP0_Base;
	
 	if(port->rstat & 1)	{					// receiver overrun error occurred (i.e. halted DSP)
		Init_McASP0();						// so reinitialize the McASP to recover
		return 1;
	}
	return 0;
}


Uint32 Init_AIC3106(Uint8 nFs)
///////////////////////////////////////////////////////////////////////
// Purpose:   Performs initialization of the OMAP-L138's onboard codec
//
// Input:     nFs - selects sample rate to use
//
// Returns:   Nothing
//
// Calls:     Reset_AIC3106, SetSampleRate_AIC3106, AIC3106_write_reg 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
    if(!Reset_AIC3106())    
   		return 0;
	if(!SetSampleRate_AIC3106(nFs))
		return 0;
    if(!AIC3106_write_reg(AIC3106_REG0_CODEC_DATAPATH, 0x0A))  		// l/r DACs set to l/r
		return 0;
    if(!AIC3106_write_reg(AIC3106_REG0_SERIAL_DATA_INTFC_A, 0xF0))	// codec is master
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_SERIAL_DATA_INTFC_B, 0x40))	// DSP mode, 16-bit
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_SERIAL_DATA_INTFC_C, 0x00))	// no offset
		return 0;

#if CodecType == LCDK_LineInput
	if(!AIC3106_write_reg(AIC3106_REG0_LEFT_ADC_PGA, 0x00))			// PGA gain=0db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_RIGHT_ADC_PGA, 0x00))		// PGA gain=0db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_LINE1L_LEFT_ADC, 0x04))		// left SE, 0db, ADC powered-up
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_LINE1R_RIGHT_ADC, 0x04))		// right SE, 0db, ADC powered-up
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_MIC3LR_LEFT_ADC, 0xFF))		// mic input left off
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_MIC3LR_RIGHT_ADC, 0xFF))		// mic input right off
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_MICBIAS, 0x00))				// mic bias off
		return 0;
#elif CodecType == LCDK_MicInput_0db
	if(!AIC3106_write_reg(AIC3106_REG0_LEFT_ADC_PGA, 0x00))			// PGA gain=0db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_RIGHT_ADC_PGA, 0x00))		// PGA gain=0db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_LINE1L_LEFT_ADC, 0x7C))		// input 1 off, ADC powered up
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_LINE1R_RIGHT_ADC, 0x7C))		// input 1 off, ADC powered up
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_MIC3LR_LEFT_ADC, 0x0F))		// mic input left 0dB
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_MIC3LR_RIGHT_ADC, 0xF0))		// mic input right 0dB
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_MICBIAS, 0x80))				// mic bias to 2.5V
		return 0;
#elif CodecType == LCDK_MicInput_20db
	if(!AIC3106_write_reg(AIC3106_REG0_LEFT_ADC_PGA, 0x28))			// PGA gain=20db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_RIGHT_ADC_PGA, 0x28))		// PGA gain=20db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_LINE1L_LEFT_ADC, 0x7C))		// input 1 off, ADC powered up
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_LINE1R_RIGHT_ADC, 0x7C))		// input 1 off, ADC powered up
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_MIC3LR_LEFT_ADC, 0x0F))		// mic input left 0dB
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_MIC3LR_RIGHT_ADC, 0xF0))		// mic input right 0dB
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_MICBIAS, 0x80))				// mic bias to 2.5V
		return 0;
#else
#error Invalid CodecType set - check DSP_Config.h
#endif

	if(!AIC3106_write_reg(AIC3106_REG0_LEFT_AGC_CNTL_A, 0x00))		// agc disabled
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_RIGHT_AGC_CNTL_A, 0x00))		// agc disabled
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_DAC_POWER_OUTPUT_DRVR, 0xE0))// l/r DACs on, HPLCOM single-ended
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_HI_POWER_OUTPUT_DRVR, 0x10))	// HPRCOM single-ended
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_LEFT_DAC_VOLUME, 0x00))		// gain=0db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_RIGHT_DAC_VOLUME, 0x00))		// gain=0db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_DAC_L1_TO_HPLOUT, 0x80))		// DAC_L1 is routed to HPLOUT
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_HPLOUT_OUTPUT_LEVEL, 0x09))	// HPLOUT on
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_HPLCOM_OUTPUT_LEVEL, 0x00))	// 0db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_DAC_R1_TO_HPROUT, 0x80))		// DAC_R1 is routed to HPROUT
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_HPROUT_OUTPUT_LEVEL, 0x09))	// HPROUT on
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_HPRCOM_OUTPUT_LEVEL, 0x00))	// 0db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_DAC_L1_TO_LEFT_LOP, 0x80))	// DAC_L1 is routed to LEFT_LOP/M
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_LEFT_LOP_OUTPUT_LEVEL, 0x09))// 0db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_DAC_R1_TO_RIGHT_LOP, 0x80))	// DAC_R1 is routed to RIGHT_LOP/M
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_RIGHT_LOP_OUTPUT_LEVEL, 0x09))// 0db
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_ADDNL_GPIO_CONTROL_B, 0x01))	// CODEC_CLKIN uses CLKDIV_OUT
		return 0;
	if(!AIC3106_write_reg(AIC3106_REG0_CLOCK_GEN_CONTROL, 0x02))	// CLKDIV_IN/PLLCLK_IN uses MCLK, /2
		return 0;
	return 1;
}

#define AIC3106_I2C_ADDR          0x18   

Uint32 AIC3106_write_reg(Uint8 address, Uint8 data)
///////////////////////////////////////////////////////////////////////
// Purpose:   Writes to AIC3106 reg on I2C control link
//
// Input:     address - register address
//	          data - data to write to register
//
// Returns:   Nothing
//
// Calls:     Write_I2C 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
    Uint8 i2c_data[2];
    i2c_data[0] = address & 0x7F; 		// register address
    i2c_data[1] = data;					// data

    return Write_I2C(AIC3106_I2C_ADDR,i2c_data, 2);
}

Uint32 Reset_AIC3106()
///////////////////////////////////////////////////////////////////////
// Purpose:   Reset AIC3106 codec
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     AIC3106_write_reg 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{ 
    if(!AIC3106_write_reg(AIC3106_REG0_PAGESELECT, 0 ))				 // page 0
		return 0;
    if(!AIC3106_write_reg(AIC3106_REG0_RESET, 0x80 ))				// software reset
		return 0;
	return 1;
}

Uint32 SetSampleRate_AIC3106(Uint8 nFs)
///////////////////////////////////////////////////////////////////////
// Purpose:   Sets AIC3106 sample rate to one of the enumerated values
//
// Input:     nFs - selects sample rate to use
//
// Returns:   Nothing
//
// Calls:     AIC3106_write_reg 
//
// Notes:     Update SampleFreq variable with new Fs
///////////////////////////////////////////////////////////////////////
{
	Uint8	csr, pll_a;

	switch(nFs) {
	case AIC3106Fs96kHz:
		pll_a = REG_PLL_A_96KHZ;
		csr = REG_CODEC_SAMPLE_RATE_96KHZ;
		SampleFreq = 96000.0F;
		break;
	case AIC3106Fs32kHz:
		pll_a = REG_PLL_A_32KHZ;
		csr = REG_CODEC_SAMPLE_RATE_32KHZ;
		SampleFreq = 32000.0F;
		break;
	case AIC3106Fs24kHz:
		pll_a = REG_PLL_A_24KHZ;
		csr = REG_CODEC_SAMPLE_RATE_24KHZ;
		SampleFreq = 24000.0F;
		break;
	case AIC3106Fs16kHz:
		pll_a = REG_PLL_A_16KHZ;
		csr = REG_CODEC_SAMPLE_RATE_16KHZ;
		SampleFreq = 16000.0F;
		break;
	case AIC3106Fs12kHz:
		pll_a = REG_PLL_A_12KHZ;
		csr = REG_CODEC_SAMPLE_RATE_12KHZ;
		SampleFreq = 12000.0F;
		break;
	case AIC3106Fs8kHz:
		pll_a = REG_PLL_A_8KHZ;
		csr = REG_CODEC_SAMPLE_RATE_8KHZ;
		SampleFreq = 8000.0F;
		break;
	//case AIC3106Fs48kHz:
	default:
		pll_a = REG_PLL_A_48KHZ;
		csr = REG_CODEC_SAMPLE_RATE_48KHZ;
		SampleFreq = 48000.0F;
		break;
	}
    if(!AIC3106_write_reg(AIC3106_REG0_CODEC_SAMPLE_RATE, csr)) // set N(ADC/DAC)
		return 0;
    if(!AIC3106_write_reg(AIC3106_REG0_PLL_A, pll_a)) 		// set Fs(ref)
		return 0;
	return 1;
}

void Init_McASP0()	
///////////////////////////////////////////////////////////////////////
// Purpose:   Configures McASP to operate with AIC3106 acting as master
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	McASP* mcasp = McASP0_Base;

	// reset McASP
    mcasp->gblctl  = 0;       
    mcasp->rgblctl = 0;   
    mcasp->xgblctl = 0;  
    mcasp->srctl[13] = 0;
    mcasp->srctl[14] = 0;

	wait(10000);

    // receiver configuration
    mcasp->rmask      = 0xffffffff; 	// no padding used
    mcasp->rfmt       = 0x000080F8; 	// MSB 32bit, 0-delay, no pad, CFGBus
    mcasp->afsrctl    = 0x00000000; 	// burst, 1bit, ext fs,  rising
    mcasp->aclkrctl   = 0x00000000; 	// bit clock from tx side
    mcasp->ahclkrctl  = 0x00000000; 	// ext clock (from tx side)
    mcasp->rtdm       = 0x00000001; 	// Slot 0
    mcasp->rintctl    = 0x00000001; 	// overrun interrupt only
    mcasp->rclkchk    = 0x00FF0008; 	// 255-MAX 0-MIN, div-by-256

    // transmitter configuration
    mcasp->xmask      = 0xffffffff; 	// no padding used
    mcasp->xfmt       = 0x000080F8; 	// MSB 32bit, 0-delay, no pad, CFGBus
    mcasp->afsxctl    = 0x00000000; 	// burst, 1bit, ext fs,  rising
    mcasp->aclkxctl   = 0x00000000; 	// sync rx clocks
    mcasp->ahclkxctl  = 0x00000000; 	// ext clk
    mcasp->xtdm       = 0x00000001; 	// slot 0
    mcasp->xintctl    = 0x00000020; 	// data interrupts
    mcasp->xclkchk    = 0x00FF0008; 	// 255-MAX 0-MIN, div-by-256

    // serializer configuration
    mcasp->srctl[13]  = 0x0000000D;   	// axr1[13] output
    mcasp->srctl[14]  = 0x0000000E;   	// axr1[14] input
    mcasp->pfunc      = 0x00000000;   	// use as mcasp
    mcasp->pdir       = 0x00002000; 	// inputs except axr1[13]

    mcasp->ditctl     = 0x00000000; 	// not used
    mcasp->dlbctl     = 0x00000000; 	// not used
    mcasp->amute      = 0x00000000; 	// not used

    // start clocks
//    mcasp->rgblctl |= GBLCTL_RHCLKRST;                         
//    while(!(mcasp->rgblctl & GBLCTL_RHCLKRST));
//    mcasp->xgblctl |= GBLCTL_XHCLKRST;  
//    while(!(mcasp->xgblctl & GBLCTL_XHCLKRST));
    mcasp->rgblctl |= GBLCTL_RCLKRST;  
    while(!(mcasp->rgblctl & GBLCTL_RCLKRST));
	mcasp->xgblctl |= GBLCTL_XCLKRST;                     
    while(!(mcasp->xgblctl & GBLCTL_XCLKRST));

	// clear any pending errors                    
    mcasp->xstat = 0x0000FFFF;        
    mcasp->rstat = 0x0000FFFF;      

	// clear serializers
    mcasp->rgblctl |= GBLCTL_RSRCLR;
    while(!(mcasp->rgblctl & GBLCTL_RSRCLR));
    mcasp->xgblctl |= GBLCTL_XSRCLR;
    while(!(mcasp->xgblctl & GBLCTL_XSRCLR));

	// state machine resets
	mcasp->rgblctl |= GBLCTL_RSMRST;
    while(!(mcasp->rgblctl & GBLCTL_RSMRST));
    mcasp->xgblctl |= GBLCTL_XSMRST;
    while(!(mcasp->xgblctl & GBLCTL_XSMRST));

	// prevent output underun
	mcasp->xbuf[13] = 0;

	// start frame syncs
    mcasp->rgblctl |= GBLCTL_RFRST;
    while(!(mcasp->rgblctl & GBLCTL_RFRST));
    mcasp->xgblctl |= GBLCTL_XFRST;
    while(!(mcasp->xgblctl & GBLCTL_XFRST));
}

#define UART_CLOCK  	150000000	// uart clock is 300MHz/2

void Init_UART2(Uint32 baud_rate)
///////////////////////////////////////////////////////////////////////
// Purpose:   Configures UART2 to operate at the specified baud rate
//
// Input:     baud_rate - desired baud rate
//
// Returns:   Nothing
//
// Calls:     wait 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	Uint32 divisor;
	volatile Uint8 dummy;
	UART* pUART2 =  UART2_Base;

	// divisor is UART_CLOCK/(baud_rate * 16) (16x oversampling)
	// divisor is rounded by adding baud_rate*8 before division
	divisor = (UART_CLOCK + (baud_rate << 3)) / (baud_rate << 4);

	pUART2->pwremu_mgmt = 0;				// reset UART
	wait(500);

	pUART2->dll = (divisor & 0xff);			// baud rate divisor
	pUART2->dlh = (divisor >> 8);
	pUART2->iir_fcr = 0x0000;				// disable FIFOs
	pUART2->ier = 0x0000;					// disable interrupts
	pUART2->lcr = 0x0003;					// 8-N-1
	pUART2->mcr = 0x0000;					// no flow control, no loopback
	pUART2->mdr	= 0;						// use 16x oversampling

	pUART2->pwremu_mgmt = 0x6001;    		// tx/rx enabled

	dummy = pUART2->rbr_thr;				// flush rx data
}

void Write_UART2(Uint8 c)
///////////////////////////////////////////////////////////////////////
// Purpose:   Writes a byte to the UART2 transmitter
//
// Input:     c - 8-bit data to write
//
// Returns:   Nothing
//
// Calls:     Nothing 
//
// Notes:     Check if transmitter ready before writing
///////////////////////////////////////////////////////////////////////
{
	UART* pUART2 =  UART2_Base;

	pUART2->rbr_thr = c;
}

void Puts_UART2(char *s)
///////////////////////////////////////////////////////////////////////
// Purpose:   Writes a null-terminated string to the UART2 transmitter
//
// Input:     s - pointer to string
//
// Returns:   Nothing
//
// Calls:     IsTxReady_UART2, Write_UART2 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	while(*s) {								// loop until null terminator
		while(!IsTxReady_UART2())			// wait for transmitter to be ready
			;
		Write_UART2(*s++);					// send next charactor
	}
}
	
Uint8 Read_UART2()
///////////////////////////////////////////////////////////////////////
// Purpose:   Reads a byte to the UART2 receiver
//
// Input:     None
//
// Returns:   Data read
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	UART* pUART2 =  UART2_Base;

	return pUART2->rbr_thr;
}

Uint8 IsDataReady_UART2()
///////////////////////////////////////////////////////////////////////
// Purpose:   Checks if the UART2 receiver has data available
//
// Input:     None
//
// Returns:   Non-zero if data is ready
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	UART* pUART2 =  UART2_Base;

	return pUART2->lsr & UART_LSR_DR;
}

Uint8 IsTxReady_UART2()
///////////////////////////////////////////////////////////////////////
// Purpose:   Checks if the UART2 transmitter is ready for more data
//
// Input:     None
//
// Returns:   Non-zero if transmitter is ready for data
//
// Calls:     Nothing 
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	UART* pUART2 =  UART2_Base;

	return pUART2->lsr & UART_LSR_THRE;
}



