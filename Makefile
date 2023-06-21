CC := gcc
CFLAGS := -Wall -O2 -o

BUILDDIR := ./PPB/build
INCLUDE := ./PPB/include
SRC := ./PPB/src
SRCFILES := main.c mapHandler.c rayCasting.c
OBJFILES := $(patsubst %.c, $(BUILDDIR)/%.o, $(SRCFILES))

all: $(BUILDDIR) libisentlib.a output

output: $(OBJFILES)
	$(CC) $(CFLAGS) $@ $^ gfxlib/build/libisentlib.a -lm -lglut -lGL -lX11

$(BUILDDIR):
	mkdir $(BUILDDIR)

libisentlib.a:
	make -C gfxlib/

$(BUILDDIR)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $@ -c $<

$(BUILDDIR)/mapHandler.o: $(SRC)/mapHandler.c $(INCLUDE)/mapHandler.h
	$(CC) $(CFLAGS) $@ -c $< -Wno-unused-result

clean:
	make clean -C gfxlib/
	rm -f $(BUILDDIR)/*.o
	rm -r $(BUILDDIR)