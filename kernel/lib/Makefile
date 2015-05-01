all: kmemcpy.o printk.o strlen.o kmalloc.o
	@echo Building lib 
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
.PHONY: clean
clean:
	rm ./*.o
