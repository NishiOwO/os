/* $Id$ */

#ifndef __PARSE_H__
#define __PARSE_H__

#ifdef DEFINE_PARSE_H
#include <stddef.h>
#define PARSE_EXTERN
#define DEFAULT = NULL
#else
#define PARSE_EXTERN extern
#define DEFAULT
#endif

PARSE_EXTERN char* cc		DEFAULT;
PARSE_EXTERN char* cflags	DEFAULT;
PARSE_EXTERN char* ldflags	DEFAULT;

#endif
