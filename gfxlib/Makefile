CC := gcc
CFLAGS := -Wall -O2 -o

BUILDDIR := build
INCLUDE := ./include
SRC := ./src
SRCFILES := BmpLib.c ErreurLib.c ESLib.c GfxLib.c OutilsLib.c SocketLib.c ThreadLib.c VectorLib.c WavLib.c
OBJFILES := $(patsubst %.c, $(BUILDDIR)/%.o, $(SRCFILES))

all: $(BUILDDIR) $(BUILDDIR)/libisentlib.a

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(BUILDDIR)/libisentlib.a: $(OBJFILES)
	ar rcs $@ $^
	ranlib $@

$(BUILDDIR)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $@ -c $< -Wno-unused-result

clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/libisentlib.a
	rm -fr $(BUILDDIR)