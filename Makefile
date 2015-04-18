CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Werror
CPPFLAGS:=$(CFLAGS) -D__is_tomos_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

PREFIX=/usr
BOOTDIR=/boot
INCLUDEDIR=$(PREFIX)/include

CC=i686-elf-gcc --sysroot=$(CURDIR)/sysroot -isystem=$(INCLUDEDIR)
export CC CFLAGS CPPFLAGS LDFLAGS PREFIX BOOTDIR INCLUDEDIR LIBS



all: kernel

.PHONY: kernel test clean
kernel:
	make -C kernel

clean:
	make clean -C kernel
