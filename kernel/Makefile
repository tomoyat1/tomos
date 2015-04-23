export CC CFLAGS CPPFLAGS LDFLAGS LIBS
OBJECTS=./*.o ./x86/*.o ./lib/*.o
all: kernel.o x86 lib
	@echo "Building kernel image"
	$(CC) -T ./linker.ld -o ../sysroot/boot/kernel -ffreestanding -O2 -nostdlib $(OBJECTS) -lgcc

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.PHONY: x86 lib clean
x86:
	$(MAKE) -C x86

lib:
	$(MAKE) -C lib 

clean:
	rm ./*.o && $(MAKE) clean -C x86 && $(MAKE) clean -C lib
