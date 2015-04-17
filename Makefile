CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Werror
CPPFLAGS:=$(CFLAGS) -D__is_tomos_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
PREFIX?=$(shell pwd)
BOOTDIR?=$(PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/sysroot/usr/include
LIBS:=$(LIBS) -nostdlib -lk -lgcc

export CC CFLAGS CPPFLAGS LDFLAGS BOOTDIR INCLUDEDIR LIBS



all: kernel
	echo "Building all"	

.PHONY: kernel test
kernel:
	make -C kernel
