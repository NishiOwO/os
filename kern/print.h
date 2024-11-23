/* $Id$ */

#ifndef __KERN_PRINT_H__
#define __KERN_PRINT_H__

void kprintinit(void);
extern void(*kprint)(const char* str);

#endif
