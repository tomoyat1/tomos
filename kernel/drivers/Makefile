all: vga.o
	@echo Building drivers 
%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
.PHONY: clean
clean:
	rm ./*.o
