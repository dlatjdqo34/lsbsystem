obj-m := kdt_device_tree.o
KERNEL_MODULES := $(patsubst %.o,%.ko, $(obj-m))
KERNEL_MODULES += kdt.dtbo

RPI_SERVER_IP?=root@192.168.20.60
RPI_DIR=/root
RPI_PATH?=$(RPI_SERVER_IP):$(RPI_DIR)

export BUILDROOT=/home/lsb/kdt/buildroot
export ARCH=arm64
export CROSS_COMPILE=$(BUILDROOT)/output/host/bin/aarch64-buildroot-linux-gnu-
export DTC=/home/lsb/kdt/buildroot/output/host/bin/dtc

KERNELDIR ?= $(BUILDROOT)/output/build/linux-custom

all default: modules dt
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) $@

dt: kdt.dts
	$(DTC) -@ -I dts -O dtb -o kdt.dtbo kdt.dts

.PHONY: scp
scp:
	scp $(KERNEL_MODULES) $(RPI_PATH)
