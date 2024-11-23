/* $Id$ */

#include <kern/memory.h>

extern uintptr_t mem_last_alloc;
extern uintptr_t mem_heap_begin;
extern uintptr_t mem_heap_end;
extern uintptr_t mem_pheap_begin;
extern uintptr_t mem_pheap_end;
extern uintptr_t* mem_pheap_desc;
extern uintptr_t mem_used;

void* _malloc(size_t size){
        if(!size) return NULL;
        uint8_t* mem = (uint8_t*)mem_heap_begin;
        while((uint32_t)mem < mem_last_alloc){
                struct alloc_t* a = (struct alloc_t*)mem;
                if(!a->size) goto nalloc;
                if(a->status){
                        mem += a->size;
                        mem += sizeof(struct alloc_t);
                        mem += 4;
                        continue;
                }
                if(a->size >= size){
                        a->status = 1;
                        memset(mem + sizeof(struct alloc_t), 0, size);
                        mem_used += size + sizeof(struct alloc_t);
                        return (uint8_t*)(mem + sizeof(struct alloc_t));
                }
                mem += a->size;
                mem += sizeof(struct alloc_t);
                mem += 4;
        }
nalloc:
        if(mem_last_alloc + size + sizeof(struct alloc_t) >= mem_heap_end){
                return NULL;
        }
        struct alloc_t* alloc = (struct alloc_t*)mem_last_alloc;
        alloc->status = 1;
        alloc->size = size;
        mem_last_alloc += size;
        mem_last_alloc += sizeof(struct alloc_t);
        mem_last_alloc += 4;
        mem_used += size + 4 + sizeof(struct alloc_t);
        memset((uint8_t*)((uintptr_t)alloc + sizeof(struct alloc_t)), 0, size);
        return (uint8_t*)((uintptr_t)alloc + sizeof(struct alloc_t));
}

void _free(void* mem){
	struct alloc_t* alloc = ((struct alloc_t*)mem - sizeof(struct alloc_t));
	mem_used -= alloc->size + sizeof(struct alloc_t);
	alloc->status = 0;
}

void* memset(void* ptr, uint8_t data, uintptr_t size){
	uint8_t* dst = (uint8_t*)ptr;
	for(; size != 0; size--) *dst++ = data;
	return ptr;
}
