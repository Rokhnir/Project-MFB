CC := gcc
CFLAGS := -Wall -O2 -o

BUILDDIR := ./Project-MFB/build
INCLUDE := ./Project-MFB/include
SRC := ./Project-MFB/src
SRCFILES := main.c mapHandler.c playerHandler.c rayCasting.c ui.c weapon.c outils.c opacité.c
OBJFILES := $(patsubst %.c, $(BUILDDIR)/%.o, $(SRCFILES))

all: $(BUILDDIR) libisentlib.a output

$(BUILDDIR):
	mkdir $(BUILDDIR)

libisentlib.a:
	make -C gfxlib/

output: $(OBJFILES)
	$(CC) $(CFLAGS) $@ $^ gfxlib/build/libisentlib.a -lm -lglut -lGL -lX11

$(BUILDDIR)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $@ -c $< -Wno-unused-result

clean:
	make clean -C gfxlib/
	rm -f $(BUILDDIR)/*.o
	rm -fr $(BUILDDIR)
	rm -f output