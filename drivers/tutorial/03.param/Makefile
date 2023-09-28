obj-m := module_param.o
KERNEL_MODULES := $(patsubst %.o,%.ko, $(obj-m))

RPI_SERVER_IP?=root@192.168.20.60
RPI_DIR=/root
RPI_PATH?=$(RPI_SERVER_IP):$(RPI_DIR)

export BUILDROOT=/home/lsb/kdt/buildroot/
export ARCH=arm64
export CROSS_COMPILE=$(BUILDROOT)/output/host/bin/aarch64-buildroot-linux-gnu-

KERNELDIR ?= $(BUILDROOT)/output/build/linux-custom

all default: modules
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) $@

.PHONY: scp
scp:
	scp $(KERNEL_MODULES) $(RPI_PATH)