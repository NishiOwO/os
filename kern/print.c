/* $Id$ */

#include <kern/print.h>

void(*kprint)(const char* str);

void kprintinit(void){
	extern void kprint_handler(const char* str);
	kprint = kprint_handler;
}
