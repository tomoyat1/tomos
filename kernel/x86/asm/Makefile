all: boot.o segsel.o
	@echo Building asm 
%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
.PHONY: clean

clean:
	rm ./*.o
