all: boot.o port.o tables.o
	@echo Building i386
%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
.PHONY: clean
clean:
	rm ./*.o
