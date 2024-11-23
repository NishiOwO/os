/* $Id$ */

#ifndef __ARCH_TYPES_H__
#define __ARCH_TYPES_H__

typedef unsigned long long int uint64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef uint32_t uintptr_t;
typedef uintptr_t size_t;
typedef uint8_t bool;

struct alloc_t {
        uint8_t status;
        uint32_t size;
};

#define true 1
#define false 0
#define NULL (void*)0

#endif
