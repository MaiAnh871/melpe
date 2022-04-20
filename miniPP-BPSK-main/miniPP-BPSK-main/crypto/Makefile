
CC = gcc
MAKEFILENAME = makefile

MODE=NORM



CFLAGS_NORM  = -I. -g
CFLAGS_DEBUG = -g -DDEBUG
CFLAGS = -Wall  $(CFLAGS_$(MODE))
CFLAGSDEPEND = -MM $(CFLAGS)                    # for make depend

TMP=$(MODE:NORM=)
TMP2=$(TMP:DEBUG=_debug)


#
# source/object files
#
TESTER_SRCS=curve.c sponge.c sprng.c havege.c timing.c b64.c wordlist.c test.c
LIBCRP_SRCS=curve.c sponge.c sprng.c havege.c timing.c b64.c wordlist.c

TESTER_OBJS=$(TESTER_SRCS:.c=.o) 
LIBCRP_OBJS=$(LIBCRP_SRCS:.c=.o)

ALL_SRCS=$(TESTER_SRCS)

#default: libcrypto.a test
default: libcrypto.a


test: $(TESTER_OBJS)
	$(CC) -o test $(CFLAGS) $(TESTER_OBJS) $(LDFLAGS) -lm

libcrypto.a: $(LIBCRP_OBJS)
	ar cr libcrypto.a $(LIBCRP_OBJS)
	ranlib libcrypto.a


#
# remove object/executable files
#
clean:
	del -f *.o test test.exe libcrypto.a


# DO NOT DELETE THIS LINE -- make depend depends on it.
curve.o: curve.c curve.h 
sponge.o: sponge.c sponge.h Keccak512_data.h
sprng.o: sprng.c sprng.h
havege.o: havege.c havege.h 
timing.o: timing.c timing.h 
b64.o: b64.c b64.h 
wordlist.o: wordlist.c wordlist.h 
test.o: test.c

