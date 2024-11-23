/* $Id$ */

#include <arch/debug.h>
#include <kern/version.h>

void drivers_init(void);

void kernel_main(void){
	kdebug("MP/RT V" MPRT_VERSION " (" TARGET " system) booting up");
	drivers_init();
	while(1);
}
