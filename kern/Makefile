# $Id$

include ../build/config.mk

.PHONY: all
.SUFFIXES: .c .o

all: $(BUILDDIR)/kern/init.o $(BUILDDIR)/kern/memory.o

$(BUILDDIR)/kern/init.o: init.c
	$(CC) $(CFLAGS) -c -o $@ init.c

$(BUILDDIR)/kern/memory.o: memory.c
	$(CC) $(CFLAGS) -c -o $@ memory.c

clean:
	rm -f $(BUILDDIR)/kern/*.o
