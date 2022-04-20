CC = gcc

CFLAGS = -Wall -O4


LOBJECTS = fec_golay2412.o 
LSRC = $(LOBJECTS:.o=.c)

LIBS = -lm

libfec.a: $(LOBJECTS)

	ar cr libfec.a $(LOBJECTS)
	ranlib libfec.a
 


clean:
	rm -f $(PROGRAM) $(LOBJECTS) $(ARCH) core *.bak *.a

depend:
	makedepend -- $(CFLAGS) -- $(LSRC)


# DO NOT DELETE
