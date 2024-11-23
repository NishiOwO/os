/* $Id$ */

#include <arch/io.h>
#include <arch/debug.h>

void kernel_main(void);

void init_kernel(unsigned long magic, unsigned long addr){
	unsigned char* vram = (unsigned char*)0xb8000;
	int i;
	kdebug("Clearing screen");
	for(i = 0; i < 80 * 25; i++){
		vram[i * 2 + 0] = 0;
		vram[i * 2 + 1] = 0x07;
	}
	kdebug("Putting colorbar");
	for(i = 0; i < 80; i++){
		vram[i * 2 + 0] = 0;
		vram[i * 2 + 1] = ((i / 5) & 0xf) << 4;
	}
	kdebug("Moving cursor");
        outb(0x3D4, 0x0F);
        outb(0x3D5, 80);
        outb(0x3D4, 0x0E);
        outb(0x3D5, 0);
	kdebug("Calling kernel main");
	kernel_main();
	while(1);
}
