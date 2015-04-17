all: kernel.o i386
	@echo "Build final kernel image here"	 

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
	$(CC) -v

.PHONY: i386
i386:
	make -C i386
