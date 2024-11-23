/* $Id$ */

#include <kern/debug.h>
#include <arch/io.h>
#include <c/string.h>

#define ATA_PRIMARY		0x1f0
#define ATA_PRIMARY_CTRL	0x3f6
#define ATA_SECONDARY		0x170
#define ATA_SECONDARY_CTRL	0x376

void ata_reset(int base){
	outb(base, 0x4);
	while(inb(base) & 0x80);
	outb(base, 0x2);
}

void ata_select(int slave, int bus, int ctrl){
	outb(bus + 6, 0xa0 | slave<<4);
	inb(ctrl);
	inb(ctrl);
	inb(ctrl);
	inb(ctrl);
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
		str[0] = 0;
		strcat(str, bus == ATA_PRIMARY ? "Primary" : "Secondary");
		strcat(str, " ");
		strcat(str, list[i]);
		strcat(str, " ");
		outb(bus + 6, i == 0 ? 0xa0 : 0xb0);
		inb(ctrl);
		inb(ctrl);
		inb(ctrl);
		inb(ctrl);
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
				uint16_t buffer[256];
				if(!(status & 8)) goto read_again;
				for(j = 0; j < 256; j++) buffer[j] = inw(bus);
				strcat(str, "exists");
				exists = 1;
				hdd = buffer[0] != 0;
			}
		}
		kdebug(str);
		if(exists){
			if(hdd){
				kdebug("        is hard disk");
			}else{
				kdebug("        is not hard disk");
			}
		}
	}
}

void ata_init(void){
	bool pri = false;
	bool sec = false;
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

	if(pri || sec){
		kdebug("Probing for ATA drives");
		if(pri) ata_probe(ATA_PRIMARY, ATA_PRIMARY_CTRL);
		if(sec) ata_probe(ATA_SECONDARY, ATA_SECONDARY_CTRL);
	}else{
		kdebug("No ATA bus detected");
	}
}
