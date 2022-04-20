CC = gcc

CFLAGS = -Wall -O4


LOBJECTS = audio.o 
LSRC = $(LOBJECTS:.o=.c)

LIBS = -lm

libaudio.a: $(LOBJECTS)
	ar cr libaudio.a $(LOBJECTS)
	ranlib libaudio.a


clean:
	rm -f $(PROGRAM) $(LOBJECTS) $(ARCH) core *.bak *.a

depend:
	makedepend -- $(CFLAGS) -- $(LSRC)


# DO NOT DELETE
