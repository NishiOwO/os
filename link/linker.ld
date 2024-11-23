/* $Id$ */
/* This is a template linker file ; edit it to fit your port */

ENTRY(_start)

SECTIONS {
	. = 0x100000;
	.text : {
#ifdef MULTIBOOT
		*(.multiboot)
#endif
		*(.text)
	}
	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}
 
	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}
 
	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON)
		*(.bss)
	}

	.comment BLOCK(4K) : ALIGN(4K) {
		*(.comment)
	}
}
