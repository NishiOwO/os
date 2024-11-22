# $Id$

YACC = yacc
YFLAGS = -y -d

LEX = lex
LFLAGS =

CC = cc
CFLAGS =
LDFLAGS =
LIBS =

OBJS = config.o y.tab.o lex.yy.o

.PHONY: all clean
.SUFFIXES: .c .o

all: ./config

./config: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

y.tab.c: parse.y
	$(YACC) $(YFLAGS) parse.y

lex.yy.c: parse.l y.tab.c
	$(LEX) $(LFLAGS) parse.l

clean:
	rm -f config *.o y.tab.c y.tab.h lex.yy.c
