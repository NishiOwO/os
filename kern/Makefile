# $Id$

include ../build/config.mk

.PHONY: all
.SUFFIXES: .c .o

all: init.o

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(BUILDDIR)/kern/*.o
