/* $Id$ */
/* This is a template linker file ; edit it to fit your port */

ENTRY(_start)

SECTIONS {
#ifdef MULTIBOOT
	.multiboot : {
		*(.multiboot)
	}
#endif
}
