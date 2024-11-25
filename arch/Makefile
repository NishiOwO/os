# $Id$

ARCH = i386
KERNEL = GENERIC

.PHONY: all fresh-kernel kernel clean ./config/config

all: ./config/config

kernel: ./config/config $(ARCH)/conf/$(KERNEL)
	if [ ! -f "../build/Makefile" ]; then \
		rm -rf ../build ; \
		./config/config $(ARCH)/conf/$(KERNEL) ; \
	fi
	$(MAKE) -C ../build BOOTLOADER=$(BOOTLOADER)

fresh-kernel: ./config/config $(ARCH)/conf/$(KERNEL)
	rm -rf ../build 
	./config/config $(ARCH)/conf/$(KERNEL)
	$(MAKE) -C ../build BOOTLOADER=$(BOOTLOADER)

./config/config::
	$(MAKE) -C ./config

clean:
	$(MAKE) -C ./config clean
