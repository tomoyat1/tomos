CFLAGS:=$(CFLAGS) -ffreestanding -Wall  -O0 -g -Werror #-DDEBUG
CPPFLAGS:=$(CFLAGS) -D__is_tomos_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

PREFIX=/usr
BOOTDIR=/boot
INCLUDEDIR=$(PREFIX)/include

CC:=i686-elf-gcc --sysroot=$(CURDIR)/sysroot -isystem=$(INCLUDEDIR)
MAKE=make
export CC CFLAGS CPPFLAGS LDFLAGS PREFIX BOOTDIR INCLUDEDIR LIBS



all: kernel

.PHONY: kernel clean run
kernel:
	$(MAKE) -C kernel

clean:
	$(MAKE) clean -C kernel

run: all
	./boot.sh
