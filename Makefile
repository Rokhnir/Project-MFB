CC := gcc
CFLAGS := -Wall -O2 -o

BUILDDIR := ./PPB/build
INCLUDE := ./PPB/include
SRC := ./PPB/src
SRCFILES := main.c mapHandler.c rayCasting.c
OBJFILES := $(patsubst %.c, $(BUILDDIR)/%.o, $(SRCFILES))

all:output

output: libisentlib.a $(OBJFILES)
	$(CC) $(CFLAGS) $@ $(filter-out $<, $^) gfxlib/build/$< -lm -lglut -lGL -lX11

$(BUILDDIR):
	mkdir $(BUILDDIR)

libisentlib.a:
	make -C gfxlib/

$(BUILDDIR)/%.o: $(SRC)/%.c $(BUILDDIR)
	$(CC) $(CFLAGS) $@ -c $<

$(BUILDDIR)/mapHandler.o: $(SRC)/mapHandler.c $(INCLUDE)/mapHandler.h
	$(CC) $(CFLAGS) $@ -c $< -Wno-unused-result

clean:
	make clean -C gfxlib/
	rm -f $(BUILDDIR)/*.o
	rm -r $(BUILDDIR)