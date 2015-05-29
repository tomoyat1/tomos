all: boot.o segsel.o irq_handler.o
	@echo Building asm 
%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
.PHONY: clean

clean:
	rm ./*.o
