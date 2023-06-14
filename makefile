all: test



test:test.c libisentlib.a outils.o
	gcc -Wall -o output test.c outils.o temp/libisentlib.a -lm -lglut -lGL -lX11

#############################################################################################################################################################################

outils.o : outils.c
	gcc -Wall -c outils.c

libisentlib.a:
	make -C gfxlib/src

clean:
	rm -f temp/*.o