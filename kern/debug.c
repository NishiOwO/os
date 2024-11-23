/* $Id$ */

#include <kern/debug.h>

void(*kdebug)(const char* str);

void kdebug_null(const char* str){
}

void kdebuginit(void){
#ifdef DEBUG
	extern void kdebug_handler(const char* str);
	kdebug = kdebug_handler;
#else
	kdebug = kdebug_null;
#endif
}
