/* $Id$ */

#include <arch/io.h>

void kprint_handler(const char* str){
	int i;
	for(i = 0; str[i] != 0; i++){
		while((inb(0x3f8 + 5) & 0x20) == 0);
		outb(0x3f8, str[i]);
	}
}
