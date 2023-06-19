all: test



test:test.c libisentlib.a outils.o weapon.o
	gcc -Wall -o output test.c outils.o weapon.o temp/libisentlib.a -lm -lglut -lGL -lX11

#############################################################################################################################################################################

outils.o : outils.c
	gcc -Wall -c outils.c

weapon.o : weapon.c
	gcc -Wall -c weapon.c

libisentlib.a:
	make -C gfxlib/src

clean:
	rm -f temp/*.o