/* $Id$ */

#include <arch/io.h>

void init_kernel(unsigned long magic, unsigned long addr){
	unsigned char* vram = (unsigned char*)0xb8000;
	int i;
	for(i = 0; i < 80 * 25; i++){
		vram[i * 2 + 0] = 0;
		vram[i * 2 + 1] = 0x07;
	}
        outb(0x3D4, 0x0F);
        outb(0x3D5, 0);
        outb(0x3D4, 0x0E);
        outb(0x3D5, 0);
	while(1);
}
