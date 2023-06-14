all : libisentlib.a

vpath %.o ../temp
vpath %.h ../include

libisentlib.a: BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o SocketLib.o ThreadLib.o VectorLib.o WavLib.o
	ar r libisentlib.a BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o SocketLib.o ThreadLib.o VectorLib.o WavLib.o
	ranlib ../temp/libisentlib.a

BmpLib.o: BmpLib.c BmpLib.h OutilsLib.h
	gcc -Wall -O2 -o BmpLib.o -c BmpLib.c

ESLib.o: ESLib.c ESLib.h ErreurLib.h
	gcc -Wall -O2 -o ESLib.o -c ESLib.c

ErreurLib.o: ErreurLib.c ErreurLib.h
	gcc -Wall -O2 -o ErreurLib.o -c ErreurLib.c

GfxLib.o: GfxLib.c GfxLib.h ESLib.h
	gcc -Wall -O2 -o GfxLib.o -c GfxLib.c -I/usr/include/GL

OutilsLib.o: OutilsLib.c OutilsLib.h
	gcc -Wall -O2 -o OutilsLib.o -c OutilsLib.c

SocketLib.o: SocketLib.c SocketLib.h
	gcc -Wall -O2 -o SocketLib.o -c SocketLib.c

ThreadLib.o: ThreadLib.c ThreadLib.h
	gcc -Wall -O2 -o ThreadLib.o -c ThreadLib.c

WavLib.o: WavLib.c WavLib.h OutilsLib.h
	gcc -Wall -O2 -o WavLib.o -c WavLib.c -Wno-unused-result

VectorLib.o: VectorLib.c VectorLib.h
	gcc -Wall -O2 -o VectorLib.o -c VectorLib.c -msse3

clean:
	rm -f temp/*.?