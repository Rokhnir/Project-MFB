test:test.c libisentlib.a
	gcc -Wall -o output test.c  outils.c weapon.c temp/libisentlib.a -lm -lglut -lGL -lX11 -pthread

#############################################################################################################################################################################

libisentlib.a: BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o SocketLib.o ThreadLib.o VectorLib.o WavLib.o
	ar r temp/libisentlib.a temp/BmpLib.o temp/ErreurLib.o temp/ESLib.o temp/GfxLib.o temp/OutilsLib.o temp/SocketLib.o temp/ThreadLib.o temp/VectorLib.o temp/WavLib.o
	ranlib temp/libisentlib.a

BmpLib.o: gfxlib/src/BmpLib.c gfxlib/include/BmpLib.h gfxlib/include/OutilsLib.h
	gcc -Wall -O2 -o temp/BmpLib.o -c gfxlib/src/BmpLib.c

ESLib.o: gfxlib/src/ESLib.c gfxlib/include/ESLib.h gfxlib/include/ErreurLib.h
	gcc -Wall -O2 -o temp/ESLib.o -c gfxlib/src/ESLib.c

ErreurLib.o: gfxlib/src/ErreurLib.c gfxlib/include/ErreurLib.h
	gcc -Wall -O2 -o temp/ErreurLib.o -c gfxlib/src/ErreurLib.c

GfxLib.o: gfxlib/src/GfxLib.c gfxlib/include/GfxLib.h gfxlib/include/ESLib.h
	gcc -Wall -O2 -o temp/GfxLib.o -c gfxlib/src/GfxLib.c -I/usr/include/GL

OutilsLib.o: gfxlib/src/OutilsLib.c gfxlib/include/OutilsLib.h
	gcc -Wall -O2 -o temp/OutilsLib.o -c gfxlib/src/OutilsLib.c

SocketLib.o: gfxlib/src/SocketLib.c gfxlib/include/SocketLib.h
	gcc -Wall -O2 -o temp/SocketLib.o -c gfxlib/src/SocketLib.c

ThreadLib.o: gfxlib/src/ThreadLib.c gfxlib/include/ThreadLib.h
	gcc -Wall -O2 -o temp/ThreadLib.o -c gfxlib/src/ThreadLib.c

WavLib.o: gfxlib/src/WavLib.c gfxlib/include/WavLib.h gfxlib/include/OutilsLib.h
	gcc -Wall -O2 -o temp/WavLib.o -c gfxlib/src/WavLib.c -Wno-unused-result

VectorLib.o: gfxlib/src/VectorLib.c gfxlib/include/VectorLib.h
	gcc -Wall -O2 -o temp/VectorLib.o -c gfxlib/src/VectorLib.c -msse3
outils.o : outils.c
	gcc -Wall -c outils.c

weapon.o : weapon.c
	gcc -Wall -c weapon.c
clean:
	rm -f temp/*.?
