/* $Id$ */

#include <arch/io.h>
#include <arch/debug.h>

void kernel_main(void);
void setup_memory(void);

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
#ifdef DEBUG
		const char str[] = "Debug";
		vram[i * 2 + 0] = str[i % 5];
#else
		vram[i * 2 + 0] = 0;
#endif
		vram[i * 2 + 1] = (((i / 5) & 0xf) << 4) | (0xf - ((i / 5) & 0xf));
	}
	kdebug("Moving cursor");
        outb(0x3D4, 0x0F);
        outb(0x3D5, 80);
        outb(0x3D4, 0x0E);
        outb(0x3D5, 0);
	setup_memory();
	kdebug("Calling kernel main");
	kernel_main();
	while(1);
}
