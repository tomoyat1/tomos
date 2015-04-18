export CC CFLAGS CPPFLAGS LDFLAGS LIBS
all: kernel.o i386
	@echo "Build final kernel image here"	 

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.PHONY: i386 clean
i386:
	make -C i386

clean:
	rm ./*.o && make clean -C i386
