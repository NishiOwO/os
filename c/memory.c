/* $Id$ */

extern #include <c/memory.h>

void* memset(void* ptr, uint8_t data, uintptr_t size){
	uint8_t* dst = (uint8_t*)ptr;
	for(; size != 0; size--) *dst++ = data;
	return ptr;
}
