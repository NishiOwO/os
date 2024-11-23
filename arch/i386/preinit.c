/* $Id$ */

#include <arch/io.h>
#include <kern/debug.h>
#include <kern/print.h>

void kernel_main(void);
void setup_memory(void);

void init_kernel(unsigned long magic, unsigned long addr){
	unsigned char* vram = (unsigned char*)0xb8000;
	int i;
	kdebuginit();
	kprintinit();
	kdebug("Putting colorbar");
	for(i = 0; i < 80 * 25; i++){
		vram[i * 2 + 0] = 0;
		vram[i * 2 + 1] = (((i / 5) & 0xf) << 4) | (0xf - ((i / 5) & 0xf));
	}
	kdebug("Moving cursor");
	outb(0x3d4, 0x0a);
	outb(0x3d5, 0x20);
	setup_memory();
	kdebug("Calling kernel main");
	kernel_main();
	while(1);
}
