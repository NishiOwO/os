/* $Id$ */

#include <arch/io.h>
#include <kern/debug.h>
#include <kern/print.h>

void kernel_main(int argc, char** argv);
void setup_memory(void);

struct multiboot_info {
        uint32_t flags;
        uint32_t lower;
        uint32_t upper;
        uint32_t boot;
        char* cmdline;
        uint32_t mods_count;
        uint32_t mods_addr;
};

void init_kernel(unsigned long magic, unsigned long addr){
	unsigned char* vram = (unsigned char*)0xb8000;
	int i;
	int argc = 0;
	char* argv[64];
	struct multiboot_info* info = (struct multiboot_info*)addr;
	kdebuginit();
	kprintinit();
	kdebug(info->cmdline);
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
	kernel_main(argc, argv);
	while(1);
}
