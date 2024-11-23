/* $Id$ */

#ifndef __KERN_DEBUG_H__
#define __KERN_DEBUG_H__

void kdebuginit(void);
extern void(*kdebug)(const char* str);

#endif
