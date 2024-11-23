/* $Id$ */

#include <arch/io.h>
#include <kern/debug.h>
#include <kern/print.h>
#include <c/string.h>

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

#define DEFINE(x) { \
	static char argv ## x[64]; \
	argv[x] = (char*)argv ## x; \
}

void init_kernel(unsigned long magic, unsigned long addr){
	unsigned char* vram = (unsigned char*)0xb8000;
	int i;
	int argc = 0;
	char* argv[16];
	int count = 0;
	struct multiboot_info* info = (struct multiboot_info*)addr;
	DEFINE(0); DEFINE(1); DEFINE(2); DEFINE(3); DEFINE(4); DEFINE(5); DEFINE(6); DEFINE(7); DEFINE(8); DEFINE(9);
	DEFINE(10); DEFINE(11); DEFINE(12); DEFINE(13); DEFINE(14); DEFINE(15);
	kdebuginit();
	kprintinit();
	for(i = 0; i < 16; i++) argv[i][0] = 0;
	for(i = 0; info->cmdline[i] != 0; i++){
		if(info->cmdline[i] == ' '){
			count++;
		}else if(count >= 0){
			int l = strlen(argv[count]);
			argv[count][l + 0] = info->cmdline[i];
			argv[count][l + 1] = 0;
		}
	}
	for(i = 0; i < 16; i++){
		if(strlen(argv[i]) == 0){
			argc = i;
			break;
		}
	}
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
	kernel_main(argc, (char**)argv);
	while(1);
}
