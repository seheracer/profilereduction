LIBDIR = lib
INCDIR = inc
SRCDIR = src
BINDIR = bin
CFILES = main.c mm.c pth.c mohp.c weightedgreed.c sm.c heap.c stack.c
PCFILES = $(addprefix $(SRCDIR)/,$(CFILES))
LIBS = -lpatoh -lm
CFLAGS = -O2

all:	$(PCFILES)
	gcc -o $(BINDIR)/mohp $(CFLAGS) $(PCFILES) -I$(INCDIR) -L$(LIBDIR) $(LIBS)
