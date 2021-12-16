
# Debugging options  
DEBUG = -O -DHEXDUMP

#Full duplex:

CC = gcc -Wall -g # for GNU's gcc compiler


#   Compiler flags

CFLAGS = $(DEBUG) $(PKOPTS) -Imeple $(CARGS) $(DUPLEX) $(CCFLAGS) $(DOMAIN)

BINARIES = main

PROGRAMS = $(BINARIES) $(SCRIPTS) 

DIRS =  melpe

all:	$(PROGRAMS)

SPKROBJS = main.o


main: $(SPKROBJS) 
	$(CC) $(SPKROBJS)  melpe/libmelpe.a $(LFLAGS) -o melpe

#   Object file dependencies

main.o: Makefile main.c

#	Clean everything

clean:
	del -f core *.out *.o *.bak $(PROGRAMS) *.shar *.exe *.a

# DO NOT DELETE