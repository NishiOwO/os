/* $Id$ */

#include <arch/debug.h>
#include <kern/version.h>

void kernel_main(void){
	kdebug("MP/RT V" MPRT_VERSION " (" TARGET " system) booting up");
	while(1);
}
