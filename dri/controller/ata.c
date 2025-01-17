/* $Id$ */

#include <kern/debug.h>
#include <kern/device.h>
#include <arch/io.h>
#include <c/string.h>
#include <c/memory.h>

#define ATA_PRIMARY		0x1f0
#define ATA_PRIMARY_CTRL	0x3f6
#define ATA_SECONDARY		0x170
#define ATA_SECONDARY_CTRL	0x376
#define ATA_TERTIARY		0x1e8
#define ATA_TERTIARY_CTRL	0x3e6
#define ATA_QUINARY		0x168
#define ATA_QUINARY_CTRL	0x366

struct ata_device {
	bool configured;
	int base;
	int ctrl;
	int lba;
	int lba_mode;
};
struct ata_device ata_devices[4][2];

void ata_reset(int base){
	outb(base, 0x4);
	while(inb(base) & 0x80);
	outb(base, 0x2);
}

void ata_select(int slave, int bus, int ctrl){
	int j;
	outb(bus + 6, 0xa0 | slave<<4);
	for(j = 0; j < 15; j++) inb(ctrl);
}

int ata_devctl(devctl_t devctl, void* userdata){
	struct ata_device* devptr = (struct ata_device*)userdata;
	return 0;
}

void ata_probe(int bus, int ctrl){
	int i;
	const char* list[] = {
		"master",
		"slave"
	};
	for(i = 0; i <= 1; i++){
		int j;
		char str[512];
		int exists = 0;
		int hdd = 0;
		uint16_t buffer[256];
		int ind = 0;
		if(bus == ATA_PRIMARY){
			ind = 0;
		}else if(bus == ATA_SECONDARY){
			ind = 1;
		}else if(bus == ATA_TERTIARY){
			ind = 2;
		}else if(bus == ATA_QUINARY){
			ind = 3;
		}
		ata_devices[ind][i].configured = false;
		ata_devices[ind][i].base = bus;
		ata_devices[ind][i].ctrl = ctrl;
		ata_devices[ind][i].lba = 0;
		ata_devices[ind][i].lba_mode = 28;
		str[0] = 0;
		strcat(str, bus == ATA_PRIMARY ? "Primary" : (bus == ATA_SECONDARY ? "Secondary" : (bus == ATA_TERTIARY ? "Tertialy" : "Quinary")));
		strcat(str, " ");
		strcat(str, list[i]);
		strcat(str, " ");
		outb(bus + 6, i == 0 ? 0xa0 : 0xb0);
		for(j = 0; j < 15; j++) inb(ctrl);
		for(j = 2; j <= 5; j++) outb(bus + j, 0);
		outb(bus + 7, 0xec);
		if(inb(bus + 7) == 0){
			strcat(str, "does not exist");
		}else{
			int status;
			while(inb(bus + 7) & 0x80);
			if(inb(bus + 4) != 0 && inb(bus + 5) != 0){
				status = 8;
				goto no_poll;
			}
read_again:
			status = inb(bus + 7);
no_poll:
			if(status & 1){
				strcat(str, "has error");
			}else{
				if(!(status & 8)) goto read_again;
				for(j = 0; j < 256; j++) buffer[j] = inw(bus);
				strcat(str, "exists");
				exists = 1;
				hdd = buffer[0] != 0;
			}
		}
		kdebug(str);
		if(exists){
			char cbuf[2];
			char devstr[5];
			cbuf[1] = 0;
			devstr[0] = 0;
			strcat(devstr, "DI");
			cbuf[0] = 'A' + ind;
			strcat(devstr, cbuf);
			cbuf[0] = '0' + i;
			strcat(devstr, cbuf);
			ata_devices[ind][i].configured = true;
			if(hdd){
				uint32_t lba28;
				uint64_t lba48;
				uint64_t lba;
				char sectors[512];
				char bytes[512];
				char str[1024];
				str[0] = 0;
				memcpy(&lba28, buffer + 60, 32 / 8);
				memcpy(&lba48, buffer + 100, 64 / 8);
				lba = lba48 != 0 ? lba48 : lba28;
				ata_devices[ind][i].lba = lba;
				if(lba28 != 0){
					kdebug("\tsupports LBA28");
				}
				if(lba48 != 0){
					kdebug("\tsupports LBA48");
					if(lba48 < 0x10000000){
						kdebug("\tbut does not need to use LBA48");
					}else{
						ata_devices[ind][i].lba_mode = 48;
					}
				}
				numstr(sectors, lba);
				numstr(bytes, lba * 512);
				strcat(str, "\t");
				strcat(str, sectors);
				strcat(str, " sectors");
				strcat(str, " / ");
				strcat(str, bytes);
				strcat(str, " bytes");
				kdebug(str);
				kdebug("\tis hard disk");
			}else{
				kdebug("\tis not hard disk");
			}
			register_device(devstr, ata_devctl, &ata_devices[ind][i]);
		}
	}
}

void ata_init(void){
	bool pri = false;
	bool sec = false;
	bool ter = false;
	bool qui = false;
	if(inb(ATA_PRIMARY + 7) == 0xff){
		kdebug("ATA primary bus does not exist");
	}else{
		ata_reset(ATA_PRIMARY_CTRL);
		kdebug("ATA primary bus ready");
		pri = true;
	}

	if(inb(ATA_SECONDARY + 7) == 0xff){
		kdebug("ATA secondary bus does not exist");
	}else{
		ata_reset(ATA_SECONDARY_CTRL);
		kdebug("ATA secondary bus ready");
		sec = true;
	}

	if(inb(ATA_TERTIARY + 7) == 0xff){
		kdebug("ATA tertiary bus does not exist");
	}else{
		ata_reset(ATA_TERTIARY_CTRL);
		kdebug("ATA tertiary bus ready");
		ter = true;
	}

	if(inb(ATA_QUINARY + 7) == 0xff){
		kdebug("ATA quinary bus does not exist");
	}else{
		ata_reset(ATA_QUINARY_CTRL);
		kdebug("ATA quinary bus ready");
		qui = true;
	}

	if(pri || sec || ter || qui){
		kdebug("Probing for ATA drives");
		if(pri) ata_probe(ATA_PRIMARY, ATA_PRIMARY_CTRL);
		if(sec) ata_probe(ATA_SECONDARY, ATA_SECONDARY_CTRL);
		if(ter) ata_probe(ATA_TERTIARY, ATA_TERTIARY_CTRL);
		if(qui) ata_probe(ATA_QUINARY, ATA_QUINARY_CTRL);
	}else{
		kdebug("No ATA bus detected");
	}
}
