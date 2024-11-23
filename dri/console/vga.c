/* $Id$ */

#include <arch/types.h>
#include <arch/io.h>
#include <kern/debug.h>
#include <kern/print.h>

int vga_x = 0;
int vga_y = 0;
int vga_color = 0x07;

void vga_putchar(char c){
	uint16_t pos = vga_y * 80 + vga_x;
	uint8_t* vram = (uint8_t*)0xb8000;

	vram += pos * 2;
	if(c == '\r'){
		vga_x = 0;
	}else if(c == '\n'){
		vga_y++;
	}else{
		*vram++ = c;
		*vram++ = vga_color;
		vga_x++;
		if(vga_x == 80){
			vga_x = 0;
			vga_y++;
		}
	}
	if(vga_y == 25){
		uint8_t* base = (uint8_t*)0xb8000;
		int i;
		for(i = 0; i < 80 * 24; i++){
			base[i * 2 + 0] = base[i * 2 + 80 * 2 + 0];
			base[i * 2 + 1] = base[i * 2 + 80 * 2 + 1];
		}
		for(i = 0; i < 80; i++){
			base[i * 2 + 80 * 24 * 2 + 0] = 0;
			base[i * 2 + 80 * 24 * 2 + 1] = 0x07;
		}
		vga_y = 24;
	}
	pos = vga_y * 80 + vga_x;
	vram = (uint8_t*)0xb8000 + pos * 2;
	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint8_t)((((uintptr_t)vram - 0xb8000) / 2) & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint8_t)((((uintptr_t)vram - 0xb8000) / 2) >> 8) & 0xff);
}

void vga_kdebug(const char* str){
#ifdef DEBUG
	vga_color = 0x1f;
	int i;
	for(i = 0; str[i] != 0; i++){
		vga_putchar(str[i]);
	}
	vga_putchar('\r');
	vga_putchar('\n');
	vga_color = 7;
#endif
}

void vga_kprint(const char* str){
	int i;
	for(i = 0; str[i] != 0; i++){
		vga_putchar(str[i]);
	}
}

void vga_init(void){
	uint8_t* vram = (uint8_t*)0xb8000;
	int i;
	for(i = 0; i < 80 * 25; i++){
		vram[i * 2 + 0] = 0;
		vram[i * 2 + 1] = vga_color;
	}
	outb(0x3d4, 0x0a);
	outb(0x3d5, (inb(0x3d5) & 0xc0) | 0x0);
	outb(0x3d4, 0x0b);
	outb(0x3d5, (inb(0x3d5) & 0xe0) | 0xf);

	kdebug("Switching to VGA console");

	kdebug = vga_kdebug;
	kprint = vga_kprint;

	kdebug("VGA console initialized");
}
