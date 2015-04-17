CC=i686-elf-gcc --sysroot=$(shell pwd)/sysroot 
CFLAGS=$(CFLAGS) -ffreestanding -Wall -Werror
CPPFLAGS=$(CFLAGS) -D__is_tomos_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
PREFIX=$(shell pwd)
BOOTDIR?=$(PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/sysroot/usr/include
LIBS:=$(LIBS) -nostdlib -lk -lgcc





all: kernel
	echo "Building all"	

.PHONY: kernel test
kernel:
	make -C kernel

test:
	$(CC) --version
