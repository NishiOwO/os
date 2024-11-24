/* $Id$ */

#include <kern/device.h>
#include <kern/debug.h>
#include <c/string.h>

struct devctl_entry {
	char name[512];
	void* userdata;
	devctl_handler handler;
};

struct devctl_entry devctl_list[128];
int devctl_list_index = 0;

void register_device(const char* name, devctl_handler handler, void* userdata){
	char buf[512];
	buf[0] = 0;
	strcat(buf, "Registering device: ");
	strcat(buf, name);
	kdebug(buf);
	strcpy(devctl_list[devctl_list_index].name, name);
	devctl_list[devctl_list_index].userdata = userdata;
	devctl_list[devctl_list_index].handler = handler;
	devctl_list_index++;
}
