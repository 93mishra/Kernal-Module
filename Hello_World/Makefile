obj-m := hello-1.o
KDIR := /lib/modules/`uname -r`/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD)

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
