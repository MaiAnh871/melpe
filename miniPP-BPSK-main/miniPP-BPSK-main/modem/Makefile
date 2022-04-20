CC =gcc
CFLAGS = -Wall -O0 -g


LOBJECTS = modem.o 
LSRC = $(LOBJECTS:.o=.c)

LIBS = -lm

libaudio.a: $(LOBJECTS)
	ar cr libmodem.a $(LOBJECTS)
	ranlib libmodem.a

clean:
	rm -f $(PROGRAM) $(LOBJECTS) $(ARCH) core *.bak *.a

depend:
	makedepend -- $(CFLAGS) -- $(LSRC)


# DO NOT DELETE