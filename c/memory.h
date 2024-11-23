/* $Id$ */

#ifndef __C_MEMORY_H__
#define __C_MEMORY_H__

#include <arch/types.h>

void* memset(void* ptr, uint8_t data, uintptr_t size);
void memcpy(void* _dest, const void* _src, uintptr_t len);

#endif
