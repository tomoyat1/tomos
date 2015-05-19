all: kmemcpy.o printk.o kmalloc.o kfree.o pow.o kstrcpy.o kstrlen.o kstrcmp.o
	@echo Building lib 
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
.PHONY: clean
clean:
	rm ./*.o
