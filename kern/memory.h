/* $Id$ */

#ifndef __KERN_MEMORY_H__
#define __KERN_MEMORY_H__

#include <arch/types.h>

void _free(void* mem);
void* _malloc(uintptr_t size);

#endif
