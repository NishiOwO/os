# $Id$

ARCH = i386
KERNEL = GENERIC

.PHONY: all fresh build clean ./config/config

all: ./config/config

build: ./config/config $(ARCH)/conf/$(KERNEL)
	if [ ! -f "../build/Makefile" ]; then \
		rm -rf ../build ; \
		./config/config $(ARCH)/conf/$(KERNEL) ; \
	fi
	$(MAKE) -C ../build BOOTLOADER=$(BOOTLOADER)

fresh: ./config/config $(ARCH)/conf/$(KERNEL)
	rm -rf ../build 
	./config/config $(ARCH)/conf/$(KERNEL)
	$(MAKE) -C ../build BOOTLOADER=$(BOOTLOADER)

./config/config::
	$(MAKE) -C ./config

clean:
	$(MAKE) -C ./config clean
