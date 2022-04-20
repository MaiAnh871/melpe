CC = gcc

CFLAGS = -Wall -O4

LOBJECTS = classify.o coeff.o dsp_sub.o fec_code.o fft_lib.o \
       fs_lib.o fsvq_cb.o global.o harm.o lpc_lib.o mathdp31.o \
       mathhalf.o math_lib.o mat_lib.o melp_ana.o melp_chn.o melp_sub.o \
       melp_syn.o msvq_cb.o npp.o pitch.o pit_lib.o postfilt.o qnt12.o \
       qnt12_cb.o vq_lib.o melpe.o

LSRC = $(LOBJECTS:.o=.c)

LIBS = -lm

libmelpe.a: $(LOBJECTS)
	ar cr libmelpe.a $(LOBJECTS)
	ranlib libmelpe.a
clean:
	del -f $(PROGRAM) $(LOBJECTS) $(ARCH) core *.bak *.a *.o

depend:
	makedepend -- $(CFLAGS) -- $(LSRC)


# DO NOT DELETE
