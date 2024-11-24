/* $Id$ */

#ifndef __C_STRING_H__
#define __C_STRING_H__

#include <arch/types.h>

int strlen(const char* str);
void strcpy(char* dest, const char* src);
void strcat(char* dest, const char* src);
void numstr(char* dest, uint64_t num);

#endif
