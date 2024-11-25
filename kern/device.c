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

int devctl(const char* devname, devctl_t dev){
	int i;
	for(i = 0; i < devctl_list_index; i++){
		if(strequ(devctl_list[i].name, devname)){
			return devctl_list[i].handler(dev, devctl_list[i].userdata);
		}
	}
	return -1;
}

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
