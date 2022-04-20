// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017
 
///////////////////////////////////////////////////////////////////////
// Filename: OMAPL138_defines.h
//
// Synopsis: Register definitions
//
///////////////////////////////////////////////////////////////////////

#ifndef OMAPL138_DEFINES_H_INCLUDED
#define OMAPL138_DEFINES_H_INCLUDED

#include "tistdtypes.h"

//  AEMIF Controller   
#define AEMIF_BASE              0x68000000
#define AEMIF_AWCCR             *( volatile Uint32* )( AEMIF_BASE + 0x04 )
#define AEMIF_A1CR              *( volatile Uint32* )( AEMIF_BASE + 0x10 )
#define AEMIF_A2CR              *( volatile Uint32* )( AEMIF_BASE + 0x14 )
#define AEMIF_A3CR              *( volatile Uint32* )( AEMIF_BASE + 0x18 )
#define AEMIF_A4CR              *( volatile Uint32* )( AEMIF_BASE + 0x1C )
#define AEMIF_EIRR              *( volatile Uint32* )( AEMIF_BASE + 0x40 )
#define AEMIF_EIMR              *( volatile Uint32* )( AEMIF_BASE + 0x44 )
#define AEMIF_EIMSR             *( volatile Uint32* )( AEMIF_BASE + 0x48 )
#define AEMIF_EIMCR             *( volatile Uint32* )( AEMIF_BASE + 0x4C )
#define AEMIF_NANDFCR           *( volatile Uint32* )( AEMIF_BASE + 0x60 )
#define AEMIF_NANDFSR           *( volatile Uint32* )( AEMIF_BASE + 0x64 )
#define AEMIF_NANDECC2          *( volatile Uint32* )( AEMIF_BASE + 0x70 )
#define AEMIF_NANDECC3          *( volatile Uint32* )( AEMIF_BASE + 0x74 )
#define AEMIF_NANDECC4          *( volatile Uint32* )( AEMIF_BASE + 0x78 )
#define AEMIF_NANDECC5          *( volatile Uint32* )( AEMIF_BASE + 0x7C )

#define AEMIF_MAX_TIMEOUT_8BIT  0x3FFFFFFC
#define AEMIF_MAX_TIMEOUT_16BIT 0x3FFFFFFD

#define EMIF_CS2                2
#define EMIF_CS3                3
#define EMIF_CS4                4
#define EMIF_CS5                5

#define EMIF_CS0_BASE           0x02000000
#define EMIF_CS1_BASE           0x04000000

#define EMIF_NAND_MODE          1
#define EMIF_NORMAL_MODE        0

//  Device System Controller
#define SYS_BASE            0x01C14000
#define REVID               *(unsigned int*)(SYS_BASE + 0x000)  
#define DIEIDR0             *(unsigned int*)(SYS_BASE + 0x008)
#define DIEIDR1             *(unsigned int*)(SYS_BASE + 0x00C)
#define DIEIDR2             *(unsigned int*)(SYS_BASE + 0x010)
#define DIEIDR3             *(unsigned int*)(SYS_BASE + 0x014)
#define DEVIDR0             *(unsigned int*)(SYS_BASE + 0x018)
#define DEVIDR1             *(unsigned int*)(SYS_BASE + 0x01C)
#define BOOTCFG             *(unsigned int*)(SYS_BASE + 0x020)
#define CHIPREVIDR          *(unsigned int*)(SYS_BASE + 0x024)
#define KICK0R              *(unsigned int*)(SYS_BASE + 0x038)  
#define KICK1R              *(unsigned int*)(SYS_BASE + 0x03c)  
#define HOST0CFG            *(unsigned int*)(SYS_BASE + 0x040)
#define HOST1CFG            *(unsigned int*)(SYS_BASE + 0x044)
#define IRAWSTAT            *(unsigned int*)(SYS_BASE + 0x0E0)  
#define IENSTAT             *(unsigned int*)(SYS_BASE + 0x0E4)  
#define IENSET              *(unsigned int*)(SYS_BASE + 0x0E8)  
#define IENCLR              *(unsigned int*)(SYS_BASE + 0x0EC)  
#define EOI                 *(unsigned int*)(SYS_BASE + 0x0F0)  
#define FLTADDRR            *(unsigned int*)(SYS_BASE + 0x0F4)  
#define FLTSTAT             *(unsigned int*)(SYS_BASE + 0x0F8)  
#define MSTPRI0             *(unsigned int*)(SYS_BASE + 0x110)  
#define MSTPRI1             *(unsigned int*)(SYS_BASE + 0x114)  
#define MSTPRI2             *(unsigned int*)(SYS_BASE + 0x118) 
#define PINMUX0				*(unsigned int*)(SYS_BASE + 0x120)
#define PINMUX1				*(unsigned int*)(SYS_BASE + 0x124)
#define PINMUX2				*(unsigned int*)(SYS_BASE + 0x128)
#define PINMUX3				*(unsigned int*)(SYS_BASE + 0x12C)
#define PINMUX4				*(unsigned int*)(SYS_BASE + 0x130)
#define PINMUX5				*(unsigned int*)(SYS_BASE + 0x134)
#define PINMUX6				*(unsigned int*)(SYS_BASE + 0x138)
#define PINMUX7				*(unsigned int*)(SYS_BASE + 0x13C)
#define PINMUX8				*(unsigned int*)(SYS_BASE + 0x140)
#define PINMUX9				*(unsigned int*)(SYS_BASE + 0x144)
#define PINMUX10			*(unsigned int*)(SYS_BASE + 0x148)
#define PINMUX11			*(unsigned int*)(SYS_BASE + 0x14C)
#define PINMUX12			*(unsigned int*)(SYS_BASE + 0x150)
#define PINMUX13			*(unsigned int*)(SYS_BASE + 0x154)
#define PINMUX14			*(unsigned int*)(SYS_BASE + 0x158)
#define PINMUX15			*(unsigned int*)(SYS_BASE + 0x15C)
#define PINMUX16			*(unsigned int*)(SYS_BASE + 0x160)
#define PINMUX17			*(unsigned int*)(SYS_BASE + 0x164)
#define PINMUX18			*(unsigned int*)(SYS_BASE + 0x168)
#define PINMUX19			*(unsigned int*)(SYS_BASE + 0x16C)
#define SUSPSRC		 	    *(unsigned int*)(SYS_BASE + 0x170) 
#define CHIPSIG	            *(unsigned int*)(SYS_BASE + 0x174) 
#define CHIPSIG_CLR         *(unsigned int*)(SYS_BASE + 0x178) 
#define CFGCHIP0            *(unsigned int*)(SYS_BASE + 0x17C) 
#define CFGCHIP1            *(unsigned int*)(SYS_BASE + 0x180) 
#define CFGCHIP2            *(unsigned int*)(SYS_BASE + 0x184) 
#define CFGCHIP3           	*(unsigned int*)(SYS_BASE + 0x188) 
#define CFGCHIP4		 	*(unsigned int*)(SYS_BASE + 0x18C) 

// GPIO         
#define GPIO_BASE               0x01E26000

#define GPIO_PCR                *( volatile Uint32* )( GPIO_BASE + 0x04 )
#define GPIO_BINTEN             *( volatile Uint32* )( GPIO_BASE + 0x08 )

// GPIO
#define GPIO_DIR_BASE           ( 0x10 )   // Direction Cntl
#define GPIO_OUT_DATA_BASE      ( 0x14 )   // Output data
#define GPIO_SET_DATA_BASE      ( 0x18 )   // Set data
#define GPIO_CLR_DATA_BASE      ( 0x1C )   // Clear data
#define GPIO_IN_DATA_BASE       ( 0x20 )   // Input data
#define GPIO_SET_RIS_TRIG_BASE  ( 0x24 )   // Set rising edge intr
#define GPIO_CLR_RIS_TRIG_BASE  ( 0x28 )   // Clear rising edge intr
#define GPIO_SET_FAL_TRIG_BASE  ( 0x2C )   // Set falling edge intr
#define GPIO_CLR_FAL_TRIG_BASE  ( 0x30 )   // Clear falling edge intr
#define GPIO_INSTAT_BASE        ( 0x34 )   // Intr status
#define GPIO_BASE_OFFSET        0x28

#define GPIO_01_BASE            ( GPIO_BASE + 0x10 )
#define GPIO_23_BASE            ( GPIO_01_BASE + GPIO_BASE_OFFSET )
#define GPIO_45_BASE            ( GPIO_23_BASE + GPIO_BASE_OFFSET )
#define GPIO_67_BASE            ( GPIO_45_BASE + GPIO_BASE_OFFSET )

// GPIO01
#define GPIO_DIR01              *( volatile Uint32* )( GPIO_BASE + 0x10 )
#define GPIO_OUT_DATA01         *( volatile Uint32* )( GPIO_BASE + 0x14 )
#define GPIO_SET_DATA01         *( volatile Uint32* )( GPIO_BASE + 0x18 )
#define GPIO_CLR_DATA01         *( volatile Uint32* )( GPIO_BASE + 0x1C )
#define GPIO_IN_DATA01          *( volatile Uint32* )( GPIO_BASE + 0x20 )
#define GPIO_SET_RIS_TRIG01     *( volatile Uint32* )( GPIO_BASE + 0x24 )
#define GPIO_CLR_RIS_TRIG01     *( volatile Uint32* )( GPIO_BASE + 0x28 )
#define GPIO_SET_FAL_TRIG01     *( volatile Uint32* )( GPIO_BASE + 0x2C )
#define GPIO_CLR_FAL_TRIG01     *( volatile Uint32* )( GPIO_BASE + 0x30 )
#define GPIO_INSTAT01           *( volatile Uint32* )( GPIO_BASE + 0x34 )

// GPIO23
#define GPIO_DIR23              *( volatile Uint32* )( GPIO_BASE + 0x38 )
#define GPIO_OUT_DATA23         *( volatile Uint32* )( GPIO_BASE + 0x3C )
#define GPIO_SET_DATA23         *( volatile Uint32* )( GPIO_BASE + 0x40 )
#define GPIO_CLR_DATA23         *( volatile Uint32* )( GPIO_BASE + 0x44 )
#define GPIO_IN_DATA23          *( volatile Uint32* )( GPIO_BASE + 0x48 )
#define GPIO_SET_RIS_TRIG23     *( volatile Uint32* )( GPIO_BASE + 0x4C )
#define GPIO_CLR_RIS_TRIG23     *( volatile Uint32* )( GPIO_BASE + 0x50 )
#define GPIO_SET_FAL_TRIG23     *( volatile Uint32* )( GPIO_BASE + 0x54 )
#define GPIO_CLR_FAL_TRIG23     *( volatile Uint32* )( GPIO_BASE + 0x58 )
#define GPIO_INSTAT23           *( volatile Uint32* )( GPIO_BASE + 0x5C )

// GPIO45
#define GPIO_DIR45              *( volatile Uint32* )( GPIO_BASE + 0x60 )
#define GPIO_OUT_DATA45         *( volatile Uint32* )( GPIO_BASE + 0x64 )
#define GPIO_SET_DATA45         *( volatile Uint32* )( GPIO_BASE + 0x68 )
#define GPIO_CLR_DATA45         *( volatile Uint32* )( GPIO_BASE + 0x6C )
#define GPIO_IN_DATA45          *( volatile Uint32* )( GPIO_BASE + 0x70 )
#define GPIO_SET_RIS_TRIG45     *( volatile Uint32* )( GPIO_BASE + 0x74 )
#define GPIO_CLR_RIS_TRIG45     *( volatile Uint32* )( GPIO_BASE + 0x78 )
#define GPIO_SET_FAL_TRIG45     *( volatile Uint32* )( GPIO_BASE + 0x7C )
#define GPIO_CLR_FAL_TRIG45     *( volatile Uint32* )( GPIO_BASE + 0x80 )
#define GPIO_INSTAT45           *( volatile Uint32* )( GPIO_BASE + 0x84 )

// GPIO67
#define GPIO_DIR67              *( volatile Uint32* )( GPIO_BASE + 0x88 )
#define GPIO_OUT_DATA67         *( volatile Uint32* )( GPIO_BASE + 0x8C )
#define GPIO_SET_DATA67         *( volatile Uint32* )( GPIO_BASE + 0x90 )
#define GPIO_CLR_DATA67         *( volatile Uint32* )( GPIO_BASE + 0x94 )
#define GPIO_IN_DATA67          *( volatile Uint32* )( GPIO_BASE + 0x98 )
#define GPIO_SET_RIS_TRIG67     *( volatile Uint32* )( GPIO_BASE + 0x9C )
#define GPIO_CLR_RIS_TRIG67     *( volatile Uint32* )( GPIO_BASE + 0xA0 )
#define GPIO_SET_FAL_TRIG67     *( volatile Uint32* )( GPIO_BASE + 0xA4 )
#define GPIO_CLR_FAL_TRIG67     *( volatile Uint32* )( GPIO_BASE + 0xA8 )
#define GPIO_INSTAT67           *( volatile Uint32* )( GPIO_BASE + 0xAC )

// INTCx data structure 
typedef struct {
	volatile Uint32 evtflag[4];			// 0x000
	volatile Uint32	resv_010[4];		// 0x010
	volatile Uint32 evtset[4];			// 0x020
	volatile Uint32	resv_030[4];		// 0x030
	volatile Uint32	evtclr[4];			// 0x040
	volatile Uint32	resv_050[12];		// 0x050
	volatile Uint32	evtmask[4];			// 0x080
	volatile Uint32	resv_090[4];		// 0x090
	volatile Uint32 mevtflag[4];		// 0x0A0
	volatile Uint32	resv_0b0[21];		// 0x0b0
	volatile Uint32 intmux1;			// 0x104
	volatile Uint32 intmux2;			// 0x108
	volatile Uint32 intmux3;			// 0x10C
	volatile Uint32	resv_110[12];		// 0x110
	volatile Uint32 aegmux0;			// 0x140
	volatile Uint32 aegmux1;			// 0x144
	volatile Uint32	resv_148[14];		// 0x148
	volatile Uint32 intxstat;			// 0x180
	volatile Uint32 intxclr;			// 0x184
	volatile Uint32 intdmask;			// 0x188
} INTCx;
                       
// I2C base addresses 
#define INTC0_Base    ((INTCx *)0x01800000)

// I2C data structure 
typedef struct {
	volatile Uint32 oar;			// 0x00
	volatile Uint32 imr;			// 0x04
	volatile Uint32	str;			// 0x08
	volatile Uint32	clkl;			// 0x0c
	volatile Uint32 clkh;			// 0x10
	volatile Uint32 cnt;			// 0x14
	volatile Uint32 drr;			// 0x18
	volatile Uint32 sar;			// 0x1c
	volatile Uint32 dxr;			// 0x20
	volatile Uint32 cmdr;			// 0x24
	volatile Uint32 ivr;			// 0x28
	volatile Uint32 emdr;			// 0x2c
	volatile Uint32 psc;			// 0x30
	volatile Uint32 revid1;			// 0x34
	volatile Uint32 revid2;			// 0x38
	volatile Uint32 resv40[2];		// 0x40-0x44
	volatile Uint32 pfunc;			// 0x48
	volatile Uint32 pdir;			// 0x4c
	volatile Uint32 pdin;			// 0x50
	volatile Uint32 pdout;			// 0x54
	volatile Uint32 pdset;			// 0x58
	volatile Uint32 pdclr;			// 0x5c
} I2C;
                       
// declare I2C base addresses 
#define I2C0_Base     ((I2C *)0x01c22000)
#define I2C1_Base     ((I2C *)0x01e28000)

// I2C Field Definitions 
#define ICOAR_MASK_7                    0x007F
#define ICOAR_MASK_10                   0x03FF
#define ICSAR_MASK_7                    0x007F
#define ICSAR_MASK_10                   0x03FF
#define ICOAR_OADDR                     0x007f
#define ICSAR_SADDR                     0x0050

#define ICSTR_SDIR                      0x4000
#define ICSTR_NACKINT                   0x2000
#define ICSTR_BB                        0x1000
#define ICSTR_RSFULL                    0x0800
#define ICSTR_XSMT                      0x0400
#define ICSTR_AAS                       0x0200
#define ICSTR_AD0                       0x0100
#define ICSTR_SCD                       0x0020
#define ICSTR_ICXRDY                    0x0010
#define ICSTR_ICRRDY                    0x0008
#define ICSTR_ARDY                      0x0004
#define ICSTR_NACK                      0x0002
#define ICSTR_AL                        0x0001

#define ICCMDR_NACKMOD                   0x8000
#define ICCMDR_FREE                      0x4000
#define ICCMDR_STT                       0x2000
#define ICCMDR_IDLEEN                    0x1000
#define ICCMDR_STP                       0x0800
#define ICCMDR_MST                       0x0400
#define ICCMDR_TRX                       0x0200
#define ICCMDR_XA                        0x0100
#define ICCMDR_RM                        0x0080
#define ICCMDR_DLB                       0x0040
#define ICCMDR_IRS                       0x0020
#define ICCMDR_STB                       0x0010
#define ICCMDR_FDF                       0x0008
#define ICCMDR_BC_MASK                   0x0007

// UART data structure 
typedef struct {
	volatile Uint32 rbr_thr;		// 0x00
	volatile Uint32 ier;			// 0x04
	volatile Uint32	iir_fcr;		// 0x08
	volatile Uint32	lcr;			// 0x0c
	volatile Uint32 mcr;			// 0x10
	volatile Uint32 lsr;			// 0x14
	volatile Uint32 resv18;			// 0x18
	volatile Uint32 resv1c;			// 0x1c
	volatile Uint32 dll;			// 0x20
	volatile Uint32 dlh;			// 0x24
	volatile Uint32 revid1;			// 0x28
	volatile Uint32 resv2c;			// 0x2c
	volatile Uint32 pwremu_mgmt;	// 0x30
	volatile Uint32 mdr;			// 0x34
} UART;
                       
// UART base addresses 
#define UART0_Base   	 	((UART *)0x01c42000)
#define UART1_Base   	  	((UART *)0x01D0C000)
#define UART2_Base     		((UART *)0x01D0D000)

// uart register bit definitions
#define UART_LSR_DR			0x01
#define UART_LSR_THRE		0x20
#define UART_LSR_TEMT		0x40

// McASP data structure 
typedef struct {
	volatile Uint32 pid;			// 0x00
	volatile Uint32	resv04[3];		// 0x04-0x0c
	volatile Uint32 pfunc;			// 0x10
	volatile Uint32 pdir;			// 0x14
	volatile Uint32 pdout;			// 0x18
	volatile Uint32 pdin;			// 0x1c
	volatile Uint32 pclr;			// 0x20
	volatile Uint32	resv24[8];		// 0x24-0x40
	volatile Uint32 gblctl;			// 0x44
	volatile Uint32 amute;			// 0x48
	volatile Uint32 dlbctl;			// 0x4c
	volatile Uint32 ditctl;			// 0x50
	volatile Uint32	resv54[3];		// 0x54-0x5c
	volatile Uint32 rgblctl;		// 0x60
	volatile Uint32 rmask;			// 0x64
	volatile Uint32 rfmt;			// 0x68
	volatile Uint32 afsrctl;		// 0x6c
	volatile Uint32 aclkrctl;		// 0x70
	volatile Uint32 ahclkrctl;		// 0x74
	volatile Uint32 rtdm;			// 0x78
	volatile Uint32 rintctl;		// 0x7c
	volatile Uint32 rstat;			// 0x80
	volatile Uint32 rslot;			// 0x84
	volatile Uint32 rclkchk;		// 0x88
	volatile Uint32 revtctl;		// 0x8c
	volatile Uint32	resv90[4];		// 0x90-0xac
	volatile Uint32 xgblctl;		// 0xa0
	volatile Uint32	xmask;			// 0xa4
	volatile Uint32	xfmt;			// 0xa8
	volatile Uint32	afsxctl;		// 0xac
	volatile Uint32	aclkxctl;		// 0xb0
	volatile Uint32	ahclkxctl;		// 0xb4
	volatile Uint32	xtdm;			// 0xb8
	volatile Uint32	xintctl;		// 0xbc
	volatile Uint32	xstat;			// 0xc0
	volatile Uint32	xslot;			// 0xc4
	volatile Uint32	xclkchk;		// 0xc8
	volatile Uint32	xevtctl;		// 0xcc
	volatile Uint32 resv_d0[12];	// 0xd0-0xFC
	volatile Uint32	dit_regs[24];	// 0x100-0x15c
	volatile Uint32 resv_160[8];	// 0x160-0x17C
    volatile Uint32 srctl[16];		// 0x180-0x1bC
             Uint32 rsvd_1c0[16];	// 0x1c0-0x1fC
    volatile Uint32 xbuf[16];		// 0x200-0x23C
             Uint32 rsvd_240[16];	// 0x240-0x27C
    volatile Uint32 rbuf[16]; 		// 0x280-0x2bC
} McASP;
                       
// McASP base addresses 
#define McASP0_Base     ((McASP *)0x01d00000)
#define McASP1_Base     ((McASP *)0x01d04000)
#define McASP2_Base     ((McASP *)0x01d08000)

// GBLCTL bit masks                                               *
#define GBLCTL_XFRST			0x1000
#define GBLCTL_XSMRST			0x0800
#define GBLCTL_XSRCLR			0x0400
#define GBLCTL_XHCLKRST			0x0200
#define GBLCTL_XCLKRST			0x0100
#define GBLCTL_RFRST			0x0010
#define GBLCTL_RSMRST			0x0008
#define GBLCTL_RSRCLR			0x0004
#define GBLCTL_RHCLKRST			0x0002
#define GBLCTL_RCLKRST			0x0001


// define EDMA3_0_CC registers 
#define EDMA3_0_CC_BASE			0x01C00000
#define EDMA3_0_CC_DRAE0		(EDMA3_0_CC_BASE + 0x0340)	// DMA region access enable 0
#define EDMA3_0_CC_DRAE1		(EDMA3_0_CC_BASE + 0x0348)	// DMA region access enable 1
#define EDMA3_0_CC_DRAE2		(EDMA3_0_CC_BASE + 0x0350)	// DMA region access enable 2
#define EDMA3_0_CC_DRAE3		(EDMA3_0_CC_BASE + 0x0358)	// DMA region access enable 3
#define EDMA3_0_CC_QRAE0		(EDMA3_0_CC_BASE + 0x0380)	// QDMA region access enable 0
#define EDMA3_0_CC_QRAE1		(EDMA3_0_CC_BASE + 0x0384)	// QDMA region access enable 1
#define EDMA3_0_CC_QRAE2		(EDMA3_0_CC_BASE + 0x0388)	// QDMA region access enable 2
#define EDMA3_0_CC_QRAE3		(EDMA3_0_CC_BASE + 0x038C)	// QDMA region access enable 3

#define EDMA3_0_CC_ER			(EDMA3_0_CC_BASE + 0x1000)	// event register
#define EDMA3_0_CC_ECR			(EDMA3_0_CC_BASE + 0x1008)	// event clear register
#define EDMA3_0_CC_ESR			(EDMA3_0_CC_BASE + 0x1010)	// event set register
#define EDMA3_0_CC_CER			(EDMA3_0_CC_BASE + 0x1018)	// chained event register
#define EDMA3_0_CC_EER			(EDMA3_0_CC_BASE + 0x1020)	// event enable register
#define EDMA3_0_CC_EECR			(EDMA3_0_CC_BASE + 0x1028)	// event enable clear register
#define EDMA3_0_CC_EESR			(EDMA3_0_CC_BASE + 0x1030)	// event enable set register
#define EDMA3_0_CC_SER			(EDMA3_0_CC_BASE + 0x1038)	// event enable register
#define EDMA3_0_CC_SECR			(EDMA3_0_CC_BASE + 0x1040)	// secondary event clear register
#define EDMA3_0_CC_IER			(EDMA3_0_CC_BASE + 0x1050)	// interrupt enable register
#define EDMA3_0_CC_IECR			(EDMA3_0_CC_BASE + 0x1058)	// interrupt enable clear register
#define EDMA3_0_CC_IESR			(EDMA3_0_CC_BASE + 0x1060)	// interrupt enable set register
#define EDMA3_0_CC_IPR			(EDMA3_0_CC_BASE + 0x1068)	// interrupt pending register
#define EDMA3_0_CC_ICR			(EDMA3_0_CC_BASE + 0x1070)	// interrupt clear register
#define EDMA3_0_CC_IEVAL		(EDMA3_0_CC_BASE + 0x1078)	// interrupt evaluate register

// define EDMA3_0 events
#define EDMA3_EVENT_MCASP0_RX	0
#define EDMA3_EVENT_MCASP0_TX	1

// declare EDMA parameter RAM data structure 
typedef struct {
	volatile Uint32 option;  
	volatile Uint32 source;   
	volatile Uint32 a_b_count;  
	volatile Uint32 dest;   
	volatile Uint32 src_dest_b_index;  
	volatile Uint32 link_reload;   
	volatile Uint32 src_dest_c_index;  
	volatile Uint32 c_count;   
} EDMA_params;

// define EDMA3_0 parameter RAM addresses 
#define EDMA3_0_PARAM_BASE	 	0x01C04000
#define EDMA3_0_PARAM_OFFSET 	0x20
#define EDMA3_0_PARAM(x)		(EDMA3_0_PARAM_BASE + (x * EDMA3_0_PARAM_OFFSET))





#endif
