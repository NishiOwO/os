/* $Id$ */

#ifndef __ARCH_IO_H__
#define __ARCH_IO_H__

#include <arch/types.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

#endif
