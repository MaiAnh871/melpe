#   Make file for PairPhone
#   Tested with gcc 4.4.3 under Ubuntu 10.04 (requires libsound2-dev)
#   and MinGW (gcc 4.8.1, make 3.81) under Windows XP SP3
# Debugging options  
DEBUG = -O0 -DHEXDUMP

#Full duplex:
CCFLAGS =  -DAUDIO_BLOCKING -DLINUX -DM_LITTLE_ENDIAN -DNEEDED_LINEAR -DLINUX_DSP_SMALL_BUFFER -DHAVE_DEV_RANDOM

CC = gcc -Wall -g -O0 # for GNU's gcc compiler
CELPFLAGS = -fomit-frame-pointer -ffast-math -funroll-loops
LFLAGS = -lm
CCFLAGS = -DLINUX_ALSA -DM_LITTLE_ENDIAN
SOUNDLIB = -lasound 

#   Compiler flags

CFLAGS = $(DEBUG) $(PKOPTS) -Iaudio -Icrypto -Imelpe -Imodem -Ifec -Ivad $(CARGS) $(DUPLEX) $(CCFLAGS) $(DOMAIN)

BINARIES = tx rx

PROGRAMS = $(BINARIES) $(SCRIPTS)

DIRS = audio crypto melpe modem fec vad

all:	$(PROGRAMS)


SPKROBJS_TX = pptx.o crp.o ctr.o tx.o
SPKROBJS_RX = pprx.o crp.o ctr.o rx.o 


#Link

tx: $(SPKROBJS_TX) 
	$(CC) $(SPKROBJS_TX)  audio/libaudio.a crypto/libcrypto.a melpe/libmelpe.a modem/libmodem.a fec/libfec.a vad/libvad.a $(LFLAGS) -lcomctl32 -lwinmm -lws2_32 -o tx
	
rx: $(SPKROBJS_RX) 
	$(CC) $(SPKROBJS_RX)  audio/libaudio.a crypto/libcrypto.a melpe/libmelpe.a modem/libmodem.a fec/libfec.a vad/libvad.a $(LFLAGS) -lcomctl32 -lwinmm -lws2_32 -o rx

#   Object file dependencies

crp.o: Makefile crp.c
	gcc -Wall -g -O0 -c -o crp.o crp.c	
ctr.o: Makefile ctr.c
	gcc -Wall -g -O0 -c -o ctr.o ctr.c
rx.o: Makefile rx.c
	gcc -Wall -g -O0 -c -o rx.o rx.c
tx.o: Makefile tx.c
	gcc -Wall -g -O0 -c -o tx.o tx.c

#	Clean everything

clean:
	del -f core *.out *.o *.bak $(PROGRAMS) *.shar *.exe *.a
	

# DO NOT DELETE