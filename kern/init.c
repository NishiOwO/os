/* $Id$ */

#include <kern/debug.h>
#include <kern/print.h>
#include <kern/version.h>

void drivers_init(void);

void kernel_main(int argc, char** argv){
	drivers_init();
	kprint(KERNEL_NAME " V" KERNEL_VERSION " (" TARGET " system) " IDENT " kernel\r\n");
	while(1);
}
