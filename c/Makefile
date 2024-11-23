# $Id$

OBJS = $(BUILDDIR)/c/memory.o $(BUILDDIR)/c/string.o

include ../build/config.mk

.PHONY: all
.SUFFIXES: .c .o

all: $(BUILDDIR)/c/libc.a

$(BUILDDIR)/c/libc.a: $(OBJS)
	ar rcs $@ $(OBJS)

$(BUILDDIR)/c/memory.o: memory.c
	$(CC) $(CFLAGS) -c -o $@ memory.c

$(BUILDDIR)/c/string.o: string.c
	$(CC) $(CFLAGS) -c -o $@ string.c

clean:
	rm -f $(BUILDDIR)/c/*.o $(BUILDDIR)/c/libc.a
