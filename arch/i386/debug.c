/* $Id$ */

#include <arch/debug.h>
#include <arch/io.h>

void kdebug(const char* str){
#ifdef DEBUG
	int i;
	for(i = 0; str[i] != 0; i++){
		while((inb(0x3f8 + 5) & 0x20) == 0);
		outb(0x3f8, str[i]);
	}
	while((inb(0x3f8 + 5) & 0x20) == 0);
	outb(0x3f8, '\r');
	while((inb(0x3f8 + 5) & 0x20) == 0);
	outb(0x3f8, '\n');
#endif
}
