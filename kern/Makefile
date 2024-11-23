# $Id$

include ../build/config.mk

.PHONY: all
.SUFFIXES: .c .o

all: $(BUILDDIR)/kern/init.o

$(BUILDDIR)/kern/init.o: init.c
	$(CC) $(CFLAGS) -c -o $@ init.c

clean:
	rm -f $(BUILDDIR)/kern/*.o
