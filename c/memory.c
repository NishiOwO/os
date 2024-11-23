/* $Id$ */

#include <c/memory.h>

void* memset(void* ptr, uint8_t data, uintptr_t size){
	uint8_t* dst = (uint8_t*)ptr;
	for(; size != 0; size--) *dst++ = data;
	return ptr;
}

void memcpy(void* _dest, const void* _src, uintptr_t len){
	uint8_t* dest = (uint8_t*)_dest;
	uint8_t* src = (uint8_t*)_src;
	for(; len != 0; len--) *dest++ = *src++;
}
