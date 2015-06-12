all: pminit.o interrupts.o paging.o apic.o pit.o asm
	@echo Building x86
%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
.PHONY: clean asm
asm:
	$(MAKE) -C ./asm

clean:
	rm ./*.o && $(MAKE) clean -C asm
