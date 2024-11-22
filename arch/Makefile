# $Id$

ARCH = i386
KERNEL = GENERIC

.PHONY: all kernel clean ./config/config

all: ./config/config

kernel: ./config/config
	./config/config $(ARCH)/$(KERNEL)

./config/config::
	$(MAKE) -C ./config

clean:
	$(MAKE) -C ./config clean
