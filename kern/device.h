/* $Id$ */

#ifndef __KERN_DEVICE_H__
#define __KERN_DEVICE_H__

struct devctl {
};
typedef struct devctl devctl_t;

typedef int(*devctl_handler)(devctl_t devctl, void* userdata);

void register_device(const char* name, devctl_handler handler, void* userdata);

#endif
