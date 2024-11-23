# $Id$

include ../build/config.mk

.PHONY: all
.SUFFIXES: .c .o

all: $(BUILDDIR)/kern/init.o $(BUILDDIR)/kern/memory.o $(BUILDDIR)/kern/debug.o $(BUILDDIR)/kern/print.o

$(BUILDDIR)/kern/init.o: init.c
	$(CC) $(CFLAGS) -c -o $@ init.c

$(BUILDDIR)/kern/memory.o: memory.c
	$(CC) $(CFLAGS) -c -o $@ memory.c

$(BUILDDIR)/kern/debug.o: debug.c
	$(CC) $(CFLAGS) -c -o $@ debug.c

$(BUILDDIR)/kern/print.o: print.c
	$(CC) $(CFLAGS) -c -o $@ print.c

clean:
	rm -f $(BUILDDIR)/kern/*.o
