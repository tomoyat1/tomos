export CC CFLAGS CPPFLAGS LDFLAGS LIBS
OBJECTS=./*.o ./x86/*.o ./x86/asm/*.o ./lib/*.o ./drivers/*.o
all: kernel.o panic.o slaballoc.o mm.o x86 lib drivers
	@echo "Building kernel image"
	$(CC) -T ./linker.ld -o ../sysroot/boot/kernel -ffreestanding -O2 -nostdlib $(OBJECTS) -lgcc

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.PHONY: x86 lib clean drivers
x86:
	$(MAKE) -C x86

lib:
	$(MAKE) -C lib 

drivers:
	$(MAKE) -C drivers 

clean:
	rm ./*.o && $(MAKE) clean -C x86 && $(MAKE) clean -C lib && $(MAKE) clean -C drivers 
