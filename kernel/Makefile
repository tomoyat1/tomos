all: kernel.o i386
	gcc i386/boot.o -o kernel 

kernel.o: 
	gcc -c kernel.c -o kernel.o

.PHONY: i386
i386:
	make -C i386
