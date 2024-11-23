/* $Id$ */

#include <arch/types.h>
#include <arch/debug.h>
#include <kern/memory.h>

uintptr_t mem_last_alloc;
uintptr_t mem_heap_begin;
uintptr_t mem_heap_end;
uintptr_t mem_pheap_begin;
uintptr_t mem_pheap_end;
uintptr_t* mem_pheap_desc;
uintptr_t mem_used;

extern uintptr_t kernel_end;

void setup_memory(void){
	kdebug("Setting up memory");
	mem_used = 0;
	mem_last_alloc = kernel_end + 0x1000;
	mem_heap_begin = mem_last_alloc;
	mem_heap_end = mem_heap_begin + MAX_ALLOCS * 4096;
	mem_pheap_begin = mem_heap_end;
	mem_pheap_end = mem_pheap_begin + (MAX_ALLOCS * 1024);

	mem_pheap_desc = (uintptr_t*)_malloc(sizeof(struct alloc_t) * MAX_ALLOCS);
}
