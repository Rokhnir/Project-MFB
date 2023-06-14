CC = gcc
CFLAGS = -Wall -O2 -c

vpath %.c ./src
vpath %.h ./include
vpath %.o ../temp
vpath %.a ../temp

libisentlib.a: BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o SocketLib.o ThreadLib.o VectorLib.o WavLib.o
	ar r libisentlib.a BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o SocketLib.o ThreadLib.o VectorLib.o WavLib.o
	ranlib libisentlib.a

BmpLib.o: BmpLib.c BmpLib.h OutilsLib.h
	${CC} ${CFLAGS} $<

ESLib.o: ESLib.c ESLib.h ErreurLib.h
	${CC} ${CFLAGS} $<

ErreurLib.o: ErreurLib.c ErreurLib.h
	${CC} ${CFLAGS} $<

GfxLib.o: GfxLib.c GfxLib.h ESLib.h
	${CC} ${CFLAGS} $< -I/usr/include/GL

OutilsLib.o: OutilsLib.c OutilsLib.h
	${CC} ${CFLAGS} $<

SocketLib.o: SocketLib.c SocketLib.h
	${CC} ${CFLAGS} $<

ThreadLib.o: ThreadLib.c ThreadLib.h
	${CC} ${CFLAGS} $<

WavLib.o: WavLib.c WavLib.h OutilsLib.h
	${CC} ${CFLAGS} $< -Wno-unused-result

clean:
	rm -f *~ *.o

deepclean: clean
	rm -f libisentlib.a
