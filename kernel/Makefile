export CC CFLAGS CPPFLAGS LDFLAGS LIBS
all: kernel.o x86 lib
	@echo "Build final kernel image here"	 

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.PHONY: x86 lib clean
x86:
	make -C x86

lib:
	make -C lib 

clean:
	rm ./*.o && make clean -C x86 && make clean -C lib
