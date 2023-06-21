CC := gcc
CFLAGS := -Wall -O2 -o

BUILDDIR := build
INCLUDE := ./include
SRC := ./src
SRCFILES := BmpLib.c ErreurLib.c ESLib.c GfxLib.c OutilsLib.c SocketLib.c ThreadLib.c VectorLib.c WavLib.c
OBJFILES := $(patsubst %.c, $(BUILDDIR)/%.o, $(SRCFILES))

all: $(BUILDDIR) $(BUILDDIR)/libisentlib.a

$(BUILDDIR)/libisentlib.a: $(OBJFILES)
	ar rcs $@ $^
	ranlib $@

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $@ -c $<

$(BUILDDIR)/GfxLib.o: $(SRC)/GfxLib.c $(INCLUDE)/GfxLib.h $(INCLUDE)/ESLib.h
	$(CC) $(CFLAGS) $@ -c $< -I/usr/include/GL

$(BUILDDIR)/WavLib.o: $(SRC)/WavLib.c $(INCLUDE)/WavLib.h $(INCLUDE)/OutilsLib.h
	$(CC) $(CFLAGS) $@ -c $< -Wno-unused-result

clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/libisentlib.a
	rm -fr $(BUILDDIR)